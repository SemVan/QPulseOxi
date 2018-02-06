#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QElapsedTimer>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <QElapsedTimer>
#include <QFile>


const double pi = 3.14;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->show();
    createAllObjects();
    initGraphsAndDataContents();
    initComPortsSearch();



}

MainWindow::~MainWindow()
{
    file.close();
    delete ui;
}


void MainWindow::writeFIle(QVector<double> signal, QString fileName) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            for (int i =0; i< signal.length(); i++) {
                stream << QString::number(signal.at(i))+"\n" ;
            }
        }
        file.close();
}


/*!
 * \brief MainWindow::initGraphsAndDataContents Creates graphs on the plotting widget, inits QVectors to contain measurement results
 */
void MainWindow::initGraphsAndDataContents() {
    signalSize = 4096;

    setGraphParams(ui->widget, "Signal count", "ADC counts", "Contact pletysmograph");
    setGraphParams(ui->widget_2, "Signal count", "ADC counts", "Contactless pletysmograph");
    setGraphParams(ui->widget_3, "Signal count", "ADC counts", "Video pletysmograph");


    ui->horizontalSlider->setMaximum(-1);
    ui->horizontalSlider->setMinimum(-15);
    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    ui->horizontalSlider->setValue(-6);
    tool->setExposure(-6);

    contactContainer=new KeepNcalc();
    contactContainer->init(ui->widget,0);
    bezcontactContainer=new KeepNcalc();
    bezcontactContainer->init(ui->widget_2,0);
    cameraContainer = new KeepNcalc();
    cameraContainer->init(ui->widget_3, 0);



}

void MainWindow::setGraphParams(QCustomPlot *chart, QString xName, QString yName, QString title) {
    chart->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
    chart->addGraph();
    chart->graph(0)->setPen(QPen(QColor(0,0,255)));
    chart->setInteraction(QCP::iRangeZoom,true);
    chart->setInteraction(QCP::iRangeDrag,true);
    chart->xAxis->setLabel(xName);
    chart->yAxis->setLabel(yName);
    chart->xAxis->setRange(0, 1000);
    chart->plotLayout()->insertRow(0);
    chart->plotLayout()->addElement(0, 0, new QCPTextElement(chart, title, QFont("sans", 8, QFont::Bold)));

}

void MainWindow::createAllObjects() {
    tool = new CameraTool();
    tool->startCamera();

    disp = new displayer();
    disp->init(ui->graphicsView);

    proc = new ImageProcessor();

    qRegisterMetaType<cv::Mat>("cv::Mat");

}

/*!
 * \brief MainWindow::initComPortsSearch Looks for available COM-ports and puts them in combo-box
 */
void MainWindow::initComPortsSearch() {

    QSerialPortInfo portsInfo;
    portToConnect=new QSerialPort(this);
    portToConnect_2=new QSerialPort(this);

    for (int i=0;i<portsInfo.availablePorts().size();i++) {

        QSerialPort currentPort;
        currentPort.setPort(portsInfo.availablePorts().at(i));
        ui->comboBox->addItem(currentPort.portName());
        ui->comboBox_2->addItem(currentPort.portName());

    }
}


void MainWindow::on_pushButton_clicked()
{
    createThreads();
    initDevices();
    startMeasurement();

}

void MainWindow::initDevices() {
    pulseOxi = new protocol("m");
    connectToPort(portToConnect,ui->comboBox, pulseOxi, 7, "1pulse", contactContainer);
    connectDeviceToThread(device1thread, pulseOxi);

    distMeas = new protocol("\r");
    connectToPort(portToConnect_2,ui->comboBox_2, distMeas, 7,"2pulse", bezcontactContainer);
    connectDeviceToThread(device2thread, distMeas);

    tool->moveToThread(cameraThread);
    proc->moveToThread(procThread);

    QObject::connect(cameraThread, SIGNAL(started()), tool, SLOT(start()));
    QObject::connect(tool, SIGNAL(sendMat(cv::Mat)),proc,SLOT(fullOneFrameProcess(cv::Mat)));
    QObject::connect(proc, SIGNAL(sendImage(QImage&)),disp,SLOT(showImage(QImage&)), Qt::BlockingQueuedConnection);
    QObject::connect(proc, SIGNAL(sendMeasResult(double,double,double)), cameraContainer, SLOT(addNewData(double,double,double)));
}

void MainWindow::createThreads() {
    cameraThread = new QThread();
    device1thread = new QThread();
    device2thread = new QThread();
    procThread = new QThread();

}

void MainWindow::startMeasurement() {
    procThread->start();
    cameraThread->start();
    device1thread->start();
    device2thread->start();
}

void MainWindow::stopMeasurement() {

}

void MainWindow::connectToPort(QSerialPort *port, QComboBox *box, protocol *device, int length, QString name, KeepNcalc *container) {
    int number=box->currentIndex();
    port->setPort(QSerialPortInfo::availablePorts().at(number));

    if (!port->open(QIODevice::ReadWrite)) {//port can not be opened
        qDebug() <<(port->errorString());
        ui->statusBar->showMessage(port->errorString(),0);
        return;

    } else {
        device->init(port, length, name);
        QObject::connect(device, SIGNAL(sendMeasResult(double,double)), container, SLOT(addNewData(double,double)));
    }
}


void MainWindow::connectDeviceToThread(QThread *thr, protocol *device) {
    QObject::connect(thr, SIGNAL(started()), device, SLOT(start()));
    QObject::connect(thr, SIGNAL(finished()), device, SLOT(stop()));
    device->moveToThread(thr);
}

void MainWindow::sliderValueChanged(int value) {
    ui->label->setText(QString::number(value));
    tool->setExposure((double)value);
}
