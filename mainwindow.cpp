#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
#include "QWidget"
#include "QDialog"
#include "QGridLayout"
#include "QHBoxLayout"
#include "QTextEdit"
#include "qcustomplot.h"
#include "QDebug"

#define CEN_WIDGET_STYLE "background: #505050; color: white"
#define CONSOLE_STYLE "border: 1px solid grey"

using namespace::std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumHeight(600);
    cenWidget = new QWidget(this);
    cenWidget->setStyleSheet(CEN_WIDGET_STYLE);
    setCentralWidget(cenWidget);

    QGridLayout *cenLayout = new QGridLayout;
    QHBoxLayout *setLayout = new QHBoxLayout;

    QWidget *settingsWidget = new QWidget(cenWidget);
    QPushButton *bFile = new QPushButton("Load File", settingsWidget);
    QPushButton *bSwitch = new QPushButton("Switch Mode", settingsWidget);
    QPushButton *bStart = new QPushButton("Start System", settingsWidget);
    QPushButton *bSim = new QPushButton("Simulation", cenWidget);
    QPushButton *bSearch = new QPushButton("Range Query", cenWidget);
    searchText = new QLineEdit(cenWidget);
    searchText->setText("searchText");

    QVector<QString> inames = QVector<QString>() << "item1" << "item2";
    initPlot(2, 4, inames);

    txtSystemStatus = new QTextEdit("txtSystemStatus", cenWidget);
    txtSystemStatus->setStyleSheet(CONSOLE_STYLE);
    txtSystemStatus->setReadOnly(true);

    txtSearchResult = new QTextEdit("txtSearchReuslt", cenWidget);
    txtSearchResult->setStyleSheet(CONSOLE_STYLE);
    txtSearchResult->setReadOnly(true);

    txtMessage = new QTextEdit("txtMessage", cenWidget);
    txtMessage->setStyleSheet(CONSOLE_STYLE);
    txtMessage->setReadOnly(true);

    cenLayout->setColumnStretch(0, 1);
    cenLayout->setColumnStretch(1, 1);
    cenLayout->setColumnStretch(2, 1);
    cenLayout->setColumnStretch(3, 1);
    cenLayout->setColumnStretch(4, 1);
    cenLayout->setColumnStretch(5, 1);

    cenLayout->setRowStretch(0, 1);
    cenLayout->setRowStretch(1, 1);
    cenLayout->setRowStretch(2, 1);
    cenLayout->setRowStretch(3, 1);
    cenLayout->setRowStretch(4, 1);

    cenLayout->setMargin(15);

    cenLayout->addWidget(settingsWidget, 0, 0, 1, 3);
    cenLayout->addWidget(bSim, 0, 3);
    cenLayout->addWidget(txtSystemStatus, 1, 0, 1, 3);
    cenLayout->addWidget(plotArea, 1, 3, 3, 3);
    cenLayout->addWidget(searchText, 2, 0, 1, 2);
    cenLayout->addWidget(bSearch, 2, 2);
    cenLayout->addWidget(txtSearchResult, 3, 0, 1, 3);
    cenLayout->addWidget(txtMessage, 4, 0, 1, 6);
    setLayout->addWidget(bFile);
    setLayout->addWidget(bSwitch);
    setLayout->addWidget(bStart);

    cenWidget->setLayout(cenLayout);
    settingsWidget->setLayout(setLayout);

    connect(bFile, SIGNAL(clicked()), this, SLOT(fileBtnOnClick()));
    connect(bSwitch, SIGNAL(clicked()), this, SLOT(switchBtnOnClick()));
    connect(bStart, SIGNAL(clicked()), this, SLOT(startBtnOnClick()));
    connect(bSim, SIGNAL(clicked()), this, SLOT(simBtnOnClick()));
    connect(bSearch, SIGNAL(clicked()), this, SLOT(searchBtnOnClick()));
}

