#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    delay(1000);

    Serial.println("AHT20 test start");

    if (!aht.begin(&Wire))
    {
        Serial.println("AHT20 initialization FAILED");

        while (true)
        {
            delay(1000);
        }
    }

    Serial.println("AHT20 initialization SUCCESS");
}

void loop()
{
    sensors_event_t humidity;
    sensors_event_t temperature;

    aht.getEvent(&humidity, &temperature);

    Serial.print("Temperature: ");
    Serial.print(temperature.temperature);
    Serial.println(" degC");

    Serial.print("Humidity: ");
    Serial.print(humidity.relative_humidity);
    Serial.println(" %");

    Serial.println("--------------------");

    delay(3000);
}