#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTextEdit"
#include "qcustomplot.h"
#include "QLineEdit"
#include "QPushButton"
#include "QFileDialog"
#include "QVector"

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
    QLineEdit *searchText;
    QCustomPlot *plotArea;
    QFileDialog *fileDialog;
    QVector<QCPBars*> bars;

public slots:
    void fileBtnOnClick();
    void switchBtnOnClick();
    void startBtnOnClick();
    void simBtnOnClick();
    void searchBtnOnClick();
};

#endif // MAINWINDOW_H
