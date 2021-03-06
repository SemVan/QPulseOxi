#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <QSerialPort>
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <keepncalc.h>


class protocol:public QObject
{
    Q_OBJECT

public:
    protocol(std::string mes);
    void init(QSerialPort *y, int length, QString name);
    void refreshFile();

signals:
    void sendMeasResult(double blChRes,double grChRes, QDateTime time);


private:
    QDateTime shotTime;
    int number;
    QString devName;
    QTimer *timer;
    QElapsedTimer *elTimer;
    KeepNcalc *container;
    int MESSAGE_LENGTH;
    QByteArray command;
    QSerialPort *portUSB;
    QByteArray totalIncome;
    int maxBufferSize;
    void CommandDispatcher(QByteArray data);
    void parceAnswer(QByteArray answer);
    void parceFingerSensor(QByteArray answer);
    void findPort();


public slots:
    void sendRequest();
    void start();
    void stop();
private slots:
    void getData();

};

#endif // PROTOCOL_H
