#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
#include "QWidget"
#include "QDialog"
#include "QGridLayout"
#include "QHBoxLayout"
#include "QTextEdit"
#include "qcustomplot.h"
#include <QRadioButton>
#include "QDebug"
#include "QGridLayout"

#define CEN_WIDGET_STYLE "background: #505050; color: white"
#define CONSOLE_STYLE "border: 1px solid grey"

int numTrajSys=0,numPointSys=0,runningTaskType=-1,queryIDNow=-1;
int chooseDataSize = 1;
char chooseMode = 'h';
char systemMode = 'h';
QVector<double> datax = {2.5,2.5};
QVector<double> datay1 = {0,0};
QVector<double> datay2 = {0,0};
QVector<double> datay3 = {0,0};
QVector<int> testSampleNumBatch = {0,0,0};
QVector<int> testSampleNumRecov = {0,0,0};

using namespace::std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumHeight(600);
    setMinimumWidth(1000);
    cenWidget = new QWidget(this);
    cenWidget->setStyleSheet(CEN_WIDGET_STYLE);
    setCentralWidget(cenWidget);

    QGridLayout *cenLayout = new QGridLayout;
    QHBoxLayout *setLayout = new QHBoxLayout;
//    QHBoxLayout *sizeLayout = new QHBoxLayout;
//    QHBoxLayout *modeLayout = new QHBoxLayout;

    QGridLayout *buttonGroupLayout = new QGridLayout;
    QWidget *buttonGroupWidget = new QWidget(cenWidget);

    QWidget *settingsWidget = new QWidget(cenWidget);
    bFile = new QPushButton("Load File", settingsWidget);
    bSwitch = new QPushButton("Switch Mode", settingsWidget);
    bBatch = new QPushButton("Batch Query", settingsWidget);
    bConn = new QPushButton("Connect", settingsWidget);
    bSim = new QPushButton("Start Demo", settingsWidget);
    bSearch = new QPushButton("Single Range Query", cenWidget);
    bClean = new QPushButton("Clean", settingsWidget);
    bClose = new QPushButton("Close", cenWidget);




    searchText = new QLineEdit(cenWidget);
    searchText->setPlaceholderText("Lon1,Lon2,Lat1,Lat2,");

    QVector<QString> inames = QVector<QString>() << "pure DRAM" << "DRAM-SCM" << "pure SCM";
    initPlot(3, 1, inames);

    txtSystemStatus = new QTextEdit("txtSystemStatus", cenWidget);
    txtSystemStatus->setStyleSheet(CONSOLE_STYLE);
    txtSystemStatus->setReadOnly(true);

    txtSearchResult = new QTextEdit("txtSearchReuslt", cenWidget);
    txtSearchResult->setStyleSheet(CONSOLE_STYLE);
    txtSearchResult->setReadOnly(true);

    txtMessage = new QTextEdit("txtMessage", cenWidget);
    txtMessage->setStyleSheet(CONSOLE_STYLE);
    txtMessage->setReadOnly(true);

    QButtonGroup *sizeButtonGroup = new QButtonGroup(this);
    sizeButtonGroup->setExclusive(true);
//    QWidget *sizeWidget = new QWidget(cenWidget);
    QRadioButton *radioButton[5];
    for(int i=1;i<=5;i++){
//        radioButton[i-1] = new QRadioButton(QString::number(i)+" GB DATA",sizeWidget);
        //edit
        radioButton[i-1] = new QRadioButton(QString::number(i)+"00MB DATA",buttonGroupWidget);
        buttonGroupLayout->addWidget(radioButton[i-1], 1, 2*(i-1), 1, 2);
        //end edit

        //radioButton[i-1]->setStyleSheet("padding :1px");
//         sizeLayout->addWidget(radioButton[i-1]);
        connect(radioButton[i-1],&QRadioButton::clicked, [i](){
            chooseDataSize = i;
        });
        sizeButtonGroup->addButton(radioButton[i-1]);
    }
    QButtonGroup *modeButtonGroup = new QButtonGroup(this);
    modeButtonGroup->setExclusive(true);
