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
    String inputIndex;
    int indexBus;
    String inputValue;
    while(true){
    Serial.println("Prease Write the Index:");
    if(Serial.available()>0)    inputIndex = Serial.readStringUntil('\n');

    if(inputIndex == "温度")        indexBus = IndexSelector::TEMPERATURE;
    else if(inputIndex == "湿度")   indexBus = IndexSelector::HUMIDITY;
    else if(inputIndex == "CO2")    indexBus = IndexSelector::CO2;
    else if(inputIndex == "騒音")   indexBus = IndexSelector::NOISE;
    else if(inputIndex == "総合")   indexBus = IndexSelector::OVERALL;
    else{
        Serial.println("Input err");
        continue;
    }
    break;
    }
    while (true){
    Serial.println("Prease Write the Value:");
    if(Serial.available()>0)    inputValue = Serial.readStringUntil('\n');
    

    break;
    }

    Serial.println("test output");
    ErrCode::checkErr(outputInterFace.outputNumberOfBus());
    

}