#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt headers
#include <QMainWindow>
#include <QAction>
#include <QDebug>
#include <QDateTime>
#include <QtDataVisualization>
#include <QLinearGradient>
#include <QPainter>
#include <QTableWidget>
#include <QComboBox>
#include <QtCharts>
#include <QPieSeries>
#include <QMap>
#include <QChartView>
#include <QElapsedTimer>
#include <QtDataVisualization/QValue3DAxis>
#include <QLinearGradient>
// Third-party libraries
#include "xlsxdocument.h"

// Custom headers
#include "processdata.h"
#include "databasehandler.h"
#include "drawplot.h"
#include "util.h"
#include "debug.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<QString> getColumnDataByColumnName(QString ColumnName, int row=0);
    QVector<QString> getColumnDataByColumnIndex(int index, int row=0);

protected:
    bool eventFilter(QObject* obj, QEvent* event);

private:
    // UI 指针
    Ui::MainWindow *ui;

    // 数据
    QVector<QVector<QString>> Data;

    // 接口函数
    void initialPlot(const QStringList& columnNames);  // 初始化所有图
    void drawTablePlot(const QVector<QVector<QString>>& Data);
    void drawThreeD(const QVector<QString>& timeData);
    void drawPiePlot(const QStringList& columnNames);
    void drawAnomalyDetection();
    void drawLargeLine();
    void initRepresentativePage(const QString &filePath);
    void loadDatasetAndUpdateRepresentativePage(const QString &selectedFile);
    void handleUploadButtonTriggered();
    void drawMultiLineChart(const QVector<QCheckBox*>& checkedCheckBoxList);
    void drawTablePlotByDB();
    void drawThreeDByDB();
    void drawPiePlotByDB();

    // 内部状态
    int inputstatus = 2;   // 数据来源：1=Excel, 2=CSV, 3=TXT, 4=OpenGauss
    int timeStart = 0;     // 动态折线图起始时间索引
    int timeSize = 1000;   // 每次提取的数据长度
    bool isPlaying = false;
    int playSpeed = 1;
    QTimer *timer = nullptr;
    double epsilon = 0; //折线图数据聚合的时间间隔大小,误差小于0.1说明0.1秒之内的数据，直接取平均

    bool lineDrawn = false;
    bool adDrawn = false;

    QString timeColumnName; // 横坐标:“飞行时间”或者“FXSJ”或者“flightTime”或者第一列的列名

    // 代表选择的预处理数据
    QVector<QVector<double>> m4SampledData; // 用于贪心的压缩后数据
    QVector<QVector<double>> cachedDtwMatrix; // DTW预计算矩阵
    QVector<double> cachedAvgValues; // 平均距离矩阵
    QVector<int> cachedValidIndices; // rawdata数值列索引
    bool isCacheValid = false; // 缓存是否可用

    // 数据类型分类
    QSet<QString> textColumn;
    QSet<QString> numColumn;

    // 折线图相关
    QVector<QCheckBox*> checkBoxList;
    QVector<QCheckBox*> checkedCheckBoxList;
    QVBoxLayout* lineChartLayout = nullptr;
    QCPItemText* tracerXText = nullptr;
    QCPItemTracer* tracer = nullptr;

    //代表选择界面相关全局变量
    QVector<QVector<QString>> rawData;     // 存放当前加载的数据集
    QVector<QVector<QString>> selectedData; // 当前选中的代表列
    QMap<QString, QColor> columnColors;    // 列名到颜色的映射
    int selectedK = 5;
    double selectedAlpha = 0.0;
    QString selectedFolder;

    // 拖动相关
    bool m_dragging = false;
    QPoint m_dragStartPosition;
    QList<QFrame*> draggableFrames;

    // 数据库
    DatabaseHandler handler;

    // 工具函数（内部用）
    void setupCheckboxesForColumns(const QStringList& columnNames);
    QCustomPlot* createLineChart(const QString& yText);
    QChartView* createChartView(QPieSeries *series, const QString& title);
    QMap<QString, int> classifyDataByColumnName(QString ColumnName);
    void updateColumnScrollArea(QScrollArea* scrollArea,
                                            const QVector<QVector<QString>>& data,
                                            const QMap<QString, QColor>& colorMap,
                                            const QString& emptyText = "No data available");
    void updateSelectedDataAndPlot();
private slots:
    // 槽函数（内部信号响应）
    void handleActionExcelTriggered();
    void handleActionDatabaseTriggered();
    void handleActionCsvTriggered();
    void handleActionTxtTriggered();
    void handleColumnTypeSplitterTriggered();

    void onComboBoxIndexChanged();
    void onPlayPauseButtonClicked();
    void onRestartButtonClicked();
    void updateLineChart();
    void handleSpeedValueChanged();
    void handleSelectKValueChanged();
    void handleSelectAlphaValueChanged();
    void handleCheckBoxStateChanged(int state);
    void onPlotClicked(QCPAbstractPlottable *plottable);

};

#endif // MAINWINDOW_H
