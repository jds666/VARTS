// drawplot.cpp
#include "drawplot.h"

double DrawPlot::epsilonDrawPlot = 0.0;

/**
 * @brief DrawPlot::drawLineChart
 * @param xdata: columnData的时间列
 * @param ydata: columnData的展示列
 */
void DrawPlot::drawSingleLineChart(QCustomPlot* Plot, const QVector<QString>& xdata,const QVector<QString>& ydata)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(Plot);
    if (customPlot)
    {
        QVector<double> x, y;

        //数据处理
        ProcessData::processData(xdata,ydata,DrawPlot::epsilonDrawPlot,x,y);

        // 创建折线图并设置数据:
        customPlot->addGraph();
        customPlot->graph(0)->setData(x, y);

        // 设置坐标轴标签:
        customPlot->xAxis->setLabel("Time");
        customPlot->yAxis->setLabel(ydata[0]);

        // 设置坐标轴范围:
        //显示x坐标
        customPlot->xAxis->setTickLabels(true); // 设置刻度标签可见
        customPlot->xAxis->setRange(*std::min_element(x.constBegin(), x.constEnd()), *std::max_element(x.constBegin(), x.constEnd())); // 使用 x 向量中的最小和最大值作为范围
        customPlot->yAxis->setRange(*std::min_element(y.constBegin(), y.constEnd()), *std::max_element(y.constBegin(), y.constEnd())); // 使用 y 向量中的最小和最大值作为范围

        // 重新绘制折线图:
        // 设置一些基本的customPlot配置：
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        customPlot->axisRect()->setupFullAxesBox();
        customPlot->rescaleAxes();
        customPlot->replot();
//        customPlot->savePdf("D://研究生//科研//飞行数据项目//数据可视化//图片//customPlot.pdf", 1000, 400);
    }
}

/**
 * @brief MainWindow::drawScatterPlot
 * 绘制散点图
 * @param data
 */
void DrawPlot::drawScatterPlot(QCustomPlot* Plot, const QVector<QString>& xdata, const QVector<QString>& ydata)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(Plot);
    if (customPlot)
    {

        QVector<double> x, y;

        // 数据处理
         DEBUG_LOG(xdata[0] << "未处理数据长度:" << xdata.size() << ydata[0] << "未处理数据长度:" << ydata.size()) ;
        ProcessData::processData(xdata, ydata, DrawPlot::epsilonDrawPlot, x, y);
         DEBUG_LOG(xdata[0] << "处理后长度:" << x.size() << ydata[0] << "处理后长度:" << y.size()) ;

        // **创建原始散点图并设置数据**
        customPlot->addGraph();
        customPlot->graph(0)->setData(x, y);

        // 设置散点图样式
        customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

        // 设置坐标轴标签以及字体大小
        customPlot->xAxis->setLabel("Time");
        customPlot->yAxis->setLabel(ydata[0]);

        // 设置坐标轴范围...
        customPlot->xAxis->setRange(*std::min_element(x.constBegin(), x.constEnd()),
                                    *std::max_element(x.constBegin(), x.constEnd()));
        customPlot->yAxis->setRange(*std::min_element(y.constBegin(), y.constEnd()),
                                    *std::max_element(y.constBegin(), y.constEnd()));

        // 重新绘制
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        customPlot->axisRect()->setupFullAxesBox();
        customPlot->rescaleAxes();
        customPlot->replot();
    }
}
/**
 * @brief MainWindow::drawScatterPlot
 * 绘制散点图
 * @param data
 */
