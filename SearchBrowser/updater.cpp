#define CURL_STATICLIB
#include "updater.h"
#include <string>
#include <curl/curl.h>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    static_cast<std::string*>(userp)->append(
        static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

bool Updater::updateAvailable()
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://damian-tomczak.pl/searchbrowser.txt");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl); //perform a blocking file transfer
        curl_easy_cleanup(curl);

        size_t found = readBuffer.find("0");
    }

    if (readBuffer[0] > VERSION[0])
    {
        return true;

    }
    else if (readBuffer[2] > VERSION[2])
    {
        return true;
    }
    else if (readBuffer[4] > VERSION[4])
    {
        return true;
    }
    else
    {
        return false;
    }
}
