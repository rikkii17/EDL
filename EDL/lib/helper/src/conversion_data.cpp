#include"conversion_data.hpp"

namespace {
// 符号付きの小数計算で基準値未満の桁あふれを防ぐ。0は欠測を示す。
int score(float deviation) {
    if (!std::isfinite(deviation)) return 0;
    int result = 8 - static_cast<int>(std::floor(deviation));
    return result < 1 ? 1 : (result > 8 ? 8 : result);
}
}
int Conversion::temperature(float data) {
    return score(std::fabs(data - ReferenceValue::TEMPERATURE) / Evaluation_Unit::TEMPERATURE);
}
int Conversion::humidity(float data) {
    return score(std::fabs(data - ReferenceValue::HUMIDITY) / Evaluation_Unit::HUMIDITY);
}
int Conversion::noise(float data) {
    // 既存のマイク評価の丸めしきい値を保ち、最後に表示範囲へ制限する。
    if (!std::isfinite(data)) return 0;
    int result = 8 - static_cast<int>(std::round((data - ReferenceValue::NOISE) / Evaluation_Unit::NOISE));
    return result < 1 ? 1 : (result > 8 ? 8 : result);
}
int Conversion::co2(uint16_t data) {
    return score(std::fmax(0.0f, static_cast<float>(data) - ReferenceValue::CO2) / Evaluation_Unit::CO2);
}

int Conversion::overall(int temperature,int humidity,int eco2,int noise){
    float returnData = (temperature + humidity + noise + eco2) / 4.0f;

    if(returnData <= 2) return(1);
    else if(returnData<= 4 && returnData > 2)   return(2);
    else if(returnData <= 6 && returnData > 4)  return(3);
    else if(returnData > 6) return(4);
    else    return(0);

}