#include "esp.h"


CURL* esp::curlHandle = nullptr;

static esp globalEspInitializer;

   

// Callback function for curl to write response data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

esp::esp()
{
    if (!curlHandle)
    {
        curlHandle = curl_easy_init();
        if (!curlHandle)
            printf("Failed to initialize libcurl\n");
    }
}

esp::~esp()
{
    if (curlHandle)
    {
        curl_easy_cleanup(curlHandle);
        curlHandle = nullptr;
    }
}

bool esp::set(const std::string& command)
{
    if (command.empty() || !curlHandle)
        return false;

    const std::string url = "http://192.168.4.1/" + command;
    char errorBuffer[CURL_ERROR_SIZE] = {0};

    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 6L);
    curl_easy_setopt(curlHandle, CURLOPT_ERRORBUFFER, errorBuffer);

    CURLcode result = curl_easy_perform(curlHandle);
    if (result != CURLE_OK)
    {
        printf("Error performing HTTP request: %s\n", errorBuffer);
        return false;
    }

    return true;
}

bool esp::get(const std::string& endpoint, std::string& responseBuffer)
{
    if (endpoint.empty() || !curlHandle)
        return false;

    const std::string url = "http://192.168.4.1/" + endpoint;
    char errorBuffer[CURL_ERROR_SIZE] = {0};

    responseBuffer.clear();

    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 6L);
    curl_easy_setopt(curlHandle, CURLOPT_ERRORBUFFER, errorBuffer);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &responseBuffer);

    CURLcode result = curl_easy_perform(curlHandle);
    if (result != CURLE_OK)
    {
        printf("Error performing HTTP GET request: %s\n", errorBuffer);
        return false;
    }

    return true;
}

bool esp::sendRaw(std::vector<int> rawData){
    if (!curlHandle)
        return false;

    std::string json = "{\"raw\":[";
    for(size_t i = 0; i < rawData.size(); i++){
        json += std::to_string(rawData[i]);
        if(i != rawData.size() - 1 ){json += ",";}  // nach jedem wert ein , ausser beim letzten
    }

    json += "],\"freq\":38}";

    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(headers,"Content-Type: application/json");

    curl_easy_setopt(curlHandle, CURLOPT_URL, "http://192.168.4.1/sendRaw");
    curl_easy_setopt(curlHandle, CURLOPT_POST, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, json.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 1L);

    
    char errorBuffer[CURL_ERROR_SIZE] = {0};

    curl_easy_setopt(curlHandle, CURLOPT_ERRORBUFFER, errorBuffer);
    

    CURLcode result = curl_easy_perform(curlHandle);
    if (result != CURLE_OK)
    {
        printf("Error performing HTTP GET request: %s\n", errorBuffer);
        return false;
    }

    return true;
}
