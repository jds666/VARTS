#include "util.h"

Util::Util()
{
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

void Util::splitCsvByColumnType(const QString &filePath)
{
    QFile csvFile(filePath);
    if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "错误", "无法打开 CSV 文件：" + csvFile.errorString());
        return;
    }

    QTextStream in(&csvFile);
    in.setCodec("UTF-8");
    QString headerLine;

    // === 1. 读取表头 ===
    if (!in.readLineInto(&headerLine))
    {
        QMessageBox::warning(nullptr, "错误", "文件为空或无法读取列名。");
        csvFile.close();
        return;
    }

    QStringList columnNames = headerLine.trimmed().split(',', QString::SkipEmptyParts);
    int columnCount = columnNames.size();

    // === 2. 读取所有数据行 ===
    QVector<QStringList> rows;
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList parts = line.split(',', QString::KeepEmptyParts);
        while (parts.size() < columnCount)
            parts.append("");
        rows.append(parts);
    }
    csvFile.close();

    // === 3. 判断每一列是否是数值列 ===
    QVector<bool> isNumeric(columnCount, true);
    for (int c = 0; c < columnCount; ++c)
    {
        for (const QStringList &row : rows)
        {
            QString value = row[c].trimmed();
            if (value.isEmpty()) continue;

            bool ok;
            value.toDouble(&ok);
            if (!ok)
            {
                isNumeric[c] = false;
                break;
            }
        }
    }

    // === 4. 生成两个新文件路径 ===
    QFileInfo fileInfo(filePath);
    QString dirPath = fileInfo.absolutePath();
    QString baseName = fileInfo.completeBaseName();

    QString numFilePath = dirPath + "/" + baseName + "_numColumn.csv";
    QString textFilePath = dirPath + "/" + baseName + "_textColumn.csv";

    QFile numFile(numFilePath);
    QFile textFile(textFilePath);

    if (!numFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "错误", "无法写入数值列文件：" + numFile.errorString());
        return;
    }
    if (!textFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "错误", "无法写入文本列文件：" + textFile.errorString());
        numFile.close();
        return;
    }

    QTextStream numOut(&numFile);
    QTextStream textOut(&textFile);
    numOut.setCodec("UTF-8");
    textOut.setCodec("UTF-8");

    // === 5. 写入表头 ===
    QStringList numHeaders, textHeaders;
    for (int i = 0; i < columnCount; ++i)
    {
        if (isNumeric[i])
            numHeaders.append(columnNames[i]);
        else
            textHeaders.append(columnNames[i]);
    }
    numOut << numHeaders.join(',') << "\n";
    textOut << textHeaders.join(',') << "\n";

    // === 6. 写入数据 ===
    for (const QStringList &row : rows)
    {
        QStringList numRow, textRow;
        for (int i = 0; i < columnCount; ++i)
        {
            if (isNumeric[i])
                numRow.append(row[i]);
            else
                textRow.append(row[i]);
        }
        if (!numRow.isEmpty())
            numOut << numRow.join(',') << "\n";
        if (!textRow.isEmpty())
            textOut << textRow.join(',') << "\n";
    }

    numFile.close();
    textFile.close();

    // === 7. 弹窗提示 ===
    QString message =
        QString("✅ 文件拆分完成！\n\n数值列文件：%1\n文本列文件：%2")
            .arg(numFilePath)
            .arg(textFilePath);

    QMessageBox::information(nullptr, "拆分完成", message);
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

QVector<QVector<QString>> Util::M4GreedySelectColumns(
    const QVector<QVector<QString>>& rawData, int k, double alpha)
{
    QVector<QVector<QString>> selected;

    // ===== 安全检查 =====
    if (rawData.isEmpty() || rawData[0].isEmpty()) {
        qWarning() << "M4GreedySelectColumns: rawData is empty.";
        return selected;
    }
    if (k <= 0 || k > rawData.size()) {
        qWarning() << "M4GreedySelectColumns: invalid k =" << k;
        return selected;
    }

    // ===== Step 1. 过滤空列并记录列名和原始索引 =====
    QVector<int> validIndices;
    QVector<QString> columnNames;
    for (int colIdx = 0; colIdx < rawData.size(); ++colIdx) {
        const auto& col = rawData[colIdx];
        if (col.size() < 2) continue; // 至少有列名 + 一个数值
        bool hasNumeric = false;
        for (int r = 1; r < col.size(); ++r) {
            bool ok = false;
            col[r].toDouble(&ok);
            if (ok) {
                hasNumeric = true;
                break;
            }
        }
        if (hasNumeric) {
            validIndices.append(colIdx);
            columnNames.append(col[0]);
        }
    }

    if (validIndices.isEmpty()) {
        qWarning() << "No numeric data to process.";
        return selected;
    }

    int n = validIndices.size();

    // ===== Step 2. M4 采样降维 =====
    QVector<QVector<double>> sampledData(n);
    for (int i = 0; i < n; ++i) {
        const auto& col = rawData[validIndices[i]];
        QVector<double> series;
        series.reserve(col.size() - 1);
        for (int r = 1; r < col.size(); ++r) {
            bool ok = false;
            double val = col[r].toDouble(&ok);
            if (ok) series.append(val);
        }

        QVector<double> time(series.size());
        std::iota(time.begin(), time.end(), 0);
        auto sampled = ProcessData::m4Sample(time, series, rawData[0].size()/1000);
        sampledData[i] = std::move(sampled.second); // 只保存降采样值
    }

    // ===== Step 3. 计算 DTW 矩阵 =====
    int window = 10;
     DEBUG_LOG("计算全局 DTW 矩阵。。。") ;
    QVector<QVector<double>> dtwMatrix = ProcessData::computeDtwMatrix(sampledData, window);

    // ===== Step 4. 计算平均距离矩阵 =====
     DEBUG_LOG("计算平均距离矩阵。。。") ;
    QVector<double> avgValues = ProcessData::computeAverageMatrix(dtwMatrix);

    // ===== Step 5. 贪心选择索引 =====
     DEBUG_LOG("调用贪心选择算法。。。参数 k:" << k << " alpha: " << alpha) ;
    QVector<int> selectedRelativeIndices =
        ProcessData::greedySelectIndices(sampledData, dtwMatrix, avgValues, k, alpha);

    // ===== Step 6. 根据索引直接输出原始列 =====
    for (int relIdx : selectedRelativeIndices) {
        if (relIdx >= 0 && relIdx < validIndices.size()) {
            int origIdx = validIndices[relIdx]; // 原 rawData 列索引
            selected.append(rawData[origIdx]);
        }
    }

    return selected;
}

QJsonObject Util::languageConfig;  // 静态成员定义

bool Util::loadLanguageFile(const QString& langCode)
{
    QString path = QString(":/config/lang_%1.json").arg(langCode);
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开语言文件:" << path;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "语言文件格式错误:" << path;
        return false;
    }

    languageConfig = doc.object();
     DEBUG_LOG("已加载语言文件:" << path) ;
    return true;
}

QString Util::getText(const QString& category, const QString& key)
{
    if (!languageConfig.contains(category))
        return key;

    QJsonObject cat = languageConfig.value(category).toObject();
    return cat.value(key).toString(key);
}

