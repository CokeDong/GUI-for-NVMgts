#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTextEdit"
#include "qcustomplot.h"
#include "QLineEdit"
#include "QPushButton"
#include "QFileDialog"
#include "QVector"
#include <QTcpSocket>
#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QTimer>
#include <QTimerEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void MainEventHandler();
    //plot
    void initPlot(int groupSize, int dataxSize, QVector<QString> itemName);
    void updatePlot(QVector<double> x, QVector<double> y, int barIndex);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *cenWidget;
    QTextEdit *txtSystemStatus, *txtSearchResult, *txtMessage;
    QPushButton *bFile, *bSwitch, *bBatch, *bConn,*bSim,*bSearch,*bClean,*bClose;
    QLineEdit *searchText;
    QCustomPlot *plotArea;
    QFileDialog *fileDialog;
    QVector<QCPBars*> bars;
    QTcpSocket *cli_socket;
    QTimer *timer;


public slots:
    void fileBtnOnClick();
    void switchBtnOnClick();
    void simBtnOnClick();
    void searchBtnOnClick();
    void batchQueryBtnOnClick();
    void connBtnOnClick();
    void cleanBtnOnClick();
    void closeBtnOnClick();


    void readyread();
    void connectFinish();
    void flushState();
};

#endif // MAINWINDOW_H
