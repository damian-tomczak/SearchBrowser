#include "scanner.h"

Scanner::Scanner(std::string _input, int _time, QObject* parent) : QObject(parent)
{
    input = _input;
    time = _time;
}

void Scanner::process()
{
    std::string path = "C:\\Users\\" + getUserName() + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\History";

    sqlite3* db;
    int rc = sqlite3_open(path.c_str(), &db);
    std::vector<BrowserHistory> histories;
    sqlite3_stmt* stmt;

    switch (time)
    {
    case 0:
        rc = sqlite3_prepare_v2(db, QUERY1, -1, &stmt, NULL);
        break;
    case 1:
        rc = sqlite3_prepare_v2(db, QUERY24, -1, &stmt, NULL);
        break;
    case 2:
        rc = sqlite3_prepare_v2(db, QUERY72, -1, &stmt, NULL);
        break;
    case 3:
        rc = sqlite3_prepare_v2(db, QUERY720, -1, &stmt, NULL);
        break;
    case 4:
        rc = sqlite3_prepare_v2(db, QUERY2160, -1, &stmt, NULL);
        break;
    default:
        SearchBrowser::errorMessage();
        break;
    }

    if (rc != SQLITE_OK)
    {
        SearchBrowser::errorMessage();
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        std::string lasttime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string url = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        histories.push_back(BrowserHistory(lasttime, url));
    }

    if (rc != SQLITE_DONE)
    {
        SearchBrowser::errorMessage();
    }

    sqlite3_finalize(stmt);

    int j = 0;
    for (auto& i : histories)
    {
        if (input != "")
        {
            if (checkUrlOnInternet(i, input))
            {
                emit progress(i.url);
            }
        }
        else
        {
            emit progress(i.url);
        }
        emit numberLoop(histories.size(), ++j);
    }

	emit doneProcess();
}

std::string Scanner::getUserName()
{
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameA(username, &username_len);
	return std::string(username);
}