//    QWidget *modeWidget = new QWidget(cenWidget);
    QRadioButton *radioButtonMode[3];
//    radioButtonMode[0] = new QRadioButton("pure DRAM", modeWidget);
    //edit
    radioButtonMode[0] = new QRadioButton("pure DRAM", buttonGroupWidget);
    buttonGroupLayout->addWidget(radioButtonMode[0], 0, 0, 1, 2);
    //end edit

    //radioButtonMode[0]->setStyleSheet("padding :1px");

    connect(radioButtonMode[0],&QRadioButton::clicked, [](){
        chooseMode = 'd';
    });
//    radioButtonMode[1] = new QRadioButton("hybrid DRAM-NVM", modeWidget);
    //edit
    radioButtonMode[1] = new QRadioButton("hybrid DRAM-NVM", buttonGroupWidget);
    buttonGroupLayout->addWidget(radioButtonMode[1], 0, 2, 1, 2);
    //end edit

    //radioButtonMode[1]->setStyleSheet("padding :1px");

    connect(radioButtonMode[1],&QRadioButton::clicked, [](){
        chooseMode = 'h';
    });
//    radioButtonMode[2] = new QRadioButton("pure NVM", modeWidget);
    //edit
    radioButtonMode[2] = new QRadioButton("pure NVM", buttonGroupWidget);
    buttonGroupLayout->addWidget(radioButtonMode[2], 0, 4, 1, 2);
    //end edit

    buttonGroupWidget->setLayout(buttonGroupLayout);

    //radioButtonMode[2]->setStyleSheet("padding :1px");

    connect(radioButtonMode[2],&QRadioButton::clicked, [](){
        chooseMode = 's';
    });


    cenLayout->setColumnStretch(0, 1);
    cenLayout->setColumnStretch(1, 1);
    cenLayout->setColumnStretch(2, 1);
    cenLayout->setColumnStretch(3, 1);
    cenLayout->setColumnStretch(4, 1);
    cenLayout->setColumnStretch(5, 1);
    cenLayout->setColumnStretch(6, 1);
    cenLayout->setColumnStretch(7, 1);
    cenLayout->setColumnStretch(8, 1);

    cenLayout->setRowStretch(0, 0.3);
    cenLayout->setRowStretch(1, 1);
    cenLayout->setRowStretch(2, 1);
    cenLayout->setRowStretch(3, 1);
    cenLayout->setRowStretch(4, 1);
    cenLayout->setRowStretch(5, 0.3);
    cenLayout->setRowStretch(6, 0.3);
    cenLayout->setMargin(15);

    cenLayout->addWidget(settingsWidget, 0, 0, 1, 6);
//    cenLayout->addWidget(bConn, 0, 3);
//    cenLayout->addWidget(bSim,0, 4);
//    cenLayout->addWidget(bClean,0, 5);
    cenLayout->addWidget(txtSystemStatus, 1, 0, 1, 3);
    cenLayout->addWidget(plotArea, 1, 3, 3, 3);
    cenLayout->addWidget(plotArea1, 1, 6, 3, 3);
    cenLayout->addWidget(searchText, 2, 0, 1, 2);
    cenLayout->addWidget(bSearch, 2, 2);
    cenLayout->addWidget(txtSearchResult, 3, 0, 1, 3);
    cenLayout->addWidget(txtMessage, 4, 0, 1, 9);
    cenLayout->addWidget(buttonGroupWidget, 5, 0, 2, 9);
//    cenLayout->addWidget(sizeWidget, 5 ,0, 1, 9);
    cenLayout->addWidget(bClose, 5, 8);
