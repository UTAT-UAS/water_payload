#ifndef VEL_CONTROLLER_HPP
#define VEL_CONTROLLER_HPP
#include <Arduino.h>
#include <ESP32Servo.h>
class VelController {
public:
    VelController(int pin_, int lbound_, int ubound_);
    void setup(int start_us_);
    void setRate(double increment_, int period_); // increment in microseconds, period in ms
    static void vIncrementTask(void* parameters);
private:
    int pin;
    double current_us;
    double increment;
    int period;
    int lbound; // lower bound in microseconds
    int ubound; // upper bound in microseconds
    Servo servo;
};

#endif // VEL_CONTROLLER_HPP