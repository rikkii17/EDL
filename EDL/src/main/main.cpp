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

void setup() {
  bool findTempratureAndHumidityDevice = 0;
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
  Serial.print("\tStariting Wire: ");
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

      if(checkAddress == Ens160SensorAddress::TEMPERATURE_HUMIDITY) findTempratureAndHumidityDevice = true;
      else if(checkAddress == Ens160SensorAddress::CO2)  findi2cDevice = true;
    }
  }

  if(findTempratureAndHumidityDevice){
    u_int16_t initStatus = 0;

    //温湿度センサの初期化
    do{
      Serial.print("\t find and initlizing temperature and humidity sensor: ");
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
      Ens160Sensor::Aht20Request::getReceve(&Wire,Ens160SensorAddress::TEMPERATURE_HUMIDITY,(uint8_t*)&initStatus,sizeof(initStatus));
      if(initStatus != 1){
        //AHT20の初期化
        Serial.println("\tAHT sensor is not initlized");
        Serial.println("\t\tinitlizing AHT sensor...");
        Wire.beginTransmission(Ens160SensorAddress::TEMPERATURE_HUMIDITY);
        Wire.write(Ens160Sensor::Aht20Request::INITLIZE);
        Wire.write(Ens160Sensor::Aht20Request::INITLIZE_PARAM1);
        Wire.write(Ens160Sensor::Aht20Request::INITLIZE_PARAM2);
        if(Wire.endTransmission() != 0){
          Serial.println("\t\t\tCan not send initlize comand to temperature and humidity sensor.\n Retrying...");
          continue;
        }
        delay(10);  //初期化待機時間

        //初期化コマンドを入力後の再検査
        Ens160Sensor::Aht20Request::getReceve(&Wire,Ens160SensorAddress::TEMPERATURE_HUMIDITY,(uint8_t*)&initStatus,sizeof(initStatus));
        if(initStatus != 1){
          Serial.println("\t\t\tAHT sensor initlizing failed.\n Retrying...");
          continue;
        }
        Serial.println("\t\t\tAHT sensor initlized");

        //受信Test
        

      }

    }while();
  }

  //CO2センサの初期化
  if(findi2cDevice){
    Serial.println("\tfind and initializing CO2 meter status: ");
    if(i2c.begin() == 0){
      Serial.println("\t\tCO2 meter started");
      i2c.setPWRMode(ENS160_STANDARD_MODE);

      //初期化が完了するためのウォームアップ時間の確保
      Serial.println("\t\tCO2 meter wormup time: ");
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
  Wire.beginTransmission(Ens160SensorAddress::TEMPERATURE_HUMIDITY);


  //等価CO2濃度の取得
  uint16_t eco2 = i2c.getECO2();
}