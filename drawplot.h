#ifndef DRAWPLOT_H
#define DRAWPLOT_H

// Qt headers
#include <QVector>
#include <QString>
#include <QDebug>
#include <QElapsedTimer>

// Third-party libraries
#include "qcustomplot.h"

// Custom headers
#include "processdata.h"
#include "debug.h"
class DrawPlot
{
public:
    //图2 折线图
    static void drawSingleLineChart(QCustomPlot* Plot, const QVector<QString>& xdata,const QVector<QString>& ydata);
    //图3 散点图
    static void drawScatterPlot(QCustomPlot* Plot, const QVector<QString>& xdata, const QVector<QString>& ydata);
    //图3 不同采样方式测试
    static void drawScatterPlotAndSample(QCustomPlot* Plot, const QVector<QString>& xdata, const QVector<QString>& ydata);
    //图6 箱线图
    static void drawBoxPlot(QCustomPlot* Plot, const QVector<QString>& xdata,const QVector<QString>& ydata, const QString &label);

    // 数据库数据
    //图2 折线图
    static void drawSingleLineChartByDB(QCustomPlot* Plot, const QVector<double> &xdata, const QVector<double> &ydata, QString xColumnName, QString yColumnName);
    //图3 散点图
    static void drawScatterPlotByDB(QCustomPlot* Plot, const QVector<double>& xdata,const QVector<double>& ydata,QString xColumnName,QString yColumnName);
    //图6 箱线图
    static void drawBoxPlotByDB(QCustomPlot* Plot, const QVector<double>& ydata, const QString &label);

    // 被选中的几条线绘制在一张图里面
    static double drawSelectedLineChart(QCustomPlot* Plot, const QVector<QVector<QString>>& myData, const QMap<QString, QColor> &columnColors);
    // 绘制M4采样之后的数据
    static double drawSelectedLineChartByM4Sample(QCustomPlot *Plot, const QVector<QVector<QString>> &myData, const QMap<QString, QColor> &columnColors);

    // 滑动平均函数
    static void applySlidingAverageWithStep(const QVector<double>& x, const QVector<double>& y, int windowSize, QVector<double>& smoothedX, QVector<double>& smoothedY);
    // 固定窗口采样函数：每隔 windowSize 个数据点采样一次
    static void applyFixedWindowSampling(const QVector<double>& x, const QVector<double>& y, int windowSize, QVector<double>& fixedX, QVector<double>& fixedY);

    static double epsilonDrawPlot; //折线图数据聚合的时间间隔大小,误差小于0.1说明0.1秒之内的数据，直接取平均
};
#endif // DRAWPLOT_H


