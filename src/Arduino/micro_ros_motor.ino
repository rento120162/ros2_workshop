//ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyUSB0
//ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888　ROS2で通信
//ros2 run joy joy_node
//ros2 run joy2twist joy2twist

#include <micro_ros_arduino.h>
#include <Arduino.h>
#include "BasicStepperDriver.h"


#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/string.h>
#include <geometry_msgs/msg/twist.h>

#if !defined(ESP32) && !defined(TARGET_PORTENTA_H7_M7) && !defined(ARDUINO_GIGA) && !defined(ARDUINO_NANO_RP2040_CONNECT) && !defined(ARDUINO_WIO_TERMINAL) && !defined(ARDUINO_UNOR4_WIFI) && !defined(ARDUINO_OPTA)
#error This example is only available for Arduino Portenta, Arduino Giga R1, Arduino Nano RP2040 Connect, ESP32 Dev module, Wio Terminal, Arduino Uno R4 WiFi and Arduino OPTA WiFi 
#endif

rcl_publisher_t publisher;
rcl_subscription_t subscriber;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;
geometry_msgs__msg__Twist pub_msg;
geometry_msgs__msg__Twist sub_msg;

#define STR_SIZE (100) //最大の受信文字数

#define LED_PIN 21

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

//char *ssid = "pr500m-266a83-1";
//char *wifi_passwd = "331f0b043babb";
//char *IP_adress = "192.168.1.10";
//char *ssid = "TP-Link_FBCA";
//char *wifi_passwd = "0918235610";
//char *IP_address = "10.10.129.125";
char *ssid = "elecom2g-bcff17";
char *wifi_passwd = "i7j7yaj35jui";
char *IP_address = "10.10.128.105";
unsigned int port = 8888;

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120
#define MICROSTEPS 1
#define DIR_PIN 12
#define STEP_PIN 14

BasicStepperDriver stepper(MOTOR_STEPS, DIR_PIN, STEP_PIN);

void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

void callback(const void *msgin) {
  const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;
  //デバッグ用
  pub_msg.linear.x = msg->linear.x;
  pub_msg.linear.y = msg->linear.y;
  RCSOFTCHECK(rcl_publish(&publisher, &pub_msg, NULL));

  //using angle to move motor
  stepper.rotate(msg->linear.x * 15.0);
  //using steps to move motor 
  //stepper.move(-MOTOR_STEPS*MICROSTEPS);
}

void setup() {
  set_microros_transports();
  //set_microros_wifi_transports(ssid, wifi_passwd, IP_adress, port);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  
  
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));

  // create publisher
  RCCHECK(rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "micro_ros_wifi_pub"));

  //create subscriber
  RCCHECK(rclc_subscription_init_default(&subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "cmd_vel"));

  // create executor
  int callback_size = 1;
  RCCHECK(rclc_executor_init(&executor, &support.context, callback_size, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &sub_msg, &callback, ON_NEW_DATA));

  stepper.begin(RPM, MICROSTEPS);
}

void loop() {
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  delay(10);

}
