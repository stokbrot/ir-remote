
#include "web.h"



const char* ssid = "esp32";
const char* password = "password";

WebServer server(80);

#define RECV_PIN 14
#define SEND_PIN 21

IRrecv irrecv(RECV_PIN);
IRsend irsend(SEND_PIN);

decode_results results;

// ---------- STRUCT ----------
struct IRCommand {
  decode_type_t protocol;
  uint64_t value;
  uint32_t address;
  uint32_t command;
  uint16_t bits;
  uint16_t rawLen;
  uint16_t rawData[100];  // enough for most remotes
};

IRCommand lastCommand;

// define the shared JSON document and LED pin (declared extern in web.h)
DynamicJsonDocument lastDoc(2048);
const int ledPin = 2;
bool record = false;

void storeCommand(decode_results *results) {
  lastDoc.clear();

  // populate JSON document for the web endpoint
  lastDoc["protocol"] = typeToString(results->decode_type);
  lastDoc["value"] = (uint32_t)results->value;
  lastDoc["address"] = results->address;
  lastDoc["command"] = results->command;
  lastDoc["bits"] = results->bits;

  // results->rawbuf uses 0.5µs ticks and usually starts at index 1.
  // Skip the start-offset placeholder and convert to real microseconds.
  uint16_t copyLen = results->rawlen;
  if (copyLen > 0) copyLen -= 1;
  if (copyLen > 100) copyLen = 100;
  lastDoc["rawLen"] = copyLen;

  JsonArray raw = lastDoc.createNestedArray("raw");

  for (int i = 0; i < copyLen; i++) {
    uint16_t val = (uint16_t)(results->rawbuf[i + kStartOffset] * kRawTick);
    raw.add(val);
    lastCommand.rawData[i] = val;
  }

  // fill remaining command metadata
  lastCommand.protocol = results->decode_type;
  lastCommand.value = results->value;
  lastCommand.address = results->address;
  lastCommand.command = results->command;
  lastCommand.bits = results->bits;
  lastCommand.rawLen = copyLen;
}

// ---------- PRINT ----------
void printCommand(IRCommand &cmd) {

  Serial.println("========== STORED COMMAND ==========");

  Serial.print("Protocol: ");
  Serial.println(typeToString(cmd.protocol));

  Serial.print("Value: 0x");
  Serial.println((uint32_t)cmd.value, HEX);

  Serial.print("Address: 0x");
  Serial.println(cmd.address, HEX);

  Serial.print("Command: 0x");
  Serial.println(cmd.command, HEX);

  Serial.print("Bits: ");
  Serial.println(cmd.bits);

  Serial.print("RawLen: ");
  Serial.println(cmd.rawLen);

  Serial.print("RAW DATA: ");
  for (int i = 0; i < cmd.rawLen; i++) {
    Serial.print(String(cmd.rawData[i]) + ",");
  }

  Serial.println("\n ====================================");
}

// ---------- REPLAY ----------
void replayCommand(IRCommand &cmd) {

  Serial.println("REPLAYING COMMAND...");

  irsend.sendRaw(cmd.rawData, cmd.rawLen, 38);

  Serial.println("DONE");
}



void setup() {

  Serial.begin(115200);

  while (!Serial)
    delay(50);

  irrecv.enableIRIn();
  irsend.begin();

  Serial.println("IR system ready");

  pinMode(ledPin, OUTPUT);

  WiFi.softAP(ssid, password);

  Serial.println(WiFi.softAPIP());

  setupWeb(server);
  server.begin();
}

void loop() {

  if (record && irrecv.decode(&results)) {

    storeCommand(&results);
    printCommand(lastCommand);
    record = false;
    digitalWrite(ledPin, LOW);
    irrecv.resume();
  }

  server.handleClient();

  delay(1);
}