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
}


void KeepNcalc::init(QCustomPlot *chart, int number) {
    plot = chart;
    graphNum = number;
}

void KeepNcalc::addNewData(double green, double blue) {
    qDebug()<<numberOfElements<<"got from receiver";
    blueChannel.append(blue);
    greenChannel.append(green);
    time.append(numberOfElements);
    //QFuture<void> future = QtConcurrent::run(this,&KeepNcalc::plotGraph,numberOfElements, blue);
    plotGraph(numberOfElements, green);
    numberOfElements++;
}


void KeepNcalc::plotGraph(double x, double y) {
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
