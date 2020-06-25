#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //对话框类应用中文
    QTranslator user;
    user.load("qt_zh_CN.qm", ":/");
    a.installTranslator(&user);

    MainWindow w;

    if (argc >= 2)
    {
        QString s = QString::fromLocal8Bit(argv[1]);
        w.loadFromFile(s);
    }

    w.show();
    return a.exec();
}
