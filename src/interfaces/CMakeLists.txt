cmake_minimum_required(VERSION 3.8)
project(interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(std_msgs REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(geometry_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/BrainStatus.msg"
  "msg/Camera.msg"
  "msg/InventoryStatus.msg"
  "srv/ArmMovement.srv"
  "srv/Command.srv"
  "srv/Inventory.srv"
  "srv/HybridCmd.srv"
  DEPENDENCIES std_msgs geometry_msgs
)


ament_package()
