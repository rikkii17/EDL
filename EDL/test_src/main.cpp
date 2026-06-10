#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_ENS160.h>

// ENS160のI2Cアドレス
// 通常: 0x53
// SDOピンをGNDに落としている場合: 0x52
DFRobot_ENS160_I2C ens160(&Wire, 0x53);

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("================================");
    Serial.println("ENS160 eCO2 Sensor Test");
    Serial.println("================================");

    Wire.begin();

    Serial.println("I2C scan start...");

    bool found_sensor = false;

    for (uint8_t address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at 0x");

            if (address < 16) {
                Serial.print("0");
            }

            Serial.println(address, HEX);

            if (address == 0x53 || address == 0x52) {
                found_sensor = true;
            }
        }
    }

    if (found_sensor) {
        Serial.println("ENS160 candidate address found.");
    } else {
        Serial.println("ENS160 address was not found.");
        Serial.println("Check SDA / SCL / VCC / GND / I2C address.");
    }

    Serial.println("ENS160 begin...");

    while (ens160.begin() != NO_ERR) {
        Serial.println("ENS160 communication failed.");
        Serial.println("Retry after 3 seconds...");
        delay(3000);
    }

    Serial.println("ENS160 begin OK.");

    ens160.setPWRMode(ENS160_STANDARD_MODE);

    // 温湿度補正値
    // 温湿度センサがない場合は、とりあえず固定値でよい
    ens160.setTempAndHum(25.0, 50.0);

    Serial.println("ENS160 standard mode set.");
    Serial.println("Temperature compensation: 25.0 C");
    Serial.println("Humidity compensation: 50.0 %RH");
    Serial.println();

    Serial.println("status, AQI, TVOC(ppb), eCO2(ppm)");
}

void loop() {
    uint8_t status = ens160.getENS160Status();
    uint8_t aqi = ens160.getAQI();
    uint16_t tvoc = ens160.getTVOC();
    uint16_t eco2 = ens160.getECO2();

    Serial.print("status: ");
    Serial.print(status);

    Serial.print(" | AQI: ");
    Serial.print(aqi);

    Serial.print(" | TVOC: ");
    Serial.print(tvoc);
    Serial.print(" ppb");

    Serial.print(" | eCO2: ");
    Serial.print(eco2);
    Serial.println(" ppm");

    if (status == 0) {
        Serial.println("Sensor status: normal operation");
    } else if (status == 1) {
        Serial.println("Sensor status: warm-up phase");
    } else if (status == 2) {
        Serial.println("Sensor status: initial startup phase");
    } else {
        Serial.println("Sensor status: invalid output");
    }

    Serial.println();

    delay(1000);
}