void DrawPlot::drawScatterPlotAndSample(QCustomPlot* Plot, const QVector<QString>& xdata, const QVector<QString>& ydata)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(Plot);
    if (customPlot)
    {

        QVector<double> x, y;

        // 数据处理
         DEBUG_LOG(xdata[0] << "未处理数据长度:" << xdata.size() << ydata[0] << "未处理数据长度:" << ydata.size());
        ProcessData::processData(xdata, ydata, DrawPlot::epsilonDrawPlot, x, y);
         DEBUG_LOG(xdata[0] << "长度:" << x.size() << ydata[0] << "长度:" << y.size());

        // **创建原始散点图并设置数据**
        customPlot->addGraph();
        customPlot->graph(0)->setData(x, y);

        // 设置散点图样式
        customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

        // 设置坐标轴标签以及字体大小
        customPlot->xAxis->setLabel("Time");
        customPlot->yAxis->setLabel(ydata[0]);

        // 设置坐标轴范围...
        customPlot->xAxis->setRange(*std::min_element(x.constBegin(), x.constEnd()),
                                    *std::max_element(x.constBegin(), x.constEnd()));
        customPlot->yAxis->setRange(*std::min_element(y.constBegin(), y.constEnd()),
                                    *std::max_element(y.constBegin(), y.constEnd()));

        int windowSize = x.size()/1000;  // 窗口大小为数据长度/屏幕分辨率

        // **添加固定窗口采样的散点图**
        QVector<double> fixedX, fixedY;
        DrawPlot::applyFixedWindowSampling(x, y, windowSize, fixedX, fixedY); // 执行滑动平均

        // 添加第2条散点图
        customPlot->addGraph();
        customPlot->graph(1)->setData(fixedX, fixedY);

        // 设置滑动平均散点图样式（比如，使用不同的颜色和大小）
        customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, 3));
        customPlot->graph(1)->setPen(QPen(Qt::green)); // 设置为黄色


        // **添加滑动平均后的散点图**
        QVector<double> smoothedX, smoothedY;
        DrawPlot::applySlidingAverageWithStep(x, y, windowSize, smoothedX, smoothedY); // 执行滑动平均

        // 添加第3条散点图
        customPlot->addGraph();
        customPlot->graph(2)->setData(smoothedX, smoothedY);

        // 设置滑动平均散点图样式（比如，使用不同的颜色和大小）
        customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 3));
        customPlot->graph(2)->setPen(QPen(Qt::red)); // 设置为红色

        // 重新绘制
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        customPlot->axisRect()->setupFullAxesBox();
        customPlot->rescaleAxes();
        customPlot->replot();
    }
}



void DrawPlot::drawBoxPlot(QCustomPlot* Plot, const QVector<QString>& xdata,const QVector<QString>& ydata, const QString &label)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(Plot);

    if (customPlot)
    {
        customPlot->clearPlottables();
        customPlot->clearItems();

        QVector<double> x, y;
        //数据处理
        ProcessData::processData(xdata,ydata,DrawPlot::epsilonDrawPlot,x,y);

        QCPStatisticalBox *statistical = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
        QBrush boxBrush(QColor(60, 60, 255, 100));
        boxBrush.setStyle(Qt::Dense6Pattern); // make it look oldschool
        statistical->setBrush(boxBrush);
        // 计算统计值
        double minimum = *std::min_element(y.begin(), y.end());
        double maximum = *std::max_element(y.begin(), y.end());
        std::sort(y.begin(), y.end());
        double median = y[y.size() / 2];
        double lowerQuartile = y[y.size() / 4];
        double upperQuartile = y[3 * y.size() / 4];

        // 添加数据
        statistical->addData(1, minimum, lowerQuartile, median, upperQuartile, maximum);

        // 添加标签
        QCPItemText *minimumLabel = new QCPItemText(customPlot);
        minimumLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        minimumLabel->position->setType(QCPItemPosition::ptPlotCoords);
        minimumLabel->position->setCoords(1, minimum);
        minimumLabel->setText(QString("Minimum: %1").arg(minimum));

        QCPItemText *lowerQuartileLabel = new QCPItemText(customPlot);
        lowerQuartileLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        lowerQuartileLabel->position->setType(QCPItemPosition::ptPlotCoords);
        lowerQuartileLabel->position->setCoords(1, lowerQuartile);
        lowerQuartileLabel->setText(QString("Lower quartile: %1").arg(lowerQuartile));

        QCPItemText *medianLabel = new QCPItemText(customPlot);
        medianLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        medianLabel->position->setType(QCPItemPosition::ptPlotCoords);
        medianLabel->position->setCoords(1, median);
        medianLabel->setText(QString("Median: %1").arg(median));

        QCPItemText *upperQuartileLabel = new QCPItemText(customPlot);
        upperQuartileLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        upperQuartileLabel->position->setType(QCPItemPosition::ptPlotCoords);
        upperQuartileLabel->position->setCoords(1, upperQuartile);
        upperQuartileLabel->setText(QString("Upper Quartile: %1").arg(upperQuartile));

        QCPItemText *maximumLabel = new QCPItemText(customPlot);
        maximumLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        maximumLabel->position->setType(QCPItemPosition::ptPlotCoords);
        maximumLabel->position->setCoords(1, maximum);
        maximumLabel->setText(QString("Maximum: %1").arg(maximum));

        // prepare manual x axis labels:
        customPlot->xAxis->setSubTicks(false);
        customPlot->xAxis->setTickLength(0, 4);
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTick(1, label);
        customPlot->xAxis->setTicker(textTicker);

        // 准备坐标轴
        customPlot->yAxis->setLabel(QString::fromUtf8("Distribution of Data"));
        customPlot->rescaleAxes();
        customPlot->xAxis->scaleRange(1.7, customPlot->xAxis->range().center());
        customPlot->yAxis->setRange(minimum - 1, maximum + 1);
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        customPlot->replot();
    }
}




