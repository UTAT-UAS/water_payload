#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <ESP32Servo.h>
#include "actuators/pump_interface.hpp"
#include "actuators/pos_controller.hpp"

#include "main.hpp"

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

const int DEBUG_LED = 2;

rcl_subscription_t pump_subscriber;
std_msgs__msg__Int32 pump_msg;
rcl_subscription_t servo_subscriber;
std_msgs__msg__Int32 servo_msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){ \
  digitalWrite(DEBUG_LED, HIGH); \
  error_loop(); \
}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() {
  while(1) {
    delay(100);
  }
}

void pump_callback(const void *msgin) {
  const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *) msgin;
  actuators::pump::writeToPump(msg->data);
}

PosController servo(37, 4, 500, 2400); // pin, channel, lbound, ubound should only respond to 546-2383
// channels 0-3 (pump) use same hardware clock, 4-7 on another?

void servo_callback(const void *msgin) {
  const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *) msgin;
  int us = msg->data;
  
  servo.setUs(us);
}

void setup() {
  Serial.begin(115200);

  // Create queue and spawn servoTask
  servo.setup(1465);
  BaseType_t t = xTaskCreate(PosController::vMonitorTask, "servoTask", 4096, &servo, 1, NULL);

  // Microros setup
  delay(500);
  set_microros_serial_transports(Serial);
  delay(2000);
  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "micro_ros_water_payload_node", "", &support));

  // Create publishers
  RCCHECK(rclc_subscription_init_default(
    &pump_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "set_pump_throttle"));
  RCCHECK(rclc_subscription_init_default(
    &servo_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "set_servo_us"));

  // Create executors
  RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &pump_subscriber, &pump_msg, &pump_callback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &servo_subscriber, &servo_msg, &servo_callback, ON_NEW_DATA));

  pump_msg.data = 0;
  servo_msg.data = center_us;

  actuators::pump::initPump();
}

void loop() {
  delay(10);
  RCSOFTCHECK(rclc_executor_spin(&executor));
}