//    cenLayout->addWidget(modeWidget, 6 ,0, 1, 9);

    cenWidget->setLayout(cenLayout);
    settingsWidget->setLayout(setLayout);
    setLayout->addWidget(bFile);
    setLayout->addWidget(bSwitch);
    setLayout->addWidget(bBatch);
    setLayout->addWidget(bConn);
    setLayout->addWidget(bSim);
    setLayout->addWidget(bClean);
//    sizeWidget->setLayout(sizeLayout);
//    for(int i=1;i<=5;i++){
//        cenLayout->addWidget(radioButton[i-1]);
//    }
//    modeLayout->addWidget(radioButtonMode[0]);
//    modeLayout->addWidget(radioButtonMode[1]);
//    modeLayout->addWidget(radioButtonMode[2]);
//    modeWidget->setLayout(modeLayout);



    connect(bFile, SIGNAL(clicked()), this, SLOT(fileBtnOnClick()));
    connect(bSwitch, SIGNAL(clicked()), this, SLOT(switchBtnOnClick()));
    connect(bBatch, SIGNAL(clicked()), this, SLOT(batchQueryBtnOnClick()));
    connect(bSearch, SIGNAL(clicked()), this, SLOT(searchBtnOnClick()));
    connect(bConn,SIGNAL(clicked()),this,SLOT(connBtnOnClick()));
    connect(bSim, SIGNAL(clicked()), this, SLOT(simBtnOnClick()));
    connect(bClean,SIGNAL(clicked()),this,SLOT(cleanBtnOnClick()));
    connect(bClose,SIGNAL(clicked()),this,SLOT(closeBtnOnClick()));




    // tcp connect
    this->cli_socket = new QTcpSocket(this);
    connect(this->cli_socket, SIGNAL(readyRead()),this,SLOT(readyread()));
    connect(this->cli_socket,SIGNAL(connected()),this,SLOT(connectFinish()));
    this->timer = new QTimer();
    this->timer->start(100);
    connect(this->timer,SIGNAL(timeout()),this,SLOT(flushState()));
}

void MainWindow:: plotSettings(QCustomPlot* plot, double maxY)
{
    // move bars above graphs and grid below bars:
    plot->addLayer("abovemain", plot->layer("main"), QCustomPlot::limAbove);
    plot->addLayer("belowmain", plot->layer("main"), QCustomPlot::limBelow);
    plot->xAxis->grid()->setLayer("belowmain");
    plot->yAxis->grid()->setLayer("belowmain");

    // set some pens, brushes and backgrounds:
    plot->xAxis->setBasePen(QPen(Qt::white, 1));
    plot->yAxis->setBasePen(QPen(Qt::white, 1));
    plot->xAxis->setTickPen(QPen(Qt::white, 1));
    plot->yAxis->setTickPen(QPen(Qt::white, 1));
    plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    plot->xAxis->setTickLabelColor(Qt::white);
    plot->yAxis->setTickLabelColor(Qt::white);
    plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plot->xAxis->grid()->setSubGridVisible(true);
    plot->yAxis->grid()->setSubGridVisible(true);
    plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    plot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    plot->axisRect()->setBackground(axisRectGradient);

    plot->rescaleAxes();

    //Set Axis Range
    plot->xAxis->setRange(0, 5);
    plot->yAxis->setRange(0, maxY);
}

