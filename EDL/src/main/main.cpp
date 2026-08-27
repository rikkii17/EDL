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

  if(findi2cDevice){
    Serial.println("\tfind and initializing I2C meter status: ");
    if(i2c.begin() == 0){
      Serial.println("\t\tI2C meter started");
      i2c.setPWRMode(ENS160_STANDARD_MODE);

      Serial.println("\t\tI2C meter wormup time: ");
      while(i2c.getENS160Status() != 0){
        if(i2c.getENS160Status() == 2){
          Serial.println("\t\t\tsensor Note: Sensor is latest device.There is a possibility that the accuracy is poor.");
          break;
        }
        delay(100);
      }
    }
  }
  else{
    Serial.println("\tI2C meter not found");
  }

  Serial.print("\ttest Interface:"); 
  outputInterFace.begin();
  Serial.println("OK");
  Serial.println("-----Setting end-----");
}

void loop() {
  
}