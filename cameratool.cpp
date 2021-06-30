#include "cameratool.h"
#include <QDebug>

CameraTool::CameraTool(QObject *parent) : QObject(parent)
{
    doWrite = false;
}


void CameraTool::startCamera() {
    for (int i=1;i<20;i++) {
        if(cam.open(i)) {
            break;
        }
    }
    cam.set(CV_CAP_PROP_AUTO_EXPOSURE, 0.0);
    cam.set(CV_CAP_PROP_EXPOSURE, -6.0);
    cam.set(CV_CAP_PROP_FPS, 30);


}

void CameraTool::convertMatToImage(cv::Mat &frame) {
    cv::Mat temp;
    cv::cvtColor(frame,temp,CV_BGR2RGB);
    image = QImage((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);;
    image = image.rgbSwapped();

}


void CameraTool::getImage() {
    timeShot = QDateTime::currentDateTime();
    if (!cam.read(img)) {
        fileEnded(readingFileName);
        return;
    }
    if (doWrite && writer.isOpened()) {
        writer.write(img);
    }
    sendMat(img, timeShot);
}


void CameraTool::setExposure(double exposure) {
    cam.set(CV_CAP_PROP_EXPOSURE, exposure);

}


void CameraTool::timeOutHandler() {
    qDebug()<<elTimer.nsecsElapsed()<<"between captures";
    elTimer.start();
    getImage();
//    qDebug()<<"image grabbed"<<elTimer.elapsed();
}


void CameraTool::start() {
    timer = new QTimer();
    QObject::connect(timer,SIGNAL(timeout()), this, SLOT(timeOutHandler()));
    timer->setInterval(40);
    timer->setSingleShot(false);
    timer->setTimerType(Qt::PreciseTimer);
    timer->start();
}


void CameraTool::setVideoWriter(QString name,  bool flag) {
    if (flag) {
        int height = cam.get(CV_CAP_PROP_FRAME_HEIGHT);
        int width = cam.get(CV_CAP_PROP_FRAME_WIDTH);
        int four = CV_FOURCC('M','G','P','J');
        bool suc = writer.open(name.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(width, height), true);

        doWrite = flag;
    }
}


void CameraTool::getFrameSize(int *hi, int *wi) {
    *hi = cam.get(CV_CAP_PROP_FRAME_WIDTH);
    *wi = cam.get(CV_CAP_PROP_FRAME_HEIGHT);
}


void CameraTool::stop() {
    if (doWrite) {
        doWrite = false;
        writer.release();
    }
    timer->stop();
}


void CameraTool::setReadFIleName(QString readFileName) {
    readingFileName = readFileName;
    //readFileName.append(".mov");
    if (cam.isOpened()) {
        cam.release();
    }
    cam.open(readFileName.toStdString());
    if (cam.isOpened()) {
        qDebug()<<"video file opened to read";
    } else {
        qDebug()<<"video file NOT opened to read";
    }
}
