#ifndef ENS160SENSOR_HPP
#define ENS160SENSOR_HPP

    #include <Arduino.h>
    #include <stdint.h>
    #include<Wire.h>
    #include<DFRobot_ENS160.h>

    #include"device_path.hpp"
    #include"err.hpp"

    class Ens160Sensor{
    public:
        class Aht20Request{
        public:
            static const uint8_t GET_DEVICE_STATUS = 0x71;
            static const uint8_t INITLIZE = 0xBE;
            static const uint8_t INITLIZE_PARAM1 = 0x08;
            static const uint8_t INITLIZE_PARAM2 = 0x00;

            
            static bool getReceve(TwoWire *wire, uint8_t address, uint8_t *data, uint8_t dataLength);
            
        };
        
    };

#endif //ENS160SENSOR_HPP