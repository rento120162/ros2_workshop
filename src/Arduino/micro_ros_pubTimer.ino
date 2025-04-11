//ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888　ROS2で通信
//ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyUSB0

#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/string.h>

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
std_msgs__msg__Int32 int32_msg;
std_msgs__msg__String string_msg;


#define LED_PIN 21

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

char* ssid = "pr500m-266a83-1";
char* wifi_passwd = "331f0b043babb";
char* IP_address = "192.168.1.10";
//char* ssid = "TP-Link_FBCA";
//char* wifi_passwd = "0918235610";
//char* IP_address = "10.10.129.125";
unsigned int port_num = 8888;

void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

void timer_callback(rcl_timer_t * timer, int64_t last_call_time){
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    String s ="Timer : ";
    s += int32_msgs.data;  //ｓに追加するときは絶対に分けて書く
    char strBuf[120]; s.toCharArray(strBuf, 120);
    string_msg.data.size = s.length();
    string_msg.data.data = strBuf;
    RCSOFTCHECK(rcl_publish(&publisher, &string_msg, NULL));
    int32_msg.data++;
  }
}


void setup() {
  set_microros_transports();
  //set_microros_wifi_transports(ssid, wifi_passwd, IP_address, port_num);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));
  // create publisher
  RCCHECK(rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "micro_ros_wifi_pub"));

  // create timer,
  const unsigned int timer_timeout = 1000;
  RCCHECK(rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(timer_timeout), timer_callback));

  // create executor
  int callback_size = 1;
  RCCHECK(rclc_executor_init(&executor, &support.context, callback_size, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));

  int32_msg.data = 0;
}

void loop() {
  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}