void MainWindow:: initPlot(int groupSize, int dataxSize, QVector<QString> itemName)
{
    // group size is the num of
    // prepare data:
    int i = 0;
    plotArea = new QCustomPlot(cenWidget);
    plotArea1 = new QCustomPlot(cenWidget);
    // preset color R,G,B,transparency.
    // ONLY 2 BAR COLOR ARE SPECIFIED. ADD YOUR OWN COLOR IF YOUR GROUP SIZE IS LARGER THAN 2.
    QVector<int> R = QVector<int>() << 190 << 254 << 33;
    QVector<int> G = QVector<int>() << 51 << 254 << 221;
    QVector<int> B = QVector<int>() << 18 << 0 << 221;
    QVector<int> T = QVector<int>() << 100 << 100 << 100;

    // create and configure plottables:
    QCPBarsGroup *group = new QCPBarsGroup(plotArea);
    QCPBarsGroup *group1 = new QCPBarsGroup(plotArea1);
    bars = QVector<QCPBars*>(groupSize);
    bars1 = QVector<QCPBars*>(groupSize);
    QCPBars *bar, *bar1;

    for(i = 0; i < groupSize; i++)
    {
        bar = new QCPBars(plotArea->xAxis, plotArea->yAxis);
        bar->setWidth(2/(double)dataxSize/(double)groupSize);
        bar->setName(itemName[i]);
        bar->setPen(Qt::NoPen);
        bar->setBrush(QColor(R[i], G[i], B[i], T[i]));
        bar->setBarsGroup(group);
        bars[i] = bar;

        bar1 = new QCPBars(plotArea1->xAxis, plotArea1->yAxis);
        bar1->setWidth(2/(double)dataxSize/(double)groupSize);
        bar1->setName(itemName[i]);
        bar1->setPen(Qt::NoPen);
        bar1->setBrush(QColor(R[i], G[i], B[i], T[i]));
        bar1->setBarsGroup(group1);
        bars1[i] = bar1;
    }

    plotSettings(plotArea, 90);
    plotSettings(plotArea1,10);


    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 2.5;
    labels << "Batch";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks,labels);
    plotArea->xAxis->setTicker(textTicker);
    ticks[0] = 2.5;
    labels[0] = "Recovery";
    QSharedPointer<QCPAxisTickerText> textTicker1(new QCPAxisTickerText);
    textTicker1->addTicks(ticks,labels);
    plotArea1->xAxis->setTicker(textTicker1);
}

void MainWindow:: updatePlot(QVector<double> x, QVector<double> y, int barIndex)
{
    QVector<double> xTemp,yTemp;
    xTemp << x[0];
    yTemp << y[0];
    bars[barIndex]->setData(xTemp, yTemp);
    xTemp[0] = x[1];
    yTemp[0] = y[1];
    bars1[barIndex]->setData(xTemp, yTemp);
    plotArea->legend->setVisible(true);
    plotArea->replot();
    plotArea1->legend->setVisible(true);
    plotArea1->replot();
}

//Slots
void MainWindow:: fileBtnOnClick() //Load File
{
    /*
    //default desktop folder
    QString src = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), src, tr("Text Files (*.txt)"));
    qDebug() << fileName;      //Test if file name is correct.
    */
    QString msg;
    msg = "0;" + QString::number(chooseDataSize) + ";0;|";
    char *sendBuff;
    QByteArray ba = msg.toLatin1();
    sendBuff = ba.data();
    qDebug()<<msg;
    this->cli_socket->write(sendBuff);
    runningTaskType = 0;
    //Your code to deal with input file address.
}

void MainWindow:: switchBtnOnClick() //Switch Mode
{
//    QVector<double> datax = QVector<double>() << 1 << 2 << 3 << 4;
//    QVector<double> datay1 = QVector<double>() << 0.6 << 0.5 << 0.3 << 0.15;
//    QVector<double> datay2 = QVector<double>() << 0.3 << 0.28 << 0.2 << 0.1;
//    updatePlot(datax, datay1, 0);
//    updatePlot(datax, datay2, 1);
    // switch mode
    QString msg;
    msg = msg + "3;" + QString(chooseMode) + ";0;|";
    char *sendBuff;
    QByteArray ba = msg.toLatin1();
    sendBuff = ba.data();
    this->cli_socket->write(sendBuff);
    runningTaskType = 3;
    qDebug() << msg;

}

