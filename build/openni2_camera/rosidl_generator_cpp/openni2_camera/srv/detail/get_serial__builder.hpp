// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from openni2_camera:srv/GetSerial.idl
// generated code does not contain a copyright notice

#ifndef OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__BUILDER_HPP_
#define OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "openni2_camera/srv/detail/get_serial__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace openni2_camera
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::openni2_camera::srv::GetSerial_Request>()
{
  return ::openni2_camera::srv::GetSerial_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace openni2_camera


namespace openni2_camera
{

namespace srv
{

namespace builder
{

class Init_GetSerial_Response_serial
{
public:
  Init_GetSerial_Response_serial()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::openni2_camera::srv::GetSerial_Response serial(::openni2_camera::srv::GetSerial_Response::_serial_type arg)
  {
    msg_.serial = std::move(arg);
    return std::move(msg_);
  }

private:
  ::openni2_camera::srv::GetSerial_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::openni2_camera::srv::GetSerial_Response>()
{
  return openni2_camera::srv::builder::Init_GetSerial_Response_serial();
}

}  // namespace openni2_camera

#endif  // OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__BUILDER_HPP_