void DrawPlot::drawSingleLineChartByDB(QCustomPlot* Plot, const QVector<double> &xdata, const QVector<double> &ydata, QString xColumnName, QString yColumnName)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(Plot);
    if (customPlot)
    {

         DEBUG_LOG(xColumnName);
        // 创建折线图并设置数据:
        customPlot->addGraph();
        customPlot->graph(0)->setData(xdata, ydata);

        // 设置坐标轴标签:
        customPlot->xAxis->setLabel("Time");
        customPlot->yAxis->setLabel(yColumnName);

        // 设置坐标轴范围:
        //显示x坐标
        customPlot->xAxis->setTickLabels(true); // 设置刻度标签可见
        customPlot->xAxis->setRange(*std::min_element(xdata.constBegin(), xdata.constEnd()), *std::max_element(xdata.constBegin(), xdata.constEnd())); // 使用 x 向量中的最小和最大值作为范围
        customPlot->yAxis->setRange(*std::min_element(ydata.constBegin(), ydata.constEnd()), *std::max_element(ydata.constBegin(), ydata.constEnd())); // 使用 y 向量中的最小和最大值作为范围

        // 重新绘制折线图:
        // 设置一些基本的customPlot配置：
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        customPlot->axisRect()->setupFullAxesBox();
        customPlot->rescaleAxes();
        customPlot->replot();
//        customPlot->savePdf("D://研究生//科研//飞行数据项目//数据可视化//图片//customPlot.pdf", 1000, 400);
    }
}


/**
 * @brief MainWindow::drawScatterPlot
 * 绘制散点图
 * @param data
 */
void DrawPlot::drawScatterPlotByDB(QCustomPlot* Plot, const QVector<double>& xdata,const QVector<double>& ydata,QString xColumnName,QString yColumnName)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(Plot);
    if (customPlot)
    {
         DEBUG_LOG(xColumnName);

        // 创建散点图并设置数据:
        customPlot->addGraph();
        customPlot->graph(0)->setData(xdata, ydata);

        // 设置散点图样式
        customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

        // 设置坐标轴标签以及字体大小
        customPlot->xAxis->setLabel("Time");
        customPlot->yAxis->setLabel(yColumnName);

        // 设置坐标轴范围...
        customPlot->xAxis->setRange(*std::min_element(xdata.constBegin(), xdata.constEnd()),
                                    *std::max_element(xdata.constBegin(), xdata.constEnd()));
        customPlot->yAxis->setRange(*std::min_element(ydata.constBegin(), ydata.constEnd()),
                                    *std::max_element(ydata.constBegin(), ydata.constEnd()));


        // 重新绘制折线图:
        // 设置一些基本的customPlot配置：
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        customPlot->axisRect()->setupFullAxesBox();
        customPlot->rescaleAxes();
        customPlot->replot();
    }
}



