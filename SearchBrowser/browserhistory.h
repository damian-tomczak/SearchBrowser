#pragma once

#include <string>

#define QUERY24 "SELECT datetime(last_visit_time / 1000000 - 11644473600, 'unixepoch', 'localtime') as datte, url FROM urls WHERE datte >= datetime('now', '-1 day') ORDER BY last_visit_time DESC;"
#define QUERY72 "SELECT datetime(last_visit_time / 1000000 - 11644473600, 'unixepoch', 'localtime') as datte, url FROM urls WHERE datte >= datetime('now', '-1 day') ORDER BY last_visit_time DESC;"
#define QUERY720 "SELECT datetime(last_visit_time / 1000000 - 11644473600, 'unixepoch', 'localtime') as datte, url FROM urls WHERE datte >= datetime('now', '-1 day') ORDER BY last_visit_time DESC;"

class BrowserHistory
{
public:
	std::string lasttime;
	std::string url;
public:
	BrowserHistory(const std::string &lasttime, const std::string &url);
};

