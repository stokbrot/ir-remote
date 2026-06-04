#include "web.h"

const char PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <style>
    body {
      margin: 0;
      font-family: "Consolas", "Courier New", monospace;
      background: #1E1E1E;
      color: white;
      text-align: center;
    }

    h2 {
      margin: 20px 0;
      font-size: 22px;
    }

    .btn {
      display: block;
      width: 90%;
      margin: 12px auto;
      padding: 18px;
      font-size: 20px;
      border: none;
      border-radius: 10px;
      background: #4aa3ff;
      color: white;
      text-decoration: none;
    }

    .btn:active {
      transform: scale(0.98);
      background: #1b4fa8;
    }
  </style>
</head>

<body>

  <h2>ESP32 Remote</h2>

  <a class="btn" href="/record">RECORD</a>
  


  

</body>
</html>
)rawliteral";

void setupWeb(WebServer& server){

    server.on("/", [&](){
      server.send(200, "text/html", PAGE);
    });

    server.on("/record", [&](){
      record = true;
      digitalWrite(ledPin, HIGH);
      server.sendHeader("Location", "/");
      server.send(303);
    });

    

    server.on("/last", HTTP_GET, [&]() {

      String json;
      serializeJson(lastDoc, json);
      server.send(200, "application/json", json);

    });

    server.on("/sendRaw", HTTP_POST, [&]() {
      String body = server.arg("plain");

      StaticJsonDocument<2048> doc;
      DeserializationError err = deserializeJson(doc, body);
      if (err) {
        server.send(400, "text/plain", "invalid json");
        return;
      }

      JsonArray raw = doc["raw"].as<JsonArray>();

      uint16_t buffer[100];
      int len = raw.size();
      if (len > 100) len = 100;

      for (int i = 0; i < len; i++) {
        buffer[i] = raw[i];
        Serial.println(buffer[i]);
      }


      

      uint16_t freq = doc["freq"] | 38;

      irsend.sendRaw(buffer, len, freq);

      server.send(200, "text/plain", "OK");
    });


}