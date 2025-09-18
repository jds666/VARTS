// processdata.cpp
#include "processdata.h"



QVector<double> ProcessData::preprocessDatatoDB(const QVector<QString>& xdata, const QVector<QString>& ydata, double epsilon)
{
    // 数据处理为时间序列，即时间间隔相等，0.2s每个时间片
    double sum = 0;
    int temp = 0;
    int index = 0;

    QVector<double> x,y;

    x.append(xdata[0].toDouble()); // 添加时间索引的第一个元素
    for (int i = 0; i < ydata.size(); ++i)
    {
        if (fabs(xdata[i].toDouble() - x[index]) > epsilon)
        {
            y.append(sum / (i - temp));
            index++;
            x.append(xdata[i].toDouble());
            temp = i;
            sum = 0;
        }
        sum = sum + ydata[i].toDouble();
    }
    y.append(sum / (ydata.size() - temp));
    return y;
}

QVector<double> ProcessData::preprocessQStringtoDouble(const QVector<QString> &QStringdata)
{
    QVector<double> doubleData; // 初始化一个用于存放double值的QVector

    for (const QString &str : QStringdata) {
        bool conversionOk = false;
        double value = str.toDouble(&conversionOk); // 尝试将QString转换为double

        if (conversionOk) {
            // 如果转换成功，则将值添加到doubleData中
            doubleData.append(value);
        } else {
            // 如果转换失败，可以根据需要处理错误，例如跳过该值或记录错误信息
            qDebug() << "Conversion failed for:" << str;
            // 这里可以选择抛出异常、跳过或者以其他方式处理无效输入
        }
    }

    return doubleData; // 返回转换后的QVector<double>
}

void ProcessData::processData(const QVector<QString>& xdata, const QVector<QString>& ydata, double epsilon, QVector<double>& x, QVector<double>& y)
{
    // 取时间有效的数据，时间为0之后的数据
    QVector<QString> filteredXData;
    QVector<QString> filteredYData;

    int startIndex = -1;
    for (int i = 0; i < xdata.size(); ++i) {
        if (xdata[i] == "0") {
            startIndex = i;
            break;
        }
    }

//    qDebug()<<"startIndex:"<<startIndex;
    if (startIndex != -1) {
        for (int i = startIndex+1; i < xdata.size(); ++i) {
            filteredXData.append(xdata[i]);
            filteredYData.append(ydata[i]);
        }
    }

    // 数据处理为时间序列，即时间间隔相等，0.2s每个时间片
    double sum = 0;
    int temp = 0;
    int index = 0;

    x.clear();
    y.clear();

    x.append(filteredXData[0].toDouble()); // 添加时间索引的第一个元素
    for (int i = 0; i < filteredYData.size(); ++i)
    {
        if (fabs(filteredXData[i].toDouble() - x[index]) > epsilon)
        {
            y.append(sum / (i - temp));
            index++;
            x.append(filteredXData[i].toDouble());
            temp = i;
            sum = 0;
        }
        sum = sum + filteredYData[i].toDouble();
    }
    y.append(sum / (filteredYData.size() - temp));
}

