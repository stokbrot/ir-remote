#pragma once
#include <WebServer.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>

extern DynamicJsonDocument lastDoc;
extern IRsend irsend;
extern IRrecv irrecv;
extern bool record;

void setupWeb(WebServer& server);

extern const int ledPin;