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
DFRobot_ENS160_I2C temperatureAndHumidity(&Wire,Ens160SensorAddress::TEMPERATURE_HUMIDITY);
DFRobot_ENS160_I2C co2(&Wire,Ens160SensorAddress::CO2);

void setup() {
  bool findTempratureAndHumidityDevice = 0;
  bool findCo2Device = 0;

  delay(20000);
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
      else if(checkAddress == Ens160SensorAddress::CO2)  findCo2Device = true;
    }
  }

  if(findTempratureAndHumidityDevice){
    Serial.print("\tfind and initializing Temperature and humidity meter status: ");
    temperatureAndHumidity.begin();
    Serial.println("OK");
  }
  else{
    Serial.println("\tTemperature and humidity meter not found");
  }

  if(findCo2Device){
    Serial.print("\tfind and initializing CO2 meter status: ");
    co2.begin();
    Serial.println("OK");
  }
  else{
    Serial.println("\tCO2 meter not found");
  }

  Serial.print("\ttest Interface:"); 
  outputInterFace.begin();
  Serial.println("OK");
}

void loop() {
  
}