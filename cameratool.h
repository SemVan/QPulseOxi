#ifndef CAMERATOOL_H
#define CAMERATOOL_H

#include <QObject>
#include <QImage>
#include <QElapsedTimer>
#include <QTimer>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QDateTime>


class CameraTool : public QObject
{
    Q_OBJECT
public:
    explicit CameraTool(QObject *parent = 0);

    void getImage();
    void convertMatToImage(cv::Mat &frame);
    void startCamera();
    void setExposure(double exposure);



private:
    QDateTime timeShot;
    cv::VideoCapture cam;
    cv::Mat img;
    QTimer *timer;
    QElapsedTimer elTimer;
    QImage image;

signals:
    void sendImage(QImage &image);
    void sendMat(cv::Mat image, QDateTime time);

private slots:
    void timeOutHandler();

public slots:
    void start();
    void stop();
};

#endif // CAMERATOOL_H
