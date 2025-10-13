#include "main.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>

Servo servo = Servo();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo.attach(pin);
  //servo.write(90); for writing angles
  Serial.println("Ready");
  servo.writeMicroseconds(us);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    us = input.toInt();
    servo.write(us);
  }
}

// Tommy's PID code from last year: https://github.com/UTAT-UAS/ARCHIVE_hardware_integration/blob/main/packages/ros-payload-control/src/payload_control.cpp
