#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_ENS160.h>

#include"interface_main.hpp"
#include"pins_config.hpp"


void setup(){
    Serial.begin(9600);
    delay(5000);
    Serial.println("OutputTest");

}

void loop(){
    InterFace outputInterFace;
    

}