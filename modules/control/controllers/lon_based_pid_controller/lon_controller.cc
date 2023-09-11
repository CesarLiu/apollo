/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#include "modules/control/controllers/lon_based_pid_controller/lon_controller.h"

#include <algorithm>
#include <utility>

#include "absl/strings/str_cat.h"

#include "cyber/common/log.h"
#include "cyber/time/clock.h"
#include "cyber/time/time.h"
#include "modules/common/configs/vehicle_config_helper.h"
#include "modules/common/math/math_utils.h"
#include "modules/control/control_component/common/control_gflags.h"
#include "modules/localization/common/localization_gflags.h"

namespace apollo {
namespace control {

using apollo::common::ErrorCode;
using apollo::common::Status;
using apollo::common::TrajectoryPoint;
using apollo::common::VehicleStateProvider;
using apollo::cyber::Time;
using apollo::planning::ADCTrajectory;
using apollo::planning::StopReasonCode;

constexpr double GRA_ACC = 9.8;

LonController::LonController() : name_("PID-basesd Longitudinal Controller") {
  if (FLAGS_enable_csv_debug) {
    time_t rawtime;
    char name_buffer[80];
    std::time(&rawtime);
    std::tm time_tm;
    localtime_r(&rawtime, &time_tm);
    strftime(name_buffer, 80, "/tmp/speed_log__%F_%H%M%S.csv", &time_tm);
    speed_log_file_ = fopen(name_buffer, "w");
    if (speed_log_file_ == nullptr) {
      AERROR << "Fail to open file:" << name_buffer;
      FLAGS_enable_csv_debug = false;
    }
    if (speed_log_file_ != nullptr) {
      fprintf(speed_log_file_,
              "station_reference,"
              "station_error,"
              "station_error_limited,"
              "preview_station_error,"
              "speed_reference,"
              "speed_error,"
              "speed_error_limited,"
              "preview_speed_reference,"
              "preview_speed_error,"
              "preview_acceleration_reference,"
              "acceleration_cmd_closeloop,"
              "acceleration_cmd,"
              "acceleration_lookup,"
              "acceleration_lookup_limit,"
              "speed_lookup,"
              "calibration_value,"
              "throttle_cmd,"
              "brake_cmd,"
              "is_full_stop,"
              "\r\n");

      fflush(speed_log_file_);
    }
    AINFO << name_ << " used.";
  }
}

void LonController::CloseLogFile() {
  if (FLAGS_enable_csv_debug) {
    if (speed_log_file_ != nullptr) {
      fclose(speed_log_file_);
      speed_log_file_ = nullptr;
    }
  }
}
void LonController::Stop() { CloseLogFile(); }

LonController::~LonController() { CloseLogFile(); }

Status LonController::Init(std::shared_ptr<DependencyInjector> injector) {
  if (!ControlTask::LoadConfig<LonBasedPidControllerConf>(
          &lon_based_pidcontroller_conf_)) {
    AERROR << "failed to load control conf";
    return Status(ErrorCode::CONTROL_INIT_ERROR,
                  "failed to load lon control_conf");
  }

  if (!ControlTask::LoadCalibrationTable(&calibration_table_)) {
    AERROR << "failed to load calibration table";
    return Status(ErrorCode::CONTROL_INIT_ERROR,
                  "lon failed to load calibration table");
  }

  injector_ = injector;
  // const LonControllerConf &lon_controller_conf =
  //     control_conf_->lon_controller_conf();
  double ts = lon_based_pidcontroller_conf_.ts();
  bool enable_leadlag =
      lon_based_pidcontroller_conf_.enable_reverse_leadlag_compensation();

  station_pid_controller_.Init(
      lon_based_pidcontroller_conf_.station_pid_conf());
  speed_pid_controller_.Init(
      lon_based_pidcontroller_conf_.low_speed_pid_conf());

  if (enable_leadlag) {
    station_leadlag_controller_.Init(
        lon_based_pidcontroller_conf_.reverse_station_leadlag_conf(), ts);
    speed_leadlag_controller_.Init(
        lon_based_pidcontroller_conf_.reverse_speed_leadlag_conf(), ts);
  }

  vehicle_param_.CopyFrom(
      common::VehicleConfigHelper::Instance()->GetConfig().vehicle_param());

  SetDigitalFilterPitchAngle();

  InitControlCalibrationTable();
  controller_initialized_ = true;

  return Status::OK();
}

void LonController::SetDigitalFilterPitchAngle() {
  double cutoff_freq =
      lon_based_pidcontroller_conf_.pitch_angle_filter_conf().cutoff_freq();
  double ts = lon_based_pidcontroller_conf_.ts();
  SetDigitalFilter(ts, cutoff_freq, &digital_filter_pitch_angle_);
}

void LonController::InitControlCalibrationTable() {
  AINFO << "Control calibration table size is "
        << calibration_table_.calibration_size();
  Interpolation2D::DataType xyz;
  for (const auto &calibration : calibration_table_.calibration()) {
    xyz.push_back(std::make_tuple(calibration.speed(),
                                  calibration.acceleration(),
                                  calibration.command()));
  }
  control_interpolation_.reset(new Interpolation2D);
  ACHECK(control_interpolation_->Init(xyz))
      << "Fail to load control calibration table";
}

Status LonController::ComputeControlCommand(
    const localization::LocalizationEstimate *localization,
    const canbus::Chassis *chassis,
    const planning::ADCTrajectory *planning_published_trajectory,
    control::ControlCommand *cmd) {
  localization_ = localization;
  chassis_ = chassis;

  trajectory_message_ = planning_published_trajectory;
  if (!control_interpolation_) {
    AERROR << "Fail to initialize calibration table.";
    return Status(ErrorCode::CONTROL_COMPUTE_ERROR,
                  "Fail to initialize calibration table.");
  }

  if (trajectory_analyzer_ == nullptr ||
      trajectory_analyzer_->seq_num() !=
          trajectory_message_->header().sequence_num()) {
    trajectory_analyzer_.reset(new TrajectoryAnalyzer(trajectory_message_));
  }

  auto debug = cmd->mutable_debug()->mutable_simple_lon_debug();
  debug->Clear();

  double brake_cmd = 0.0;
  double throttle_cmd = 0.0;
  double ts = lon_based_pidcontroller_conf_.ts();
  double preview_time = lon_based_pidcontroller_conf_.preview_window() * ts;
  bool enable_leadlag =
      lon_based_pidcontroller_conf_.enable_reverse_leadlag_compensation();

  if (preview_time < 0.0) {
    const auto error_msg =
        absl::StrCat("Preview time set as: ", preview_time, " less than 0");
    AERROR << error_msg;
    return Status(ErrorCode::CONTROL_COMPUTE_ERROR, error_msg);
  }
  ComputeLongitudinalErrors(trajectory_analyzer_.get(), preview_time, ts,
                            debug);

  double station_error_limit =
      lon_based_pidcontroller_conf_.station_error_limit();
  double station_error_limited = 0.0;
  if (lon_based_pidcontroller_conf_.enable_speed_station_preview()) {
    station_error_limited =
        common::math::Clamp(debug->preview_station_error(),
                            -station_error_limit, station_error_limit);
  } else {
    station_error_limited = common::math::Clamp(
        debug->station_error(), -station_error_limit, station_error_limit);
  }

  if (trajectory_message_->gear() == canbus::Chassis::GEAR_REVERSE) {
    station_pid_controller_.SetPID(
        lon_based_pidcontroller_conf_.reverse_station_pid_conf());
    speed_pid_controller_.SetPID(
        lon_based_pidcontroller_conf_.reverse_speed_pid_conf());
    if (enable_leadlag) {
      station_leadlag_controller_.SetLeadlag(
          lon_based_pidcontroller_conf_.reverse_station_leadlag_conf());
      speed_leadlag_controller_.SetLeadlag(
          lon_based_pidcontroller_conf_.reverse_speed_leadlag_conf());
    }
  } else if (injector_->vehicle_state()->linear_velocity() <=
             lon_based_pidcontroller_conf_.switch_speed()) {
    station_pid_controller_.SetPID(
        lon_based_pidcontroller_conf_.station_pid_conf());
    speed_pid_controller_.SetPID(
        lon_based_pidcontroller_conf_.low_speed_pid_conf());
  } else {
    station_pid_controller_.SetPID(
        lon_based_pidcontroller_conf_.station_pid_conf());
    speed_pid_controller_.SetPID(
        lon_based_pidcontroller_conf_.high_speed_pid_conf());
  }

  double speed_offset =
      station_pid_controller_.Control(station_error_limited, ts);
  if (enable_leadlag) {
    speed_offset = station_leadlag_controller_.Control(speed_offset, ts);
  }

  double speed_controller_input = 0.0;
  double speed_controller_input_limit =
      lon_based_pidcontroller_conf_.speed_controller_input_limit();
  double speed_controller_input_limited = 0.0;
  if (lon_based_pidcontroller_conf_.enable_speed_station_preview()) {
    speed_controller_input = speed_offset + debug->preview_speed_error();
  } else {
    speed_controller_input = speed_offset + debug->speed_error();
  }
  speed_controller_input_limited =
      common::math::Clamp(speed_controller_input, -speed_controller_input_limit,
                          speed_controller_input_limit);

  double acceleration_cmd_closeloop = 0.0;

  acceleration_cmd_closeloop =
      speed_pid_controller_.Control(speed_controller_input_limited, ts);
  debug->set_pid_saturation_status(
      speed_pid_controller_.IntegratorSaturationStatus());
  if (enable_leadlag) {
    acceleration_cmd_closeloop =
        speed_leadlag_controller_.Control(acceleration_cmd_closeloop, ts);
    debug->set_leadlag_saturation_status(
        speed_leadlag_controller_.InnerstateSaturationStatus());
  }

  if (chassis->gear_location() == canbus::Chassis::GEAR_NEUTRAL) {
    speed_pid_controller_.Reset_integral();
    station_pid_controller_.Reset_integral();
  }

  double slope_offset_compensation = digital_filter_pitch_angle_.Filter(
      GRA_ACC * std::sin(injector_->vehicle_state()->pitch()));

  if (std::isnan(slope_offset_compensation)) {
    slope_offset_compensation = 0;
  }

  debug->set_slope_offset_compensation(slope_offset_compensation);

  double acceleration_cmd =
      acceleration_cmd_closeloop + debug->preview_acceleration_reference() +
      lon_based_pidcontroller_conf_.enable_slope_offset() *
          debug->slope_offset_compensation();

  // Check the steer command in reverse trajectory if the current steer target
  // is larger than previous target, free the acceleration command, wait for
  // the current steer target
  double current_steer_interval =
      cmd->steering_target() - chassis->steering_percentage();
  if (lon_based_pidcontroller_conf_.use_steering_check() &&
      (trajectory_message_->trajectory_type() ==
       apollo::planning::ADCTrajectory::UNKNOWN) &&
      std::abs(current_steer_interval) >
          lon_based_pidcontroller_conf_.steer_cmd_interval()) {
    ADEBUG << "steering_target is " << cmd->steering_target()
           << " steering_percentage is " << chassis->steering_percentage();
    ADEBUG << "Steer cmd interval is larger than "
           << lon_based_pidcontroller_conf_.steer_cmd_interval();

    speed_pid_controller_.Reset_integral();
    station_pid_controller_.Reset_integral();
    acceleration_cmd = 0;
    debug->set_is_wait_steer(true);
  } else {
    debug->set_is_wait_steer(false);
  }
  debug->set_current_steer_interval(current_steer_interval);

  // At near-stop stage, replace the brake control command with the standstill
  // acceleration if the former is even softer than the latter
  debug->set_is_full_stop(false);
  debug->set_is_full_stop_soft(false);
  GetPathRemain(debug);
  IsStopByDestination(debug);
  IsPedestrianStopLongTerm(debug);
  if (lon_based_pidcontroller_conf_.use_preview_reference_check() &&
      (std::fabs(debug->preview_acceleration_reference()) <=
       FLAGS_max_acceleration_when_stopped) &&
      std::fabs(debug->preview_speed_reference()) <=
          vehicle_param_.max_abs_speed_when_stopped() &&
      trajectory_message_->trajectory_type() != ADCTrajectory::OPEN_SPACE) {
    if (debug->is_stop_reason_by_destination() ||
        debug->is_stop_reason_by_prdestrian()) {
      debug->set_is_full_stop(true);
      ADEBUG << "Into full stop within preview acc and reference speed, "
             << "is_full_stop is " << debug->is_full_stop();
    } else {
      debug->set_is_full_stop_soft(true);
      ADEBUG << "Into full stop soft within preview acc and reference speed, "
             << "is_full_stop_soft is " << debug->is_full_stop_soft();
    }
  }

  if (std::abs(debug->path_remain()) < FLAGS_max_path_remain_when_stopped) {
    if (debug->is_stop_reason_by_destination() ||
        debug->is_stop_reason_by_prdestrian() ||
        trajectory_message_->trajectory_type() == ADCTrajectory::OPEN_SPACE) {
      debug->set_is_full_stop(true);
      ADEBUG << "Current path remain distance: " << debug->path_remain()
             << " is within max_path_remain threshold: "
             << FLAGS_max_path_remain_when_stopped
             << ", into full stop because vehicle is in destination: "
             << debug->is_stop_reason_by_destination()
             << " or pedestrian is in long time stop: "
             << debug->is_stop_reason_by_prdestrian()
             << "is_full_stop flag: " << debug->is_full_stop();
    } else {
      debug->set_is_full_stop_soft(true);
      ADEBUG << "Current path remain distance: " << debug->path_remain()
             << " is within max_path_remain threshold: "
             << FLAGS_max_path_remain_when_stopped
             << ", but not into full stop because stop not in destination: "
             << debug->is_stop_reason_by_destination()
             << " or pedestrian is not long time stop: "
             << debug->is_stop_reason_by_prdestrian()
             << "is_full_stop_soft flag: " << debug->is_full_stop_soft();
    }
  }

  if (chassis_->speed_mps() < vehicle_param_.max_abs_speed_when_stopped() &&
      debug->is_stop_reason_by_prdestrian()) {
    ADEBUG << "Current stop is for long time pedestrian stop, "
           << debug->is_stop_reason_by_prdestrian();
    debug->set_is_full_stop(true);
  }

  if (debug->is_full_stop()) {
    acceleration_cmd =
        (chassis->gear_location() == canbus::Chassis::GEAR_REVERSE)
            ? std::max(acceleration_cmd,
                       -lon_based_pidcontroller_conf_.standstill_acceleration())
            : std::min(acceleration_cmd,
                       lon_based_pidcontroller_conf_.standstill_acceleration());
    speed_pid_controller_.Reset_integral();
    station_pid_controller_.Reset_integral();
  }

  if (debug->is_full_stop_soft()) {
    acceleration_cmd =
        (acceleration_cmd >= 0)
            ? std::min(acceleration_cmd, lon_based_pidcontroller_conf_
                                             .standstill_narmal_acceleration())
            : lon_based_pidcontroller_conf_.standstill_narmal_acceleration();
    speed_pid_controller_.Reset_integral();
    station_pid_controller_.Reset_integral();
  }

  double throttle_lowerbound =
      std::max(vehicle_param_.throttle_deadzone(),
               lon_based_pidcontroller_conf_.throttle_minimum_action());
  double brake_lowerbound =
      std::max(vehicle_param_.brake_deadzone(),
               lon_based_pidcontroller_conf_.brake_minimum_action());
  double calibration_value = 0.0;
  double acceleration_lookup =
      (chassis->gear_location() == canbus::Chassis::GEAR_REVERSE)
          ? -acceleration_cmd
          : acceleration_cmd;

  double acceleration_lookup_limited =
      vehicle_param_.max_acceleration() +
      lon_based_pidcontroller_conf_.enable_slope_offset() *
          debug->slope_offset_compensation();
  double acceleration_lookup_limit = 0.0;

  if (lon_based_pidcontroller_conf_.use_acceleration_lookup_limit()) {
    acceleration_lookup_limit =
        (acceleration_lookup > acceleration_lookup_limited)
            ? acceleration_lookup_limited
            : acceleration_lookup;
  }

  if (FLAGS_use_preview_speed_for_table) {
    if (lon_based_pidcontroller_conf_.use_acceleration_lookup_limit()) {
      calibration_value = control_interpolation_->Interpolate(std::make_pair(
          debug->preview_speed_reference(), acceleration_lookup_limit));
    } else {
      calibration_value = control_interpolation_->Interpolate(std::make_pair(
          debug->preview_speed_reference(), acceleration_lookup));
    }
  } else {
    if (lon_based_pidcontroller_conf_.use_acceleration_lookup_limit()) {
      calibration_value = control_interpolation_->Interpolate(
          std::make_pair(chassis_->speed_mps(), acceleration_lookup_limit));
    } else {
      calibration_value = control_interpolation_->Interpolate(
          std::make_pair(chassis_->speed_mps(), acceleration_lookup));
    }
  }

  if (acceleration_lookup >= 0) {
    if (calibration_value >= 0) {
      throttle_cmd = std::max(calibration_value, throttle_lowerbound);
    } else {
      throttle_cmd = throttle_lowerbound;
    }
    brake_cmd = 0.0;
  } else {
    throttle_cmd = 0.0;
    if (calibration_value >= 0) {
      brake_cmd = brake_lowerbound;
    } else {
      brake_cmd = std::max(-calibration_value, brake_lowerbound);
    }
  }

  debug->set_station_error_limited(station_error_limited);
  debug->set_speed_offset(speed_offset);
  debug->set_speed_controller_input_limited(speed_controller_input_limited);
  debug->set_acceleration_cmd(acceleration_cmd);
  debug->set_throttle_cmd(throttle_cmd);
  debug->set_brake_cmd(brake_cmd);
  debug->set_acceleration_lookup(acceleration_lookup);
  debug->set_acceleration_lookup_limit(acceleration_lookup_limit);
  debug->set_speed_lookup(chassis_->speed_mps());
  debug->set_calibration_value(calibration_value);
  debug->set_acceleration_cmd_closeloop(acceleration_cmd_closeloop);

  if (FLAGS_enable_csv_debug && speed_log_file_ != nullptr) {
    fprintf(speed_log_file_,
            "%.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f,"
            "%.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %d,\r\n",
            debug->station_reference(), debug->station_error(),
            station_error_limited, debug->preview_station_error(),
            debug->speed_reference(), debug->speed_error(),
            speed_controller_input_limited, debug->preview_speed_reference(),
            debug->preview_speed_error(),
            debug->preview_acceleration_reference(), acceleration_cmd_closeloop,
            acceleration_cmd, debug->acceleration_lookup(),
            debug->acceleration_lookup_limit(), debug->speed_lookup(),
            calibration_value, throttle_cmd, brake_cmd, debug->is_full_stop());
  }

  // if the car is driven by acceleration, disgard the cmd->throttle and brake
  cmd->set_throttle(throttle_cmd);
  cmd->set_brake(brake_cmd);
  if (lon_based_pidcontroller_conf_.use_acceleration_lookup_limit()) {
    cmd->set_acceleration(acceleration_lookup_limit);
  } else {
    cmd->set_acceleration(acceleration_cmd);
  }

  if (std::fabs(injector_->vehicle_state()->linear_velocity()) <=
          vehicle_param_.max_abs_speed_when_stopped() ||
      chassis->gear_location() == trajectory_message_->gear() ||
      chassis->gear_location() == canbus::Chassis::GEAR_NEUTRAL) {
    cmd->set_gear_location(trajectory_message_->gear());
  } else {
    cmd->set_gear_location(chassis->gear_location());
  }

  return Status::OK();
}

Status LonController::Reset() {
  speed_pid_controller_.Reset();
  station_pid_controller_.Reset();
  return Status::OK();
}

std::string LonController::Name() const { return name_; }

void LonController::ComputeLongitudinalErrors(
    const TrajectoryAnalyzer *trajectory_analyzer, const double preview_time,
    const double ts, SimpleLongitudinalDebug *debug) {
  // the decomposed vehicle motion onto Frenet frame
  // s: longitudinal accumulated distance along reference trajectory
  // s_dot: longitudinal velocity along reference trajectory
  // d: lateral distance w.r.t. reference trajectory
  // d_dot: lateral distance change rate, i.e. dd/dt
  double s_matched = 0.0;
  double s_dot_matched = 0.0;
  double d_matched = 0.0;
  double d_dot_matched = 0.0;

  auto vehicle_state = injector_->vehicle_state();
  auto matched_point = trajectory_analyzer->QueryMatchedPathPoint(
      vehicle_state->x(), vehicle_state->y());

  trajectory_analyzer->ToTrajectoryFrame(
      vehicle_state->x(), vehicle_state->y(), vehicle_state->heading(),
      vehicle_state->linear_velocity(), matched_point, &s_matched,
      &s_dot_matched, &d_matched, &d_dot_matched);

  // double current_control_time = Time::Now().ToSecond();
  double current_control_time = ::apollo::cyber::Clock::NowInSeconds();
  double preview_control_time = current_control_time + preview_time;

  TrajectoryPoint reference_point =
      trajectory_analyzer->QueryNearestPointByAbsoluteTime(
          current_control_time);
  TrajectoryPoint preview_point =
      trajectory_analyzer->QueryNearestPointByAbsoluteTime(
          preview_control_time);

  debug->mutable_current_matched_point()->mutable_path_point()->set_x(
      matched_point.x());
  debug->mutable_current_matched_point()->mutable_path_point()->set_y(
      matched_point.y());
  debug->mutable_current_reference_point()->mutable_path_point()->set_x(
      reference_point.path_point().x());
  debug->mutable_current_reference_point()->mutable_path_point()->set_y(
      reference_point.path_point().y());
  debug->mutable_preview_reference_point()->mutable_path_point()->set_x(
      preview_point.path_point().x());
  debug->mutable_preview_reference_point()->mutable_path_point()->set_y(
      preview_point.path_point().y());

  ADEBUG << "matched point:" << matched_point.DebugString();
  ADEBUG << "reference point:" << reference_point.DebugString();
  ADEBUG << "preview point:" << preview_point.DebugString();

  double heading_error = common::math::NormalizeAngle(vehicle_state->heading() -
                                                      matched_point.theta());
  double lon_speed = vehicle_state->linear_velocity() * std::cos(heading_error);
  double lon_acceleration =
      vehicle_state->linear_acceleration() * std::cos(heading_error);
  double one_minus_kappa_lat_error = 1 - reference_point.path_point().kappa() *
                                             vehicle_state->linear_velocity() *
                                             std::sin(heading_error);

  debug->set_station_reference(reference_point.path_point().s());
  debug->set_current_station(s_matched);
  debug->set_station_error(reference_point.path_point().s() - s_matched);
  debug->set_speed_reference(reference_point.v());
  debug->set_current_speed(lon_speed);
  debug->set_speed_error(reference_point.v() - s_dot_matched);
  debug->set_acceleration_reference(reference_point.a());
  debug->set_current_acceleration(lon_acceleration);
  debug->set_acceleration_error(reference_point.a() -
                                lon_acceleration / one_minus_kappa_lat_error);
  double jerk_reference =
      (debug->acceleration_reference() - previous_acceleration_reference_) / ts;
  double lon_jerk =
      (debug->current_acceleration() - previous_acceleration_) / ts;
  debug->set_jerk_reference(jerk_reference);
  debug->set_current_jerk(lon_jerk);
  debug->set_jerk_error(jerk_reference - lon_jerk / one_minus_kappa_lat_error);
  previous_acceleration_reference_ = debug->acceleration_reference();
  previous_acceleration_ = debug->current_acceleration();

  debug->set_preview_station_error(preview_point.path_point().s() - s_matched);
  debug->set_preview_speed_error(preview_point.v() - s_dot_matched);
  debug->set_preview_speed_reference(preview_point.v());
  debug->set_preview_acceleration_reference(preview_point.a());
}

void LonController::SetDigitalFilter(double ts, double cutoff_freq,
                                     common::DigitalFilter *digital_filter) {
  std::vector<double> denominators;
  std::vector<double> numerators;
  common::LpfCoefficients(ts, cutoff_freq, &denominators, &numerators);
  digital_filter->set_coefficients(denominators, numerators);
}

// TODO(all): Refactor and simplify
void LonController::GetPathRemain(SimpleLongitudinalDebug *debug) {
  int stop_index = 0;
  static constexpr double kSpeedThreshold = 1e-3;
  static constexpr double kForwardAccThreshold = -1e-2;
  static constexpr double kBackwardAccThreshold = 1e-1;
  static constexpr double kParkingSpeed = 0.1;

  if (trajectory_message_->gear() == canbus::Chassis::GEAR_DRIVE) {
    while (stop_index < trajectory_message_->trajectory_point_size()) {
      auto &current_trajectory_point =
          trajectory_message_->trajectory_point(stop_index);
      if (fabs(current_trajectory_point.v()) < kSpeedThreshold &&
          current_trajectory_point.a() > kForwardAccThreshold &&
          current_trajectory_point.a() < 0.0) {
        break;
      }
      ++stop_index;
    }
  } else {
    while (stop_index < trajectory_message_->trajectory_point_size()) {
      auto &current_trajectory_point =
          trajectory_message_->trajectory_point(stop_index);
      if (current_trajectory_point.v() > -kSpeedThreshold &&
          current_trajectory_point.a() < kBackwardAccThreshold &&
          current_trajectory_point.a() > 0.0) {
        break;
      }
      ++stop_index;
    }
  }
  ADEBUG << "stop_index is, " << stop_index;
  if (stop_index == trajectory_message_->trajectory_point_size()) {
    --stop_index;
    if (fabs(trajectory_message_->trajectory_point(stop_index).v()) <
        kParkingSpeed) {
      ADEBUG << "the last point is selected as parking point";
    } else {
      ADEBUG << "the last point found in path and speed > speed_deadzone";
    }
  }
  debug->set_path_remain(
      trajectory_message_->trajectory_point(stop_index).path_point().s() -
      debug->current_station());
}

bool LonController::IsStopByDestination(SimpleLongitudinalDebug *debug) {
  auto stop_reason = trajectory_message_->decision().main_decision().stop();
  ADEBUG << "Current stop reason is \n" << stop_reason.DebugString();
  if (trajectory_message_->decision().main_decision().has_mission_complete()) {
    ADEBUG << "Current main decision is mission complete: "
           << trajectory_message_->decision()
                  .main_decision()
                  .mission_complete()
                  .DebugString();
  }

  StopReasonCode stop_reason_code = stop_reason.reason_code();

  if (stop_reason_code == StopReasonCode::STOP_REASON_DESTINATION ||
      stop_reason_code == StopReasonCode::STOP_REASON_SIGNAL ||
      stop_reason_code == StopReasonCode::STOP_REASON_REFERENCE_END ||
      trajectory_message_->decision().main_decision().has_mission_complete()) {
    ADEBUG << "[IsStopByDestination]Current stop reason is in destination.";
    debug->set_is_stop_reason_by_destination(true);
    return true;
  }
  debug->set_is_stop_reason_by_destination(false);
  return false;
}

bool LonController::IsPedestrianStopLongTerm(SimpleLongitudinalDebug *debug) {
  auto stop_reason = trajectory_message_->decision().main_decision().stop();
  ADEBUG << "Current stop reason is \n" << stop_reason.DebugString();
  StopReasonCode stop_reason_code = stop_reason.reason_code();

  if (stop_reason_code == StopReasonCode::STOP_REASON_PEDESTRIAN ||
      stop_reason_code == StopReasonCode::STOP_REASON_OBSTACLE) {
    ADEBUG << "[IsPedestrianStopLongTerm]Stop reason for pedestrian.";
    is_stop_by_pedestrian_ = true;
  } else {
    is_stop_by_pedestrian_ = false;
  }

  ADEBUG << "Current is_stop_by_pedestrian: " << is_stop_by_pedestrian_
         << ", is_stop_by_pedestrian_previous: "
         << is_stop_by_pedestrian_previous_;

  if (is_stop_by_pedestrian_) {
    if (!(is_stop_by_pedestrian_ && is_stop_by_pedestrian_previous_)) {
      start_time_ = ::apollo::cyber::Clock::NowInSeconds();
      ADEBUG << "Stop reason for pedestrian, start time(s) is " << start_time_;
    } else {
      ADEBUG
          << "Last time stop is already pedestrian, skip the init start_time.";
    }
    double end_time = ::apollo::cyber::Clock::NowInSeconds();
    ADEBUG << "Stop reason for pedestrian, current time(s) is " << end_time;
    wait_time_diff_ = end_time - start_time_;
  } else {
    start_time_ = 0.0;
    wait_time_diff_ = 0.0;
  }

  is_stop_by_pedestrian_previous_ = is_stop_by_pedestrian_;

  if (wait_time_diff_ > lon_based_pidcontroller_conf_.pedestrian_stop_time()) {
    ADEBUG << "Current pedestrian stop lasting time(s) is " << wait_time_diff_
           << ", larger than threshold: "
           << lon_based_pidcontroller_conf_.pedestrian_stop_time();
    debug->set_is_stop_reason_by_prdestrian(true);
    return true;
  } else {
    ADEBUG << "Current pedestrian stop lasting time(s) is " << wait_time_diff_
           << ", not reach the threshold: "
           << lon_based_pidcontroller_conf_.pedestrian_stop_time();
    debug->set_is_stop_reason_by_prdestrian(false);
    return false;
  }
}

}  // namespace control
}  // namespace apollo
