#include "util.h"

Util::Util()
{
    // 简单读取CSV函数，返回QVector<QVector<double>>
    QVector<QVector<double>> readCSV(const QString &filePath)
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
    QVector<QVector<double>> randomSelect(const QVector<QVector<double>> &rawData, int k)
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
}
