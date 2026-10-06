#pragma once
#include <stdint.h>
#include <vector>
// NACK・受信不足・応答データを再現する、テスト専用の模擬I2C。
class TwoWire {
public:
    std::vector<uint8_t> response, written;
    uint8_t transmissionStatus = 0, returnedCount = 7, lastAddress = 0;
    size_t position = 0;
    bool registerMode = false;
    std::vector<uint8_t> registerResponses[256];
    void beginTransmission(uint8_t address) { lastAddress = address; written.clear(); }
    size_t write(uint8_t value) { written.push_back(value); return 1; }
    uint8_t endTransmission() { return transmissionStatus; }
    uint8_t requestFrom(uint8_t address, uint8_t) {
        lastAddress = address; position = 0;
        if (registerMode && !written.empty()) response = registerResponses[written[0]];
        return returnedCount == 255 ? static_cast<uint8_t>(response.size()) : returnedCount;
    }
    int read() { return position < response.size() ? response[position++] : -1; }
};
