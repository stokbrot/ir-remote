#include "esp.h"

std::shared_ptr<httplib::Client> esp::client = nullptr;

static esp globalEspInitializer;

esp::esp()
{
    if (!client)
    {
        client = std::make_shared<httplib::Client>("http://192.168.4.1");
        client->set_connection_timeout(1, 0);
        client->set_read_timeout(6, 0);
        if (!client)
            printf("Failed to initialize httplib client\n");
    }
}

esp::~esp()
{
    if (client)
    {
        client = nullptr;
    }
}

bool esp::set(const std::string& command)
{
    if (command.empty() || !client)
        return false;

    auto res = client->Get("/" + command);
    if (!res || res->status < 200 || res->status >= 300)
    {
        printf("Error performing HTTP request\n");
        return false;
    }

    return true;
}

bool esp::get(const std::string& endpoint, std::string& responseBuffer)
{
    if (endpoint.empty() || !client)
        return false;

    responseBuffer.clear();

    auto res = client->Get("/" + endpoint);
    if (!res || res->status < 200 || res->status >= 300)
    {
        printf("Error performing HTTP GET request\n");
        return false;
    }

    responseBuffer = res->body;
    return true;
}

bool esp::sendRaw(std::vector<int> rawData){
    if (!client)
        return false;

    std::string json = "{\"raw\":[";
    for(size_t i = 0; i < rawData.size(); i++){
        json += std::to_string(rawData[i]);
        if(i != rawData.size() - 1 ){json += ",";}
    }

    json += "],\"freq\":38}";

    auto res = client->Post("/sendRaw", json, "application/json");
    if (!res || res->status < 200 || res->status >= 300)
    {
        printf("Error performing HTTP POST request\n");
        return false;
    }

    return true;
}
