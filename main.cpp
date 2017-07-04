#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QIcon>
#include "BinFileWindow.h"

//#define makeFile

const QString AppName = "ESL Podcast";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/esl.ico"));
    //a.setQuitOnLastWindowClosed(false);
#ifdef makeFile
    BinFileWindow binWin;
    binWin.show();
#else
    MainWindow w;
    w.setWindowTitle(AppName);    
    w.showMaximized();
#endif
    return a.exec();
}
