#include "keepncalc.h"
#include <QObject>
#include <fftw3.h>
#include <math.h>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

KeepNcalc::KeepNcalc(QObject *parent) : QObject(parent)
{
    blueChannel.clear();
    greenChannel.clear();
    time.clear();
    numberOfElements=0;
    max = 0;
    min = 4096;

    measurementComplete = false;
}


void KeepNcalc::init(QCustomPlot *chart, int number, QString name) {
    plot = chart;
    graphNum = number;
    containerName = name;
}

void KeepNcalc::addNewData(double green, double blue, QDateTime shotTime) {
    blueChannel.append(blue);
    greenChannel.append(green);
    time.append(numberOfElements);
    times.append(shotTime);
    //QFuture<void> future = QtConcurrent::run(this,&KeepNcalc::plotGraph,numberOfElements, blue);
    plotGraph(numberOfElements, green);
    numberOfElements++;
    if (numberOfElements==1024) {
        measurementComplete = true;
        completedMeasuring();
    }
}


void KeepNcalc::addNewData(double green, double blue, double red, QDateTime shotTime) {
    blueChannel.append(blue);
    greenChannel.append(green);
    redChannel.append(red);
    time.append(numberOfElements);
    times.append(shotTime);
    //QFuture<void> future = QtConcurrent::run(this,&KeepNcalc::plotGraph,numberOfElements, blue);
    plotGraph(numberOfElements, green);
    numberOfElements++;
    if (numberOfElements==1024) {
        measurementComplete = true;
        completedMeasuring();
    }
}


void KeepNcalc::plotGraph(double x, double y) {
    if (y>max) {
        max = y+1;
    } else {
        if (y<min) {
            min = y-1;
        }
    }
    if (numberOfElements%1000==0) {
        plot->xAxis->setRange(0, numberOfElements*2);
    }
    plot->yAxis->setRange(min, max);
    plot->graph(graphNum)->addData(x, y);
    plot->replot( );
}



void KeepNcalc::clerContainers() {
    blueChannel.clear();
    greenChannel.clear();
    redChannel.clear();
    time.clear();
    numberOfElements=0;
}

void KeepNcalc::write1Channels(QString filepath) {
    QString filename= filepath+"\\"+containerName+".txt";
    QFile data(filename);
    if(data.open(QFile::WriteOnly |QFile::Truncate)) {
        QTextStream output(&data);
        //output<<0<<","<<greenChannel.at(0)<<"\r";
        for(int i=1; i<greenChannel.length(); i++) {
            //qint64 period = times.at(i).toMSecsSinceEpoch()-times.at(i-1).toMSecsSinceEpoch();
            QString now = times.at(i).toString("hh:mm:ss:zzz");
            output<<now<<","<<greenChannel.at(i)<<"\r";
        }
    }
}

void KeepNcalc::write3Channels(QString filepath) {
    QString filename= filepath+"\\"+containerName+".txt";
    QFile data(filename);
    if(data.open(QFile::WriteOnly |QFile::Truncate)) {
        QTextStream output(&data);
        //output<<0<<","<<greenChannel.at(0)<<"\r";
        for(int i=0; i<greenChannel.length(); i++) {
            //qint64 period = times.at(i).toMSecsSinceEpoch()-times.at(i-1).toMSecsSinceEpoch();
            QString now = times.at(i).toString("hh:mm:ss:zzz");
            output<<now<<","<<greenChannel.at(i)<<","<<blueChannel.at(i)<<","<<redChannel.at(i)<<"\r";
        }
    }
}
