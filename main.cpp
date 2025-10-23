#include "processdata.h"
#include "drawplot.h"
#include "mainwindow.h"
#include "databasehandler.h"

#include <QApplication>

// 数据库
#include "databasehandler.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //===静态程序启动画面===
    QPixmap pix(":/start1");
    QSplashScreen splash(pix);
    splash.show();
    a.processEvents();

    MainWindow w;
    w.show();


    splash.finish(&w);//程序启动画面结束

    return a.exec();
}