void MainWindow:: batchQueryBtnOnClick() //Batch Query
{
//    QVector<double> datax = QVector<double>() << 1 << 2 << 3 << 4;
//    QVector<double> datay1 = QVector<double>() << 1.6 << 1.5 << 1.3 << 1.15;
//    QVector<double> datay2 = QVector<double>() << 1.3 << 1.28 << 1.2 << 1.1;
//    updatePlot(datax, datay1, 0);
//    updatePlot(datax, datay2, 1);
    // Batch Query
    QString msg;
    msg = msg + "1;" + "whichBatch" + ";0;|"; // whichBatch need to be added
    char *sendBuff;
    QByteArray ba = msg.toLatin1();
    sendBuff = ba.data();
    this->cli_socket->write(sendBuff);
    runningTaskType = 1;
    qDebug() << msg;
}

void MainWindow:: simBtnOnClick(){ //Start Simulation
    // Simulation
    QString msg;
    msg = msg + "6;" + QString::number(chooseDataSize) + ";0;|";
    char *sendBuff;
    QByteArray ba = msg.toLatin1();
    sendBuff = ba.data();
    this->cli_socket->write(sendBuff);
    runningTaskType = 6;
    qDebug() << msg;
}

void MainWindow:: searchBtnOnClick(){ //Single Query
    // Single Query
    QString msg;
    QString MBRsearch = searchText->text();
    msg = msg + "2;" + MBRsearch + ";0;|";
    char *sendBuff;
    QByteArray ba = msg.toLatin1();
    sendBuff = ba.data();
    this->cli_socket->write(sendBuff);
    runningTaskType = 2;
    qDebug() << msg;
}

void MainWindow:: connBtnOnClick(){
    this->cli_socket->connectToHost("127.0.0.1",4080, QTcpSocket::ReadWrite);
}
void MainWindow:: cleanBtnOnClick(){ //clean
    // Clean Data
    QString msg;
    msg = msg + "5;" + ";0;|";
    char *sendBuff;
    QByteArray ba = msg.toLatin1();
    sendBuff = ba.data();
    this->cli_socket->write(sendBuff);
    runningTaskType = 5;
    datay1[0] = 0;
    datay1[1] = 0;
    datay2[0] = 0;
    datay2[1] = 0;
    datay3[0] = 0;
    datay3[1] = 0;
    testSampleNumBatch[0] = 0;
    testSampleNumBatch[1] = 0;
    testSampleNumBatch[2] = 0;
    testSampleNumRecov[0] = 0;
    testSampleNumRecov[1] = 0;
    testSampleNumRecov[2] = 0;
    qDebug() << msg;
}

void MainWindow:: closeBtnOnClick(){ //close
    // Clean Data
    QString msg;
    msg = msg + "13;" + ";0;|";
    char *sendBuff;
    QByteArray ba = msg.toLatin1();
    sendBuff = ba.data();
    this->cli_socket->write(sendBuff);
    runningTaskType = -1;
    qDebug() << msg;
}

void MainWindow::connectFinish(){
    txtMessage->append("Connect to the NVMgts\n.");
}