void DrawPlot::drawBoxPlotByDB(QCustomPlot* Plot,  const QVector<double> &ydata, const QString &label)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(Plot);

    if (customPlot)
    {
        customPlot->clearPlottables();
        customPlot->clearItems();

        QCPStatisticalBox *statistical = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
        QBrush boxBrush(QColor(60, 60, 255, 100));
        boxBrush.setStyle(Qt::Dense6Pattern); // make it look oldschool
        statistical->setBrush(boxBrush);
        // 计算统计值
        QVector<double> newydata = ydata;
        double minimum = *std::min_element(newydata.begin(), newydata.end());
        double maximum = *std::max_element(newydata.begin(), newydata.end());
        std::sort(newydata.begin(), newydata.end());
        double median = newydata[newydata.size() / 2];
        double lowerQuartile = newydata[newydata.size() / 4];
        double upperQuartile = newydata[3 * newydata.size() / 4];


        // 添加数据
        statistical->addData(1, minimum, lowerQuartile, median, upperQuartile, maximum);

        // 添加标签
        QCPItemText *minimumLabel = new QCPItemText(customPlot);
        minimumLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        minimumLabel->position->setType(QCPItemPosition::ptPlotCoords);
        minimumLabel->position->setCoords(1, minimum);
        minimumLabel->setText(QString("Minimum: %1").arg(minimum));

        QCPItemText *lowerQuartileLabel = new QCPItemText(customPlot);
        lowerQuartileLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        lowerQuartileLabel->position->setType(QCPItemPosition::ptPlotCoords);
        lowerQuartileLabel->position->setCoords(1, lowerQuartile);
        lowerQuartileLabel->setText(QString("Lower quartile: %1").arg(lowerQuartile));

        QCPItemText *medianLabel = new QCPItemText(customPlot);
        medianLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        medianLabel->position->setType(QCPItemPosition::ptPlotCoords);
        medianLabel->position->setCoords(1, median);
        medianLabel->setText(QString("Median: %1").arg(median));

        QCPItemText *upperQuartileLabel = new QCPItemText(customPlot);
        upperQuartileLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        upperQuartileLabel->position->setType(QCPItemPosition::ptPlotCoords);
        upperQuartileLabel->position->setCoords(1, upperQuartile);
        upperQuartileLabel->setText(QString("Upper Quartile: %1").arg(upperQuartile));

        QCPItemText *maximumLabel = new QCPItemText(customPlot);
        maximumLabel->setPositionAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        maximumLabel->position->setType(QCPItemPosition::ptPlotCoords);
        maximumLabel->position->setCoords(1, maximum);
        maximumLabel->setText(QString("Maximum: %1").arg(maximum));

        // prepare manual x axis labels:
        customPlot->xAxis->setSubTicks(false);
        customPlot->xAxis->setTickLength(0, 4);
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTick(1, label);
        customPlot->xAxis->setTicker(textTicker);

        // 准备坐标轴
        customPlot->yAxis->setLabel(QString::fromUtf8("Distribution of Data"));
        customPlot->rescaleAxes();
        customPlot->xAxis->scaleRange(1.7, customPlot->xAxis->range().center());
        customPlot->yAxis->setRange(minimum - 1, maximum + 1);
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        customPlot->replot();
    }
}

double DrawPlot::drawSelectedLineChart(QCustomPlot *Plot, const QVector<QVector<QString>> &myData, const QMap<QString, QColor> &columnColors)
{
    QElapsedTimer timer;
    timer.start(); // 开始计时

    QCustomPlot* plot = qobject_cast<QCustomPlot*>(Plot);
    if (!plot || myData.isEmpty()) return timer.elapsed() / 1000.0; // 如果数据为空或转换失败，返回已过时间（秒）

    plot->clearGraphs();

    for (int i = 0; i < myData.size(); ++i) {
        // 跳过空列
        if (myData[i].isEmpty()) continue;

        // 提取列名（第一个元素）
        QString columnName = myData[i][0];

        // 处理数据：跳过列名，将字符串转为double
        QVector<double> yData;
        for (int j = 1; j < myData[i].size(); ++j) {
            bool ok;
            double value = myData[i][j].toDouble(&ok);
            if (ok) {
                yData.append(value);
            } else {
                // 处理转换失败的情况
                qWarning() << "数据转换失败: " << myData[i][j];
                yData.append(0); // 或者使用NaN表示无效数据
            }
        }

        // 如果没有有效数据，跳过该列
        if (yData.isEmpty()) continue;

        // 创建X轴数据（使用索引作为X值）
        QVector<double> xData(yData.size());
        for (int j = 0; j < xData.size(); ++j) {
            xData[j] = j; // X值为数据点索引（从0开始）
        }

        // 添加图形并设置数据
        int graphIndex = plot->graphCount();
        plot->addGraph();
        plot->graph(graphIndex)->setData(xData, yData);

        // 设置线条颜色和样式：优先使用map中的颜色，没有则使用默认
        QColor color;
        if (columnColors.contains(columnName)) {
            color = columnColors[columnName];
        } else {
            // 如果map中没有该列的颜色，使用默认的HSV颜色
            color.setHsv((i * 40) % 360, 200, 255);
        }
        plot->graph(graphIndex)->setPen(QPen(color, 2));

        // 设置图例名称（使用列名）
        plot->graph(graphIndex)->setName(columnName);
    }

//    // 设置图例可见
//    plot->legend->setVisible(true);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->axisRect()->setupFullAxesBox();

    // 调整坐标轴范围以适应数据
    plot->rescaleAxes();
//  plot->setFixedWidth(1000 + plot->axisRect()->margins().left() + plot->axisRect()->margins().right());

    // 重绘图表
    plot->replot();

    //  返回经过的时间（秒）
    return timer.elapsed() / 1000.0;
}

