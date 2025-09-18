// DatabaseHandler.cpp
#include "databasehandler.h"
#include "processdata.h"

DatabaseHandler::DatabaseHandler(QObject *parent) : QObject(parent)
{
}

bool DatabaseHandler::initialize(const QString &host, const QString &dbName, const QString &userName, const QString &password, int port)
{
    m_db = QSqlDatabase::addDatabase("QODBC");
    if (!m_db.isValid()) {
        qDebug() << "Cannot add database";
        return false;
    }

    m_db.setHostName(host);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(userName);
    m_db.setPassword(password);
    m_db.setPort(port);

    if (!m_db.open()) {
        qDebug() << "Cannot open database:" << m_db.lastError().text();
        return false;
    }

    return true;
}

QVector<double> DatabaseHandler::getDataByColumn( const QString &columnName)
{
    QVector<double> columnDataVector;

    if (!m_db.isOpen()) {
        qDebug() << "Database is not open.";
        return columnDataVector;
    }

    QSqlQuery query(m_db);

    // 先查询preprocesseddata表中的记录数量
    query.prepare("SELECT COUNT(*) FROM numdata WHERE \"" + columnName + "\" IS NOT NULL");


    if (query.exec() && query.next() )
    {
        int count = query.value(0).toInt();
        if (count > 0)
        {
            QVector<QString> columnNumDataVector,flyTimeDataVector;//用于处理数据的索引
            query.prepare("SELECT \"" + columnName + "\" FROM numdata WHERE 飞行时间  IS NOT NULL");
            qDebug()<<"SELECT " + columnName + " FROM numdata WHERE 飞行时间  IS NOT NULL";
            if (query.exec())
            {
                while (query.next())
                {
    //                QString value = query.value(0).toString(); // 获取字符串值
    //                bool ok;
    //                double doubleValue = value.toDouble(&ok); // 尝试转换为double
    //                if (ok)
    //                {
    //                    columnDataVector.append(doubleValue);
    //                    qDebug()<<columnName<<":"<<doubleValue;
    //                }
                    QString value = query.value(0).toString();
                    columnNumDataVector.append(value);
//                    qDebug()<<value;

    //
                }
                columnDataVector =  ProcessData::preprocessQStringtoDouble(columnNumDataVector);
                qDebug()<<columnDataVector.size()<<"SELECT " + columnName + " FROM numdata is over";
            }
            else
            {
                qWarning() << "Query failed:" << query.lastError().text();
            }
        }
        else
        {
            QVector<QString> columnRawDataVector,flyTimeDataVector;//用于处理数据的索引
            query.prepare("SELECT \"" + columnName + "\",飞行时间 FROM rawdata WHERE \"" + columnName + "\" IS NOT NULL");
            qDebug()<<"SELECT " + columnName + ",飞行时间 FROM rawdata WHERE " + columnName + " IS NOT NULL";
            if (query.exec())
            {
                while (query.next())
                {
                    QString value = query.value(0).toString(); // 这里假设columnName对应的列是结果集的第一列
                    QString flyTime = query.value(1).toString(); // "飞行时间"列应该是第二列
                    columnRawDataVector.append(value);
                    flyTimeDataVector.append(flyTime);
                }
                // columnDataVector、flyTimeDataVector经过预处理之后 存入 preprocesseddata 表
                qDebug()<<columnName<<"的columnDataVector处理之前的长度是："<<columnDataVector.size();
                double epsilon = 0.1;
                columnDataVector =  ProcessData::preprocessDatatoDB(flyTimeDataVector, columnRawDataVector, epsilon);

                qDebug()<<columnName<<"的columnDataVector处理之后的长度是："<<columnDataVector.size();

                // double类型的 columnDataVector 存入 preprocesseddata 表，列名是columnName
                // 预防SQL注入，使用占位符
//                QString insertStatement = QString("INSERT INTO preprocesseddata (%1) VALUES (?)").arg(columnName);
//                qDebug()<<"INSERT INTO preprocesseddata"+columnName+" VALUES" ;

//                query.prepare(insertStatement);

//                // 遍历columnDataVector，逐条插入数据
//                foreach(double value, columnDataVector)
//                {
//                    query.addBindValue(value); // 绑定值到占位符
//                    if (!query.exec())
//                    { // 执行插入
//                        qCritical() << "Failed to insert into preprocesseddata: " << query.lastError().text();
//                        // 错误处理
//                    }
//                }

//                // 不要忘记提交事务，尤其是当你插入多条记录时
//                if (!m_db.commit())
//                {
//                    qCritical() << "Failed to commit transaction: " << m_db.lastError().text();
//                    // 错误处理
//                }

            }
            else
            {
                qWarning() << "Query failed:" << query.lastError().text();
            }
        }

    }
    else
    {
        qWarning() << "Error counting records in preprocesseddata: " << query.lastError().text();
    }

    return columnDataVector;
}

