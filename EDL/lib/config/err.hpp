#ifndef ERR_HPP
#define ERR_HPP

    #include<Arduino.h>
    //HardwareSerialを使うために必要

    class ErrCode{
        public:
            static constexpr int EXCEEDED_VALUE_RANGE = -1;
            static constexpr int OK = 0;

            static void checkErr(int errCode,HardwareSerial *Serial);
    };

#endif