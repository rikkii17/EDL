#ifndef CONVERSION_DATA_HPP
#define CONVERSION_DATA_HPP
#include<stdint.h>
#include<stdlib.h>
#include<cmath>

    class Conversion{
    private:
    class Evaluation_Unit{
    public:
        static constexpr int TEMPERATURE = 2;
        static constexpr int HUMIDITY = 6;  //正確には6.25
        static constexpr int NOISE = 5;
        static constexpr int CO2 = 575;
    };
    class ReferenceValue{
    public:
        static constexpr int TEMPERATURE = 23;
        static constexpr int HUMIDITY = 50;
        static constexpr int NOISE = 35;
        static constexpr int CO2 = 400;
    };

    public:
        static int temperature(uint32_t data);
        static int humidity(uint32_t data);
        static int noise(float data);
        static int co2(uint16_t data);
        static int overall(int temperature,int humidity,int eco2,int noise);

    };

#endif  //CONVERSION_DATA_HPP