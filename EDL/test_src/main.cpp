//test_src/main.cpp

#include <Arduino.h>
#include "hardware_helper.hpp"
#include<DFRobot_ENS160.h>

DFRobot_ENS160_I2C ens160_i2c(&Wire, 0x52); //ENS160_I2Cクラスのインスタンス化。引数はI2C通信のためのWireオブジェクトと、SDOピンの接続に応じたI2Cアドレス。
//ENS160_I2Cクラスのコンストラクタ。

void setup() {
  Serial.begin(9600);
  //PCとの通信に必要。

  if (ens160_i2c.begin() != 0) { //ENS160センサーの初期化。begin()メソッドはセンサーの初期化を行い、成功した場合は0を返す。
    Serial.println("Failed to initialize ENS160 sensor!"); //初期化に失敗した場合のエラーメッセージ。
    while (1); //無限ループで停止。
  }

}

void loop() {
  /*
  Analog通信のテストコード
  float A0_value = AnalogReader::convertToVoltage(analogRead(A0));
  Serial.print("A0_value: ");
  Serial.println(A0_value);
  //Serial.println("\n");
  delay(1000);
  */


}