/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020-2023 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-Cyphal/graphs/contributors.
 */

#ifndef INC_107_ARDUINO_CYPHAL_SERVICE_CLIENT_HPP
#define INC_107_ARDUINO_CYPHAL_SERVICE_CLIENT_HPP

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <memory>

#include "SubscriptionBase.h"

#include "libcanard/canard.h"

/**************************************************************************************
 * FORWARD DECLARATION
 **************************************************************************************/

class Node;

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace impl
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

template <typename T_REQ>
class ServiceClientBase : public SubscriptionBase
{
public:
  ServiceClientBase() : SubscriptionBase{CanardTransferKindResponse} { }
  virtual ~ServiceClientBase() { }
  virtual bool request(CanardNodeID const remote_node_id, T_REQ const & req) = 0;
};

template<typename T_REQ, typename T_RSP, typename OnResponseCb>
class ServiceClient : public ServiceClientBase<T_REQ>
{
public:
  ServiceClient(Node & node_hdl, CanardPortID const port_id, CanardMicrosecond const tx_timeout_usec, OnResponseCb on_response_cb)
  : _node_hdl{node_hdl}
  , _port_id{port_id}
  , _tx_timeout_usec{tx_timeout_usec}
  , _on_response_cb{on_response_cb}
  , _transfer_id{0}
  { }
  virtual ~ServiceClient();


  virtual bool request(CanardNodeID const remote_node_id, T_REQ const & req) override;
  virtual bool onTransferReceived(CanardRxTransfer const & transfer) override;


private:
  Node & _node_hdl;
  CanardPortID const _port_id;
  CanardMicrosecond const _tx_timeout_usec;
  OnResponseCb _on_response_cb;
  CanardTransferID _transfer_id;
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* impl */

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

template <typename T_REQ>
using ServiceClient = std::shared_ptr<impl::ServiceClientBase<T_REQ>>;

/**************************************************************************************
 * TEMPLATE IMPLEMENTATION
 **************************************************************************************/

#include "ServiceClient.ipp"

#endif /* INC_107_ARDUINO_CYPHAL_SERVICE_CLIENT_HPP */
