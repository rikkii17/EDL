# EDL Wi-Fi telemetry (`ex-web`)

## 構成

- `main.cpp`: 既存の測定・Serialログ・10秒待機・評価表示を維持し、最後にHTTPS送信を追加。
- `lib/hardware/ens160sensor.*`: 既存AHT20取得のCRC／受信数チェックを修正。ENS160の短い受信を検出する補助関数を追加。
- `lib/telemetry/measurement_json.cpp`: Arduinoに依存しないJSON契約。固定長バッファ、非有限値・範囲外はnull。
- `lib/telemetry/telemetry.cpp`: WiFiS3によるHTTPS送信と再接続。
- `lib/helper/conversion_data.cpp`: 評価計算。符号なし整数の減算による桁あふれを防ぎ、1〜8に制限する。

## 既存実装の継承

AHT20の換算式、ENS160の使用アドレスと通常モード、マイク電圧換算式、
GPIOのセレクタ／出力バス、起動時の表示テスト、10秒周期を継承する。
温度23℃／単位2、湿度50%／単位6、eCO2 400ppm／単位575、騒音35dB／単位5の評価基準を保つ。
騒音のroundによるしきい値も維持する。温湿度は符号なし減算の不具合を直し、小数を保つ。
既存の起動I2Cスキャン・初期化・表示テスト・Serialログは保持する。
無期限の初期化再試行だけ最大3回に、暖機の繰り返し待ちは最大60秒に制限する。
センサー故障で全体が停止する不具合を避けるための最小変更。
AHT20状態値は1バイトへ修正し、校正状態は値1ではなくbit 0x08で判定する。

## ローカル設定

`EDL/lib/telemetry/include/network_config.example.hpp`を同じ場所の
`network_config.local.hpp`へコピーし、ホットスポットのSSID・パスワードと送信キーを記入する。
ローカル設定はGit除外済み。空の設定でもビルド可能で、その場合はネットワーク送信を停止する。
送信キーはサイトで生成した32文字以上のランダム値を使用し、ブラウザ用の管理パスワードとは分ける。
ドメインやAPIパスはこのファイルで設定する。Hostはスキーム・パスを含まない名前を使用する。

Windowsのモバイルホットスポットは2.4GHz、WPA2を選択する。
HTTPS用のCA bundleはUNO R4 WiFiの接続用ファームウェアが管理する。
証明書エラー時はHTTPへ切り替えず、Arduino IDE等でWiFiファームウェアの更新・証明書対応を確認する。
秘密値はシリアルへ出力しない。

## 通信と欠測

`POST https://edl-site.ricky17.com/api/ingest.php`、認証ヘッダーは`X-Device-Key`。
JSONは`device_id=edl-arduino`、`uptime_ms`、`temperature_c`、`humidity_pct`、
`eco2_ppm`、`noise_db`と`validity`（temperature/humidity/eco2/noiseのbool）を送る。
欠測は値がnull、対応flagはfalse。端末時計を使わずサーバー受信時刻を採用する。

AHT20は7バイトの受信数・busy・calibration・CRCを確認してから変換する。
ENS160は初期化・暖機中・初回起動中を欠測とし、正常時だけ等価CO2を送信する。
eCO2は実測CO2とは異なる。マイクのdB値は既存の電圧換算式による参考値で、校正済みの騒音計ではない。
ADCは10bitに明示設定し、0／飽和値／非有限値を欠測とする。

ネットワーク接続を待ち続けるループは設けず、再接続は30秒以上空ける。
WiFi接続待ち2秒、TLS接続と各モデムコマンド2.5秒、HTTP応答待ち2.5秒を設定する。
WiFiS3自体は同期APIのため、個別のコマンド時間が積み重なる場合は測定周期が遅れる。
既存の10秒待機は保持する。通信処理が加わるため送信周期は約10秒＋測定／通信時間になる。
障害中もローカル測定・表示は続け、再接続時は次の新しい測定だけを送信する。
古い値の再送はせず履歴は欠測として残る。再接続の経過時間判定は`millis()`の49日周期の桁あふれにも対応する。

## 表示・評価

個別スコアは1〜8、欠測は0。既存3bit出力バスは8を`000`として送る方式を維持する。
そのため物理表示だけでは最高評価8と欠測0のビット列が一致する。Webでは欠測を明確に区別する。
負数や表示範囲外の評価は1〜8へ制限し、異常値がGPIO出力へ流れる不具合を防ぐ。
総合評価は全4項目が有効な場合だけ1〜4を表示し、欠測がある場合は0とする。
起動時の既存表示テストは維持しているため、最初の測定までテスト時間がかかる。

## ビルドと検証

```powershell
# プロジェクトのルートから
& "$env:USERPROFILE/.platformio/penv/Scripts/platformio.exe" run -d EDL -e main
# ホストC++コンパイラで実シリアライザと評価の回帰を検証
./EDL/test/contract/run.ps1
```

native testは`.pio/firmware_fixture.jsonl`へ有効2件・欠測1件を出力する。
サイトのHTTP結合テストはこの実装由来のfixtureを利用する。
模擬I2Cで実AHT20取得コードのCRC、短い受信、NACK、busy、未校正と正常値変換も検証する。
コンパイラの場所が異なる場合は`run.ps1 -Compiler <clang++等のパス>`で指定する。
実機のセンサー・WiFi・公開ドメインのTLS接続は配置後に別途確認が必要。
