#include"interface_main.hpp"
#include"pins_config.hpp"
#include"err.hpp"



InterFace::InterFace(){

}

int InterFace::begin(){
    pinMode(IndexSelector::TEMPERATURE,OUTPUT);
    pinMode(IndexSelector::HUMIDITY,OUTPUT);
    pinMode(IndexSelector::NOISE,OUTPUT);
    pinMode(IndexSelector::CO2,OUTPUT);
    pinMode(IndexSelector::OVERALL,OUTPUT);

    pinMode(OutputBus::BIT0,OUTPUT);
    pinMode(OutputBus::BIT1,OUTPUT);
    pinMode(OutputBus::BIT2,OUTPUT);

    for(int pinPosition = IndexSelector::TEMPERATURE;pinPosition <= IndexSelector::OVERALL;pinPosition++)   digitalWrite(pinPosition,LOW);
    for(int pinPosition = OutputBus::BIT0;pinPosition <= OutputBus::BIT2;pinPosition++) digitalWrite(pinPosition,LOW);


    //すべての利用するデジタルBピンを出力にする。
    InterFace::testOutput(IndexSelector::TEMPERATURE);
    InterFace::testOutput(IndexSelector::HUMIDITY);
    InterFace::testOutput(IndexSelector::NOISE);
    InterFace::testOutput(IndexSelector::CO2);
    InterFace::testOutput(IndexSelector::OVERALL);

}

int InterFace::testOutput(int index){
    //テスト用の出力関数
    digitalWrite(index, LOW);
    delay(1000);
    //2を出力
    digitalWrite(OutputBus::BIT0, HIGH);
    digitalWrite(index, HIGH);
    delay(1000);
    digitalWrite(index, LOW);
    digitalWrite(OutputBus::BIT0, LOW);

    //4を出力
    digitalWrite(OutputBus::BIT1, HIGH);
    digitalWrite(index, HIGH);
    delay(1000);
    
    //5を出力
    digitalWrite(index, LOW);
    digitalWrite(OutputBus::BIT0, HIGH);
    digitalWrite(index, HIGH);
    delay(1000);

    digitalWrite(OutputBus::BIT0, LOW);
    digitalWrite(OutputBus::BIT1, LOW);
    digitalWrite(index, LOW);
    //4を出力
    digitalWrite(OutputBus::BIT2, HIGH);
    digitalWrite(index, HIGH);
    delay(1000);
    digitalWrite(index, LOW);
    
    if(index != IndexSelector::OVERALL){
        //5を出力
        digitalWrite(OutputBus::BIT0, HIGH);
        digitalWrite(index, HIGH);
        delay(1000);
        digitalWrite(index, LOW);
        digitalWrite(OutputBus::BIT0, LOW);

        //6を出力
        digitalWrite(OutputBus::BIT1, HIGH);
        digitalWrite(index, HIGH);
        delay(1000);
        digitalWrite(index, LOW);
        //7を出力
        digitalWrite(OutputBus::BIT0, HIGH);
        digitalWrite(index, HIGH);
        delay(1000);
    }
    //終了コード
    digitalWrite(index, LOW);
    digitalWrite(OutputBus::BIT0, LOW);
    digitalWrite(OutputBus::BIT1, LOW);
    digitalWrite(OutputBus::BIT2, LOW);
    digitalWrite(index, HIGH);
    digitalWrite(index, LOW);
    return(ErrCode::OK);    
    

}

int InterFace::outputNumberOfBus(int index,int value){
    if(value > 255) return(ErrCode::EXCEEDED_VALUE_RANGE);  //1byteを超えた場合、エラーによって処理を中断
    int8_t value8_t = value;

    //indexの信号をLOWに設定する（レジスタに0を保存させないため
    for(int pinPosition = IndexSelector::TEMPERATURE;pinPosition <= IndexSelector::OVERALL;pinPosition++)   digitalWrite(pinPosition,LOW);
    //値を全てLOWに変えて０として設定する。
    for(int pinPosition = OutputBus::BIT0;pinPosition <= OutputBus::BIT2;pinPosition++) digitalWrite(pinPosition,LOW);

    //それぞれのBITで信号を与える。
    if((value >> 0) & 1)    digitalWrite(OutputBus::BIT0,HIGH);
    if((value >> 1) & 1)    digitalWrite(OutputBus::BIT1,HIGH);
    if((value >> 2) & 1)    digitalWrite(OutputBus::BIT2,HIGH);
    //信号を任意のレジスタに保存させる。
    digitalWrite(index,HIGH);

    return(ErrCode::OK);

}
