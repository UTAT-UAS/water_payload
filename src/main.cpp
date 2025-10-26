#include "main.hpp"
#include "pos_controller.hpp"
#include "vel_controller.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>

PosController pcontroller = PosController(pin, lbound, ubound);
//VelController vcontroller = VelController(pin, lbound, ubound);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pcontroller.setup(center_us);
  //vcontroller.setup(center_us);
  //xTaskCreate(VelController::vIncrementTask, "vIncrementTask", 1024, &vcontroller, 1, NULL);
  Serial.println("Ready");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove whitespace
    double in_value = input.toDouble();
    Serial.println("Received: " + String(in_value));
    //pcontroller.setUs((int) in_value, increment, period);
    pcontroller.setAngle(in_value, increment, period, servo_angle_param);
    //vcontroller.setRate(in_value, period);
    Serial.println("Set to: " + String(in_value));
  }
}

// Tommy's PID code from last year: https://github.com/UTAT-UAS/ARCHIVE_hardware_integration/blob/main/packages/ros-payload-control/src/payload_control.cpp
