// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from openni2_camera:srv/GetSerial.idl
// generated code does not contain a copyright notice

#ifndef OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__TRAITS_HPP_
#define OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "openni2_camera/srv/detail/get_serial__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace openni2_camera
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetSerial_Request & msg,
  std::ostream & out)
{
  (void)msg;
  out << "null";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetSerial_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  (void)msg;
  (void)indentation;
  out << "null\n";
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetSerial_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace openni2_camera

namespace rosidl_generator_traits
{

[[deprecated("use openni2_camera::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const openni2_camera::srv::GetSerial_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  openni2_camera::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use openni2_camera::srv::to_yaml() instead")]]
inline std::string to_yaml(const openni2_camera::srv::GetSerial_Request & msg)
{
  return openni2_camera::srv::to_yaml(msg);
}

template<>
inline const char * data_type<openni2_camera::srv::GetSerial_Request>()
{
  return "openni2_camera::srv::GetSerial_Request";
}

template<>
inline const char * name<openni2_camera::srv::GetSerial_Request>()
{
  return "openni2_camera/srv/GetSerial_Request";
}

template<>
struct has_fixed_size<openni2_camera::srv::GetSerial_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<openni2_camera::srv::GetSerial_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<openni2_camera::srv::GetSerial_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace openni2_camera
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetSerial_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: serial
  {
    out << "serial: ";
    rosidl_generator_traits::value_to_yaml(msg.serial, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetSerial_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: serial
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "serial: ";
    rosidl_generator_traits::value_to_yaml(msg.serial, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetSerial_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace openni2_camera

namespace rosidl_generator_traits
{

[[deprecated("use openni2_camera::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const openni2_camera::srv::GetSerial_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  openni2_camera::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use openni2_camera::srv::to_yaml() instead")]]
inline std::string to_yaml(const openni2_camera::srv::GetSerial_Response & msg)
{
  return openni2_camera::srv::to_yaml(msg);
}

template<>
inline const char * data_type<openni2_camera::srv::GetSerial_Response>()
{
  return "openni2_camera::srv::GetSerial_Response";
}

template<>
inline const char * name<openni2_camera::srv::GetSerial_Response>()
{
  return "openni2_camera/srv/GetSerial_Response";
}

template<>
struct has_fixed_size<openni2_camera::srv::GetSerial_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<openni2_camera::srv::GetSerial_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<openni2_camera::srv::GetSerial_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<openni2_camera::srv::GetSerial>()
{
  return "openni2_camera::srv::GetSerial";
}

template<>
inline const char * name<openni2_camera::srv::GetSerial>()
{
  return "openni2_camera/srv/GetSerial";
}

template<>
struct has_fixed_size<openni2_camera::srv::GetSerial>
  : std::integral_constant<
    bool,
    has_fixed_size<openni2_camera::srv::GetSerial_Request>::value &&
    has_fixed_size<openni2_camera::srv::GetSerial_Response>::value
  >
{
};

template<>
struct has_bounded_size<openni2_camera::srv::GetSerial>
  : std::integral_constant<
    bool,
    has_bounded_size<openni2_camera::srv::GetSerial_Request>::value &&
    has_bounded_size<openni2_camera::srv::GetSerial_Response>::value
  >
{
};

template<>
struct is_service<openni2_camera::srv::GetSerial>
  : std::true_type
{
};

template<>
struct is_service_request<openni2_camera::srv::GetSerial_Request>
  : std::true_type
{
};

template<>
struct is_service_response<openni2_camera::srv::GetSerial_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__TRAITS_HPP_
