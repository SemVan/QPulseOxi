#ifndef KEEPNCALC_H
#define KEEPNCALC_H

#include <QVector>
#include <QObject>
#include <fftw3.h>
#include <qcustomplot.h>


class KeepNcalc : public QObject
{
    Q_OBJECT
public:
    explicit KeepNcalc(QObject *parent = 0);
    void init(QCustomPlot* chart, int number);

    QVector<double> greenChannel;
    QVector<double> blueChannel;
    QVector<double> redChannel;
    QVector<double> time;
    QVector<double> filTime;
    double max;
    double min;
    int numberOfElements;

    QVector<double> FftresultY;
    QVector<double> FftresultX;

    double findHeartRate();

    void clerContainers();
    void plotGraph(double x, double y);

private:

    QCustomPlot *plot;
    int graphNum;

signals:

public slots:
    void addNewData(double green, double blue);
    void addNewData(double green, double blue, double red);

};

#endif // KEEPNCALC_H
