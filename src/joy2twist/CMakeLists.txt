cmake_minimum_required(VERSION 3.8)
project(joy2twist)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

add_executable(joy2twist src/joy2twist_node.cpp)
ament_target_dependencies(joy2twist rclcpp std_msgs sensor_msgs geometry_msgs)

install(TARGETS joy2twist
  DESTINATION lib/${PROJECT_NAME})


ament_package()