QMap<QString, int> DatabaseHandler::groupDataByColumnName(const QString &columnName)
{
    QMap<QString, int> aggregatedData;

    // 构建SQL查询语句，使用GROUP BY来进行分类计数
    QString queryStr = QString("SELECT %1, COUNT(*) FROM rawdata GROUP BY %1").arg(columnName);
    qDebug()<<queryStr;
    QSqlQuery query(m_db);
    if (query.exec(queryStr))
    {
        while (query.next())
        {
            QString key = query.value(0).toString(); // 分类的键（列名对应的值）
            int count = query.value(1).toInt();     // 对应的计数值
            aggregatedData[key] = count;
        }
    } else {
        qWarning() << "Query failed:" << query.lastError().text();
    }

    return aggregatedData;
}

QStringList DatabaseHandler::getColumnNames(const QString &tableName)
{
    QStringList columnNames;
    QSqlQuery query(m_db);
    if (query.exec(QString("SELECT column_name FROM information_schema.columns WHERE table_name = '%1'").arg(tableName))) {
        while (query.next()) {
            columnNames << query.value(0).toString();
        }
    } else {
        qDebug() << "Error fetching column names:" << query.lastError().text();
    }
    return columnNames;
}
/**
 * @brief DatabaseHandler::getColumnNum
 * @param tableName
 * @return 数据库列数
 */
int DatabaseHandler::getColumnNum(const QString &tableName)
{
    int columnCount=0;
    if (!m_db.isValid() || !m_db.isOpen()) {
          qDebug() << "Database not open or invalid.";
          return columnCount;
      }

      QSqlQuery query(m_db);
      QString sql = QString("SELECT COUNT(*) FROM information_schema.columns WHERE table_name = '%1'")
                         .arg(tableName); // 转义tableName中的单引号

      if (query.exec(sql)) {
          if (query.next()) {
              columnCount = query.value(0).toInt(); // 获取计数结果
              qDebug() << tableName << "has" << columnCount << "columns.";
          } else {
              qDebug() << "No result for column count query.";
          }
      } else {
          qDebug() << "Error executing query:" << query.lastError().text();
      }
      return columnCount;
}

int DatabaseHandler::getRowNum(const QString &tableName)
{
    if (!m_db.isValid() || !m_db.isOpen()) {
        qDebug() << "Database not open or invalid.";
        return -1; // 或者抛出异常，表示数据库未正确打开
    }

    QSqlQuery query(m_db);
    QString sql = QString("SELECT COUNT(*) FROM %1").arg(tableName); // 转义tableName中的单引号以避免SQL注入

    if (query.exec(sql)) {
        if (query.next()) {
            int rowCount = query.value(0).toInt(); // 获取计数结果
            return rowCount;
        } else {
            qDebug() << "No result for row count query on table:" << tableName;
            return -1;
        }
    } else {
        qDebug() << "Error executing query for table:" << tableName << query.lastError().text();
        return -1;
    }
}

bool DatabaseHandler::getTopTenRowsFromRawTable()
{
    QSqlQuery query(m_db);
    QString sql = QString("SELECT 俯仰角 FROM rawdata LIMIT 10");

    if (query.exec(sql)) {
        while (query.next()) {
            // 假设表中有两列名为column1和column2
            QString column1Value = query.value("俯仰角").toString();

            qDebug() << "Column1: " << column1Value;
        }
        return true;
    } else {
        qDebug() << "Fetch rows failed:" << query.lastError().text();
        return false;
    }
}