void ProcessData::processThreeData(const QVector<QString>& time, const QVector<QString>& Longitudedata, const QVector<QString>& Latitudedata, const QVector<QString>& Altitudedata, double epsilon, QVector<double>& t, QVector<float>& x, QVector<float>& y, QVector<float>& z)
{
    // 取时间有效的数据，时间为0之后的数据
    QVector<QString> filteredTime;
    QVector<QString> filteredLongitude;
    QVector<QString> filteredLatitude;
    QVector<QString> filteredAltitude;

    int startIndex = -1;
    for (int i = 0; i < time.size(); ++i) {
        if (time[i] == "0") {
            startIndex = i;
            break;
        }
    }
//    qDebug()<<"startIndex:"<<startIndex;

    if (startIndex != -1 && Longitudedata.size()!=0) {
        for (int i = startIndex + 1; i < time.size(); ++i) {
            filteredTime.append(time[i]);
            filteredLongitude.append(Longitudedata[i]);
            filteredLatitude.append(Latitudedata[i]);
            filteredAltitude.append(Altitudedata[i]);
        }
    }

    // 数据处理为时间序列，即时间间隔相等，epsilon 每个时间片
    float sumLongitude = 0.0f;
    float sumLatitude = 0.0f;
    float sumAltitude = 0.0f;
    int temp = 0;
    int index = 0;

    t.clear();
    x.clear();
    y.clear();
    z.clear();

    if(Longitudedata.size()!=0 )
    {
        t.append(filteredTime[0].toDouble()); // 添加时间索引的第一个元素
        for (int i = 0; i < filteredLongitude.size(); ++i) {
            if (fabs(filteredTime[i].toDouble() - t[index]) > epsilon) {
                x.append(sumLongitude / (i - temp));
                y.append(sumLatitude / (i - temp));
                z.append(sumAltitude / (i - temp));
                index++;
                t.append(filteredTime[i].toDouble());
                temp = i;
                sumLongitude = 0.0f;
                sumLatitude = 0.0f;
                sumAltitude = 0.0f;
            }
            sumLongitude += filteredLongitude[i].toFloat();
            sumLatitude += filteredLatitude[i].toFloat();
            sumAltitude += filteredAltitude[i].toFloat();
        }
        x.append(sumLongitude / (filteredLongitude.size() - temp));
        y.append(sumLatitude / (filteredLongitude.size() - temp));
        z.append(sumAltitude / (filteredLongitude.size() - temp));
    }
    qDebug()<<"3维数据处理之后的长度:"<<x.size();
}

//间隔采样，如果间隔宽度是10000的倍数，采样结果会是周期的折线。
std::pair<QVector<double>, QVector<double>> ProcessData::intervalSampling(const QVector<double>& timeSeconds, const QVector<double>& value, int targetPoints)
{
    std::pair<QVector<double>, QVector<double>> sampledData;
    int originalSize = timeSeconds.size();

    // 如果原始数据点小于或等于目标点数，则无需采样
    if (originalSize <= targetPoints) {
        return std::make_pair(timeSeconds, value);
    }

    // 计算采样间隔
    int samplingInterval = originalSize / targetPoints;

    for (int i = 0; i < originalSize; i += samplingInterval) {
        sampledData.first.append(timeSeconds[i]); // 采样后的时间
        sampledData.second.append(value[i]); // 对应的值
    }

    // 确保最终的点数不会超过目标点数
    while (sampledData.first.size() > targetPoints) {
        sampledData.first.removeLast();
        sampledData.second.removeLast();
    }

    return sampledData;
}

// 均匀采样
std::pair<QVector<double>, QVector<double>> ProcessData::intervalAverageSampling(const QVector<double>& timeSeconds, const QVector<double>& value, int targetPoints)
{
    std::pair<QVector<double>, QVector<double>> sampledData;
    int originalSize = timeSeconds.size();

    // 如果原始数据点小于或等于目标点数，则无需采样
    if (originalSize <= targetPoints) {
        return std::make_pair(timeSeconds, value);
    }

    // 计算每个样本区间应包含的数据点数
    int samplesPerInterval = originalSize / targetPoints;
    if (originalSize % targetPoints != 0) ++samplesPerInterval; // 处理余数情况

    for (int i = 0; i < originalSize; i += samplesPerInterval) {
        double sumValue = 0.0;
        int count = 0;
        double startTime = timeSeconds[i];
        double endTime = timeSeconds[qMin(i + samplesPerInterval - 1, originalSize - 1)];

        // 计算当前区间的平均值
        for (int j = i; j < qMin(i + samplesPerInterval, originalSize); ++j) {
            sumValue += value[j];
            ++count;
        }
        double avgValue = sumValue / count;

        // 使用区间的中点时间作为代表时间
        double midTime = startTime + (endTime - startTime) / 2;
        sampledData.first.append(midTime); // 区间中间的时间点
        sampledData.second.append(avgValue); // 当前区间的平均值
    }

    // 确保最终的点数不会超过目标点数
    while (sampledData.first.size() > targetPoints) {
        sampledData.first.removeLast();
        sampledData.second.removeLast();
    }

    return sampledData;
}

