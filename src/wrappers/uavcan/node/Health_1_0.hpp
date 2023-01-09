/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020-2023 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-Cyphal/graphs/contributors.
 */

#ifndef ARDUINO_TRANSFER_UAVCAN_NODE_HEALTH_1_0_HPP_
#define ARDUINO_TRANSFER_UAVCAN_NODE_HEALTH_1_0_HPP_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <libcanard/canard.h>

#include <types/uavcan/node/Health_1_0.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace uavcan {
namespace node {

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

template <CanardPortID ID>
class Health_1_0
{

public:

  uavcan_node_Health_1_0 data;

  static constexpr CanardPortID       PORT_ID = ID;
  static constexpr size_t             MAX_PAYLOAD_SIZE = uavcan_node_Health_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_;
  static constexpr CanardTransferKind TRANSFER_KIND = CanardTransferKindMessage;

  Health_1_0()
  {
    uavcan_node_Health_1_0_initialize_(&data);
  }

  Health_1_0(Health_1_0 const & other)
  {
    memcpy(&data, &other.data, sizeof(data));
  }

  static Health_1_0 deserialize(CanardRxTransfer const & transfer)
  {
    Health_1_0 h;
    size_t inout_buffer_size_bytes = transfer.payload_size;
    uavcan_node_Health_1_0_deserialize_(&h.data, (uint8_t *)(transfer.payload), &inout_buffer_size_bytes);
    return h;
  }

  size_t serialize(uint8_t * payload) const
  {
    size_t inout_buffer_size_bytes = Health_1_0::MAX_PAYLOAD_SIZE;
    return (uavcan_node_Health_1_0_serialize_(&data, payload, &inout_buffer_size_bytes) < NUNAVUT_SUCCESS) ? 0 : inout_buffer_size_bytes;
  }
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* uavcan */
} /* node */

#endif /* ARDUINO_TRANSFER_UAVCAN_NODE_HEALTH_1_0_HPP_ */
