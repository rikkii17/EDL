#pragma once
#include <Arduino.h>
#include "measurement.hpp"
String measurementJson(const Measurement& value);
class Telemetry {
public:
    void begin();
    void publish(const Measurement& value);
private:
    uint32_t lastReconnect = 0;
    bool attemptedReconnect = false;
    bool configured = false;
};
