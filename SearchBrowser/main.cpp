#include "searchbrowser.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SearchBrowser w;
    w.show();
    return a.exec();
}
