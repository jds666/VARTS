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
             DEBUG_LOG( "Conversion failed for:" << str);
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
        if (xdata[i] == "0"||(i>1&&(xdata[i].toDouble() < xdata[i-1].toDouble()))) {
            startIndex = i;
            break;
        }
    }

     DEBUG_LOG("startIndex:"<<startIndex);
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
        if (time[i] == "0"||(i>1&&(time[i].toDouble() < time[i-1].toDouble()))) {
            startIndex = i;
            break;
        }
    }
//     DEBUG_LOG("startIndex:"<<startIndex);

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
     DEBUG_LOG("3维数据处理之后的长度:"<<x.size());
}

//间隔采样，如果间隔宽度是10000的倍数，采样结果会是周期的折线。
std::pair<QVector<double>, QVector<double>> ProcessData::intervalSample(const QVector<double>& timeSeconds, const QVector<double>& value, int targetPoints)
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
std::pair<QVector<double>, QVector<double>> ProcessData::intervalAverageSample(const QVector<double>& timeSeconds, const QVector<double>& value, int targetPoints)
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
std::pair<QVector<double>, QVector<double>> ProcessData::maxMinSample(const QVector<double>& timeSeconds, const QVector<double>& value, int interval)
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
//// M4 聚合
//std::pair<QVector<double>, QVector<double>> ProcessData::m4Sample(const QVector<double>& timeSeconds, const QVector<double>& value, int interval)
//{
//    std::pair<QVector<double>, QVector<double>> sampledData;
//    int originalSize = timeSeconds.size();

//    // 确保至少有一个间隔
//    if (interval <= 0) {
//        interval = 1;
//    }

//    // 计算每个样本区间应包含的数据点数
//    int samplesPerInterval = originalSize / interval;
//    if (originalSize % interval != 0) ++samplesPerInterval; // 处理余数情况

//    for (int i = 0; i < originalSize; i += samplesPerInterval) {
//        double minValue = std::numeric_limits<double>::max();
//        double maxValue = std::numeric_limits<double>::lowest();
//        double firstValue = value[i]; // 最早时间戳对应的值
//        double lastValue = value[qMin(i + samplesPerInterval - 1, originalSize - 1)]; // 最晚时间戳对应的值
//        int minIndex = -1, maxIndex = -1;

//        // 找到当前区间的最大值和最小值
//        for (int j = i; j < qMin(i + samplesPerInterval, originalSize); ++j) {
//            if (value[j] <= minValue) {
//                minValue = value[j];
//                minIndex = j;
//            }
//            if (value[j] >= maxValue) {
//                maxValue = value[j];
//                maxIndex = j;
//            }
//        }

//        // 使用一个临时集合来存储唯一的时间-值对
//        QSet<std::pair<double, double>> uniquePoints;

//        // 添加最小值
//        uniquePoints.insert({timeSeconds[minIndex], minValue});
//        // 添加最大值
//        uniquePoints.insert({timeSeconds[maxIndex], maxValue});
//        // 添加最早时间戳对应的值
//        uniquePoints.insert({timeSeconds[i], firstValue});
//        // 添加最晚时间戳对应的值
//        uniquePoints.insert({timeSeconds[qMin(i + samplesPerInterval - 1, originalSize - 1)], lastValue});

//        // 将唯一的时间-值对添加到结果集中
//        for (const auto& point : uniquePoints) {
//            sampledData.first.append(point.first);
//            sampledData.second.append(point.second);
//        }
//    }

