#include "util.h"

Util::Util()
{
}
// 简单读取CSV函数，返回QVector<QVector<double>>
QVector<QVector<double>> Util::readCSV(const QString &filePath)
{
    QVector<QVector<double>> data;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("无法打开CSV文件");
        return data;
    }
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        QVector<double> row;
        for (const QString &f : fields) {
            bool ok;
            double val = f.toDouble(&ok);
            if (ok) row.append(val);
        }
        if (!row.isEmpty()) data.append(row);
    }
    return data;
}

// 随机选择k条数据
QVector<QVector<double>> Util::randomSelect(const QVector<QVector<double>> &rawData, int k)
{
    QVector<QVector<double>> selected;
    if (rawData.isEmpty()) return selected;

    int n = rawData.size();
    for (int i = 0; i < k; ++i) {
        int idx = QRandomGenerator::global()->bounded(n);
        selected.append(rawData[idx]);
    }
    return selected;
}

// 读取CSV函数，返回QVector<QVector<QString>>，其中data[x][0]为列名
QVector<QVector<QString>> Util::readCSVWithColumnNames(const QString &filePath)
{
    QVector<QVector<QString>> data;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("无法打开CSV文件");
        return data;
    }
    QTextStream in(&file);

    // 读取所有行
    QVector<QVector<QString>> rows;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        rows.append(fields.toVector());
    }

    if (rows.isEmpty()) {
        return data; // 空文件直接返回
    }

    // 转置行列，第一行为列名
    int columnCount = rows[0].size();
    for (int col = 0; col < columnCount; ++col) {
        QVector<QString> columnData;
        // 第一行作为列名，放在每个列数据的第一个位置
        columnData.append(rows[0][col]);

        // 从第二行开始添加该列的数据
        for (int row = 1; row < rows.size(); ++row) {
            if (col < rows[row].size()) {
                columnData.append(rows[row][col]);
            } else {
                columnData.append(""); // 处理列数不足的情况
            }
        }
        data.append(columnData);
    }

    return data;
}

// 随机选择k列数据（包含列名）
QVector<QVector<QString>> Util::randomSelectColumns(const QVector<QVector<QString>> &rawData, int k)
{
    QVector<QVector<QString>> selected;
    if (rawData.isEmpty() || k <= 0 || k > rawData.size()) {
        return selected; // 数据为空或k值无效时返回空
    }

    // 生成不重复的随机列索引
    QVector<int> indices;
    int totalColumns = rawData.size();
    while (indices.size() < k) {
        int idx = QRandomGenerator::global()->bounded(totalColumns);
        if (!indices.contains(idx)) {
            indices.append(idx);
        }
    }

    // 根据随机索引选择列
    for (int idx : indices) {
        selected.append(rawData[idx]);
    }

    return selected;
}
