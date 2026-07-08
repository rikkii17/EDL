#include <Arduino.h>
#include<DFRobot_ENS160.h>
#include<Wire.h>
#include<cpp_helper.hpp>

#include "analog_device.hpp"
#include"interface_main.hpp"
#include"pins_config.hpp"
#include"err.hpp"

#include "analog_device.hpp"


InterFace outputInterFace;

void setup() {
  delay(20000);
  Serial.begin(9600);
  Serial.println("");
  Serial.println("maincode");
  Serial.println("");
  Serial.println("-----Setting start-----");

  Serial.print("\t(Test) Starting Serial port: ");
  Serial.println("OK");

  Serial.print("\tStariting Wire: ");
  Wire.begin();
  Serial.println("OK");


  Serial.print("\ttest Interface:"); 
  outputInterFace.begin();
  Serial.println("\tOK");
}

void loop() {
  float A0_value = AnalogReader::convertToVoltage(analogRead(A0));
  Serial.println("main");
  Serial.print("A0_value: ");
  Serial.println(A0_value);
  //Serial.println("\n");
  delay(1000);
}