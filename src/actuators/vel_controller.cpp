#include "vel_controller.hpp"

VelController::VelController(int pin_, int lbound_, int ubound_)
    : pin(pin_), lbound(lbound_), ubound(ubound_) {
    servo = Servo();
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

void VelController::vIncrementTask(void* parameters) {
    VelController* self = static_cast<VelController*>(parameters);
    while(1) {
        if (self->increment != 0) {
            self->current_us = constrain(self->current_us + self->increment, self->lbound, self->ubound);
            self->servo.writeMicroseconds((int) self->current_us);
        }
        vTaskDelay(pdMS_TO_TICKS(self->period)); // what if period = 0?
    }
}