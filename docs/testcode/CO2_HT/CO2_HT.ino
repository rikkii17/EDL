#include <Wire.h>

const uint8_t AHT20_ADDRESS = 0x38;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("AHT20 / AHT21 Temperature Humidity Test");
  Serial.println("---------------------------------------");

  Wire.begin();

  // I2C scan
  Serial.println("I2C scan start...");

  bool found_aht20 = false;

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");

      if (address < 16) {
        Serial.print("0");
      }

      Serial.println(address, HEX);

      if (address == AHT20_ADDRESS) {
        found_aht20 = true;
      }
    }
  }

  if (found_aht20) {
    Serial.println("AHT20 / AHT21 found at 0x38.");
  } else {
    Serial.println("AHT20 / AHT21 was not found.");
    Serial.println("Check wiring and I2C address.");
  }

  delay(100);

  // AHT20 initialization
  Wire.beginTransmission(AHT20_ADDRESS);
  Wire.write(0xBE);
  Wire.write(0x08);
  Wire.write(0x00);
  uint8_t init_error = Wire.endTransmission();

  if (init_error == 0) {
    Serial.println("AHT20 init command sent.");
  } else {
    Serial.print("AHT20 init failed. I2C error: ");
    Serial.println(init_error);
  }

  delay(100);

  Serial.println();
  Serial.println("humidity(%RH), temperature(C)");
}

void loop() {
  // 測定開始コマンド
  Wire.beginTransmission(AHT20_ADDRESS);
  Wire.write(0xAC);
  Wire.write(0x33);
  Wire.write(0x00);
  uint8_t trigger_error = Wire.endTransmission();

  if (trigger_error != 0) {
    Serial.print("Measurement trigger failed. I2C error: ");
    Serial.println(trigger_error);
    delay(1000);
    return;
  }

  delay(80);

  Wire.requestFrom(AHT20_ADDRESS, (uint8_t)6);

  if (Wire.available() < 6) {
    Serial.println("Read failed. Not enough data.");
    delay(1000);
    return;
  }

  uint8_t data[6];

  for (int i = 0; i < 6; i++) {
    data[i] = Wire.read();
  }

  uint8_t status = data[0];

  // status bit 7 が 1 の場合は busy
  if ((status & 0x80) != 0) {
    Serial.println("Sensor is busy.");
    delay(1000);
    return;
  }

  // 湿度 20bit
  uint32_t raw_humidity = 0;
  raw_humidity |= ((uint32_t)data[1] << 12);
  raw_humidity |= ((uint32_t)data[2] << 4);
  raw_humidity |= ((uint32_t)data[3] >> 4);

  // 温度 20bit
  uint32_t raw_temperature = 0;
  raw_temperature |= (((uint32_t)data[3] & 0x0F) << 16);
  raw_temperature |= ((uint32_t)data[4] << 8);
  raw_temperature |= data[5];

  float humidity = ((float)raw_humidity / 1048576.0) * 100.0;
  float temperature = ((float)raw_temperature / 1048576.0) * 200.0 - 50.0;

  Serial.print("Humidity: ");
  Serial.print(humidity, 2);
  Serial.print(" %RH");

  Serial.print(" | Temperature: ");
  Serial.print(temperature, 2);
  Serial.println(" C");

  delay(1000);
}