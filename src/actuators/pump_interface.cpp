#include "pump_interface.hpp"
#include <Arduino.h>

namespace actuators::pump {

    const int pwmPin = 5;     // pin 5
    const int pwmChannel = 0; // PWM channel 0-15
    const int freq = 5000;    // 5000Hz
    const int resolution = 8; // 8-bit (0–255)

    void initPump() {
        pinMode(pwmPin, OUTPUT);
        ledcSetup(pwmChannel, freq, resolution);
        ledcAttachPin(pwmPin, pwmChannel);
    }

    void writeToPump(int speedPercent) {
        int dutyCycle = map(speedPercent, 0, 100, 0, 255);
        // 194 is starting pwm value
        ledcWrite(pwmChannel, dutyCycle);
    }

} // end namespace actuators::pump