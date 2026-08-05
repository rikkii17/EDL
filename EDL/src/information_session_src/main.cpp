#include <Arduino.h>
#include<DFRobot_ENS160.h>
#include<Wire.h>
#include<cpp_helper.hpp>

#include "analog_device.hpp"
#include"interface_main.hpp"
#include"pins_config.hpp"
#include"err.hpp"

#include "analog_device.hpp"

#define K 0.628


InterFace outputInterFace;
int output[5];

void setup() {
  delay(20000);
  Serial.begin(9600);
  Serial.println("");
  Serial.println("information session code");
  Serial.println("");
  Serial.println("-----Setting start-----");

  Serial.print("\t(Test) Starting Serial port: ");
  Serial.println("OK");

  Serial.print("\ttest Interface:"); 
  outputInterFace.begin();
  Serial.println("\tOK");
}

void loop() {

    //すべての利用するデジタルBピンを出力にする。
    InterFace::testOutput(IndexSelector::TEMPERATURE);
    InterFace::testOutput(IndexSelector::HUMIDITY);
    InterFace::testOutput(IndexSelector::NOISE);
    InterFace::testOutput(IndexSelector::CO2);
    InterFace::testOutput(IndexSelector::OVERALL);


  /*

  for(int temp = 4;temp>0;temp--){
    output[temp] = output[temp-1];
  }

    float A0_value = AnalogReader::convertToVoltage(analogRead(A0));

    if(A0_value < 1*K)  output[0] = 0;
    else if(A0_value < 1*K)  output[0] = 1;
    else if(A0_value < 2*K && A0_value >= 1*K)  output[0] = 2;
    else if(A0_value < 3*K && A0_value >= 2*K)  output[0] = 3;
    else if(A0_value < 4*K && A0_value >= 3*K)  output[0] = 4;
    else if(A0_value < 5*K && A0_value >= 4*K)  output[0] = 5;
    else if(A0_value < 6*K && A0_value >= 5*K)  output[0] = 6;
    else if(A0_value < 7*K && A0_value >= 6*K)  output[0] = 7;
    else if(A0_value >= 7*K)  output[0] = 8;

  for(int temp = 0;temp < 5;temp++){
      outputInterFace.outputNumberOfBus(temp,output[temp]);
  }
  delay(1000);
  */
}