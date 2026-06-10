



#include <Wire.h>
#include <DFRobot_ENS160.h>

// ENS160 I2C address
// 基本は 0x53
// I2Cスキャンで 0x52 と出たら 0x52 に変更
DFRobot_ENS160_I2C ens160(&Wire, 0x53);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("ENS160 eCO2 Sensor Test");
  Serial.println("-----------------------");

  Wire.begin();

  Serial.println("I2C scan start...");

  bool found_sensor = false;

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");

      if (address < 16) {
        Serial.print("0");
      }

      Serial.println(address, HEX);

      if (address == 0x52 || address == 0x53) {
        found_sensor = true;
      }
    }
  }

  if (found_sensor) {
    Serial.println("ENS160 candidate address found.");
  } else {
    Serial.println("ENS160 address was not found.");
    Serial.println("Check wiring, power, and I2C address.");
  }

  Serial.println();
  Serial.println("ENS160 begin...");

  while (ens160.begin() != 0) {
    Serial.println("ENS160 communication failed.");
    Serial.println("Retry after 3 seconds...");
    delay(3000);
  }

  Serial.println("ENS160 begin OK.");

  ens160.setPWRMode(ENS160_STANDARD_MODE);

  // 温湿度補正値
  // 温湿度センサがない場合は固定値でOK
  ens160.setTempAndHum(25.0, 50.0);

  Serial.println("ENS160 standard mode set.");
  Serial.println("Temp compensation: 25.0 C");
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