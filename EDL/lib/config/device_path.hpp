#ifndef DEVICE_PATH_HPP

#define DEVICE_PATH_HPP
#include <Arduino.h>
#include<Wire.h>

class Ens160SensorAddress{
    const int8_t TEMPERATURE_HUMIDITY = 0x38; //温湿度センサのI2Cアドレス
    const int8_t CO2 = 0x53; //CO2センサのI2Cアドレス
};

#endif //DEVICE_PATH_HPP