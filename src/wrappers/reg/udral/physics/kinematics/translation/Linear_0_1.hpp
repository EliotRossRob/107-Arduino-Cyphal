/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-Cyphal/graphs/contributors.
 */

#ifndef ARDUINO_UAVCAN_TYPES_REG_DRONE_KINEMATICS_TRANSLATION_LINEAR_0_1_HPP_
#define ARDUINO_UAVCAN_TYPES_REG_DRONE_KINEMATICS_TRANSLATION_LINEAR_0_1_HPP_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <libcanard/canard.h>

#include <types/reg/udral/physics/kinematics/translation/Linear_0_1.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace reg {
namespace udral {
namespace physics {
namespace kinematics {
namespace translation {

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

template <CanardPortID ID>
class Linear_0_1
{

public:

  reg_udral_physics_kinematics_translation_Linear_0_1 data;

  static constexpr CanardPortID       PORT_ID          = ID;
  static constexpr size_t             MAX_PAYLOAD_SIZE = reg_udral_physics_kinematics_translation_Linear_0_1_SERIALIZATION_BUFFER_SIZE_BYTES_;
  static constexpr CanardTransferKind TRANSFER_KIND    = CanardTransferKindMessage;


  Linear_0_1()
  {
    reg_udral_physics_kinematics_translation_Linear_0_1_initialize_(&data);
  }

  Linear_0_1(Linear_0_1 const & other)
  {
    memcpy(&data, &other.data, sizeof(data));
  }

  static Linear_0_1 deserialize(CanardTransfer const & transfer)
  {
    Linear_0_1 b;
    size_t inout_buffer_size_bytes = transfer.payload_size;
    reg_udral_physics_kinematics_translation_Linear_0_1_deserialize_(&b.data, (uint8_t *)(transfer.payload), &inout_buffer_size_bytes);
    return b;
  }

  size_t serialize(uint8_t * payload) const
  {
    size_t inout_buffer_size_bytes = Linear_0_1::MAX_PAYLOAD_SIZE;
    return (reg_udral_physics_kinematics_translation_Linear_0_1_serialize_(&data, payload, &inout_buffer_size_bytes) < NUNAVUT_SUCCESS) ? 0 : inout_buffer_size_bytes;
  }
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* translation */
} /* kinematics */
} /* physics */
} /* udral */
} /* reg */

#endif /* ARDUINO_UAVCAN_TYPES_REG_DRONE_KINEMATICS_TRANSLATION_LINEAR_0_1_HPP_ */