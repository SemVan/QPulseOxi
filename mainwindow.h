#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <protocol.h>
#include <QSerialPort>
#include <QElapsedTimer>
#include <fstream>
#include <keepncalc.h>
#include <cameratool.h>
#include <displayer.h>
#include <imageprocessor.h>
#include <QThread>
#include <QDateTime>



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
    void sliderValueChanged(int value);
    void someoneCompletedMeasuring();


    void on_pushButton_3_clicked();

private:
    bool fileReading;
    QDateTime thisTime;
    QThread *device1thread;
    QThread *device2thread;
    QThread *cameraThread;
    QThread *procThread;

    std::ofstream file;
    int signalSize;

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
    ImageProcessor *proc;
    CameraTool *tool;
    displayer *disp;

    KeepNcalc *contactContainer;
    KeepNcalc *bezcontactContainer;
    KeepNcalc *cameraContainer;

    bool sendInfoRequest(QSerialPort &portToCheck, QString expectedAnswer);
    void setGraphParams(QCustomPlot *chart, QString xName, QString yName, QString title);
    void createAllObjects();
    void connectToPort(QSerialPort *port, QComboBox *box,protocol* device, int length, QString name, KeepNcalc *container);
    void connectDeviceToThread(QThread *thr, protocol *device);

    void initDevices();
    void initCameraTool();
    void initContaclessDevice();
    void initContactDevice();

    void createThreads();
    void startMeasurement();
    void stopMeasurement();

    double *in;
    int currentItemNumberBlue;
    int currentItemNumberGreen;

    QStringList videoFiles;
    int filesToRead;
    int curVideo;
    void videoFileReadingPrepare();

signals:
    void heyYouFreeze();
    void startCamera();

};

#endif // MAINWINDOW_H
