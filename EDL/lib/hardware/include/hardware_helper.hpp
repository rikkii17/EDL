#ifndef HARDWARE_HELPER_HPP

#define HARDWARE_HELPER_HPP
#include <Arduino.h>

class AnalogReader{
    public:
        static float convertToVoltage(int rawVoltage);
};

#endif // HARDWARE_HELPER_HPP