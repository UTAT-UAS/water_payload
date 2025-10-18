#include "vel_controller.hpp"

VelController::VelController(int pin_, int lbound_, int ubound_)
    : pin(pin_), lbound(lbound_), ubound(ubound_) {
    Servo servo = Servo();
    servo.attach(pin);
}

void VelController::setup(int start_us_) {
    current_us = constrain(start_us_, lbound, ubound);
    servo.writeMicroseconds(current_us);
}

void VelController::setRate(double increment_, int period_) {
    increment = increment_;
    period = period_;
}

void VelController::vIncrementTask() {
    while(1) {
        if (increment != 0) {
            current_us = constrain(current_us + increment, lbound, ubound);
            servo.writeMicroseconds((int) current_us);
        }
        vTaskDelay(pdMS_TO_TICKS(delay)); // what if period = 0?
    }
}