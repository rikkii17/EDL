#include"interface_main.hpp"
#include"pins_config.hpp"

InterFace::InterFace(){
    pinMode(IndexSelector::TEMPERATURE,OUTPUT);
    pinMode(IndexSelector::HUMIDITY,OUTPUT);
    pinMode(IndexSelector::NOISE,OUTPUT);
    pinMode(IndexSelector::CO2,OUTPUT);
    pinMode(IndexSelector::OVERALL,OUTPUT);

    pinMode(OutputBus::BIT0,OUTPUT);
    pinMode(OutputBus::BIT1,OUTPUT);
    pinMode(OutputBus::BIT2,OUTPUT);


    //すべての利用するデジタルBピンを出力にする。
    InterFace::testOutput(IndexSelector::TEMPERATURE);
    InterFace::testOutput(IndexSelector::HUMIDITY);
    InterFace::testOutput(IndexSelector::NOISE);
    InterFace::testOutput(IndexSelector::CO2);
    InterFace::testOutput(IndexSelector::OVERALL);

}
/*
void InterFace::begin(){
    pinMode(IndexSelector::TEMPERATURE,OUTPUT);
    pinMode(IndexSelector::HUMIDITY,OUTPUT);
    pinMode(IndexSelector::NOISE,OUTPUT);
    pinMode(IndexSelector::CO2,OUTPUT);
    pinMode(IndexSelector::OVERALL,OUTPUT);

    pinMode(OutputBus::BIT0,OUTPUT);
    pinMode(OutputBus::BIT1,OUTPUT);
    pinMode(OutputBus::BIT2,OUTPUT);


    //すべての利用するデジタルBピンを出力にする。
    InterFace::testOutput(IndexSelector::TEMPERATURE);
    InterFace::testOutput(IndexSelector::HUMIDITY);
    InterFace::testOutput(IndexSelector::NOISE);
    InterFace::testOutput(IndexSelector::CO2);
    InterFace::testOutput(IndexSelector::OVERALL);
}
*/

int InterFace::testOutput(int index){
    //テスト用の出力関数
    digitalWrite(index, HIGH);
    delay(1000);
    //2を出力
    digitalWrite(OutputBus::BIT0, HIGH);
    delay(1000);
    digitalWrite(OutputBus::BIT0, LOW);
    //4を出力
    digitalWrite(OutputBus::BIT1, HIGH);
    delay(1000);
    //5を出力
    digitalWrite(OutputBus::BIT0, HIGH);
    delay(1000);
    digitalWrite(OutputBus::BIT0, LOW);
    digitalWrite(OutputBus::BIT1, LOW);
    //4を出力
    digitalWrite(OutputBus::BIT2, HIGH);
    delay(1000);
    
    if(index != IndexSelector::OVERALL){
        //5を出力
        digitalWrite(OutputBus::BIT0, HIGH);
        delay(1000);
        digitalWrite(OutputBus::BIT0, LOW);
        //6を出力
        digitalWrite(OutputBus::BIT1, HIGH);
        delay(1000);
        //7を出力
        digitalWrite(OutputBus::BIT0, HIGH);
        delay(1000);
    }
    //終了コード
    digitalWrite(OutputBus::BIT0, LOW);
    digitalWrite(OutputBus::BIT1, LOW);
    digitalWrite(OutputBus::BIT2, LOW);
    

}

int InterFace::outputNumberOfBus(int index,int value){
    
}
