#ifndef INTERFACE_MAIN_HPP

#define INTERFACE_MAIN_HPP

    #include <Arduino.h>    
    #include "pins_config.hpp"

    class InterFace{
        private:
            static constexpr int MAX_VALUE = 7;
            static constexpr int NUMBER_OF_BUS = 5;

            static int testOutput(int index);

        public:
            InterFace();    //コンストラクタ

            int begin();  //初期化関数
            
            int outputNumberOfBus(int index,int value);

    };

#endif //INTERFACE_MAIN_HPP