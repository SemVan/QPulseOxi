#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <protocol.h>
#include <QSerialPort>
#include <QElapsedTimer>
#include <fftw3.h>
#include <fstream>
#include <keepncalc.h>
#include <QThread>




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();


private:
    QThread *device1thread;
    QThread *device2thread;
    std::ofstream file;
    int fftSize;

    QElapsedTimer timer;
    QTimer sigTime;

    void initGraphsAndDataContents();
    void initComPortsSearch();
    void writeFIle(QVector<double> signal, QString fileName);
    Ui::MainWindow *ui;
    protocol *pulseOxi;
    protocol *distMeas;
    QSerialPort *portToConnect;
    QSerialPort *portToConnect_2;
    void connectToPort(QSerialPort *port, QComboBox *box,protocol* device, int length, QString name);
    void connectDeviceToThread(QThread *thr, protocol *device);

    fftw_complex *out;
    double *in;
    int currentItemNumberBlue;
    int currentItemNumberGreen;

};

#endif // MAINWINDOW_H
