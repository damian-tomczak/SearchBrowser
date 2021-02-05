#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>

#include "ui_searchbrowser.h"
#include "browserhistory.h"
#include "updater.h"
#include "internet.h"

#include <windows.h>
#include <Lmcons.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <sqlite3.h> 
#include <string>

class SearchBrowser : public QMainWindow
{
    Q_OBJECT

public:
    SearchBrowser(QWidget* parent = Q_NULLPTR);

private:
    Ui::SearchBrowserClass ui;
    void createMenus();
    void createActions();

    QMenu* helpMenu;
    QAction* aboutAct;
    QAction* helpAct;

private:
    void about();
    void help();

private:
    void blockInerface(bool);

    int starts();
    int getBrowserProcess(int, bool);
    int runProgram(std::string, int);
    bool openBrowserMessage();
    void errorMessage();
    bool killBrowser(DWORD, UINT);
    std::string getUserName();
    void pushToScreen(std::vector<std::string>);
};
