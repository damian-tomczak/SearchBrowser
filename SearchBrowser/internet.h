#pragma once
#define CURL_STATICLIB
#include "browserhistory.h"
#include <string>
#include <curl/curl.h>

class Internet
{
public:
	static bool checkUrlOnInternet(BrowserHistory, std::string);
};

