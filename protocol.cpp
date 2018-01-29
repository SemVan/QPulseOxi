#include "protocol.h"
#include <QDebug>
#include <QTimer>

protocol::protocol(std::string mes)
{
    command = QByteArray::fromStdString(mes);
    maxBufferSize=1024;
    totalIncome.clear();
    container = new KeepNcalc();
}

void protocol::init(QSerialPort *y, int length, QString name) {

    devName = name;
    MESSAGE_LENGTH = length;
    portUSB=y;
    portUSB->setBaudRate(QSerialPort::Baud115200);
    portUSB->setDataBits(QSerialPort::Data8);
    portUSB->setFlowControl(QSerialPort::NoFlowControl);
    portUSB->setParity(QSerialPort::NoParity);

    totalIncome=portUSB->readAll();
    totalIncome.clear();
    connect(portUSB, &QSerialPort::readyRead, this, &protocol::getData);
}

/*!
 * \brief protocol::getData Gets one full MC reply (ended with CR) from buffer
 */
void protocol::getData()
{

    if((portUSB->isOpen())&&(portUSB->bytesAvailable()>0)) {
        int quan=portUSB->bytesAvailable();
        unsigned char incomeChar[quan];
        portUSB->read((char*)incomeChar,quan);

        int freeSpaceInBuffer=256-totalIncome.size();

        if (freeSpaceInBuffer>=quan) {
            totalIncome.append((char*)incomeChar,quan);
        } else {
            totalIncome.append((char*)incomeChar,freeSpaceInBuffer);
        }

    }

    if (totalIncome.at(totalIncome.size()-1)!=13) {
        return;
    }

    parceAnswer(totalIncome);
    totalIncome.clear();
}

/*!
 * \brief protocol::parceAnswer Parces MC reply and calls further functions to handle it
 * \param answer QByteArray contains full MC answer
 */
void protocol::parceAnswer(QByteArray answer) {

     if (answer.size() == MESSAGE_LENGTH) {
        QString blue=answer.mid(0,3);
        QString green=answer.mid(3,3);

        bool okBlue;
        bool okGreen;

        int resultBlue=blue.toInt(&okBlue,16);
        int resultGreen=green.toInt(&okGreen,16);

        if (okBlue && okGreen) {
            container->addNewData(resultGreen, resultBlue);
        }

    }
}


double protocol::fromBytesToDouble(QByteArray data) {
    int h = (int)data.at(1);
    if (h < 0) {
        h = h+255;
    }
    int l = (int)data.at(0);
    if (l < 0) {
        l = l+255;
    }

    int Bytes = l << 8 | h;

    double temperature = (float) Bytes / 256;
    sendMeasResult(temperature, temperature);
    return temperature;
}


/*!
 * \brief protocol::sendRequest Sends request to read ADC result
 */
void protocol::sendRequest() {
    if (portUSB->isOpen()) {
        qDebug()<<devName<<elTimer->nsecsElapsed();
        elTimer->restart();
        portUSB->write(command);
    }
}

void protocol::start() {
    elTimer = new QElapsedTimer();
    timer = new QTimer();
    timer->setInterval(10);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setSingleShot(false);

    QObject::connect(timer,SIGNAL(timeout()), this,SLOT(sendRequest()));
    elTimer->start();
    timer->start();
}

void protocol::stop() {

    timer->stop();
}