void DatabaseHandler::showAllTableContents(const QString& tableName)
{
    // 创建查询对象
    QSqlQuery query(m_db);

    // 执行 SQL 查询
    QString sql = QString("SELECT * FROM %1").arg(tableName);
    if (!query.exec(sql)) {
        qWarning() << "查询执行失败:" << query.lastError().text();
        return;
    }

    // 输出查询结果
    while (query.next()) {
        int columnCount = query.record().count();  // 获取列数
        for (int i = 0; i < columnCount; ++i) {
            QString columnName = query.record().fieldName(i);  // 获取列名
            QVariant value = query.value(i);  // 获取列值
            qDebug() << columnName << ":" << value.toString();
        }
        qDebug() << "--------------------";  // 分隔每一行的输出
    }
}

void DatabaseHandler::loadPitchAngleCsvIntoDatabase(const QString &csvFilePath, const QString &tableName)
{
    QFile file(csvFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << csvFilePath;
        return;
    }

    QTextStream in(&file);
    bool isFirstLine = true;  // 标记是否是第一行（表头）

    // 创建 SQL 查询对象
    QSqlQuery query(m_db);

    // 创建表（使用 FLOAT 类型）
    QString createTableSql = QString("CREATE TABLE IF NOT EXISTS %1 (flight_time FLOAT, pitch_angle FLOAT)").arg(tableName);
    if (!query.exec(createTableSql)) {
       qWarning() << "创建表失败:" << query.lastError().text();
       return;
    }
    // 遍历 CSV 文件中的每一行
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        // 跳过空行
        if (line.isEmpty()) {
            continue;
        }

        // 跳过表头（第一行）
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        // 按逗号分割每一行数据
        QStringList columns = line.split(",", QString::SkipEmptyParts);  // 用逗号分割

        if (columns.size() != 2) {
            qWarning() << "无效的行数据:" << line;
            continue;  // 如果列数不对，跳过这一行
        }

        // 获取飞行时间和俯仰角
        bool ok1, ok2;
        float flightTime = columns[0].toFloat(&ok1);
        float pitchAngle = columns[1].toFloat(&ok2);

        // 确保数据有效
        if (!ok1 || !ok2) {
            qWarning() << "无效数据，跳过这一行:" << line;
            continue;
        }

//        qDebug()<<flightTime<<" "<<pitchAngle;

//        // 插入数据
// 直接绑定不行，报错：QODBCResult::exec: Unable to execute statement: "ERROR: insufficient data left in message;\nError while executing the query "
//        query.prepare(QString("INSERT INTO %1 (flight_time, pitch_angle) VALUES (?, ?)").arg(tableName));
//        qDebug()<<"尝试绑定"<<flightTime;
//        query.addBindValue(flightTime, QSql::In);
//        qDebug()<<"尝试绑定"<<pitchAngle;
//        query.addBindValue(pitchAngle, QSql::In);

        //.arg()转字符串
        QString insertSql = QString("INSERT INTO %1 (flight_time, pitch_angle) VALUES (%2, %3)")
            .arg(tableName)
            .arg(QString::number(static_cast<double>(flightTime), 'f', 1))  // 显式转换为 double
            .arg(QString::number(static_cast<double>(pitchAngle), 'f', 2)); // 显式转换为 double

        query.prepare(insertSql); //不能缺
//        qDebug()<<insertSql;
        if (!query.exec()) {
            qWarning() << "插入数据失败:" << query.lastError().text();
            break;
        }
    }

    file.close();  // 关闭文件
}
void DatabaseHandler::loadTemperatureCsvIntoDatabase(const QString &csvFilePath, const QString &tableName)
{
    QFile file(csvFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << csvFilePath;
        return;
    }

    QTextStream in(&file);
    bool isFirstLine = true;  // 标记是否是第一行（表头）

    // 创建 SQL 查询对象
    QSqlQuery query(m_db);

    // 创建表（使用 FLOAT 类型）
    QString createTableSql = QString("CREATE TABLE IF NOT EXISTS \"%1\" (timestamp VARCHAR(25), value FLOAT)").arg(tableName);
    if (!query.exec(createTableSql)) {
       qWarning() << "创建表失败:" << query.lastError().text();
       return;
    }

    int i = 0;
    QString insertSqlBase = QString("INSERT INTO \"%1\" (timestamp, value) VALUES ").arg(tableName);
    QString insertSqlValues;

    // 遍历 CSV 文件中的每一行
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        // 跳过空行
        if (line.isEmpty()) {
            continue;
        }

        // 跳过表头（第一行）
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        // 按逗号分割每一行数据
        QStringList columns = line.split(",", QString::SkipEmptyParts);  // 用逗号分割

        if (columns.size() != 2) {
            qWarning() << "无效的行数据:" << line;
            continue;  // 如果列数不对，跳过这一行
        }

        // 获取时间和值
        bool ok2;
        QString timestamp = columns[0]; // 假设时间戳为 Unix 时间戳（秒）
        float value = columns[1].toFloat(&ok2);

        // 确保数据有效
        if (!ok2) {
            qWarning() << "无效数据，跳过这一行:" << line;
            continue;
        }

        // 每次插入100行数据
        if (i > 0 && i % 1000 == 0) {
            // 执行合并的插入语句
            QString insertSql = insertSqlBase + insertSqlValues.left(insertSqlValues.length() - 1);  // 去掉最后一个逗号
            query.prepare(insertSql);
            if (!query.exec()) {
                qWarning() << "插入数据失败:" << query.lastError().text();
                break;
            }
            insertSqlValues.clear(); // 重置批量插入值
            qDebug() << "已插入" << i << "行数据";
        }

        // 将数据添加到插入语句中
        insertSqlValues += QString("('%1', %2),").arg(timestamp).arg(QString::number(static_cast<double>(value), 'f', 4));

        i++;
    }

    // 最后一次插入剩余的行
    if (!insertSqlValues.isEmpty()) {
        QString insertSql = insertSqlBase + insertSqlValues.left(insertSqlValues.length() - 1);  // 去掉最后一个逗号
        query.prepare(insertSql);
        if (!query.exec()) {
            qWarning() << "插入数据失败:" << query.lastError().text();
        }
    }

    file.close();  // 关闭文件
}

