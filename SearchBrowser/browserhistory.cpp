#include "browserhistory.h"

BrowserHistory::BrowserHistory(const std::string& _lasttime, const std::string& _url)
{
	this->lasttime = _lasttime;
	this->url = _url;
}
