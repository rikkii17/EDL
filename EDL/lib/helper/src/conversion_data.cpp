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

int Conversion::overall(int temperature,int humidity,int eco2,int noise){
    float returnData = (temperature + humidity + noise + eco2) / 4.0f;

    if(returnData <= 2) return(1);
    else if(returnData<= 4 && returnData < 2)   return(2);
    else if(returnData <= 6 && returnData < 4)  return(3);
    else if(returnData > 6) return(4);
    else    return(0);

}