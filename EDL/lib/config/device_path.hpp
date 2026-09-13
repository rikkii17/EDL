#ifndef DEVICE_PATH_HPP

#define DEVICE_PATH_HPP
#include <Arduino.h>
#include<Wire.h>

//騒音センサのPath
#define NOISE_SENSOR_PATH A0

class Ens160SensorAddress{
public:
    static const int8_t TEMPERATURE_HUMIDITY = 0x38; //温湿度センサのI2Cアドレス
    static const int8_t CO2 = 0x53; //CO2センサのI2Cアドレス
};

#endif //DEVICE_PATH_HPP