void MainWindow:: initPlot(int groupSize, int dataxSize, QVector<QString> itemName)
{
    // prepare data:
    int i = 0;
    plotArea = new QCustomPlot(cenWidget);
    // preset color R,G,B,transparency.
    // ONLY 2 BAR COLOR ARE SPECIFIED. ADD YOUR OWN COLOR IF YOUR GROUP SIZE IS LARGER THAN 2.
    QVector<int> R = QVector<int>() << 10 << 10;
    QVector<int> G = QVector<int>() << 140 << 100;
    QVector<int> B = QVector<int>() << 70 << 50;
    QVector<int> T = QVector<int>() << 160 << 70;

    // create and configure plottables:
    QCPBarsGroup *group = new QCPBarsGroup(plotArea);
    bars = QVector<QCPBars*>(groupSize);
    QCPBars *bar;

    for(i = 0; i < groupSize; i++)
    {
        bar = new QCPBars(plotArea->xAxis, plotArea->yAxis);
        bar->setWidth(2/(double)dataxSize/(double)groupSize);
        bar->setName(itemName[i]);
        bar->setPen(Qt::NoPen);
        bar->setBrush(QColor(R[i], G[i], B[i], T[i]));
        bar->setBarsGroup(group);
        bars[i] = bar;
    }

    // move bars above graphs and grid below bars:
    plotArea->addLayer("abovemain", plotArea->layer("main"), QCustomPlot::limAbove);
    plotArea->addLayer("belowmain", plotArea->layer("main"), QCustomPlot::limBelow);
    plotArea->xAxis->grid()->setLayer("belowmain");
    plotArea->yAxis->grid()->setLayer("belowmain");

    // set some pens, brushes and backgrounds:
    plotArea->xAxis->setBasePen(QPen(Qt::white, 1));
    plotArea->yAxis->setBasePen(QPen(Qt::white, 1));
    plotArea->xAxis->setTickPen(QPen(Qt::white, 1));
    plotArea->yAxis->setTickPen(QPen(Qt::white, 1));
    plotArea->xAxis->setSubTickPen(QPen(Qt::white, 1));
    plotArea->yAxis->setSubTickPen(QPen(Qt::white, 1));
    plotArea->xAxis->setTickLabelColor(Qt::white);
    plotArea->yAxis->setTickLabelColor(Qt::white);
    plotArea->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    plotArea->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    plotArea->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plotArea->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plotArea->xAxis->grid()->setSubGridVisible(true);
    plotArea->yAxis->grid()->setSubGridVisible(true);
    plotArea->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    plotArea->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    plotArea->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    plotArea->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    plotArea->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    plotArea->axisRect()->setBackground(axisRectGradient);

    plotArea->rescaleAxes();
    //Set Axis Range
    plotArea->xAxis->setRange(0, 5);
    plotArea->yAxis->setRange(0, 2);
}

void MainWindow:: updatePlot(QVector<double> x, QVector<double> y, int barIndex)
{
    bars[barIndex]->setData(x, y);
    plotArea->legend->setVisible(true);
    plotArea->replot();
}

//Slots
void MainWindow:: fileBtnOnClick()
{
    //default desktop folder
    QString src = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), src, tr("Text Files (*.txt)"));
    qDebug() << fileName;      //Test if file name is correct.
    //Your code to deal with input file address.
}

void MainWindow:: switchBtnOnClick()
{
    QVector<double> datax = QVector<double>() << 1 << 2 << 3 << 4;
    QVector<double> datay1 = QVector<double>() << 0.6 << 0.5 << 0.3 << 0.15;
    QVector<double> datay2 = QVector<double>() << 0.3 << 0.28 << 0.2 << 0.1;
    updatePlot(datax, datay1, 0);
    updatePlot(datax, datay2, 1);
}

void MainWindow:: startBtnOnClick()
{
    QVector<double> datax = QVector<double>() << 1 << 2 << 3 << 4;
    QVector<double> datay1 = QVector<double>() << 1.6 << 1.5 << 1.3 << 1.15;
    QVector<double> datay2 = QVector<double>() << 1.3 << 1.28 << 1.2 << 1.1;
    updatePlot(datax, datay1, 0);
    updatePlot(datax, datay2, 1);
}

void MainWindow:: simBtnOnClick(){}

void MainWindow:: searchBtnOnClick(){}
// End Slots

MainWindow::~MainWindow()
{
    delete ui;
}
