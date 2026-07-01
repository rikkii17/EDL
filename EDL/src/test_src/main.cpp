#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_ENS160.h>
#include<cpp_helper.hpp>

#include"interface_main.hpp"
#include"pins_config.hpp"
#include"err.hpp"

InterFace outputInterFace;


void setup(){
    Serial.begin(9600);
    delay(5000);
    Serial.println("Test");
    Serial.print("\ttest Interface:"); 
    outputInterFace.begin();
    Serial.println("\tOK");

}

void loop(){
    InterFace outputInterFace;
    String inputIndex;
    int indexBus;
    String inputValue;
    int valueBus;
    while(true){
    Serial.println("Prease Write the Index:");
    while(Serial.available() == 0)  delay(100);
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
    while(Serial.available() == 0)  
    if(Serial.available()>0)    inputValue = Serial.readStringUntil('\n');

    if(!TypeConverter::checkTypeToInt(inputValue.c_str())){
        Serial.println("Input err");
        continue;
    }
    valueBus = inputValue.toInt();
    if(valueBus > 8 || valueBus < 0){
        Serial.println("Input err");
        continue;
    }

    break;
    }

    Serial.println("test output");
    ErrCode::checkErr(outputInterFace.outputNumberOfBus(indexBus,valueBus),&Serial);
    delay(100000);
    

}