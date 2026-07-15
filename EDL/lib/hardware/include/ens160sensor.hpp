#ifndef ENS160SENSOR_HPP
#define ENS160SENSOR_HPP

    #include <Arduino.h>
    #include<Wire.h>
    #include<DFRobot_ENS160.h>

    #include"device_path.hpp"
    #include"err.hpp"

    class Ens160Sensor{
    public:
        class Aht20{
        public:
            static int8_t init(TwoWire wire);
        };
    };
}

#endif //ENS160SENSOR_HPP