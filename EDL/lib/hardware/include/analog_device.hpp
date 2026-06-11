#ifndef ANALOG_DEVICE_HPP

#define ANALOG_DEVICE_HPP
#include <Arduino.h>

class AnalogReader{
    public:
        static float convertToVoltage(int rawVoltage);
        //分解能をもとにインプットボリュームを電圧に変換する関数
};

#endif // ANALOG_DEVICE_HPP