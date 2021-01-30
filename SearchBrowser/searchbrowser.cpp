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

    int result = browserWorks(ui.Browser->itemData(ui.Browser->currentIndex()).toInt());

    switch (result)
    {
        // Browser running
    case 0:
        break;
        // Error while checking processes
    case 1:
        break;
        // Browser is not running
    case 2:
        break;
    }

    return 1;
}

void PrintProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unkown>");

    // Get a handle to the process

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    // Get the process name
    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    // Release the handle to the process
    CloseHandle(hProcess);
}

int SearchBrowser::browserWorks(int index)
{


    // Get the list of process identifiers
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return 1;
    }

    // Calculate how many process identifiers were returned
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process
    for (int i =0;i<cProcesses;++i)
    {
        if (aProcesses[i] != 0)
        {
            PrintProcessNameAndID(aProcesses[i]);
        }
    }

    return 0;
}