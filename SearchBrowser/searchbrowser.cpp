#include "searchbrowser.h"

SearchBrowser::SearchBrowser(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QString message = tr("SearchBrowser 0.1 not update available");
    statusBar()->showMessage(message, 0);

    createActions();
    createMenus();

    setWindowTitle(tr("SearchBrowser"));

    connect(ui.Start, &QAbstractButton::released, this, &SearchBrowser::starts);
}

void SearchBrowser::about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Menu");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("Copyright (C) <a href='https://damian-tomczak.pl'>Damian Tomczak</a><br>"
        "Did you find a error? <a href='mailto:kontakt@damian-tomczak.pl'>kontakt@damian-tomczak.pl</a>");
    msgBox.exec();
}

void SearchBrowser::help()
{
    QMessageBox::information(this, tr("Help Menu"), tr(
        "The program is used to search visited websites in the selected browser in order to find the given phrase."));
}

void SearchBrowser::createActions()
{
    aboutAct = new QAction(tr("About"), this);
    connect(aboutAct, &QAction::triggered, this, &SearchBrowser::about);

    helpAct = new QAction(tr("Help"), this);
    connect(helpAct, &QAction::triggered, this, &SearchBrowser::help);
}

void SearchBrowser::createMenus()
{
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
}

void SearchBrowser::blockInerface(bool condition)
{
    if (condition)
    {
        ui.Start->setEnabled(false);
        ui.Browser->setEnabled(false);
        ui.Time->setEnabled(false);
        ui.Accuracy->setEnabled(false);
        ui.Input->setEnabled(false);
    }
    else
    {
        ui.Start->setEnabled(true);
        ui.Browser->setEnabled(true);
        ui.Time->setEnabled(true);
        ui.Accuracy->setEnabled(true);
        ui.Input->setEnabled(true);
    }
}

int SearchBrowser::starts()
{
    blockInerface(true);
    return 1;
}
