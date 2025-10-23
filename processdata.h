// processdata.h
#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <cmath>
#include "debug.h"
class ProcessData
{
public:
    static QVector<double> preprocessDatatoDB(const QVector<QString>& xdata, const QVector<QString>& ydata, double epsilon);
    static QVector<double> preprocessQStringtoDouble(const QVector<QString>& QStringdata);
    static void processData(const QVector<QString>& xdata, const QVector<QString>& ydata, double epsilon, QVector<double>& x, QVector<double>& y);
    static void processThreeData(const QVector<QString>& time, const QVector<QString>& longitudeData, const QVector<QString>& latitudeData, const QVector<QString>& altitudeData, double epsilon, QVector<double>& t, QVector<float>& x, QVector<float>& y, QVector<float>& z);

    // 数据压缩
    static std::pair<QVector<double>, QVector<double>> intervalSample(const QVector<double>& timeSeconds, const QVector<double>& value, int targetPoints);
    static std::pair<QVector<double>, QVector<double>> intervalAverageSample(const QVector<double>& timeSeconds, const QVector<double>& value, int targetPoints);
    static std::pair<QVector<double>, QVector<double>> maxMinSample(const QVector<double>& timeSeconds, const QVector<double>& value, int interval);
    static std::pair<QVector<double>, QVector<double>> m4Sample(const QVector<double>& timeSeconds, const QVector<double>& value, int interval);

    // 代表数据选择
    static double dtwDistanceFast(const QVector<double>& seq1, const QVector<double>& seq2, int window = 10);
    static QVector<QVector<double>> computeDtwMatrix(const QVector<QVector<double>>& data, int window = 10);
    static QVector<double> computeAverageMatrix(const QVector<QVector<double>>& dtwMatrix);
    static QVector<QVector<double>> greedySelect(const QVector<QVector<double>>& data, const QVector<QVector<double>>& dtwMatrix, const QVector<double>& averageMatrix, int k, double alpha);
    static QVector<int> greedySelectIndices(const QVector<QVector<double>>& data, const QVector<QVector<double>>& dtwMatrix, const QVector<double>& averageMatrix, int k, double alpha);
};

#endif // PROCESSDATA_H
