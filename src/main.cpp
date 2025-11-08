#include "main.hpp"
#include "actuators/pos_controller.hpp"
#include "actuators/vel_controller.hpp"
#include "actuators/pump_interface.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>

PosController pcontroller = PosController(pin, lbound, ubound);
//VelController vcontroller = VelController(pin, lbound, ubound);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    //pcontroller.setup(center_us);
    //vcontroller.setup(center_us);
    //xTaskCreate(VelController::vIncrementTask, "vIncrementTask", 1024, &vcontroller, 1, NULL);
    actuators::pump::initPump();    
    Serial.println("Ready");
}

void loop() {
    actuators::pump::writeToPump(100);
    delay(2000); 
    actuators::pump::writeToPump(0);
    delay(2000);
}

// Tommy's PID code from last year: https://github.com/UTAT-UAS/ARCHIVE_hardware_integration/blob/main/packages/ros-payload-control/src/payload_control.cpp
