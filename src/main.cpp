#include "main.hpp"
#include "pos_controller.hpp"
#include "vel_controller.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>

PosController pcontroller = PosController(pin, lbound, ubound);
VelController vcontroller = VelController(pin, lbound, ubound);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pcontroller.setup(center_us);
  // vcontroller.setup(center_us);
  // xTaskCreate(vcontroller.vIncrementTask, "vIncrementTask", 1024, NULL, 1, NULL);
  Serial.println("Ready");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove whitespace
    double in_value = input.toDouble();
    pcontroller.setUs((int) in_value, increment, period);
    // vController.setRate(in_value, period);
    Serial.println("Moved to: " + String(in_value));
  }
}

// Tommy's PID code from last year: https://github.com/UTAT-UAS/ARCHIVE_hardware_integration/blob/main/packages/ros-payload-control/src/payload_control.cpp
