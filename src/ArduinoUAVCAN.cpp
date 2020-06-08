/**
 * @brief   Arduino library for providing a convenient C++ interface for accessing UAVCAN.
 * @license MIT
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "ArduinoUAVCAN.h"

#include <assert.h>

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

ArduinoUAVCAN::ArduinoUAVCAN(uint8_t const node_id,
                             MicroSecondFunc micros)
: _canard_ins{canardInit(ArduinoUAVCAN::o1heap_allocate, ArduinoUAVCAN::o1heap_free)}
, _micros{micros}
{
  assert(_micros != nullptr);

  _canard_ins.node_id = node_id;
  _canard_ins.mtu_bytes = CANARD_MTU_CAN_CLASSIC;
  _canard_ins.user_reference = reinterpret_cast<void *>(&_o1heap);
}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoUAVCAN::onCanFrameReceived(uint32_t const id, uint8_t const * data, uint8_t const len)
{
  CanardFrame frame;
  convertToCanardFrame(_micros(), id, data, len, frame);

  CanardTransfer transfer;
  int8_t const result = canardRxAccept(&_canard_ins,
                                       &frame,
                                       0,
                                       &transfer);

  if(result == 1)
  {
    if (_rx_sub_map.count(transfer.port_id) > 0)
    {
      _rx_sub_map[transfer.port_id].transfer_complete_callback(transfer);
    }
    _o1heap.free(const_cast<void *>(transfer.payload));
  }
}

bool ArduinoUAVCAN::subscribe(CanardPortID const port_id, size_t const payload_size_max, std::function<void(CanardTransfer const &)> func)
{
  if (_rx_sub_map.count(port_id) > 0)
    return false;

  _rx_sub_map[port_id].transfer_complete_callback = func;

  if (!subscribeMessage(port_id, payload_size_max, &(_rx_sub_map[port_id].canard_rx_sub)))
    return false;

  return true;
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

void * ArduinoUAVCAN::o1heap_allocate(CanardInstance * const ins, size_t const amount)
{
  ArduinoO1Heap * o1heap = reinterpret_cast<ArduinoO1Heap *>(ins->user_reference);
  return o1heap->allocate(amount);
}

void ArduinoUAVCAN::o1heap_free(CanardInstance * const ins, void * const pointer)
{
  ArduinoO1Heap * o1heap = reinterpret_cast<ArduinoO1Heap *>(ins->user_reference);
  o1heap->free(pointer);
}

void ArduinoUAVCAN::convertToCanardFrame(unsigned long const rx_timestamp_us, uint32_t const id, uint8_t const * data, uint8_t const len, CanardFrame & frame)
{
  /* Get the reception timestamp */
  frame.timestamp_usec = rx_timestamp_us;
  /* Blank the 3 MSBits */
  frame.extended_can_id = id & 0x1FFFFFFF;
  /* Set the length */
  frame.payload_size = static_cast<size_t>(len);
  /* Set pointer to data */
  frame.payload = reinterpret_cast<const void *>(data);
}

bool ArduinoUAVCAN::subscribeMessage(CanardPortID const port_id, size_t const payload_size_max, CanardRxSubscription * canard_rx_sub)
{
  int8_t const result = canardRxSubscribe(&_canard_ins,
                                          CanardTransferKindMessage,
                                          port_id,
                                          payload_size_max,
                                          CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC,
                                          canard_rx_sub);
  bool const success = (result >= 0);
  return success;
}

bool ArduinoUAVCAN::unsubscribeMessage(CanardPortID const port_id)
{
  int8_t const result = canardRxUnsubscribe(&_canard_ins,
                                            CanardTransferKindMessage,
                                            port_id);

  /* Remove CanardRxSubscription instance from internal list since the
   * structure is no longed needed.
   */
  _rx_sub_map.erase(port_id);

  bool const success = (result >= 0);
  return success;
}