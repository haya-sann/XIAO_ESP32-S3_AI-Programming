#include <Arduino.h>
#include <WiFiManager.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// GPIOピンの定義
const int ledPin1 = 7;
const int ledPin2 = 8;
const int ledPin3 = 9;

// AsyncWebServerオブジェクトを作成
AsyncWebServer server(80);

void setup() {
  // シリアル通信を開始
  Serial.begin(115200);

  // LEDピンを出力モードに設定
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // WiFiManagerオブジェクトを作成
  WiFiManager wifiManager;

  // WiFiアクセスポイントの設定
  #include "credencial.h"

  // WiFiアクセスポイントを開始
  WiFi.softAP(ssid, password);

  // アクセスポイントのIPアドレスを取得
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  // Webサーバーのルートハンドラを設定
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", R"rawliteral(
      <!DOCTYPE HTML><html>
      <head>
      <title>ESP32 LED Control</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
      .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 34px;
      }
      .switch input {
      opacity: 0;
      width: 0;
      height: 0;
      }
      .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      transition: .4s;
      }
      .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      transition: .4s;
      }
      input:checked + .slider {
      background-color: #2196F3;
      }
      input:checked + .slider:before {
      transform: translateX(26px);
      }
      </style>
      </head>
      <body>
      <h1>ESP32 LED Control</h1>
      <p>LED 1: <label class="switch"><input type="checkbox" id="led1" onchange="toggleLED(1, this.checked ? 1 : 0)"><span class="slider"></span></label></p>
      <p>LED 2: <label class="switch"><input type="checkbox" id="led2" onchange="toggleLED(2, this.checked ? 1 : 0)"><span class="slider"></span></label></p>
      <p>LED 3: <label class="switch"><input type="checkbox" id="led3" onchange="toggleLED(3, this.checked ? 1 : 0)"><span class="slider"></span></label></p>
      <script>
      function toggleLED(led, state) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/update?led=" + led + "&state=" + state, true);
      xhr.send();
      }
      </script>
      </body>
      </html>
    )rawliteral");
  });

  // LEDの状態を更新するハンドラを設定
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
    String ledParam = request->getParam("led")->value();
    String stateParam = request->getParam("state")->value();
    int led = ledParam.toInt();
    int state = stateParam.toInt();

    if (led == 1) {
      digitalWrite(ledPin1, state);
    } else if (led == 2) {
      digitalWrite(ledPin2, state);
    } else if (led == 3) {
      digitalWrite(ledPin3, state);
    }

    request->send(200, "text/plain", "OK");
  });

  // サーバーを開始
  server.begin();
}

void loop() {
  // メインループの処理
}
