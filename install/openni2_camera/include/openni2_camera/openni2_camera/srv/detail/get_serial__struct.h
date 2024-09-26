// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from openni2_camera:srv/GetSerial.idl
// generated code does not contain a copyright notice

#ifndef OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__STRUCT_H_
#define OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/GetSerial in the package openni2_camera.
typedef struct openni2_camera__srv__GetSerial_Request
{
  uint8_t structure_needs_at_least_one_member;
} openni2_camera__srv__GetSerial_Request;

// Struct for a sequence of openni2_camera__srv__GetSerial_Request.
typedef struct openni2_camera__srv__GetSerial_Request__Sequence
{
  openni2_camera__srv__GetSerial_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} openni2_camera__srv__GetSerial_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'serial'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/GetSerial in the package openni2_camera.
typedef struct openni2_camera__srv__GetSerial_Response
{
  rosidl_runtime_c__String serial;
} openni2_camera__srv__GetSerial_Response;

// Struct for a sequence of openni2_camera__srv__GetSerial_Response.
typedef struct openni2_camera__srv__GetSerial_Response__Sequence
{
  openni2_camera__srv__GetSerial_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} openni2_camera__srv__GetSerial_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OPENNI2_CAMERA__SRV__DETAIL__GET_SERIAL__STRUCT_H_
