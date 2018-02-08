#ifndef KEEPNCALC_H
#define KEEPNCALC_H

#include <QVector>
#include <QObject>
#include <qcustomplot.h>
#include <QDateTime>


class KeepNcalc : public QObject
{
    Q_OBJECT
public:
    explicit KeepNcalc(QObject *parent = 0);
    void init(QCustomPlot* chart, int number, QString name);
    void clerContainers();
    void plotGraph(double x, double y);
    bool measurementComplete;
    void write1Channels(QString filepath);
    void write3Channels(QString filepath);

private:
    QString containerName;
    QCustomPlot *plot;
    int graphNum;
    QVector<double> greenChannel;
    QVector<double> blueChannel;
    QVector<double> redChannel;
    QVector<double> time;
    QVector<QDateTime> times;
    double max;
    double min;
    int numberOfElements;



signals:
    void completedMeasuring();

public slots:
    void addNewData(double green, double blue, QDateTime shotTime);
    void addNewData(double green, double blue, double red, QDateTime shotTime);

};

#endif // KEEPNCALC_H
