#include "searchbrowser.h"

SearchBrowser::SearchBrowser(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    Updater updater;
    std::string message;
    if (updater.updateAvailable())
    {
        message = "SearchBrowser " VERSION " UPDATE AVAILABLE";
    }
    else
    {
        message = "SearchBrowser " VERSION " not update available";
    }

    statusBar()->showMessage(QString::fromStdString(message), 0);

    createActions();
    createMenus();

    setWindowTitle(tr("SearchBrowser"));
    std::string username = getUserName();
    connect(ui.Start, &QAbstractButton::released, this, &SearchBrowser::starts);
}

void SearchBrowser::about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Menu");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("Copyright (C) 2021 <a href='https://damian-tomczak.pl'>Damian Tomczak</a><br>"
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
        ui.Input->setEnabled(false);
    }
    else
    {
        ui.Start->setEnabled(true);
        ui.Browser->setEnabled(true);
        ui.Time->setEnabled(true);
        ui.Input->setEnabled(true);
    }
}

int SearchBrowser::starts()
{
    blockInerface(true);

    int choosed = ui.Browser->itemData(ui.Browser->currentIndex()).toInt();

    int result = getBrowserProcess(choosed, false);
    switch (result)
    {
        // Browser running
    case 0:
        if (openBrowserMessage())
        {
            getBrowserProcess(choosed, true);
        }

        blockInerface(false);
        break;
    case 1:
        runProgram();
        break;
    case 2:
        errorMessage();
        break;
    }

    return 1;
}

int SearchBrowser::getBrowserProcess(int index, bool kill)
{
    std::vector<std::wstring> browsers = {
        L"chrome.exe"
    };

    const wchar_t* executableName = browsers[index].c_str();

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry))
    {
        CloseHandle(snapshot);
        return 2;
    }

    do {
        if (!_tcsicmp(entry.szExeFile, executableName))
        {
            if (kill)
            {
                killBrowser(entry.th32ProcessID, 1);
            }
            CloseHandle(snapshot);
            return 0;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return 1;

}

bool SearchBrowser::openBrowserMessage()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, tr("Browser Error"),
        tr("<b>Your browser is running in the background.\n"
            "You must close the browser!</b>"
            "\nForce shutdown the browser?"),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (reply == QMessageBox::Yes)
        return true;
    else
        return false;
}

void SearchBrowser::errorMessage()
{
    QMessageBox::critical(this, tr("Critical Error"),
        tr("Something went wrong..."),
        QMessageBox::Close);
    QApplication::quit();
}

int SearchBrowser::runProgram()
{
    std::string path = "C:\\Users\\" + getUserName() + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\History";

    sqlite3* db;
    int rc = sqlite3_open(path.c_str(), &db);
    std::vector<BrowserHistory> histories;
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, QUERY24, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        errorMessage();
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        std::string lasttime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string url = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        //statusBar()->showMessage(QString::fromStdString("Get: " + url));

        histories.push_back(BrowserHistory(lasttime, url));
    }

    if (rc != SQLITE_DONE)
    {
        errorMessage();
    }

    sqlite3_finalize(stmt);
    
    statusBar()->showMessage(tr("Saving is complete"));

    pushToScreen(histories);

    return 1;
}

bool SearchBrowser::killBrowser(DWORD dwProcessId, UINT uExitCode)
{
    DWORD dwDesirecAccess = PROCESS_TERMINATE;
    BOOL bInheritHandle = FALSE;
    HANDLE hProcess = OpenProcess(dwDesirecAccess, bInheritHandle, dwProcessId);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    BOOL result = TerminateProcess(hProcess, uExitCode);

    CloseHandle(hProcess);

    return true;

}

std::string SearchBrowser::getUserName()
{
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameA(username, &username_len);
    return std::string(username);
}

void SearchBrowser::pushToScreen(std::vector<BrowserHistory> histories)
{
    int j = 0;
    for (auto i : histories)
    {
        QListWidgetItem* item = new QListWidgetItem; 
        item->setText(QString::fromStdString(i.lasttime));
        ui.listWidget->insertItem(j,item);
        j++;
    }
}