//    return sampledData;
//}
// 修正M4采样实现
std::pair<QVector<double>, QVector<double>> ProcessData::m4Sample(const QVector<double>& timeSeconds, const QVector<double>& value, int targetPoints)
{
    std::pair<QVector<double>, QVector<double>> sampledData;
    int originalSize = timeSeconds.size();

    if (originalSize <= targetPoints || targetPoints <= 0) {
        // 数据量小于目标点数，直接返回原数据
        sampledData.first = timeSeconds;
        sampledData.second = value;
        return sampledData;
    }

    // 计算每个区间应该包含的数据点数
    int pointsPerSegment = originalSize / targetPoints;

    for (int i = 0; i < targetPoints; ++i) {
        int startIdx = i * pointsPerSegment;
        int endIdx = (i == targetPoints - 1) ? originalSize - 1 : (i + 1) * pointsPerSegment - 1;

        if (startIdx >= originalSize) break;

        // 在当前区间内找到最小值、最大值、第一个值和最后一个值
        double minVal = value[startIdx];
        double maxVal = value[startIdx];
        double firstVal = value[startIdx];
        double lastVal = value[endIdx];
        int minIdx = startIdx, maxIdx = startIdx;

        for (int j = startIdx + 1; j <= endIdx; ++j) {
            if (value[j] < minVal) {
                minVal = value[j];
                minIdx = j;
            }
            if (value[j] > maxVal) {
                maxVal = value[j];
                maxIdx = j;
            }
        }

        // 按时间顺序添加特征点，确保连续性
        QVector<std::pair<double, double>> segmentPoints;

        // 添加第一个点
        segmentPoints.append({timeSeconds[startIdx], firstVal});

        // 如果最小值不是第一个点，添加最小值
        if (minIdx != startIdx && minIdx != endIdx) {
            segmentPoints.append({timeSeconds[minIdx], minVal});
        }

        // 如果最大值不是第一个点且不是最小值点，添加最大值
        if (maxIdx != startIdx && maxIdx != endIdx && maxIdx != minIdx) {
            segmentPoints.append({timeSeconds[maxIdx], maxVal});
        }

        // 添加最后一个点（如果不是重复点）
        if (endIdx != startIdx) {
            segmentPoints.append({timeSeconds[endIdx], lastVal});
        }

        // 按时间排序并添加到结果
        std::sort(segmentPoints.begin(), segmentPoints.end(),
                 [](const std::pair<double, double>& a, const std::pair<double, double>& b) {
                     return a.first < b.first;
                 });

        for (const auto& point : segmentPoints) {
            sampledData.first.append(point.first);
            sampledData.second.append(point.second);
        }
    }

    return sampledData;
}

// DTW 距离计算
double ProcessData::dtwDistanceFast(const QVector<double>& seq1,
                                    const QVector<double>& seq2,
                                    int window)
{
    int n = seq1.size();
    int m = seq2.size();

    if (n == 0 || m == 0) {
        throw std::invalid_argument("Input sequences must not be empty.");
    }

    // 确保窗口大小至少覆盖 |n-m|
    int w = std::max(window, std::abs(n - m));

    QVector<QVector<double>> dp(n + 1, QVector<double>(m + 1, std::numeric_limits<double>::infinity()));
    dp[0][0] = 0.0;

    for (int i = 1; i <= n; ++i) {
        int jStart = std::max(1, i - w);
        int jEnd = std::min(m, i + w);
        for (int j = jStart; j <= jEnd; ++j) {
            double cost = std::abs(seq1[i - 1] - seq2[j - 1]);
            dp[i][j] = cost + std::min({dp[i - 1][j],     // 上
                                        dp[i][j - 1],     // 左
                                        dp[i - 1][j - 1]  // 左上
                                       });
        }
    }

    return dp[n][m];
}

// 计算全局 DTW 矩阵
QVector<QVector<double>> ProcessData::computeDtwMatrix(const QVector<QVector<double>>& data,
                                                       int window)
{
    int n = data.size();
    QVector<QVector<double>> distanceMatrix(n, QVector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dist = dtwDistanceFast(data[i], data[j], window);
            distanceMatrix[i][j] = dist;
            distanceMatrix[j][i] = dist; // 对称
        }
    }

    return distanceMatrix;
}

