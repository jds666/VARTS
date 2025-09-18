// processdata.h
#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <cmath>

class ProcessData
{
public:



    static QVector<double> preprocessDatatoDB(const QVector<QString>& xdata, const QVector<QString>& ydata, double epsilon);
    static QVector<double> preprocessQStringtoDouble(const QVector<QString>& QStringdata);
    static void processData(const QVector<QString>& xdata, const QVector<QString>& ydata, double epsilon, QVector<double>& x, QVector<double>& y);
    static void processThreeData(const QVector<QString>& time, const QVector<QString>& longitudeData, const QVector<QString>& latitudeData, const QVector<QString>& altitudeData, double epsilon, QVector<double>& t, QVector<float>& x, QVector<float>& y, QVector<float>& z);

    static std::pair<QVector<double>, QVector<double>> intervalSampling(const QVector<double>& timeSeconds, const QVector<double>& value, int targetPoints);
    static std::pair<QVector<double>, QVector<double>> intervalAverageSampling(const QVector<double>& timeSeconds, const QVector<double>& value, int targetPoints);
    static std::pair<QVector<double>, QVector<double>> maxMinSampling(const QVector<double>& timeSeconds, const QVector<double>& value, int interval);
    static std::pair<QVector<double>, QVector<double>> m4Sampling(const QVector<double>& timeSeconds, const QVector<double>& value, int interval);

};

#endif // PROCESSDATA_H
