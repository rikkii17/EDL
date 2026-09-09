#include"ens160sensor.hpp"

bool Ens160Sensor::Aht20Request::getReceve(uint8_t *data, uint8_t dataLength){
    if(wire->requestFrom(address, dataLength) != dataLength)    return false;
    
    for(int8_t i = 0;i < dataLength;i++){
        data[i] = wire->read();
    }
    return true;
}

bool getReceve(TwoWire *wire, uint8_t address, uint8_t *data, uint8_t dataLength){
    if(wire->requestFrom(address, dataLength) != dataLength)    return false;
    
    for(int8_t i = 0;i < dataLength;i++){
        data[i] = wire->read();
    }
    return true;
}