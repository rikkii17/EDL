#include "measurement.hpp"
#include "conversion_data.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <limits>
void runSensorTests();
int main() {
    runSensorTests();
    Measurement sample;
    sample.temperature = 23.25f; sample.humidity = 50.75f;
    sample.eco2 = 650; sample.noise = 42.5f; sample.uptime = 123456;
    sample.temperatureValid = sample.humidityValid = sample.eco2Valid = sample.noiseValid = true;
    char output[384];
    assert(serializeMeasurement(sample, output, sizeof(output)) == std::strlen(output));
    assert(std::strstr(output, "\"temperature_c\":23.25"));
    assert(std::strstr(output, "\"humidity_pct\":50.75"));
    assert(std::strstr(output, "\"eco2_ppm\":650"));
    assert(std::strstr(output, "\"noise_db\":42.50"));
    std::puts(output); // 1行目は実際のシリアライザが生成するHTTP結合テスト用データ。
    sample.temperature = 25.25f; sample.humidity = 54.75f;
    sample.eco2 = 850; sample.noise = 46.5f; sample.uptime += 10000;
    serializeMeasurement(sample, output, sizeof(output));
    std::puts(output); // 2行目はサーバー集計テスト用の2件目の有効値。
    Measurement missing;
    missing.uptime = 4294967295U;
    serializeMeasurement(missing, output, sizeof(output));
    assert(std::strstr(output, "\"temperature_c\":null"));
    assert(std::strstr(output, "\"uptime_ms\":4294967295"));
    std::puts(output); // 3行目はセンサー切断を表す全項目nullのデータ。
    sample.temperature = std::numeric_limits<float>::quiet_NaN();
    sample.humidity = std::numeric_limits<float>::infinity();
    sample.noise = -1; sample.eco2 = 65001;
    serializeMeasurement(sample, output, sizeof(output));
    assert(std::strstr(output, "\"temperature\":false"));
    assert(std::strstr(output, "\"humidity\":false"));
    assert(std::strstr(output, "\"eco2\":false"));
    assert(std::strstr(output, "\"noise\":false"));
    char tiny[8];
    assert(serializeMeasurement(sample, tiny, sizeof(tiny)) == 0 && tiny[0] == '\0');
    assert(serializeMeasurement(sample, nullptr, 0) == 0);
    // 回帰確認：基準値未満の温湿度で符号なし整数が桁あふれしていた不具合。
    assert(Conversion::temperature(21) == 7);
    assert(Conversion::temperature(25) == 7);
    assert(Conversion::humidity(44) == 7);
    assert(Conversion::humidity(56) == 7);
    assert(Conversion::temperature(-40) == 1);
    assert(Conversion::co2(0) == 8);
    assert(Conversion::co2(65535) == 1);
    assert(Conversion::noise(0) == 8);
    assert(Conversion::noise(150) == 1);
    assert(Conversion::noise(37.49f) == 8);
    assert(Conversion::noise(37.5f) == 7);
    assert(Conversion::temperature(std::numeric_limits<float>::quiet_NaN()) == 0);
    assert(Conversion::overall(8, 8, 8, 8) == 4);
    std::fprintf(stderr, "Firmware contract and evaluation tests passed\n");
}
