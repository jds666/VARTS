#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

//opengauss 数据库 sql库
#include <QSqlDatabase>
#include <QMessageBox>
#include <QtSql>
#include <QSqlQuery>
#include <regex>

class DatabaseHandler : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseHandler(QObject *parent = nullptr);
    bool initialize(const QString &host, const QString &dbName, const QString &userName, const QString &password, int port);
    bool getTopTenRowsFromRawTable();
    QVector<double> getDataByColumn(const QString &columnName);
    QMap<QString, int> groupDataByColumnName(const QString &columnName);
    QStringList getColumnNames(const QString& tableName);
    int getColumnNum(const QString &tableName);
    int getRowNum(const QString &tableName);
    void showAllTableContents(const QString& tableName);
    void loadPitchAngleCsvIntoDatabase(const QString& csvFilePath, const QString& tableName);
    void loadTemperatureCsvIntoDatabase(const QString& csvFilePath, const QString& tableName);
    void loadTempCsvFilesToDatabase(const QString& filePath);
    void loadCsvIntoDatabase(const QString &csvFilePath, const QString &tableName);
//    QStringList splitCsvLine(const QString &line);




private:
    QSqlDatabase m_db;
};

#endif // DATABASEHANDLER_H
