#pragma once
#include "external/httplib.h"
#include <string>
#include <iostream>
#include <vector>
#include <memory>

// Probleme in der classe esp:
// Wir nutzen kein Multithreading -> gui freezed wenn anfrage/timout generell
// Keine wirkliches error handling, nur true/false zurück
// ich sage einfach mal wir gehen von best case aus weil es reicht

class esp
{
public:
    esp();
    ~esp();

    // Control methods
    bool set(const std::string& command);
    bool get(const std::string& endpoint, std::string& responseBuffer);
    bool sendRaw(std::vector<int> rawData);

    // IR command data for buttons
    std::vector<int> vol_down;
    std::vector<int> vol_up;

private:
    std::shared_ptr<httplib::Client> client;
};
