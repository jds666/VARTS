#ifndef UTIL_H
#define UTIL_H
#include <QVector>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

#include "processdata.h"
#include "debug.h"
class Util
{
public:
    Util();

    // 读数据
    static QVector<QVector<QString>> readCSVWithColumnNames(const QString &filePath);
    // 拆分CSV为数值列和文本列两个文件
    static QString splitCsvByColumnType(const QString &filePath);

    // 随机选择k列数据（包含列名）
    static QVector<QVector<QString>> randomSelectColumns(const QVector<QVector<QString>> &rawData, int k);
    // M4Greedy算法选择k条数据，平衡因子是alpha
    static QVector<QVector<QString>> M4GreedySelectColumns(const QVector<QVector<QString>> &rawData, int k, double alpha,
                                                               QVector<QVector<double>>* out_sampledData = nullptr,
                                                               QVector<QVector<double>>* out_dtwMatrix = nullptr,
                                                               QVector<double>* out_avgValues = nullptr,
                                                               QVector<int>* out_validIndices = nullptr);

    // 加载语言文件
    static bool loadLanguageFile(const QString& langCode);
    static QString getText(const QString& category, const QString& key);

private:
    static QJsonObject languageConfig;

};

#endif // UTIL_H