void DatabaseHandler::loadTempCsvFilesToDatabase(const QString &filePath)
{

    //循环获取文件夹下的csv文件名字
    QDir dir(filePath);
    if (!dir.exists()) {
        qWarning("Directory does not exist.");
    }
    // 设置过滤器仅匹配csv文件
    QStringList filters;
    filters << "*.csv";
    dir.setNameFilters(filters);
    // 获取该目录下所有的csv文件
    QFileInfoList files = dir.entryInfoList(QDir::Files);
    int i=0;
    // 遍历所有csv文件并处理
    for (const QFileInfo &fileInfo : files) {
        QString fileName = fileInfo.absoluteFilePath();
        QString tableName;
        std::string fname = fileName.toStdString();
        std::regex pattern(".*_([^_]*)\\.csv");
        std::smatch matchResults;

        if (std::regex_search(fname, matchResults, pattern)) {
            tableName = "temp_"+QString::fromStdString(matchResults[1].str());
        } else {
            qWarning("Filename does not match the expected pattern.");
        }
        qDebug() <<++i<< " Processing file:" << fileName << " tableName:" << tableName;
        if (!tableName.isEmpty()) {
            // 调用handler的方法加载数据到数据库
            loadTemperatureCsvIntoDatabase(fileName, tableName);
        }
    }
}



//void DatabaseHandler::loadTemperatureCsvIntoDatabase(const QString &csvFilePath, const QString &tableName)
//{
//    QFile file(csvFilePath);
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qWarning() << "无法打开文件:" << csvFilePath;
//        return;
//    }

//    QTextStream in(&file);
//    bool isFirstLine = true;  // 标记是否是第一行（表头）

//    // 创建 SQL 查询对象
//    QSqlQuery query(m_db);

//    // 创建表（使用 FLOAT 类型）
//    QString createTableSql = QString("CREATE TABLE IF NOT EXISTS \"%1\" (timestamp VARCHAR(25), value FLOAT)").arg(tableName);
//    if (!query.exec(createTableSql)) {
//       qWarning() << "创建表失败:" << query.lastError().text();
//       return;
//    }
//    int i=0;
//    // 遍历 CSV 文件中的每一行
//    while (!in.atEnd())
//    {
//        QString line = in.readLine().trimmed();

