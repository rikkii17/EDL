#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_ENS160.h>

#include"interface_main.hpp"
#include"pins_config.hpp"
#include"err.hpp"


void setup(){
    Serial.begin(9600);
    delay(5000);
    Serial.println("OutputTest");

}

void loop(){
    InterFace outputInterFace;
    Serial.println("Prease Write the Index:");
    String inputIndex;
    if(Serial.available()>0)    inputIndex = Serial.readStringUntil('\n');
    Serial.println("Prease Write the Value:");
    String inputValue;
    if(Serial.available()>0)    inputValue = Serial.readStringUntil('\n');
    //これをint型に変換すること。

    Serial.println("test output");
    ErrCode::checkErr(outputInterFace.outputNumberOfBus());
    

}