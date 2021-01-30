#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_searchbrowser.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <QMessageBox>


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
    int browserWorks(int);

};
