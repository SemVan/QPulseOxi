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

class ImageProcessor: public QObject
{
    Q_OBJECT

public:
    ImageProcessor();
    void init(KeepNcalc *cont);
    cv::Mat findSkinRegions(cv::Mat& frame);



public slots:
    void fullOneFrameProcess(cv::Mat frame);


signals:
    void faceIsReade(cv::Mat face);
    void sendImage(QImage &image);
    void numberIsReady(double intence);
    void sendMeasResult(double red, double blue, double green);

private:
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
