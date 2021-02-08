#pragma once

#include <string>

#define QUERY1 "SELECT  datetime(last_visit_time / 1000000 + (strftime('%s', '1601-01-01')), 'unixepoch', 'localtime') as datte, url FROM urls WHERE datte >= datetime('now', '-1 hour') ORDER BY last_visit_time DESC;"
#define QUERY24 "SELECT  datetime(last_visit_time / 1000000 + (strftime('%s', '1601-01-01')), 'unixepoch', 'localtime') as datte, url FROM urls WHERE datte >= datetime('now', '-1 day') ORDER BY last_visit_time DESC;"
#define QUERY72 "SELECT  datetime(last_visit_time / 1000000 + (strftime('%s', '1601-01-01')), 'unixepoch', 'localtime') as datte, url FROM urls WHERE datte >= datetime('now', '-3 day') ORDER BY last_visit_time DESC;"
#define QUERY720 "SELECT  datetime(last_visit_time / 1000000 + (strftime('%s', '1601-01-01')), 'unixepoch', 'localtime') as datte, url FROM urls WHERE datte >= datetime('now', '-30 day') ORDER BY last_visit_time DESC;"
#define QUERY2160 "SELECT  datetime(last_visit_time / 1000000 + (strftime('%s', '1601-01-01')), 'unixepoch', 'localtime') as datte, url FROM urls WHERE datte >= datetime('now', '-90 day') ORDER BY last_visit_time DESC;"

class BrowserHistory
{
public:
	std::string lasttime;
	std::string url;
public:
	BrowserHistory(const std::string& lasttime, const std::string& url);
};
