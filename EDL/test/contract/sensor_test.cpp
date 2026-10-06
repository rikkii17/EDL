#include "ens160sensor.hpp"
#include <cassert>
#include <cmath>
namespace {
uint8_t crc(const std::vector<uint8_t>& frame) {
    uint8_t result = 0xff;
    for (int i = 0; i < 6; ++i) {
        result ^= frame[i];
        for (int bit = 0; bit < 8; ++bit)
            result = (result & 0x80) ? static_cast<uint8_t>((result << 1) ^ 0x31) : static_cast<uint8_t>(result << 1);
    }
    return result;
}
}
void runSensorTests() {
    TwoWire wire;
    Ens160Sensor::Aht20Request sensor{&wire, 0x38};
    wire.response = {0x08, 0x80, 0x00, 0x08, 0x00, 0x00, 0};
    wire.response[6] = crc(wire.response);
    uint8_t frame[7] = {};
    assert(sensor.getData(frame, 7));
    assert(wire.lastAddress == 0x38);
    assert((wire.written == std::vector<uint8_t>{0xac, 0x33, 0x00}));
    uint32_t temperature, humidity;
    sensor.divideTemperatureAndHumidityData(frame, &temperature, &humidity);
    assert(std::fabs(sensor.temperatureConvertTofloat(temperature) - 50.0f) < 0.001f);
    assert(std::fabs(sensor.humidityConvertTofloat(humidity) - 50.0f) < 0.001f);
    wire.response[6] ^= 1;
    assert(!sensor.getData(frame, 7)); // CRC不一致のデータは変換しない。
    wire.response[6] ^= 1;
    wire.returnedCount = 6;
    assert(!sensor.getData(frame, 7)); // 受信バイト不足。
    wire.returnedCount = 7;
    wire.transmissionStatus = 2;
    assert(!sensor.getData(frame, 7)); // アドレス送信への応答なし。
    wire.transmissionStatus = 0;
    wire.response[0] = 0x88; wire.response[6] = crc(wire.response);
    assert(!sensor.getData(frame, 7)); // CRCが正しくても測定中なら無効。
    wire.response[0] = 0; wire.response[6] = crc(wire.response);
    assert(!sensor.getData(frame, 7)); // 校正完了ビットがない。
    assert(!sensor.getData(frame, 6));
    assert(!sensor.getData(nullptr, 7));
    wire.response[0] = 8; wire.response[6] = crc(wire.response);
    assert(Ens160Sensor::Aht20Request::getReceive(&wire, 0x38, frame, 7));
    wire.registerMode = true; wire.returnedCount = 255;
    wire.registerResponses[0x20] = {0};
    wire.registerResponses[0x24] = {0x8a, 0x02}; // 下位バイトから並ぶ650ppmの値。
    uint16_t eco2 = 99;
    assert(Ens160Sensor::readCheckedEco2(&wire, 0x53, &eco2) && eco2 == 650);
    wire.registerResponses[0x20] = {0x04};
    assert(!Ens160Sensor::readCheckedEco2(&wire, 0x53, &eco2)); // 暖機中。
    wire.registerResponses[0x20] = {0x08};
    assert(!Ens160Sensor::readCheckedEco2(&wire, 0x53, &eco2)); // 初回起動中。
    wire.registerResponses[0x20] = {0x0c};
    assert(!Ens160Sensor::readCheckedEco2(&wire, 0x53, &eco2)); // 無効な状態。
    wire.registerResponses[0x20] = {};
    assert(!Ens160Sensor::readCheckedEco2(&wire, 0x53, &eco2)); // 状態バイトの受信失敗。
    wire.registerResponses[0x20] = {0}; wire.registerResponses[0x24] = {0x8a};
    assert(!Ens160Sensor::readCheckedEco2(&wire, 0x53, &eco2)); // 濃度の受信バイト不足。
    assert(eco2 == 650); // 受信失敗時は最後に成功した出力値を書き換えない。
    wire.registerResponses[0x24] = {0, 0};
    assert(!Ens160Sensor::readCheckedEco2(&wire, 0x53, &eco2)); // センサーの出力範囲を外れる0。
    wire.transmissionStatus = 2;
    assert(!Ens160Sensor::readCheckedEco2(&wire, 0x53, &eco2));
    assert(!Ens160Sensor::readCheckedEco2(nullptr, 0x53, &eco2));
}
