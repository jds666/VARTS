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

// Third-party libraries
#include "xlsxdocument.h"

// Custom headers
#include "processdata.h"
#include "databasehandler.h"
#include "drawplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<QVector<QString>> Data;

    int inputstatus=2; //1从excel文件导入数据，2从CSV文件导入数据，3从TXT文件导入数据，4从opengauss数据库导入数据
    int timeStart = 0;   //动态折线图的开始的时间索引
    int timeSize = 1000; // 每次提取的数据长度

    bool eventFilter(QObject* obj, QEvent* event);
    void setupCheckboxesForColumns(const QStringList& columnNames);

    QCustomPlot *createLineChart(const QString yText);

    void initialPlot();//初始化所有图
    void drawTablePlot(const QVector<QVector<QString>> Data);//图1 表格


    void drawThreeD();//图4 3D
    void drawPiePlot();//图5 箱线图

    void drawAnomalyDetection();//图7 扩展异常检测
    void drawLargeLine();//图8 大规模数据压缩

    void drawMultiLineChart(const QVector<QCheckBox*>& checkedCheckBoxList);//绘制高维图

    void drawTablePlotByDB();//图1 表格

    void drawThreeDByDB();
    void drawPiePlotByDB();


    QVector<QString> getColumnDataByColumnName(QString ColumnName,int row=0);
    QVector<QString> getColumnDataByColumnIndex(int index,int row=0);

private:
    Ui::MainWindow *ui;
    bool isPlaying;  // 动态播放状态
    int playSpeed = 1;  //播放速度
    QTimer *timer;
    QSet<QString> textColumn;
    QSet<QString> numColumn;
    QVector<QCheckBox*> checkBoxList;//折线图的高维的复选框数组
    QVector<QCheckBox*> checkedCheckBoxList;//被选中的折线图的高维的复选框数组
    bool m_dragging = false; // 当前是否正在进行拖动操作
    QPoint m_dragStartPosition;//用于保存拖动操作开始时鼠标的位置
    QList<QFrame*> draggableFrames; //可以拖动的QFrame
    QVBoxLayout* lineChartLayout; //折线图之间布局
    QCPItemText* tracerXText;
    QCPItemTracer* tracer;
    // 初始化数据库处理类
    DatabaseHandler handler;

    QChartView* createChartView(QPieSeries *series, const QString& title);
    QMap<QString, int> classifyDataByColumnName(QString ColumnName);
//    QAction *actionOpen;  // 声明动作对象

private slots:
    void handleActionExcelTriggered();
    void handleActionDatabaseTriggered();
    void handleActionCsvTriggered();
    void handleActionTxtTriggered();
    void onComboBoxIndexChanged();
    void onPlayPauseButtonClicked();
    void onRestartButtonClicked();
//    void updateLineChart1();
    void updateLineChart();
    void handleSpeedValueChanged();
    void handleCheckBoxStateChanged(int state);

//    void showLineChart();

};
#endif // MAINWINDOW_H