// 计算平均距离数组
QVector<double> ProcessData::computeAverageMatrix(const QVector<QVector<double>>& dtwMatrix)
{
    int n = dtwMatrix.size();
    QVector<double> avgDistances(n, 0.0);

    if (n == 0) return avgDistances;

    for (int i = 0; i < n; ++i) {
        double total = 0.0;
        for (int j = 0; j < n; ++j) {
            total += dtwMatrix[i][j];
        }
        // 平均距离（不包括自己）
        avgDistances[i] = (n > 1) ? total / (n - 1) : 0.0;
    }

    return avgDistances;
}

QVector<QVector<double>> ProcessData::greedySelect(const QVector<QVector<double>>& data, const QVector<QVector<double>>& dtwMatrix, const QVector<double>& averageMatrix, int k, double alpha)
{
    int n = data.size();
    QVector<QVector<double>> result;   // 最终返回的代表性时序数据

    // ==== 安全检查 ====
    if (n == 0 || k == 0) {
        return result;
    }
    if (dtwMatrix.size() != n || averageMatrix.size() != n) {
        throw std::invalid_argument("dtwMatrix 或 averageMatrix 尺寸与 data 不匹配");
    }
    for (int i = 0; i < n; i++) {
        if (dtwMatrix[i].size() != n) {
            throw std::invalid_argument("dtwMatrix 必须是 n×n 矩阵");
        }
    }

    // ==== Step 1: 选择第一个代表 ====
    int firstRep = 0;
    double minAvg = std::numeric_limits<double>::max();
    for (int i = 0; i < n; i++) {
        if (averageMatrix[i] < minAvg) {
            minAvg = averageMatrix[i];
            firstRep = i;
        }
    }
    QVector<int> representatives;
    representatives.append(firstRep);

    // 初始化候选集
    QVector<int> remainingIndices;
    for (int i = 0; i < n; i++) {
        if (i != firstRep) remainingIndices.append(i);
    }

    QVector<double> currentMinDistances(n);
    for (int i = 0; i < n; i++) {
        currentMinDistances[i] = dtwMatrix[i][firstRep];
    }

    double minRepDistances = -std::numeric_limits<double>::max();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dtwMatrix[i][j] > minRepDistances) {
                minRepDistances = dtwMatrix[i][j];
            }
        }
    }

    // ==== Step 2: 迭代选择剩余代表 ====
    for (int iter = 0; iter < k - 1; iter++) {
        double maxScore = -std::numeric_limits<double>::infinity();
        int bestC = -1;

        for (int c : remainingIndices) {
            // ---- Diversity Gain ----
            double minDistToRep = std::numeric_limits<double>::max();
            for (int r : representatives) {
                if (dtwMatrix[c][r] < minDistToRep) {
                    minDistToRep = dtwMatrix[c][r];
                }
            }
            double diversityGain = qMin(minDistToRep, minRepDistances);
            diversityGain = qMax(diversityGain, 1e-10); // 避免 log(0)
            double diversityGainComponent = alpha * log10(diversityGain);

            // ---- Coverage Gain ----
            double coverageSum = 0.0;
            for (int i = 0; i < n; i++) {
                double delta = qMin(currentMinDistances[i], dtwMatrix[i][c]);
                coverageSum += delta;
            }
            double coverageGain = coverageSum / n;
            double coverageGainComponent = -(1.0 - alpha) * log10(coverageGain);

            // ---- 综合得分 ----
            double score = diversityGainComponent + coverageGainComponent;

            if (score > maxScore) {
                maxScore = score;
                bestC = c;
            }
        }

        if (bestC == -1) break; // 出错保护

        // 更新
        double newMinRep = std::numeric_limits<double>::max();
        for (int r : representatives) {
            if (dtwMatrix[bestC][r] < newMinRep) {
                newMinRep = dtwMatrix[bestC][r];
            }
        }
        minRepDistances = qMin(minRepDistances, newMinRep);

        for (int i = 0; i < n; i++) {
            currentMinDistances[i] = qMin(currentMinDistances[i], dtwMatrix[i][bestC]);
        }

        representatives.append(bestC);
        remainingIndices.removeOne(bestC);
    }

    // ==== Step 3: 把索引转成数据 ====
    for (int idx : representatives) {
        result.append(data[idx]);
    }

    return result;
}

