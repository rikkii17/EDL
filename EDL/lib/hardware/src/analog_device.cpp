#include"analog_device.hpp"
#include"speaker_const.hpp"

float AnalogReader::convertToVoltage(int rawVoltage){
    return ((5.0/1023.0)*rawVoltage);
    //分解能をもとにしきい値を電圧に変換する
}

static float AnalogReader::voltageToVolume(float voltage){
    return(94 + 20*log10(voltage/(speakerConst::boardGain*6.31e-3)));
}