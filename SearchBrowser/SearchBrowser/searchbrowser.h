#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_searchbrowser.h"
#include <QMessageBox>


class SearchBrowser : public QMainWindow
{
    Q_OBJECT

public:
    SearchBrowser(QWidget *parent = Q_NULLPTR);

private:
    Ui::SearchBrowserClass ui;
    void createMenus();
    void createActions();

    QMenu* helpMenu;
    QAction* aboutAct;
    QAction* helpAct;
private slots:
    void about();
    void help();
};
