/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-Cyphal/graphs/contributors.
 */

#ifndef REGISTER_READ_ONLY_H_
#define REGISTER_READ_ONLY_H_

/**************************************************************************************
 * INCLUDES
 **************************************************************************************/

#include "RegisterBase.h"

#include "util/register_util.hpp"

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

template <typename T>
class Register : public RegisterBase
{
public:
  typedef std::function<void(Register<T> const &)> OnRegisterValueChangeFunc;

  enum class Access { ReadWrite, ReadOnly };

  Register(char const * name,
           Access const access,
           T const & initial_val,
           OnRegisterValueChangeFunc func)
  : RegisterBase{name, toTypeTag(initial_val)}
  , _access{access}
  , _val{initial_val}
  , _func{func}
  { }

  T get() const { return _val; }
  void set(uavcan_register_Value_1_0 const & val)
  {
    if (_access == Access::ReadOnly)
      return;
    
    _val = fromRegisterValue<T>(val);
    if (_func)
      _func(*this);
  }

  uavcan::_register::Access_1_0::Response<> toAccessResponse()
  {
    uavcan::_register::Access_1_0::Response<> rsp;

    rsp.data.value = toRegisterValue(_val);
    rsp.data.timestamp.microsecond = micros();
    rsp.data._mutable = (_access == Access::ReadOnly) ? false : true;
    rsp.data.persistent = false;

    return rsp;
  }

private:
  T _val;
  Access const _access;
  OnRegisterValueChangeFunc _func;
};

#endif /* REGISTER_READ_ONLY_H_ */