void MainWindow::readyread(){
    QString strRead = this->cli_socket->readAll();
    QStringList allMsg;
    allMsg = strRead.split("|");
    QList<QString>::iterator it = allMsg.begin(),itend = allMsg.end();
    for(;it!=itend;it++)
    {
        QStringList infos;
        infos = (*it).split(';');
        //debug
        qDebug()<<(*it);
        //debug
        if(infos.size()<=1)
            break;
        int msgType = infos[0].toInt();
        int finishFlag = infos[2].toInt();
        switch(msgType){
        case 0: // load
        {
            if(finishFlag == 1)
            {
                QStringList msg;
                QStringList dataInTCPList = infos[1].split(':');
                int numTrajs = dataInTCPList[0].toInt();
                int numPoints = dataInTCPList[1].toInt();
                double loadTime = dataInTCPList[2].toDouble();
                msg << "Data Loading Finished. "<< QString::number(numTrajs)<<"trajectories and "<<QString::number(numPoints)
                    << "points are loaded within "<< QString::number(loadTime)<<"s.";
                txtMessage->append(msg.join(""));
                runningTaskType = -1;
            }
            else
            {
                qDebug()<< "error in Load Data.\n";
            }
            break;
        }
        case 1: // batch
        {
            if(finishFlag == 1)
            {
                QStringList msg;
                QStringList dataInTCPList = infos[1].split(':');
                double loadTime = dataInTCPList[0].toDouble();
                msg << "Batch Query is finished in" << QString::number(loadTime)<<"s.";
                txtMessage->append(msg.join(""));
                runningTaskType = -1;
                // plot on figure .............

                int isInDemo = dataInTCPList[1].toInt();
                if(isInDemo == 1)
                {
                    break;
                }
                qDebug()<<"system mode:" << systemMode;
                switch (systemMode) {
                case 'd':
                    datay1[0] = (datay1[0]*testSampleNumBatch[0]+loadTime)/(++testSampleNumBatch[0]);
                    updatePlot(datax, datay1, 0);
                    break;
                case 'h':
                    datay2[0] = (datay2[0]*testSampleNumBatch[1]+loadTime)/(++testSampleNumBatch[1]);
                    updatePlot(datax, datay2, 1);
                    break;
                case 's':
                    datay3[0] = (datay3[0]*testSampleNumBatch[2]+loadTime)/(++testSampleNumBatch[2]);
                    updatePlot(datax, datay3, 2);
                    break;
                default:
                    break;
                }
                // under finishing.............
            }
            else
            {
                qDebug()<< "error in Batch Query.\n";
            }
            break;
        }
        case 2://single
        {
            if(finishFlag == 1)
            {
                QStringList msg;
                QString allresult = infos[1];
                QFile file("./resultSingleQuery.txt");
                file.open(QIODevice::ReadWrite);
                QTextStream out(&file);
                out << allresult ;
                out.flush();
                file.close();
                msg << "Single Query is finished. Results are stored in file.";
                txtMessage->append(msg.join(""));
                runningTaskType = -1;
            }
            else
            {
                qDebug()<< "error in Single Query.\n";
            }
            break;
        }
        case 3: //mode switch
        {
            if(finishFlag == 1)
            {
                QStringList msg;
                QString modeStr = infos[1];
                char mode;
                mode = modeStr.at(0).toLatin1();
                QString modeName;
                switch(mode)
                {
                    case 'd':
                    {
                    modeName = "pure DRAM";
                    break;
                    }
                case 'h':
                {
                    modeName = "DRAM-NVM hybrid";
                    break;
                }
                case 's':
                {
                    modeName = "pure NVM";
                    break;
                }
                default:
                {
                    modeName = "N/A";
                    break;
                }
                }
                msg << "System is changed to" << modeName << "mode.";
                runningTaskType = -1;
                txtMessage->append(msg.join(""));
            }
            else
            {
                qDebug()<< "error in Switch Mode.\n";
            }
            break;
        }
        case 4: // get state message
        {
            if(finishFlag == 1)
            {
                QStringList dataInTCPList = infos[1].split(':');
                int numTrajs = dataInTCPList[0].toInt();
                int numPoints = dataInTCPList[1].toInt();
                QChar systemModeChar = dataInTCPList[2][0];
                systemMode = systemModeChar.toLatin1();
                int queryID = dataInTCPList[3].toInt();
                int runningTask = dataInTCPList[4].toInt();
                numTrajSys = numTrajs;
                numPointSys = numPoints;
                queryIDNow = queryID;
                runningTaskType = runningTask;
            }
            else
            {
                qDebug()<< "error in Get Message.\n";
            }
            break;
        }
        case 5: // clean
        {
            if(finishFlag == 1)
            {
                QStringList msg;
                numTrajSys = 0;
                numPointSys = 0;
                queryIDNow = -1;
                runningTaskType = -1;
                msg << "Clean Successful.\n";
                txtMessage->append(msg.join(""));
            }
            else
            {
                qDebug()<< "error in Clean.\n";
            }
            break;
        }
        case 6: // demo
        {
            if(finishFlag == 1)
            {
                QStringList msg;
                QStringList dataInTCPList = infos[1].split(':');
                double runningTime = dataInTCPList[0].toDouble();
                int demoStage = dataInTCPList[1].toInt();
                if(demoStage == 1)
                {
                    runningTaskType = 6;
                    msg << "System Down. Waiting for restarting...\n";
                    txtMessage->append(msg.join(""));
                }
                else if(demoStage == 2)
                {
                    runningTaskType = 6;
                    msg << "System restarted. Start Recovery...\n";
                    txtMessage->append(msg.join(""));
                }
                else if(demoStage == 3)
                {
                    runningTaskType = -1;
                    msg << "Finish Demo. The Recovery Time is "
                        << QString::number(runningTime) << " s under "
                        << QString(systemMode) << " mode.\n";
                    txtMessage->append(msg.join(""));
                    switch (systemMode) {
                    case 'd':
                        datay1[1] = (datay1[1]*testSampleNumRecov[0]+runningTime)/(++testSampleNumRecov[0]);
                        updatePlot(datax, datay1, 0);
                        break;
                    case 'h':
                        datay2[1] = (datay2[1]*testSampleNumRecov[1]+runningTime)/(++testSampleNumRecov[1]);
                        updatePlot(datax, datay2, 1);
                        break;
                    case 's':
                        datay3[1] = (datay3[1]*testSampleNumRecov[2]+runningTime)/(++testSampleNumRecov[2]);
                        updatePlot(datax, datay3, 2);
                        break;
                    default:
                        break;
                    }
                }
                // plot on figure .............
                // under finishing.............
            }
            else
            {
                qDebug()<< "error in Demo.\n";
            }
            break;
        }
        case 13: // close app
        {
            if(finishFlag == 1)
            {
                QStringList msg;
                runningTaskType = -1;
                msg << "GTS closed.\n";
                txtMessage->append(msg.join(""));
            }
            else
            {
                qDebug()<< "error in Load Data.\n";
            }
            break;
        }
        default:
            break;
        }
    }

}

