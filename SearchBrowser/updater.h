#pragma once
#define CURL_STATICLIB

#include <QMessageBox>
#include <QAccessible>
#include <QtWidgets/QMainWindow>

#include <string>
#include <curl/curl.h>

#include "searchbrowser.h"
#include "ui_searchbrowser.h"

class Updater
{
public:
	Updater();

private:
	std::string newest;
	std::string current = "0.0.1";
	static void updateMessage(std::string, std::string);
};

