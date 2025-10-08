#ifndef UTIL_H
#define UTIL_H
#include <QVector>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
class Util
{
public:
    Util();
    // 简单读取CSV函数，返回QVector<QVector<double>>
    static QVector<QVector<double>> readCSV(const QString &filePath);

    // 随机选择k条数据
    static QVector<QVector<double>> randomSelect(const QVector<QVector<double>> &rawData, int k);

    static QVector<QVector<QString>> readCSVWithColumnNames(const QString &filePath);
    // 随机选择k列数据（包含列名）
    static QVector<QVector<QString>> randomSelectColumns(const QVector<QVector<QString>> &rawData, int k);
};

#endif // UTIL_H
