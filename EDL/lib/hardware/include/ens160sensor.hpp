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
            //コマンド系の定数宣言
            //note: 若干定数のクラス設計ミスったかも。本来であればこの中にコマンドクラスでも作って格納した方がきれいだったか、、、
            static const uint8_t GET_DEVICE_STATUS = 0x71;

            static const uint8_t INITIALIZE = 0xBE;
            static const uint8_t INITIALIZE_PARAM1 = 0x08;
            static const uint8_t INITIALIZE_PARAM2 = 0x00;

            static const uint8_t GET_DATA = 0xAC;
            static const uint8_t GET_DATA_PARAM1 = 0x33;
            static const uint8_t GET_DATA_PARAM2 = 0x00;

            //CRCに関する定数宣言
            static const uint8_t CRC_POLYNOMIAL = 0x31;

            //Head ByteのRead or Write をdefine定義
            #define WRITE 0
            #define READ 1

            //インスタンス設計時の共通変数宣言
            TwoWire *wire;  //基本的に歯変更はないが、内部関数を利用することからconstはつけない
            const uint8_t address;

            bool getReceive(uint8_t *data, uint8_t dataLength);
            static bool getReceive(TwoWire *wire, uint8_t address, uint8_t *data, uint8_t dataLength);

            bool getData(uint8_t *data, uint8_t dataLength);

            bool verifyUsingCrc(uint8_t *data);

            static uint8_t makeI2cHeadData(uint8_t address,bool readOrWrite);
            
        };
        
    };

#endif //ENS160SENSOR_HPP