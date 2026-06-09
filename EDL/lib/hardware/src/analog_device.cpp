#include"analog_device.hpp"

float AnalogReader::convertToVoltage(int rawVoltage){
    return ((5.0/1023.0)*rawVoltage);
}