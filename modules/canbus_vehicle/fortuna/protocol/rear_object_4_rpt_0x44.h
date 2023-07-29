/******************************************************************************
 * Copyright 2020 fortiss GmbH
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

#pragma once

#include "modules/common_msgs/chassis_msgs/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace fortuna {

class RearObject4rpt0x44 : public ::apollo::drivers::canbus::ProtocolData<
                           ::apollo::canbus::ChassisDetail> {
 public:
  static const int32_t ID;
  RearObject4rpt0x44();
  void Parse(const std::uint8_t* bytes, int32_t length,
             ChassisDetail* chassis) const override;

 private:
  // config detail: {'description': 'checksum', 'offset': 0.0,
  // 'precision': 1.0, 'len': 8, 'name': 'CRC',
  // 'is_signed_var': False, 'physical_range': '[0|255]', 'bit': 0, 'type':
  // 'int', 'order': 'intel', 'physical_unit': ''}
  int checksum(const std::uint8_t* bytes, int32_t length) const;

  // config detail: {'name': 'RearTracked', 'offset': 0.0,
  // 'precision': 1.0, 'len': 1, 'is_signed_var': False, 'physical_range':
  // '[0|1]', 'bit': 12, 'type': 'bool', 'order': 'intel', 'physical_unit': ''}
  // Tracking state of the rear object.
  bool rear_tracked(const std::uint8_t* bytes, const int32_t length) const;

  // config detail: {'name': 'RearID', 'offset': 0.0,
  // 'precision': 1.0, 'len': 5, 'is_signed_var': False, 'physical_range':
  // '[0|30]', 'bit': 13, 'type': 'int', 'order': 'intel', 'physical_unit':
  // ''}
  // ID of RearObject
  int32_t rear_id(const std::uint8_t* bytes, const int32_t length) const;
  
  // config detail: {'name': 'RearPosY', 'offset': -16.0, 'precision': 0.125,
  // 'len': 8, 'is_signed_var': False, 'physical_range': '[0|0]', 'bit': 18,
  // 'type': 'double', 'order': 'intel', 'physical_unit': ''}
  // Position of the nearest rear object (closests point) on left lane in 
  // direction of Y relative to the longitudinal axis of the ego vehicle.
  double rear_pos_y(const std::uint8_t* bytes, const int32_t length) const;

  // config detail: {'name': 'RearRelVelocityX', 'offset': -37.5,
  // 'precision': 0.25, 'len': 9, 'is_signed_var': False, 'physical_range':
  // '[0|0]', 'bit': 26, 'type': 'double', 'order': 'intel', 'physical_unit':
  // ''}
  // Relative velocity of the nearest rear object on left lane in direction of X.
  double rear_rel_velocity_x(const std::uint8_t* bytes, const int32_t length) const;

  // config detail: {'name': 'RearPosX', 'offset': -102.5, 'precision': 0.25,
  // 'len': 9, 'is_signed_var': False, 'physical_range': '[0|0]', 'bit': 35,
  // 'type': 'double', 'order': 'intel', 'physical_unit': ''}
  // Position of the nearest rear object (closests point) on left lane in 
  // direction of X relative to the rear axle of the ego vehicle.
  double rear_pos_x(const std::uint8_t* bytes, const int32_t length) const;

};

}  // namespace fortuna
}  // namespace canbus
}  // namespace apollo
