#include "measurement.hpp"
#include <cmath>
#include <cstdio>
namespace {
const char* jsonBoolean(bool value) { return value ? "true" : "false"; }
bool inRange(float value, float minimum, float maximum) {
    return std::isfinite(value) && value >= minimum && value <= maximum;
}
void numberOrNull(char* output, size_t capacity, float value, bool valid) {
    if (valid) std::snprintf(output, capacity, "%.2f", static_cast<double>(value));
    else std::snprintf(output, capacity, "null");
}
}
size_t serializeMeasurement(const Measurement& v, char* output, size_t capacity) {
    if (!output || capacity == 0) return 0;
    // NaNや無限大が混入しても不正なJSONを送らないよう、値と有効フラグを再確認する。
    bool temperature = v.temperatureValid && inRange(v.temperature, -40, 125);
    bool humidity = v.humidityValid && inRange(v.humidity, 0, 100);
    bool eco2 = v.eco2Valid && v.eco2 <= 65000;
    bool noise = v.noiseValid && inRange(v.noise, 0, 150);
    char temperatureText[24], humidityText[24], eco2Text[24], noiseText[24];
    numberOrNull(temperatureText, sizeof(temperatureText), v.temperature, temperature);
    numberOrNull(humidityText, sizeof(humidityText), v.humidity, humidity);
    numberOrNull(noiseText, sizeof(noiseText), v.noise, noise);
    if (eco2) std::snprintf(eco2Text, sizeof(eco2Text), "%u", static_cast<unsigned>(v.eco2));
    else std::snprintf(eco2Text, sizeof(eco2Text), "null");
    int length = std::snprintf(output, capacity,
        "{\"device_id\":\"edl-arduino\",\"uptime_ms\":%lu,"
        "\"temperature_c\":%s,\"humidity_pct\":%s,\"eco2_ppm\":%s,\"noise_db\":%s,"
        "\"validity\":{\"temperature\":%s,\"humidity\":%s,\"eco2\":%s,\"noise\":%s}}",
        static_cast<unsigned long>(v.uptime), temperatureText, humidityText, eco2Text, noiseText,
        jsonBoolean(temperature), jsonBoolean(humidity), jsonBoolean(eco2), jsonBoolean(noise));
    if (length < 0 || static_cast<size_t>(length) >= capacity) {
        output[0] = '\0'; return 0;
    }
    return static_cast<size_t>(length);
}
