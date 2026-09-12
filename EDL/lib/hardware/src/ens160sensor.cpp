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
    wire->write(Ens160Sensor::Aht20Request::makeI2cHeadData(Ens160SensorAddress::TEMPERATURE_HUMIDITY,Ens160Sensor::Aht20Request::READ));
    wire->write(Aht20Request::GET_DATA);
    wire->write(Aht20Request::GET_DATA_PARAM1);
    wire->write(Aht20Request::GET_DATA_PARAM2);
    if(wire->endTransmission() != 0)    return false;

    getReceive(data,dataLength);

    //CRC検証


}

bool Ens160Sensor::Aht20Request::verifyUsingCrc(uint8_t *data){
    uint8_t crc = 0xff; //リファレンスよりCRC初期値は0xFF

    for(uint8_t i = 0; i<6;i++){
        uint8_t checkByteData = data[i];
        crc ^= checkByteData;

        for(uint8_t n = 0; n < 8; n++){
            if(crc & 0x80){ //最上位ビットの確認
                crc <<= 1;
                crc ^= CRC_POLYNOMIAL;  //=0x31
            }
            else{
                crc << 1;
            }
        }
    }
    if(crc == data[6])  return true;
    else                return false;
}

uint8_t makeI2cHeadData(uint8_t address,bool ReadOrWrite){
    uint8_t returnData = 0x00;

    returnData = (address << 1) | ReadOrWrite;
    
    return(returnData);
}

void divideTemperatureAndHumidityData(uint8_t *originalData,uint32_t *temperatureData,uint32_t *humidityData){
    *humidityData =  (static_cast<uint32_t>(originalData[1]) << 12) | (static_cast<uint32_t>(originalData[2]) << 4) | (originalData[3]>>4);
    *temperatureData = ((static_cast<uint32_t>(originalData[3]) & 0x0F) << 16) | (static_cast<u_int32_t>(originalData[4]) << 8) | originalData[5];

    return;
}