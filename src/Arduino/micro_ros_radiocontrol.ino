//ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyUSB0
//ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888　ROS2で通信
//ros2 run joy joy_node
//ros2 run joy2twist joy2twist

#include <Arduino.h>
#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>
#include <cstdlib>

#if !defined(ESP32) && !defined(TARGET_PORTENTA_H7_M7) && !defined(ARDUINO_GIGA) && !defined(ARDUINO_NANO_RP2040_CONNECT) && !defined(ARDUINO_WIO_TERMINAL) && !defined(ARDUINO_UNOR4_WIFI) && !defined(ARDUINO_OPTA)
#error This example is only available for Arduino Portenta, Arduino Giga R1, Arduino Nano RP2040 Connect, ESP32 Dev module, Wio Terminal, Arduino Uno R4 WiFi and Arduino OPTA WiFi 
#endif

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

rcl_publisher_t publisher;
rcl_subscription_t subscriber;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
geometry_msgs__msg__Twist pub_msg;
geometry_msgs__msg__Twist sub_msg;

#define LED_PIN 21
#define MTa1_PIN 12//出力A1
#define MTa2_PIN 14//出力A2
#define MTb1_PIN 12//出力B1
#define MTb2_PIN 13//出力B2
#define MAX_PWM 255//モータのPWM制御の最大値
#define MIN_PWM 100//モータのPWM制御の最小値

//char* ssid = "pr500m-266a83-1";
//char* wifi_passwd = "331f0b043babb";
//char* IP_address = "192.168.1.10";
//char* ssid = "TP-Link_FBCA";
//char* wifi_passwd = "0918235610";
//char* IP_address = "10.10.129.125";
char *ssid = "elecom2g-bcff17";
char *wifi_passwd = "i7j7yaj35jui";
char *IP_address = "10.10.128.105";
unsigned int port = 8888;

void error_loop() {
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

  if(msg->linear.x >= 0.0) {
    analogWrite(MTa1_PIN, 0); //前進
    analogWrite(MTa2_PIN, abs(msg->linear.x) * MAX_PWM);
  }else {
    analogWrite(MTa1_PIN, abs(msg->linear.x) * MAX_PWM);
    analogWrite(MTa2_PIN, 0); //後進
  }

  if(msg->linear.x >= 0.0) {
    analogWrite(MTb1_PIN, 0); //ハンドル右
    analogWrite(MTb2_PIN, abs(msg->linear.x) * MIN_PWM);
  }else {
    analogWrite(MTb1_PIN, abs(msg->linear.x) * MIN_PWM);
    analogWrite(MTb2_PIN, 0); //ハンドル左
  }
}

void setup() {
  set_microros_transports();
  //set_microros_wifi_transports(ssid, wifi_passwd, IP_address, port);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  // create node
  RCCHECK(rclc_node_init_default(&node, "radio_control_node", "", &support));
  // create publisher
  RCCHECK(rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "radio_control_pub"));
  // create subscriber
  RCCHECK(rclc_subscription_init_default(&subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "cmd_vel"));
  // create executor
  int callback_size = 1;
  RCCHECK(rclc_executor_init(&executor, &support.context, callback_size, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &sub_msg, &callback, ON_NEW_DATA));

  pinMode(MTa1_PIN, OUTPUT);
  pinMode(MTa2_PIN, OUTPUT);
  pinMode(MTb1_PIN, OUTPUT);
  pinMode(MTb2_PIN, OUTPUT);
  sub_msg.linear.x = 0.0;
  sub_msg.linear.y = 0.0;
}

void loop() {
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  delay(10);
}