QVector<int> ProcessData::greedySelectIndices(
    const QVector<QVector<double>>& data,
    const QVector<QVector<double>>& dtwMatrix,
    const QVector<double>& averageMatrix,
    int k,
    double alpha)
{
    int n = data.size();
    QVector<int> representatives;   // 返回的索引

    // ==== 安全检查 ====
    if (n == 0 || k == 0) {
        return representatives;
    }
    if (dtwMatrix.size() != n || averageMatrix.size() != n) {
        throw std::invalid_argument("dtwMatrix 或 averageMatrix 尺寸与 data 不匹配");
    }
    for (int i = 0; i < n; i++) {
        if (dtwMatrix[i].size() != n) {
            throw std::invalid_argument("dtwMatrix 必须是 n×n 矩阵");
        }
    }

    // ==== Step 1: 选择第一个代表 ====
    int firstRep = 0;
    double minAvg = std::numeric_limits<double>::max();
    for (int i = 0; i < n; i++) {
        if (averageMatrix[i] < minAvg) {
            minAvg = averageMatrix[i];
            firstRep = i;
        }
    }
    representatives.append(firstRep);

    // 初始化候选集
    QVector<int> remainingIndices;
    for (int i = 0; i < n; i++) {
        if (i != firstRep) remainingIndices.append(i);
    }

    QVector<double> currentMinDistances(n);
    for (int i = 0; i < n; i++) {
        currentMinDistances[i] = dtwMatrix[i][firstRep];
    }

    double minRepDistances = -std::numeric_limits<double>::max();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dtwMatrix[i][j] > minRepDistances) {
                minRepDistances = dtwMatrix[i][j];
            }
        }
    }

    // ==== Step 2: 迭代选择剩余代表 ====
    for (int iter = 0; iter < k - 1; iter++) {
        double maxScore = -std::numeric_limits<double>::infinity();
        int bestC = -1;

        for (int c : remainingIndices) {
            // ---- Diversity Gain ----
            double minDistToRep = std::numeric_limits<double>::max();
            for (int r : representatives) {
                if (dtwMatrix[c][r] < minDistToRep) {
                    minDistToRep = dtwMatrix[c][r];
                }
            }
            double diversityGain = qMin(minDistToRep, minRepDistances);
            diversityGain = qMax(diversityGain, 1e-10); // 避免 log(0)
            double diversityGainComponent = alpha * log10(diversityGain);

            // ---- Coverage Gain ----
            double coverageSum = 0.0;
            for (int i = 0; i < n; i++) {
                double delta = qMin(currentMinDistances[i], dtwMatrix[i][c]);
                coverageSum += delta;
            }
            double coverageGain = coverageSum / n;
            double coverageGainComponent = -(1.0 - alpha) * log10(coverageGain);

            // ---- 综合得分 ----
            double score = diversityGainComponent + coverageGainComponent;

            if (score > maxScore) {
                maxScore = score;
                bestC = c;
            }
        }

        if (bestC == -1) break; // 出错保护

        // 更新
        double newMinRep = std::numeric_limits<double>::max();
        for (int r : representatives) {
            if (dtwMatrix[bestC][r] < newMinRep) {
                newMinRep = dtwMatrix[bestC][r];
            }
        }
        minRepDistances = qMin(minRepDistances, newMinRep);

        for (int i = 0; i < n; i++) {
            currentMinDistances[i] = qMin(currentMinDistances[i], dtwMatrix[i][bestC]);
        }

        representatives.append(bestC);
        remainingIndices.removeOne(bestC);
    }

    return representatives;
}