//        // 跳过空行
//        if (line.isEmpty()) {
//            continue;
//        }

//        // 跳过表头（第一行）
//        if (isFirstLine) {
//            isFirstLine = false;
//            continue;
//        }

//        // 按逗号分割每一行数据
//        QStringList columns = line.split(",", QString::SkipEmptyParts);  // 用逗号分割

//        if (columns.size() != 2) {
//            qWarning() << "无效的行数据:" << line;
//            continue;  // 如果列数不对，跳过这一行
//        }

//        // 获取时间和值
//        bool ok2;
//        QString timestamp = columns[0]; // 假设时间戳为 Unix 时间戳（秒）
//        float value = columns[1].toFloat(&ok2);

//        // 确保数据有效
//        if (!ok2) {
//            qWarning() << "无效数据，跳过这一行:" << line;
//            continue;
//        }

//        //.arg()转字符串
//        QString insertSql = QString("INSERT INTO \"%1\" (timestamp, value) VALUES ('%2', '%3')")
//            .arg(tableName)
//            .arg(timestamp)
//            .arg(QString::number(static_cast<double>(value), 'f', 4)); // 显式转换为 double

//        query.prepare(insertSql); //不能缺
////        qDebug()<<insertSql;
//        if (!query.exec()) {
//            qWarning() << "插入数据失败:" << query.lastError().text();
//            break;
//        }
//        i++;
//        if(i%100000==0)
//        {
//            qDebug()<<"插入第 "<<i<<" 行数据";
//        }
//    }

//    file.close();  // 关闭文件
//}

void DatabaseHandler::loadCsvIntoDatabase(const QString &csvFilePath, const QString &tableName)
{
    QFile file(csvFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << csvFilePath;
        return;
    }

    QTextStream in(&file);
    bool isFirstLine = true;  // 标记是否是第一行（表头）

    // 创建 SQL 查询对象
    QSqlQuery query(m_db);

    // 遍历 CSV 文件中的每一行
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        // 跳过空行
        if (line.isEmpty()) {
            continue;
        }

        // 按逗号分割每一行数据
        QStringList columns = line.split(",", QString::SkipEmptyParts);  // 用逗号分割

        if (isFirstLine) {
            // 动态创建表
            QString createTableSql = QString("CREATE TABLE IF NOT EXISTS \"%1\" (").arg(tableName);

            for (int i = 0; i < columns.size(); ++i) {
                if (i > 0) {
                    createTableSql += ", ";
                }
                // 使用双引号包裹列名，防止特殊字符或保留字引起的问题
                QString columnName = columns[i].trimmed();
                columnName = QString("\"%1\"").arg(columnName.replace("\"", "\"\""));  // 转义双引号
                createTableSql += QString("%1 TEXT").arg(columnName);
            }
            createTableSql += ")";
            qDebug()<<createTableSql;
            if (!query.exec(createTableSql)) {
                qWarning() << "创建表失败:" << query.lastError().text();
                file.close();
                return;
            }
//            // 执行空查询以初始化 QSqlQuery 的内部状态
//            query.exec(QString("SELECT * FROM \"%1\" LIMIT 0").arg(tableName));

            isFirstLine = false;
            continue;
        }

//        // 确保每一行的数据列数与表头一致
//        if (columns.size() != query.record().count()) {
//            qWarning() << "无效的行数据，列数不匹配:" << line;
//            qDebug()<<"表头列数:"<<columns.size()<<" 数据列数:"<<query.record().count();
//              continue;  // 如果列数不对，跳过这一行
//            break;
//        }

        // 构建插入语句
        QString insertSql = QString("INSERT INTO \"%1\" VALUES (").arg(tableName);

        for (int i = 0; i < columns.size(); ++i) {
            if (i > 0) {
                insertSql += ", ";
            }
            // 将每一列的数据用单引号括起来，确保作为字符串处理
            // 并转义单引号以防止 SQL 注入
            insertSql += QString("'%1'").arg(columns[i].replace("'", "''"));
        }
        insertSql += ")";

        if (!query.exec(insertSql)) {
            qWarning() << "插入数据失败:" << query.lastError().text();
            break;
        }
    }

    file.close();  // 关闭文件
}
