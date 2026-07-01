#include"err.hpp"


void ErrCode::checkErr(int errCode,HardwareSerial *Serial){
    if(errCode == ErrCode::OK){
        Serial1.println("\tOK");
    }
}