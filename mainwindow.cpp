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

    ui->progressBar->setTextVisible(false);

    ui->widget->addGraph();//for blue channel
    ui->widget->graph(0)->setName("Blue channel");
    ui->widget->graph(0)->setPen(QPen(QColor(0,0,255)));
    ui->widget->addGraph();//for green channel
    ui->widget->graph(1)->setName("Red channel");
    ui->widget->graph(1)->setPen(QPen(QColor(0,255,0)));
    ui->widget->setInteraction(QCP::iRangeZoom,true);
    ui->widget->setInteraction(QCP::iRangeDrag,true);
    ui->widget->yAxis->setRange(500, 550);
    ui->widget->xAxis->setRange(0, 1000);


    ui->widget_2->setInteraction(QCP::iRangeZoom,true);
    ui->widget_2->setInteraction(QCP::iRangeDrag,true);

    ui->widget->legend->setFont(QFont());
    ui->widget->legend->setBrush(QBrush(QColor(255,255,255,230)));
   // ui->widget->legend->setVisible(true);

    ui->widget->xAxis->setLabel("Signal count");
    ui->widget->yAxis->setLabel("ADC count");

    ui->widget_2->xAxis->setLabel("Frequency, Hz");

    ui->widget_2->addGraph();
    ui->widget_2->yAxis->setRange(500, 550);
    ui->widget_2->xAxis->setRange(0, 1000);

    fftSize=4096;

    ui->progressBar->setMaximum(fftSize);
    ui->progressBar->setValue(0);

//    contactContainer=new KeepNcalc();
//    contactContainer->init(ui->widget,0);
//    bezcontactContainer=new KeepNcalc();
//    bezcontactContainer->init(ui->widget_2,0);



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
    connectToPort(portToConnect,ui->comboBox, pulseOxi, 7, "1pulse");
    device1thread = new QThread();
    connectDeviceToThread(device1thread, pulseOxi);

    distMeas = new protocol("\r");
    connectToPort(portToConnect_2,ui->comboBox_2, distMeas, 7,"2pulse");
    device2thread = new QThread();
    connectDeviceToThread(device2thread, distMeas);

    device1thread->start();
    device2thread->start();

}


void MainWindow::connectToPort(QSerialPort *port, QComboBox *box, protocol *device, int length, QString name) {
    int number=box->currentIndex();
    port->setPort(QSerialPortInfo::availablePorts().at(number));

    if (!port->open(QIODevice::ReadWrite)) {//port can not be opened
        qDebug() <<(port->errorString());
        ui->statusBar->showMessage(port->errorString(),0);
        return;

    } else {
        device->init(port, length, name);
    }
}


void MainWindow::connectDeviceToThread(QThread *thr, protocol *device) {
    QObject::connect(thr, SIGNAL(started()), device, SLOT(start()));
    QObject::connect(thr, SIGNAL(finished()), device, SLOT(stop()));
    device->moveToThread(thr);
}

