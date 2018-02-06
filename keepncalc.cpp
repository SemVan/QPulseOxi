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
}


void KeepNcalc::init(QCustomPlot *chart, int number) {
    plot = chart;
    graphNum = number;
}

void KeepNcalc::addNewData(double green, double blue) {
    blueChannel.append(blue);
    greenChannel.append(green);
    time.append(numberOfElements);
    //QFuture<void> future = QtConcurrent::run(this,&KeepNcalc::plotGraph,numberOfElements, blue);
    plotGraph(numberOfElements, green);
    numberOfElements++;
}

void KeepNcalc::addNewData(double green, double blue, double red) {
    blueChannel.append(blue);
    greenChannel.append(green);
    redChannel.append(red);
    time.append(numberOfElements);
    //QFuture<void> future = QtConcurrent::run(this,&KeepNcalc::plotGraph,numberOfElements, blue);
    plotGraph(numberOfElements, green);
    numberOfElements++;
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
    FftresultY.clear();
    FftresultX.clear();

    time.clear();
    numberOfElements=0;
}
