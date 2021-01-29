#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_searchbrowser.h"

class SearchBrowser : public QMainWindow
{
    Q_OBJECT

public:
    SearchBrowser(QWidget *parent = Q_NULLPTR);

private:
    Ui::SearchBrowserClass ui;
};
