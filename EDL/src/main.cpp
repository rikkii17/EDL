#include <Arduino.h>
#include<DFRobot_ENS160.h>

#include "hardware_helper.hpp"

void setup() {
  Serial.begin(9600);
  //PCとの通信に必要。
}

void loop() {
  float A0_value = AnalogReader::convertToVoltage(analogRead(A0));
  Serial.print("A0_value: ");
  Serial.println(A0_value);
  //Serial.println("\n");
  delay(1000);
}