double DrawPlot::drawSelectedLineChartByM4Sample(QCustomPlot *Plot, const QVector<QVector<QString>> &myData, const QMap<QString, QColor> &columnColors)
{
    QElapsedTimer timer;
    timer.start(); // 开始计时

    QCustomPlot* plot = qobject_cast<QCustomPlot*>(Plot);
    if (!plot || myData.isEmpty()) return timer.elapsed() / 1000.0; // 如果数据为空或转换失败，返回已过时间（秒）

    plot->clearGraphs();

    const int targetPoints = 1000; // 直接使用1000个目标点

    for (int i = 0; i < myData.size(); ++i) {
        if (myData[i].isEmpty()) continue;

        QString columnName = myData[i][0];
        QVector<double> yData;

        for (int j = 1; j < myData[i].size(); ++j) {
            bool ok;
            double value = myData[i][j].toDouble(&ok);
            yData.append(ok ? value : 0);
        }

        if (yData.isEmpty()) continue;

        QVector<double> xData(yData.size());
        for (int j = 0; j < xData.size(); ++j) {
            xData[j] = j;
        }

        // 如果数据量不大，直接绘制
        if (yData.size() <= targetPoints * 2) {
            int graphIndex = plot->graphCount();
            plot->addGraph();
            plot->graph(graphIndex)->setData(xData, yData);
        } else {
            // 大数据量使用M4采样
            auto sampledData = ProcessData::m4Sample(xData, yData, targetPoints);
            int graphIndex = plot->graphCount();
            plot->addGraph();
            plot->graph(graphIndex)->setData(sampledData.first, sampledData.second);
        }

        // 设置样式
        QColor color = columnColors.contains(columnName) ? columnColors[columnName] : QColor::fromHsv((i * 40) % 360, 200, 255);
        plot->graph(plot->graphCount()-1)->setPen(QPen(color, 1.5));
        plot->graph(plot->graphCount()-1)->setName(columnName);
        plot->graph(plot->graphCount()-1)->setAntialiased(true);
    }

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->axisRect()->setupFullAxesBox();
    plot->rescaleAxes();
//    plot->setFixedWidth(1000 + plot->axisRect()->margins().left() + plot->axisRect()->margins().right());
    plot->setAntialiasedElements(QCP::aeAll);
    plot->replot();

    //  返回经过的时间（秒）
    return timer.elapsed() / 1000.0;
}

// 滑动平均函数：步长等于窗口大小
void DrawPlot::applySlidingAverageWithStep(const QVector<double>& x, const QVector<double>& y, int windowSize, QVector<double>& smoothedX, QVector<double>& smoothedY)
{
    int n = x.size();
    smoothedX.clear();
    smoothedY.clear();

    // 从第一个完整窗口开始滑动，步长为窗口大小
    for (int i = 0; i <= n - windowSize; i += windowSize) // 步长等于窗口大小
    {
        double sumY = 0;
        // 计算窗口内的y值平均
        for (int j = i; j < i + windowSize; ++j)
        {
            sumY += y[j];
        }

        // 将当前窗口的平均值添加到 smoothedY 中，x 值取当前窗口的最后一个或第一个值
        smoothedX.append(x[i + windowSize / 2]); // 取窗口右端的 x 值
        smoothedY.append(sumY / windowSize);     // 计算窗口内的平均值
    }
}
// 固定窗口采样函数：每隔 windowSize 个数据点采样一次
void DrawPlot::applyFixedWindowSampling(const QVector<double>& x, const QVector<double>& y, int windowSize, QVector<double>& fixedX, QVector<double>& fixedY)
{
    int n = x.size();
    fixedX.clear();
    fixedY.clear();

    // 每 windowSize 个数据点采样一次
    for (int i = 0; i <= n - windowSize; i += windowSize)
    {
        fixedX.append(x[i]);  // 取窗口中间的 x 值
        fixedY.append(y[i]);  // 计算窗口内的平均值
    }
}
