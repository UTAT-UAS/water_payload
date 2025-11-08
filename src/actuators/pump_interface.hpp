#include <Arduino.h>

namespace actuators::pump {

    void initPump();
    void writeToPump(int speed);  // speed: 0-100

}

