#include"ens160sensor.hpp"

bool Ens160Sensor::Aht20Request::getReceive(uint8_t *data, uint8_t dataLength){
    if(wire->requestFrom(address, dataLength) != dataLength)    return false;
    
    for(int8_t i = 0;i < dataLength;i++){
        data[i] = wire->read();
    }
    return true;
}

bool getReceive(TwoWire *wire, uint8_t address, uint8_t *data, uint8_t dataLength){
    if(wire->requestFrom(address, dataLength) != dataLength)    return false;
    
    for(int8_t i = 0;i < dataLength;i++){
        data[i] = wire->read();
    }
    return true;
}

bool Ens160Sensor::Aht20Request::getData(uint8_t *data, uint8_t dataLength){
    //データを送るためのコマンドの送信
    wire->beginTransmission(Ens160SensorAddress::TEMPERATURE_HUMIDITY);
    wire->write(Aht20Request::GET_DATA);
    wire->write(Aht20Request::GET_DATA_PARAM1);
    wire->write(Aht20Request::GET_DATA_PARAM2);
    if(wire->endTransmission() != 0)    return false;

    //ここからデータ取得コードを書く

}