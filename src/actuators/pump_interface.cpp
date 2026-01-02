#include "pump_interface.hpp"
#include <Arduino.h>

namespace actuators::pump {

    const int pwmPin = 35;     // pin 5
    const int pwmChannel = 0; // PWM channel 0-15
    const int freq = 5000;    // 5000Hz
    const int resolution = 8; // 8-bit (0–255)
    QueueHandle_t pumpQueue = xQueueCreate(8, sizeof(int));
    int dutyCycle = 0;

    void initPump() {
        pinMode(pwmPin, OUTPUT);
        ledcSetup(pwmChannel, freq, resolution);
        ledcAttachPin(pwmPin, pwmChannel);
    }

    void writeToPump(int speedPercent) {
        dutyCycle = map(speedPercent, 0, 100, 0, 255);
        // 194 is starting pwm value
        xQueueSend(pumpQueue, &dutyCycle, 0);
    }

    void vMonitorTask(void *pv) {
        uint32_t duty_local;
        while(1) {
            if (xQueueReceive(pumpQueue, &duty_local, pdMS_TO_TICKS(100)) == pdTRUE) {
                ledcWrite(pwmChannel, duty_local);
            }
            // Yield to let the ESP32 manage background wifi/serial/watchdog
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }

} // end namespace actuators::pump