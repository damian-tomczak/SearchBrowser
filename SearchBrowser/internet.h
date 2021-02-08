#pragma once
#define CURL_STATICCLIB

#include "browserhistory.h"
#include <string>
#include <curl/curl.h>

bool checkUrlOnInternet(BrowserHistory, std::string);