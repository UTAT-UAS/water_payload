#ifndef POS_CONTROLLER_HPP
#define POS_CONTROLLER_HPP
#include <Arduino.h>
#include <ESP32Servo.h>

class PosController {
public:
    PosController(int pin_, int channel_, int lbound_, int ubound_);
    void setup(int initial_us);
    void setUs(int target);
    void setAngle(double angle, int servo_angle_param);
    static void vMonitorTask(void *pv);
private:
    int pin;
    int channel = 4;
    int freq = 50;
    int resolution = 14;
    uint32_t duty;
    int lbound; // lower bound in microseconds
    int ubound; // upper bound in microseconds
    QueueHandle_t servoQueue;
};

#endif // POS_CONTROLLER_HPP