/******************************************************************************
 * Copyright 2024 fortiss GmbH
 * Authors: Tobias Kessler, Klemens Esterle, Xiangzhong Liu
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

/**
 * @file
 **/

#pragma once

#include <string>
#include "cyber/plugin_manager/plugin_manager.h"
#include "modules/common/status/status.h"
#include "modules/planning/planning_base/common/frame.h"
#include "modules/planning/planning_base/common/reference_line_info.h"
#include "modules/planning/planning_interface_base/planner_base/planner.h"
#include "modules/planning/planners/lattice/lattice_planner.h"
#include "modules/planning/planners/bark_rl_wrapper/proto/planner_config.pb.h"

namespace apollo {
namespace planning {

/**
 * @class ReferenceTrackingPlanner
 * @note LatticePlanner class from apollo served as a reference implementation!
 **/
class ReferenceTrackingPlanner : public LatticePlanner {
 public:
  virtual ~ReferenceTrackingPlanner() = default;

  std::string Name() override { return "REFERENCE_TRACKING"; }

  common::Status Init(const std::shared_ptr<DependencyInjector>& injector,
                      const std::string& config_path = "") override;

  void Stop() override;

  /**
   * @brief Override function Plan in parent class Planner.
   * @param planning_init_point The trajectory point where planning starts.
   * @param frame Current planning frame.
   * @param reference_line_info The computed reference line.
   * @return OK if planning succeeds; error otherwise.
   */
  common::Status PlanOnReferenceLine(
      const common::TrajectoryPoint& planning_init_point, Frame* frame,
      ReferenceLineInfo* reference_line_info) override;

 private:
  double minimum_valid_speed_planning_;
  double standstill_velocity_threshold_;
  std::string logdir_;
  BarkRlPlannerConfiguration config_;
};
CYBER_PLUGIN_MANAGER_REGISTER_PLUGIN(apollo::planning::ReferenceTrackingPlanner, Planner)

}  // namespace planning
}  // namespace apollo
