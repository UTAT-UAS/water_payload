#include "pos_controller.hpp"

PosController::PosController(int pin_, int channel_, int lbound_, int ubound_)
    : pin(pin_), channel(channel_), lbound(lbound_), ubound(ubound_) {
    freq = 50;      // Servos run at 50Hz
    resolution = 14;     // 14-bit resolution (0 to 16383)
}

// Method to set up the servo
void PosController::setup(int initial_us = 1465) {
    duty = constrain(initial_us, lbound, ubound);
    servoQueue = xQueueCreate(8, sizeof(int));

    // Servo setup with LEDC
    ledcSetup(channel, freq, resolution);
    ledcAttachPin(pin, channel);
}

void PosController::setUs(int target_) { // degrees per second, ms per step
    int target = constrain(target_, lbound, ubound);
    if(target == duty) return;

    if (servoQueue) {
        // Convert microseconds to duty cycle  (Target US / 20,000 Total Period US) * Max Resolution Ticks
        duty = (target * 16384) / 20000;
        xQueueSend(servoQueue, &duty, 0);
    }
}

void PosController::setAngle(double angle, int servo_angle_param) {
    double target = 1464.5 + angle * 1837 / (servo_angle_param * 1.25);
    setUs(target);
}

void PosController::vMonitorTask(void *pv) {
    PosController* self = static_cast<PosController*>(pv);
    uint32_t duty_local;
    while(1) {
        if (xQueueReceive(self->servoQueue, &duty_local, pdMS_TO_TICKS(100)) == pdTRUE) {
            ledcWrite(self->channel, duty_local);
        }
        // Yield to let the ESP32 manage background wifi/serial/watchdog
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}