#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QThread>

#include "ui_searchbrowser.h"
#include "updater.h"
#include "scanner.h"

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
    SearchBrowser(QWidget *parent = Q_NULLPTR);
    void aboutMessage();
    void helpMessage();
    static void errorMessage();
    bool openBrowserMessage();
    void pushToScreen(std::string);
    void progressBar(int, int);
    void refresh();
    void openBrowser();

    void lockInterface(bool);
    void runScanner();
    void progress(std::string);
    bool getBrowserProcess(int, bool);
    bool killBrowser(DWORD, UINT);

private:
    Ui::SearchBrowserClass ui;
    QMenu* helpMenu;
    QAction* aboutAct;
    QAction* helpAct;

    void createMenus();
    void createActions();

};
