#include "mainwindow.h"
#include <QApplication>
#include "qcustomplot.h"
#include "unistd.h"
#include "constdefine.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
