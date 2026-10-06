#include "telemetry.hpp"
#include <WiFiS3.h>
#if __has_include("network_config.local.hpp")
#include "network_config.local.hpp"
#else
#include "network_config.example.hpp"
#endif
String measurementJson(const Measurement& value) {
    char buffer[384];
    return serializeMeasurement(value, buffer, sizeof(buffer)) ? String(buffer) : String();
}
void Telemetry::begin() {
    configured = strlen(EDL_WIFI_SSID) > 0 && strlen(EDL_DEVICE_KEY) >= 32;
    WiFi.setTimeout(2000);
    if (configured) {
        modem.begin();
        modem.timeout(2500); // WiFi通信チップへの個々のコマンドにも待機上限を設定する。
    }
    Serial.println(configured ? "Telemetry configured" : "Telemetry disabled: configure local credentials (key >= 32 characters)");
}
void Telemetry::publish(const Measurement& value) {
    if (!configured) return;
    // ホットスポット切断中に接続試行を繰り返し続けないよう、間隔を空ける。
    if (WiFi.status() != WL_CONNECTED) {
        if (!attemptedReconnect || static_cast<uint32_t>(millis() - lastReconnect) >= 30000) {
            attemptedReconnect = true; lastReconnect = millis();
            WiFi.begin(EDL_WIFI_SSID, EDL_WIFI_PASSWORD);
        }
        if (WiFi.status() != WL_CONNECTED) return;
    }
    // WiFiファームウェアのCA証明書でHTTPS接続する。検証無効化やHTTPへの切替は行わない。
    WiFiSSLClient client;
    client.setConnectionTimeout(2500);
    client.setTimeout(2500);
    if (!client.connect(EDL_API_HOST, 443)) {
        client.stop(); Serial.println("Telemetry TLS connection failed"); return;
    }
    String body = measurementJson(value);
    String request; request.reserve(body.length() + 300);
    request = "POST " EDL_API_PATH " HTTP/1.1\r\nHost: " EDL_API_HOST "\r\n";
    request += "Content-Type: application/json\r\nX-Device-Key: " EDL_DEVICE_KEY "\r\n";
    request += "Connection: close\r\nContent-Length: " + String(body.length()) + "\r\n\r\n" + body;
    if (client.print(request) != request.length()) {
        client.stop(); Serial.println("Telemetry write failed"); return;
    }
    // 応答の待機時間と長さを制限する。受信確認にはHTTPステータス行だけを使う。
    uint32_t started = millis(); String status; status.reserve(96);
    while (static_cast<uint32_t>(millis() - started) < 2500 && status.length() < 96) {
        if (client.available()) {
            char c = static_cast<char>(client.read());
            if (c == '\n') break;
            status += c;
        } else { delay(5); }
    }
    client.stop();
    bool accepted = status.startsWith("HTTP/1.1 2") || status.startsWith("HTTP/1.0 2");
    Serial.println(accepted ? "Telemetry accepted" : "Telemetry rejected or timed out");
    // 再送キューは持たず次の新しい測定を送る。通信断の期間はサーバー側で欠測になる。
}

