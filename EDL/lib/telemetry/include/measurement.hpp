#pragma once
#include <stdint.h>
#include <stddef.h>
// ArduinoとPC上の契約テストで共用する、ハードウェアに依存しない測定データ。
struct Measurement {
    float temperature = 0, humidity = 0, noise = 0;
    uint16_t eco2 = 0;
    bool temperatureValid = false, humidityValid = false, eco2Valid = false, noiseValid = false;
    uint32_t uptime = 0;
};
// 書き込んだバイト数を返す。出力先が不足する場合は0を返す。
size_t serializeMeasurement(const Measurement& value, char* output, size_t capacity);
