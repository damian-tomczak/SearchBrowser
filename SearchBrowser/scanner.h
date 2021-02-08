#pragma once

#include <QObject>
#include <QDebug>

#include "browserhistory.h"
#include "searchbrowser.h"
#include "internet.h"

#include <windows.h>
#include <Lmcons.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <sqlite3.h> 
#include <string>

class Scanner : public QObject
{
	Q_OBJECT
public:
	explicit Scanner(std::string, int, QObject* parent = nullptr);

	std::string getUserName();

private:
	std::string input;
	int time;
signals:
	void doneProcess();
	void progress(std::string);
	void numberLoop(int, int);

public slots:
	void process();
};

