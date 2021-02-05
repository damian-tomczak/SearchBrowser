#include "internet.h"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	static_cast<std::string*>(userp)->append(
		static_cast<char*>(contents), size * nmemb);
	return size * nmemb;
}

bool Internet::checkUrlOnInternet(BrowserHistory i, std::string input)
{
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, i.url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		size_t found = readBuffer.find(input);

		if (found != std::string::npos)
		{
			return true;
		}
		else
		{
			return false;
		}


	}
}
