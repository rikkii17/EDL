#include <Arduino.h>
#include<DFRobot_ENS160.h>
#include<Wire.h>
#include<cpp_helper.hpp>

#include "analog_device.hpp"
#include"interface_main.hpp"
#include"pins_config.hpp"
#include"device_path.hpp"
#include"err.hpp"

#include "analog_device.hpp"
#include"ens160sensor.hpp"



InterFace outputInterFace;
DFRobot_ENS160_I2C i2c(&Wire,Ens160SensorAddress::CO2);

Ens160Sensor::Aht20Request aht20Request{&Wire, Ens160SensorAddress::TEMPERATURE_HUMIDITY};

void setup() {
  bool findTemperatureAndHumidityDevice = 0;
  bool findi2cDevice = 0;

  delay(5000);
  //Serial通信環境の立ち上げ
  Serial.begin(9600);
  Serial.println("");
  Serial.println("maincode");
  Serial.println("");
  Serial.println("-----Setting start-----");
  Serial.print("\t(Test) Starting Serial port: ");
  Serial.println("OK");

  //Wire通信環境の立ち上げ
  Serial.print("\tstarting Wire: ");
  Wire.begin();
  Serial.println("OK");
  Serial.println("\tI2C pins scanning");
    
  //I2Cデバイスのアドレススキャン
  for(int checkAddress = 1;checkAddress < 127;checkAddress++){
    Wire.beginTransmission(checkAddress);
    bool error = Wire.endTransmission();
    if(error == 0){
      Serial.print("\t\tI2C device find (address 0x");
      if(checkAddress < 16) Serial.print("0");
      Serial.print(checkAddress,HEX);
      Serial.println(")");

      if(checkAddress == Ens160SensorAddress::TEMPERATURE_HUMIDITY) findTemperatureAndHumidityDevice = true;
      else if(checkAddress == Ens160SensorAddress::CO2)  findi2cDevice = true;
    }
  }

  if(findTemperatureAndHumidityDevice){
    u_int16_t initStatus = 0;

    //温湿度センサの初期化
    while(true){
      //AHT20の操作関数の変数定義
      aht20Request.wire = &Wire;
      uint8_t testData[7] = {0};

      Serial.print("\t find and initializing temperature and humidity sensor: ");
      //本来であればこの後１００ms以上待機が必要だが、電源投入後十分な時間が経過しているためとりあえず待機なしで実行
      Wire.beginTransmission(Ens160SensorAddress::TEMPERATURE_HUMIDITY);
      //AHT20の初期化状態を取得する
      Wire.write(Ens160Sensor::Aht20Request::GET_DEVICE_STATUS);
      //正常に送られたかの確認
      if(Wire.endTransmission() != 0){
        Serial.println("Can not send comand to temperature and humidity sensor.\n Retrying...");
        continue;
      }
      //AHT20の初期化ステータスの取得
      aht20Request.getReceive((uint8_t*)&initStatus,sizeof(initStatus));
      if(initStatus != 1){
        //AHT20の初期化
        Serial.println("\tAHT sensor is not INITIALIZEd");
        Serial.println("\t\tinitializing AHT sensor...");
        Wire.beginTransmission(Ens160SensorAddress::TEMPERATURE_HUMIDITY);
        Wire.write(Ens160Sensor::Aht20Request::INITIALIZE);
        Wire.write(Ens160Sensor::Aht20Request::INITIALIZE_PARAM1);
        Wire.write(Ens160Sensor::Aht20Request::INITIALIZE_PARAM2);
        if(Wire.endTransmission() != 0){
          Serial.println("\t\t\tCan not send initialize comand to temperature and humidity sensor.\n Retrying...");
          continue;
        }
        delay(10);  //初期化待機時間

        //初期化コマンドを入力後の再検査
        aht20Request.getReceive((uint8_t*)&initStatus,sizeof(initStatus));
        if(initStatus != 1){
          Serial.println("\t\t\tAHT sensor initializing failed.\n Retrying...");
          continue;
        }
        Serial.println("\t\t\tAHT sensor INITIALIZEd");

        //受信Test
        aht20Request.getReceive(testData, 7);
        if(aht20Request.verifyUsingCrc(testData)) break;
        else{
          Serial.println("\t\tAHT20Sensor did not get Perfect data.\n\t\tRetry ...");
          continue;
        }
      }
    }
  }

  //CO2センサの初期化
  if(findi2cDevice){
    Serial.println("\tfind and initializing CO2 meter status: ");
    if(i2c.begin() == 0){
      Serial.println("\t\tCO2 meter started");
      i2c.setPWRMode(ENS160_STANDARD_MODE);

      //初期化が完了するためのウォームアップ時間の確保
      Serial.println("\t\tCO2 meter worm up time: ");
      while(i2c.getENS160Status() != 0){
        if(i2c.getENS160Status() == 2){
          Serial.println("\t\t\tsensor Note: Sensor is latest device.There is a possibility that the accuracy is poor.");
          break;
        }
        delay(100); //初期化が完了するまで待機させるためのバッファ時間
      }
    }
  }
  else{
    Serial.println("\tCO2 meter not found");
  }

  //出力インターフェースの初期化及びテスト
  Serial.print("\ttest Interface:"); 
  outputInterFace.begin();
  Serial.println("OK");
  Serial.println("-----Setting end-----");
}

void loop() {
  //温度の取得
  
  //等価CO2濃度の取得
  uint16_t eco2 = i2c.getECO2();
}