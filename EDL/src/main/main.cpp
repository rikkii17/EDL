#include <Arduino.h>
#include <cmath>
#include "telemetry.hpp"
#include<DFRobot_ENS160.h>
#include<Wire.h>
#include<cpp_helper.hpp>

#include "analog_device.hpp"
#include"interface_main.hpp"
#include"pins_config.hpp"
#include"device_path.hpp"
#include"err.hpp"
#include"conversion_data.hpp"

#include "analog_device.hpp"
#include"ens160sensor.hpp"



InterFace outputInterFace;
Telemetry telemetry;
bool eco2Initialized = false;
DFRobot_ENS160_I2C i2c(&Wire,Ens160SensorAddress::CO2);

Ens160Sensor::Aht20Request aht20Request{&Wire, Ens160SensorAddress::TEMPERATURE_HUMIDITY};

void setup() {
  bool findTemperatureAndHumidityDevice = 0;
  bool findi2cDevice = 0;
  uint8_t retryCount = 0;

  delay(5000);
  //Serial通信環境の立ち上げ
  Serial.begin(9600);
  Serial.println("");
  Serial.println("maincode");
  Serial.println("");
  Serial.println("-----Setting start-----");
  Serial.print("\t(Test) Starting Serial port: ");
  Serial.println("OK");

  //マイクセンサ（騒音センサ）のテスト
  Serial.print("\tTest noise sensor: ");
  if(analogRead(NOISE_SENSOR_PATH)  != 0) Serial.println("OK");
  else Serial.println("Noise sensor can not get data.");


  //Wire通信環境の立ち上げ
  Serial.print("\tstarting Wire: ");
  Wire.begin();
  Serial.println("OK");
  Serial.println("\tI2C pins scanning");
    
  //I2Cデバイスのアドレススキャン
  for(int checkAddress = 1;checkAddress < 127;checkAddress++){
    Wire.beginTransmission(checkAddress);
    bool error = Wire.endTransmission();

    //test
    Serial.print("Scanning:0x");
    if(checkAddress < 16) Serial.print("0");
    Serial.println(checkAddress,HEX);
    //enf of test


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
    uint8_t initStatus = 0; // AHT20の状態値は1バイト。

    //温湿度センサの初期化
    // センサーの未接続や故障で初期化が無限に続かないよう回数を制限する。
    for (uint8_t setupAttempt = 0; setupAttempt < 3; ++setupAttempt){
      //AHT20の操作関数の変数定義
      aht20Request.wire = &Wire;
      uint8_t testData[7] = {0};
      uint8_t transmissionStatus = 0;

      Serial.print("\t find and initializing temperature and humidity sensor: ");
      
      //初期化をするはずだったが、多分いらないっぽい。
      /*Wire.beginTransmission(Ens160SensorAddress::TEMPERATURE_HUMIDITY);
      Wire.write(Ens160Sensor::Aht20Request::SOFT_RESET);
      Wire.endTransmission();*/

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
      
      //test
      Serial.println(initStatus);
      if((initStatus & 0x08) == 0){
        //AHT20の初期化
        Serial.println("\tAHT sensor is not initalized");
        Serial.println("\t\tinitializing AHT sensor...");
        Wire.beginTransmission(Ens160SensorAddress::TEMPERATURE_HUMIDITY);
        //このコードいらないはずなのに書いていたので送信エラーを起こしていたらしい。（Beginで送信済みらしい）
        //Wire.write(aht20Request.makeI2cHeadData(Ens160SensorAddress::TEMPERATURE_HUMIDITY,Ens160Sensor::Aht20Request::WRITE));
        Wire.write(Ens160Sensor::Aht20Request::INITIALIZE);
        Wire.write(Ens160Sensor::Aht20Request::INITIALIZE_PARAM1);
        Wire.write(Ens160Sensor::Aht20Request::INITIALIZE_PARAM2);
        
        transmissionStatus = Wire.endTransmission();
        if(transmissionStatus == 0) Serial.println("\t\t\tSend initialize comand to temperature and humidity sensor.");
        else if(transmissionStatus == 1)  Serial.println("\t\t\terror 1:\tData cannot subside.");
        else if(transmissionStatus == 2)  Serial.println("\t\t\terror 2:\tNACK,When address sending.");
        else if(transmissionStatus == 3)  Serial.println("\t\t\terror 3:\tNACK,When data sending.");
        else if(transmissionStatus == 4)  Serial.println("\t\t\terror 4:\tInternal error.");
        else if(transmissionStatus == 5)  Serial.println("\t\t\terror 5:\tSystem is timeout.");

        if(transmissionStatus != 0 && retryCount < 3){
          retryCount++;
          Serial.println("Retrying...");
          continue;
        }
        else if(transmissionStatus != 0 && retryCount >= 3){
          Serial.println("It's not going well.Run despite error.");
          break;
        }
        delay(10);  //初期化待機時間

        //初期化コマンドを入力後の再検査
        Serial.println(initStatus);
        aht20Request.getReceive((uint8_t*)&initStatus,sizeof(initStatus));

        retryCount = 0;
        if((initStatus & 0x08) == 0 && retryCount < 3){
          Serial.println("\t\t\tAHT sensor initializing failed.\n Retrying...");
          retryCount++;
          Serial.println(retryCount);
          continue;
        }
        else if((initStatus & 0x08) == 0 && retryCount >= 3){
          Serial.println("It's not going well.Run despite error.");
          break;
        }

        Serial.println("\t\t\tAHT sensor initialized");

        //受信Test    
        Wire.beginTransmission(Ens160SensorAddress::TEMPERATURE_HUMIDITY);
        Wire.write(aht20Request.GET_DATA);
        Wire.write(aht20Request.GET_DATA_PARAM1);
        Wire.write(aht20Request.GET_DATA_PARAM2);
        if(Wire.endTransmission() != 0) Serial.println("Can not send comand to temperature and humidity sensor.\n Retrying...");
        
        delay(80);  //データが返ってくるまでの待機時間
        aht20Request.getReceive(testData, 7);
        if(aht20Request.verifyUsingCrc(testData)) break;
        else{
          Serial.println("\t\tAHT20Sensor did not get Perfect data.");
          if (retryCount < 3){
            retryCount++;
            Serial.println("Retry...");
            continue;
          }
          else{
            Serial.println("It's not going well.Run despite error.");
            break;
          }
        }
      }
      else{
        Serial.println("AHT sensor initialized.");
        delay(100);  //testwait
        //受信Test
        for (uint8_t readAttempt = 0; readAttempt < 3; ++readAttempt){
        Wire.beginTransmission(Ens160SensorAddress::TEMPERATURE_HUMIDITY);
        Wire.write(aht20Request.GET_DATA);
        Wire.write(aht20Request.GET_DATA_PARAM1);
        Wire.write(aht20Request.GET_DATA_PARAM2);
        if(Wire.endTransmission() != 0) Serial.println("Can not send comand to temperature and humidity sensor.\n Retrying...");
        delay(80);  //データが返ってくるまでの待機時間
        aht20Request.getReceive(testData, 7);
        //test
        for(int n = 0;n < 7;n++){
          Serial.println(testData[n]);
        }
        if(testData[0] & 0x80){
          Serial.println("Sensor flag is busy.Retry...");
          delay(3000);
          continue;
        }

        if(aht20Request.verifyUsingCrc(testData)) break;
        else{
          Serial.println("\t\tAHT20Sensor did not get Perfect data.");
          if (retryCount < 3){
            retryCount++;
            Serial.println("Retry...");
            continue;
          }
          else{
            Serial.println("It's not going well.Run despite error.");
            break;
          }
        }
        }
        break;
      }
    }
  }

  //CO2センサの初期化
  if(findi2cDevice){
    Serial.println("\tfind and initializing CO2 meter status: ");
    if(i2c.begin() == 0){
      eco2Initialized = true;
      Serial.println("\t\tCO2 meter started");
      i2c.setPWRMode(ENS160_STANDARD_MODE);

      //初期化が完了するためのウォームアップ時間の確保
      Serial.println("\t\tCO2 meter worm up time: ");
      // 既存の暖機待ちに上限を設け、正常になるまで測定値は欠測として扱う。
      for(uint16_t warmupCheck = 0; warmupCheck < 600 && i2c.getENS160Status() != 0; ++warmupCheck){
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
  Serial.println("-----Setting end-----\n\n\n\n\n");
  analogReadResolution(10); // 既存の電圧換算に合わせてADCを0〜1023にする。
  telemetry.begin();
}

void loop() {
  int evaluationTemperature;
  int evaluationHumidity;
  int evaluationEco2;
  int evaluationNoise;
  int evaluationOverall;
  Measurement measurement;
  measurement.uptime = millis();
  
  {
    float temperatureData = NAN;
    float humidityData = NAN;
    uint16_t eco2 = 0;
    float noiseVolume;
  
    {
      uint8_t temperatureAndHumidity[7] = {};
      u_int32_t temperatureOriginalData;
      u_int32_t humidityOriginalData;
      //温度・湿度の取得
      bool validTemperatureHumidity = aht20Request.getData(temperatureAndHumidity,7);
      if(!validTemperatureHumidity) Serial.println("error:\tTemperature and humidity data did not get using AHT20sensor. ");
      // 受信不足やCRC不一致のデータは温湿度へ変換しない。
      if(validTemperatureHumidity){
      //temperatureAndHumidityをtemperatureとHumidityの二つに分割
      aht20Request.divideTemperatureAndHumidityData(temperatureAndHumidity,&temperatureOriginalData,&humidityOriginalData);
      //温度データをfloat型に変換
      temperatureData = aht20Request.temperatureConvertTofloat(temperatureOriginalData);
      //湿度データをfloat型に変換
      humidityData = aht20Request.humidityConvertTofloat(humidityOriginalData);
      }
      measurement.temperature = temperatureData;
      measurement.humidity = humidityData;
      measurement.temperatureValid = std::isfinite(temperatureData) && temperatureData >= -40 && temperatureData <= 125;
      measurement.humidityValid = std::isfinite(humidityData) && humidityData >= 0 && humidityData <= 100;
    }

    //等価CO2濃度の取得
    // ライブラリの取得関数は受信不足を報告しないため、状態と2バイトの受信を確認する。
    measurement.eco2Valid = eco2Initialized && Ens160Sensor::readCheckedEco2(&Wire, Ens160SensorAddress::CO2, &eco2);
    measurement.eco2 = eco2;

    //騒音dataの取得
    int rawNoise = analogRead(NOISE_SENSOR_PATH);
    noiseVolume = AnalogReader::voltageToVolume(AnalogReader::convertToVoltage(rawNoise));
    measurement.noise = noiseVolume;
    measurement.noiseValid = rawNoise > 0 && rawNoise < 1023 && std::isfinite(noiseVolume) && noiseVolume >= 0 && noiseVolume <= 150;

    //それぞれのデータを数値評価
    evaluationTemperature = measurement.temperatureValid ? Conversion::temperature(temperatureData) : 0;
    evaluationHumidity = measurement.humidityValid ? Conversion::humidity(humidityData) : 0;
    evaluationEco2 = measurement.eco2Valid ? Conversion::co2(eco2) : 0;
    evaluationNoise = measurement.noiseValid ? Conversion::noise(noiseVolume) : 0;
    evaluationOverall = evaluationTemperature && evaluationHumidity && evaluationEco2 && evaluationNoise
      ? Conversion::overall(evaluationTemperature,evaluationHumidity,evaluationEco2,evaluationNoise) : 0;

    //test
    Serial.println("-----Data output-----");
    Serial.print("Temperature:\t");
    Serial.print(temperatureData);
    Serial.print("\tEvaluation:\t");
    Serial.println(evaluationTemperature);
    Serial.print("Humidity:\t");
    Serial.print(humidityData);
    Serial.print("\tEvaluation:\t");
    Serial.println(evaluationHumidity);
    Serial.print("eCO2:\t");
    Serial.print(eco2);
    Serial.print("\tEvaluation:\t");
    Serial.println(evaluationEco2);
    Serial.print("Noise:\t");
    Serial.print(noiseVolume);
    Serial.print("\tEvaluation:\t");
    Serial.println(evaluationNoise);
    Serial.print("Overall Evaluation:\t");
    Serial.println(evaluationOverall);
    delay(10000);
    
  }


  //それぞれのデータの数値評価を表示
  outputInterFace.outputNumberOfBus(IndexSelector::TEMPERATURE,evaluationTemperature);
  outputInterFace.outputNumberOfBus(IndexSelector::HUMIDITY,evaluationHumidity);
  outputInterFace.outputNumberOfBus(IndexSelector::CO2,evaluationEco2);
  outputInterFace.outputNumberOfBus(IndexSelector::NOISE,evaluationNoise);
  outputInterFace.outputNumberOfBus(IndexSelector::OVERALL,evaluationOverall);
  // 通信に失敗しても既存のローカル表示は実行済みとなるよう、最後に送信する。
  telemetry.publish(measurement);
}