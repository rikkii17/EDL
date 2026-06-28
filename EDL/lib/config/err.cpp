#include"err.hpp"


void checkErr(int errCode,HardwareSerial *Serial){
    if(errCode == ErrCode::OK){
        Serial1.println("\tOK");
    }
}