//min-max 聚合
std::pair<QVector<double>, QVector<double>> ProcessData::maxMinSampling(const QVector<double>& timeSeconds, const QVector<double>& value, int interval)
{
    std::pair<QVector<double>, QVector<double>> sampledData;
    int originalSize = timeSeconds.size();

    // 确保至少有一个间隔
    if (interval <= 0) {
        interval = 1;
    }

    // 计算每个样本区间应包含的数据点数
    int samplesPerInterval = originalSize / interval;
    if (originalSize % interval != 0) ++samplesPerInterval; // 处理余数情况

    for (int i = 0; i < originalSize; i += samplesPerInterval) {
        double minValue = std::numeric_limits<double>::max();
        double maxValue = std::numeric_limits<double>::lowest();
        int minIndex = -1, maxIndex = -1;

        // 找到当前区间的最大值和最小值
        for (int j = i; j < qMin(i + samplesPerInterval, originalSize); ++j) {
            if (value[j] <= minValue) {
                minValue = value[j];
                minIndex = j;
            }
            if (value[j] >= maxValue) {
                maxValue = value[j];
                maxIndex = j;
            }
        }

        // 使用区间的边界时间作为代表时间
        double minTime = timeSeconds[minIndex];
        double maxTime = timeSeconds[maxIndex];

        // 添加最小值和最大值及其对应的时间点到结果集中
        sampledData.first.append(minTime);
        sampledData.second.append(minValue);
        sampledData.first.append(maxTime);
        sampledData.second.append(maxValue);
    }

    return sampledData;
}
// M4 聚合
std::pair<QVector<double>, QVector<double>> ProcessData::m4Sampling(const QVector<double>& timeSeconds, const QVector<double>& value, int interval)
{
    std::pair<QVector<double>, QVector<double>> sampledData;
    int originalSize = timeSeconds.size();

    // 确保至少有一个间隔
    if (interval <= 0) {
        interval = 1;
    }

    // 计算每个样本区间应包含的数据点数
    int samplesPerInterval = originalSize / interval;
    if (originalSize % interval != 0) ++samplesPerInterval; // 处理余数情况

    for (int i = 0; i < originalSize; i += samplesPerInterval) {
        double minValue = std::numeric_limits<double>::max();
        double maxValue = std::numeric_limits<double>::lowest();
        double firstValue = value[i]; // 最早时间戳对应的值
        double lastValue = value[qMin(i + samplesPerInterval - 1, originalSize - 1)]; // 最晚时间戳对应的值
        int minIndex = -1, maxIndex = -1;

        // 找到当前区间的最大值和最小值
        for (int j = i; j < qMin(i + samplesPerInterval, originalSize); ++j) {
            if (value[j] <= minValue) {
                minValue = value[j];
                minIndex = j;
            }
            if (value[j] >= maxValue) {
                maxValue = value[j];
                maxIndex = j;
            }
        }

        // 使用一个临时集合来存储唯一的时间-值对
        QSet<std::pair<double, double>> uniquePoints;

        // 添加最小值
        uniquePoints.insert({timeSeconds[minIndex], minValue});
        // 添加最大值
        uniquePoints.insert({timeSeconds[maxIndex], maxValue});
        // 添加最早时间戳对应的值
        uniquePoints.insert({timeSeconds[i], firstValue});
        // 添加最晚时间戳对应的值
        uniquePoints.insert({timeSeconds[qMin(i + samplesPerInterval - 1, originalSize - 1)], lastValue});

        // 将唯一的时间-值对添加到结果集中
        for (const auto& point : uniquePoints) {
            sampledData.first.append(point.first);
            sampledData.second.append(point.second);
        }
    }

    return sampledData;
}
