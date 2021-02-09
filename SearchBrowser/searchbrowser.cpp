#include "searchbrowser.h"

SearchBrowser::SearchBrowser(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    Updater updater;
    
    createActions();
    createMenus();

    connect(ui.Start, &QAbstractButton::released, this, &SearchBrowser::runScanner);
}

void SearchBrowser::aboutMessage()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Menu");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("Copyright (C) 2021 <a href='https://damian-tomczak.pl'>Damian Tomczak</a><br>"
        "Did you find a error? <a href='mailto:kontakt@damian-tomczak.pl'>kontakt@damian-tomczak.pl</a>");
    msgBox.exec();
}

void SearchBrowser::helpMessage()
{
    QMessageBox::information(NULL, tr("Help Menu"), tr(
        "The program is used to search visited websites in the selected browser in order to find the given phrase."));
}

void SearchBrowser::errorMessage()
{
    QMessageBox::critical(NULL, tr("Critical Error"),
        tr("Something went wrong..."),
        QMessageBox::Close);
    QApplication::quit();
}

void SearchBrowser::createActions()
{
    aboutAct = new QAction(tr("About"), this);
    connect(aboutAct, &QAction::triggered, this, &SearchBrowser::aboutMessage);

    helpAct = new QAction(tr("Help"), this);
    connect(helpAct, &QAction::triggered, this, &SearchBrowser::helpMessage);
}

void SearchBrowser::createMenus()
{
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
}

bool SearchBrowser::openBrowserMessage()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(nullptr, tr("Browser Error"),
        tr("<b>Your browser is running in the background.\n"
            "You must close the browser!</b>"
            "\nForce shutdown the browser?"),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (reply == QMessageBox::Yes)
        return true;
    else
        return false;
}

void SearchBrowser::lockInterface(bool condition)
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

void SearchBrowser::runScanner()
{
    lockInterface(true);

    int choosed = ui.Browser->currentIndex();
    int time = ui.Time->currentIndex();
    bool run = false;

    QString qinput = ui.Input->toPlainText();
    std::string input = qinput.toLocal8Bit().constData(); // Polish characters 

    if (getBrowserProcess(choosed, false))
    {
        if (openBrowserMessage())
        {
            if (getBrowserProcess(choosed, true))
            {
                run = true;
            }
        }
    }
    else
    {
        run = true;
    }

    if (run)
    {
        ui.listWidget->clear();

        ui.listWidget->setEnabled(true);

        QThread* scannerThread = new QThread;
        Scanner* scannerObject = new Scanner(input, time);
        scannerObject->moveToThread(scannerThread);

        connect(scannerThread, &QThread::started, scannerObject, &Scanner::process, Qt::ConnectionType::DirectConnection);
        connect(scannerObject, &Scanner::progress, this, &SearchBrowser::progress, Qt::ConnectionType::QueuedConnection);
        connect(scannerObject, &Scanner::numberLoop, this, &SearchBrowser::progressBar, Qt::ConnectionType::QueuedConnection);

        connect(scannerObject, &Scanner::doneProcess, scannerThread, &QThread::quit, Qt::ConnectionType::QueuedConnection);
        connect(scannerObject, &Scanner::doneProcess, this, &SearchBrowser::refresh, Qt::ConnectionType::QueuedConnection);
        connect(scannerObject, &Scanner::doneProcess, scannerObject, &Scanner::deleteLater, Qt::ConnectionType::QueuedConnection);
        connect(scannerThread, &QThread::finished, scannerObject, &Scanner::deleteLater, Qt::ConnectionType::DirectConnection);

        scannerThread->start();
    }
    else
    {
        lockInterface(false);
    }
}

bool SearchBrowser::getBrowserProcess(int _index, bool _kill)
{
    std::vector<std::wstring> browsers = {
        L"chrome.exe"
    };

    const wchar_t* executableName = browsers[_index].c_str();

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry))
    {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (!_tcsicmp(entry.szExeFile, executableName))
        {
            if (_kill)
            {
                if (killBrowser(entry.th32ProcessID, 1))
                    return true;
                else
                    return false;
            }
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
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

void SearchBrowser::progress(std::string _url)
{
    pushToScreen(_url);
}

void SearchBrowser::progressBar(int _sizeAll, int _sizeCurrent)
{
    ui.progressBar->setValue((_sizeCurrent * 100) / _sizeAll);
}

void SearchBrowser::pushToScreen(std::string _url)
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setText(QString::fromStdString(_url));
    ui.listWidget->addItem(item);

    if(ui.listWidget->count() %2==0)
        item->setBackground(QBrush(QColor(144, 229, 126)));
    else
        item->setBackground(QBrush(QColor(119, 184, 104)));
}

void SearchBrowser::refresh()
{
    connect(ui.listWidget, &QListWidget::itemDoubleClicked, this, &SearchBrowser::openBrowser);
    
    lockInterface(false);
}

void SearchBrowser::openBrowser()
{
    QObject* obj = sender();
    QListWidget* item = qobject_cast<QListWidget*>(obj);
    QDesktopServices::openUrl(item->currentItem()->text());
}
