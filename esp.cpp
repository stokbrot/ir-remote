#include "esp.h"

esp::esp()
{
    client = std::make_shared<httplib::Client>("http://192.168.4.1");
    client->set_connection_timeout(0, 500000);  // 100ms - fast fail if device unreachable
    client->set_read_timeout(0, 500000);        // 100ms - typical LAN response time
    
    if (!client)
        printf("Failed to initialize httplib client\n");
}

esp::~esp()
{
    if (client)
        client = nullptr;
}

bool esp::set(const std::string& command)
{
    if (command.empty() || !client)
        return false;

    auto res = client->Get("/" + command);
    return (bool)res;
}

bool esp::get(const std::string& endpoint, std::string& responseBuffer)
{
    if (endpoint.empty() || !client)
        return false;

    auto res = client->Get("/" + endpoint);

    if (!res)
        return false;

    responseBuffer = res->body;
    return true;
}

bool esp::sendRaw(std::vector<int> rawData)
{
    if (!client || rawData.empty())
        return false;

    // JSON form: {"raw":[...], "freq":38}
    std::string json = "{\"raw\":[";

    for (size_t i = 0; i < rawData.size(); i++)
    {
        json += std::to_string(rawData[i]);
        if (i < rawData.size() - 1)
            json += ",";
    }
    
    json += "],\"freq\":38}";

    auto res = client->Post("/sendRaw", json, "application/json");
    return (bool)res;
}