void MainWindow::flushState(){
    if(runningTaskType != -1){
        bFile->setDisabled(true);
        bSwitch->setDisabled(true);
        bBatch->setDisabled(true);
        bConn->setDisabled(true);
        bSim->setDisabled(true);
        bSearch->setDisabled(true);
        bClean->setDisabled(true);
    }
    else
    {
        bFile->setDisabled(false);
        bSwitch->setDisabled(false);
        bBatch->setDisabled(false);
        bConn->setDisabled(false);
        bSim->setDisabled(false);
        bSearch->setDisabled(false);
        bClean->setDisabled(false);
    }
    // update this to text
    QStringList msg;
    msg << "Number of Trajectories in Mem: "<<QString::number(numTrajSys)
        << "\nNumber of Points in Mem: "<<QString::number(numPointSys)<< "\n";
           switch (runningTaskType) {
   case -1:
           msg << "Status: Free.\n" ;
           break;
   case 0:
           msg << "Status: Loading Data.\n" ;
           break;
   case 1:
           msg << "Status: Running batch query: "<< QString::number(queryIDNow) << ".\n" ;
           break;
   case 2:
           msg << "Status: Running single query.\n" ;
           break;
   case 3:
           msg << "Status: Switching Mode.\n" ;
           break;
   case 4:
           msg << "Status: Checking system status.\n" ;
           break;
   case 5:
           msg << "Status: Cleaning Data.\n" ;
           break;
   case 6:
           msg << "Status: Demo.\n" ;
           break;
   case 13:
           msg << "Status: GTS closed.\n" ;
           break;
   default:
           break;
    }
    txtSystemStatus->setText(msg.join(""));
}

// End Slots

MainWindow::~MainWindow()
{
    delete ui;
}
