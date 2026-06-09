#ifndef ANALOG_DEVICE_HPP

#define ANALOG_DEVICE_HPP
#include <Arduino.h>

class AnalogReader{
    public:
        static float convertToVoltage(int rawVoltage);
};

#endif // ANALOG_DEVICE_HPP