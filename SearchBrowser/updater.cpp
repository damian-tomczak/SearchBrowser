#include "updater.h"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	static_cast<std::string*>(userp)->append(
		static_cast<char*>(contents), size * nmemb);
	return size * nmemb;
}

Updater::Updater()
{
	CURL* curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://damian-tomczak.pl/searchbrowser.txt");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &newest);
		res = curl_easy_perform(curl); //perform a blocking file transfer
		curl_easy_cleanup(curl);

		size_t found = newest.find("0");
	}

	bool availableUpdate = false;

	if (newest[0] > current[0])
		availableUpdate = true;
	else if (newest[2] > current[2])
		availableUpdate = true;
	else if (newest[4] > current[4])
		availableUpdate = true;

	if(availableUpdate)
		Updater::updateMessage(newest, current);
}

void Updater::updateMessage(std::string _newest, std::string _current)
{
	QString newest = QString::fromStdString(_newest);
	QString current = QString::fromStdString(_current);

	QMessageBox::information(NULL, "Update Available!",
		"You are currently using version: " + current + "<br>"
		"The newest available is: " + newest + "<br>"
		"<a href='https://damian-tomczak.pl/searchbrowser'>Download the latest version</a><br>");
	
}
