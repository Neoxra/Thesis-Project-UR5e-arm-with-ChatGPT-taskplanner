// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from openni2_camera:srv/GetSerial.idl
// generated code does not contain a copyright notice

#ifndef OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__STRUCT_HPP_
#define OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__openni2_camera__srv__GetSerial_Request __attribute__((deprecated))
#else
# define DEPRECATED__openni2_camera__srv__GetSerial_Request __declspec(deprecated)
#endif

namespace openni2_camera
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetSerial_Request_
{
  using Type = GetSerial_Request_<ContainerAllocator>;

  explicit GetSerial_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  explicit GetSerial_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  // field types and members
  using _structure_needs_at_least_one_member_type =
    uint8_t;
  _structure_needs_at_least_one_member_type structure_needs_at_least_one_member;


  // constant declarations

  // pointer types
  using RawPtr =
    openni2_camera::srv::GetSerial_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const openni2_camera::srv::GetSerial_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<openni2_camera::srv::GetSerial_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<openni2_camera::srv::GetSerial_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      openni2_camera::srv::GetSerial_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<openni2_camera::srv::GetSerial_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      openni2_camera::srv::GetSerial_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<openni2_camera::srv::GetSerial_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<openni2_camera::srv::GetSerial_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<openni2_camera::srv::GetSerial_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__openni2_camera__srv__GetSerial_Request
    std::shared_ptr<openni2_camera::srv::GetSerial_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__openni2_camera__srv__GetSerial_Request
    std::shared_ptr<openni2_camera::srv::GetSerial_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetSerial_Request_ & other) const
  {
    if (this->structure_needs_at_least_one_member != other.structure_needs_at_least_one_member) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetSerial_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetSerial_Request_

// alias to use template instance with default allocator
using GetSerial_Request =
  openni2_camera::srv::GetSerial_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace openni2_camera


#ifndef _WIN32
# define DEPRECATED__openni2_camera__srv__GetSerial_Response __attribute__((deprecated))
#else
# define DEPRECATED__openni2_camera__srv__GetSerial_Response __declspec(deprecated)
#endif

namespace openni2_camera
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetSerial_Response_
{
  using Type = GetSerial_Response_<ContainerAllocator>;

  explicit GetSerial_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->serial = "";
    }
  }

  explicit GetSerial_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : serial(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->serial = "";
    }
  }

  // field types and members
  using _serial_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _serial_type serial;

  // setters for named parameter idiom
  Type & set__serial(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->serial = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    openni2_camera::srv::GetSerial_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const openni2_camera::srv::GetSerial_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<openni2_camera::srv::GetSerial_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<openni2_camera::srv::GetSerial_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      openni2_camera::srv::GetSerial_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<openni2_camera::srv::GetSerial_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      openni2_camera::srv::GetSerial_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<openni2_camera::srv::GetSerial_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<openni2_camera::srv::GetSerial_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<openni2_camera::srv::GetSerial_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__openni2_camera__srv__GetSerial_Response
    std::shared_ptr<openni2_camera::srv::GetSerial_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__openni2_camera__srv__GetSerial_Response
    std::shared_ptr<openni2_camera::srv::GetSerial_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetSerial_Response_ & other) const
  {
    if (this->serial != other.serial) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetSerial_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetSerial_Response_

// alias to use template instance with default allocator
using GetSerial_Response =
  openni2_camera::srv::GetSerial_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace openni2_camera

namespace openni2_camera
{

namespace srv
{

struct GetSerial
{
  using Request = openni2_camera::srv::GetSerial_Request;
  using Response = openni2_camera::srv::GetSerial_Response;
};

}  // namespace srv

}  // namespace openni2_camera

#endif  // OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__STRUCT_HPP_
