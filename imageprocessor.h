#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <QVector>
#include <QObject>
#include <keepncalc.h>
#include <QDateTime>


class ImageProcessor: public QObject
{
    Q_OBJECT

public:
    ImageProcessor();
    void init(KeepNcalc *cont);
    cv::Mat findSkinRegions(cv::Mat& frame);



public slots:
    void fullOneFrameProcess(cv::Mat frame, QDateTime time);


signals:
    void faceIsReade(cv::Mat face);
    void sendImage(QImage &image);
    void numberIsReady(double intence);
    void sendMeasResult(double red, double blue, double green, QDateTime time);

private:
    QDateTime shotTime;
    int face_x;
    int face_y;
    int face_r;
    KeepNcalc *container;
    cv::CascadeClassifier faceHaarCascade;


    void detectFace(cv::Mat &frame);

    cv::Mat filterSkinMask(cv::Mat& mask);
    void calculateAverage(cv::Mat& frame);
    void convertMatToImage(cv::Mat &frame);








};

#endif // IMAGEPROCESSOR_H
