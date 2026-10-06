#include"ens160sensor.hpp"

bool Ens160Sensor::Aht20Request::getReceive(uint8_t *data, uint8_t dataLength){
    if(wire->requestFrom(address, dataLength) != dataLength)    return false;
    
    for(int8_t i = 0;i < dataLength;i++){
        data[i] = wire->read();
    }
    return true;
}

bool Ens160Sensor::Aht20Request::getReceive(TwoWire *wire, uint8_t address, uint8_t *data, uint8_t dataLength){
    if(wire->requestFrom(address, dataLength) != dataLength)    return false;
    
    for(int8_t i = 0;i < dataLength;i++){
        data[i] = wire->read();
    }
    return true;
}

bool Ens160Sensor::Aht20Request::getData(uint8_t *data, uint8_t dataLength){
    // 受信不足・測定中・未校正・CRC不一致のデータを変換前に除外する。
    if (!wire || !data || dataLength != 7) return false;
    wire->beginTransmission(address);
    wire->write(GET_DATA);
    wire->write(GET_DATA_PARAM1);
    wire->write(GET_DATA_PARAM2);
    if (wire->endTransmission() != 0) return false;
    delay(80);
    if (!getReceive(data, dataLength)) return false;
    return (data[0] & 0x80) == 0 && (data[0] & 0x08) != 0 && verifyUsingCrc(data);

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
                crc <<= 1;
            }
        }
    }
    if(crc == data[6])  return true;
    else                return false;
}

uint8_t Ens160Sensor::Aht20Request::makeI2cHeadData(uint8_t address,bool ReadOrWrite){
    uint8_t returnData = 0x00;

    returnData = (address << 1) | ReadOrWrite;
    
    return(returnData);
}

void Ens160Sensor::Aht20Request::divideTemperatureAndHumidityData(uint8_t *originalData,uint32_t *temperatureData,uint32_t *humidityData){
    *humidityData =  (static_cast<uint32_t>(originalData[1]) << 12) | (static_cast<uint32_t>(originalData[2]) << 4) | (originalData[3]>>4);
    *temperatureData = ((static_cast<uint32_t>(originalData[3]) & 0x0F) << 16) | (static_cast<uint32_t>(originalData[4]) << 8) | originalData[5];

    return;
}

float Ens160Sensor::Aht20Request::temperatureConvertTofloat(uint32_t originalData){
    return (static_cast<float>(originalData) / 1048576.0f * 200.0f - 50.0f);
}

float Ens160Sensor::Aht20Request::humidityConvertTofloat(uint32_t originalData){
    return (static_cast<float>(originalData) / 1048576.0f * 100.0f);
}
namespace {
bool readEnsRegister(TwoWire *wire, uint8_t address, uint8_t reg, uint8_t *bytes, uint8_t length) {
    wire->beginTransmission(address);
    wire->write(reg);
    if (wire->endTransmission() != 0) return false;
    if (wire->requestFrom(address, length) != length) return false;
    for (uint8_t i = 0; i < length; ++i) bytes[i] = wire->read();
    return true;
}
}
bool Ens160Sensor::readCheckedEco2(TwoWire *wire, uint8_t address, uint16_t *value) {
    if (!wire || !value) return false;
    uint8_t status = 0, concentration[2] = {};
    // DATA_STATUSのbit 3:2は、0が正常、1/2が暖機中、3が無効を示す。
    if (!readEnsRegister(wire, address, 0x20, &status, 1) || (status & 0x0c) != 0) return false;
    if (!readEnsRegister(wire, address, 0x24, concentration, 2)) return false;
    uint16_t result = static_cast<uint16_t>(concentration[0]) | (static_cast<uint16_t>(concentration[1]) << 8);
    if (result < 400 || result > 65000) return false;
    *value = result;
    return true;
}
