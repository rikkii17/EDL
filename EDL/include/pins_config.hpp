#ifndef PINS_CONFIG_HPP
    #define PINS_CONFIG_HPP
    
    #include "pins_arduino.h"
    //Arduinoのピン位置を定義しているファイル。

    //各指標表示時の信号先セレクタピンの定義
    #define TEMPERATURE_INDEX 0
    #define HUMIDITY_INDEX 1
    #define NOISE_INDEX 2
    #define CO2_INDEX 3
    #define OVERALL_INDEX 4

    //Output bus に使うvalueピンの定義
    #define OUTPUT_BIT0 5
    #define OUTPUT_BIT1 6
    #define OUTPUT_BIT2 7

    //インターフェースに対してデータを出力するときに種類を決定させるセレクタピン定義
    class IndexSelector{
        public:
            static constexpr int TEMPERATURE =TEMPERATURE_INDEX;
            static constexpr int HUMIDITY = HUMIDITY_INDEX;
            static constexpr int NOISE = NOISE_INDEX;
            static constexpr int CO2 = CO2_INDEX;
            static constexpr int OVERALL = OVERALL_INDEX;
    };

    //インターフェースに値データを出力するときに使用する出力バスの定義
    class OutputBus{
        public:
            static constexpr int BIT0 = OUTPUT_BIT0;
            static constexpr int BIT1 = OUTPUT_BIT1;
            static constexpr int BIT2 = OUTPUT_BIT2;
    };

#endif //PINS_CONFIG_HPP