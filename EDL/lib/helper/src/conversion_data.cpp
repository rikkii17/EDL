#include"conversion_data.hpp"

int Conversion::temperature(uint32_t data){
        data -= ReferenceValue::TEMPERATURE;
        data /= Evaluation_Unit::TEMPERATURE;
        return(8 - std::abs(static_cast<int32_t>(data)));
}


    int Conversion::humidity(uint32_t data){
        data -= ReferenceValue::HUMIDITY;
        data /= Evaluation_Unit::HUMIDITY;
        return(8 - std::abs(static_cast<int32_t>(data)));
    }
    
    int Conversion::noise(float data){
        data -= ReferenceValue::NOISE;
        data /= Evaluation_Unit::NOISE;
        return(8 - std::round(data));
    }

    int Conversion::co2(uint16_t data){
        data -= ReferenceValue::CO2;
        data /= Evaluation_Unit::CO2;
        return(8 - std::abs(static_cast<int32_t>(data)));
    }