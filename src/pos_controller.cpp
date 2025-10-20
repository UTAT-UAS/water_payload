#include "pos_controller.hpp"

PosController::PosController(int pin_, int lbound_, int ubound_)
    : pin(pin_), lbound(lbound_), ubound(ubound_) {
    servo = Servo();
    servo.attach(pin);
}

// Method to set up the servo
void PosController::setup(int initial_us = 1000) {
    current_us = constrain(initial_us, lbound, ubound);
    servo.writeMicroseconds(current_us);
}

void PosController::setUs(int target_, int increment, int period) { // degrees per second, ms per step
    int target = constrain(target_, lbound, ubound);
    if(increment == 0 || target == current_us) return;
    if(target - current_us > 0 ^ increment > 0) {
        increment = -increment;
    }

    while (abs(current_us - target) > abs(increment)) {
      current_us += increment;
      servo.writeMicroseconds((int) current_us);
      delay(period);
    }
    servo.writeMicroseconds(target);
    current_us = target;
}
