#ifndef POS_CONTROLLER_HPP
#define POS_CONTROLLER_HPP
#include <Arduino.h>
#include <ESP32Servo.h>

class PosController {
public:
    PosController(int pin_, int lbound_, int ubound_);
    void setup(int initial_us);
    void setUs(int target_, int increment, int period);
    void setAngle(double angle, int increment, int period, int servo_angle_param);
private:
    int pin;
    double current_us;
    int lbound; // lower bound in microseconds
    int ubound; // upper bound in microseconds
    Servo servo;
};

#endif // POS_CONTROLLER_HPP