//ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyUSB0
//ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888　ROS2で通信
//ros2 topic pub --once /micro_ros_wifi_sub std_msgs/msg/Int32 'data: 5'


#include <micro_ros_arduino.h>
#include <Arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>

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
std_msgs__msg__Int32 pub_msg;
std_msgs__msg__Int32 sub_msg;

#define LED_PIN 21

char *ssid = " ";
char *wifi_passwd = " ";
char *IP_address = " ";
unsigned int port = 8888;


void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

void callback(const void *msgin) {
  const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *)msgin;
  //デバッグ用    
  pub_msg.data = msg->data;
  RCSOFTCHECK(rcl_publish(&publisher, &pub_msg, NULL));
  
  int i = 1;
  while(msg->data >= i) {
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    i++;
  }
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
  RCCHECK(rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "micro_ros_wifi_pub"));
  //create subscriber
  RCCHECK(rclc_subscription_init_default(&subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "micro_ros_wifi_sub"));

  // create executor
  int callback_size = 1;
  RCCHECK(rclc_executor_init(&executor, &support.context, callback_size, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &sub_msg, &callback, ON_NEW_DATA));
}

void loop() {
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  delay(10);
}
