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


    ui->widget->addGraph();//for blue channel
    ui->widget->graph(0)->setPen(QPen(QColor(0,0,255)));
    ui->widget->setInteraction(QCP::iRangeZoom,true);
    ui->widget->setInteraction(QCP::iRangeDrag,true);
    ui->widget->yAxis->setRange(500, 550);
    ui->widget->xAxis->setRange(0, 1000);


    ui->widget_2->setInteraction(QCP::iRangeZoom,true);
    ui->widget_2->setInteraction(QCP::iRangeDrag,true);


    ui->widget->xAxis->setLabel("Signal count");
    ui->widget->yAxis->setLabel("ADC count");

    ui->widget_2->xAxis->setLabel("Signal count");
    ui->widget_2->yAxis->setLabel("ADC count");

    ui->widget_2->addGraph();
    ui->widget_2->yAxis->setRange(500, 550);
    ui->widget_2->xAxis->setRange(0, 1000);

    fftSize=4096;

    ui->horizontalSlider->setMaximum(-1);
    ui->horizontalSlider->setMinimum(-15);
    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    ui->horizontalSlider->setValue(-6);
    tool->setExposure(-6);

    contactContainer=new KeepNcalc();
    contactContainer->init(ui->widget,0);
    bezcontactContainer=new KeepNcalc();
    bezcontactContainer->init(ui->widget_2,0);



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
    pulseOxi = new protocol("m");
    connectToPort(portToConnect,ui->comboBox, pulseOxi, 7, "1pulse", contactContainer);
    device1thread = new QThread();
    connectDeviceToThread(device1thread, pulseOxi);

    distMeas = new protocol("\r");
    connectToPort(portToConnect_2,ui->comboBox_2, distMeas, 7,"2pulse", bezcontactContainer);
    device2thread = new QThread();
    connectDeviceToThread(device2thread, distMeas);

    cameraThread = new QThread();
    QObject::connect(cameraThread, SIGNAL(started()), tool, SLOT(start()));
    //QObject::connect(cameraThread, SIGNAL(finished()), tool, SLOT(stop()));


    procThread = new QThread();
    QObject::connect(procThread, SIGNAL(started()), proc, SLOT(start()));



    QObject::connect(tool, SIGNAL(sendMat(cv::Mat)),proc,SLOT(fullOneFrameProcess(cv::Mat)));
    QObject::connect(proc, SIGNAL(sendImage(QImage&)),disp,SLOT(showImage(QImage&)), Qt::BlockingQueuedConnection);


    tool->moveToThread(cameraThread);
    proc->moveToThread(procThread);
    procThread->start();
    cameraThread->start();
    device1thread->start();
    device2thread->start();

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
