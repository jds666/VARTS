#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // openEuler22.03中自带的openGauss
    if(handler.initialize("192.168.127.131", "openGauss", "jdd", "123456@jdd", 7654))
        // openEuler20.03源代码编译的openGuass
        //    if(handler.initialize("192.168.127.134", "openGauss5.0.3", "jdd", "123456@jdd", 5432))
    {
        DEBUG_LOG("数据库连接成功");
        // 测试数据
        // handler.showAllTableContents("mytest");
        // 导入数据
        // handler.loadTemperatureCsvIntoDatabase("E://JDD//opengauss-develop//Datasets//Temperature_data_10000000.csv", "temperature_data_test7");
        // 循环导入各个测点数据
        // handler.loadTempCsvFilesToDatabase("E://JDD//pyProgram//data_csv//temperature");
    }
    else
    {
        DEBUG_LOG("数据库连接失败");
    }
    // 语言加载
    Util::loadLanguageFile("zh_CN"); // 默认中文，可改成 "en_US" 切换英文

    //列名分类
    numColumn = {"俯仰角", "滚转角", "航迹角", "俯仰角速度", "滚转角速度", "偏航角速度", "飞行时间", "升降舵反馈", "左副翼舵反馈", "右副翼舵反馈", "方向舵反馈",
                 "电源电压", "飞控版本号", "CPU电压", "升降舵输出", "左副翼舵输出", "右副翼舵输出", "BD1星数", "GPS1星数", "GLONNASS1星数", "BD2/GPS1时",
                 "BD2/GPS1分", "BD2/GPS1秒", "BD2/GPS1经度", "BD2/GPS1纬度", "BD2/GPS1高度", "BD2/GPS1地速", "BD2/GPS1天速", "BD2/GPS1航迹角",
                 "气压高度", "高度变换率", "真空速", "指示空速", "方向舵输出", "油门输出", "俯仰角给定", "滚转角给定", "航向角给定", "高度给定",
                 "滚转角积分", "磁航向角", "待飞距", "侧偏距", "航路", "航点", "GPS2星数", "BD2星数", "BD2/GPS2经度", "BD2/GPS2纬度", "BD2/GPS2高度",
                 "BD2/GPS2地速", "BD2/GPS2天速", "BD2/GPS2航迹角", "BD2/GPS2时", "BD2/GPS2分", "BD2/GPS2秒", "横向延迟", "纵向延迟", "当前绝对高度",
                 "当前天速", "当前地速", "当前相对高度", "航线航向", "机箱温度", "当前经度", "当前纬度", "大气机地高", "GPS地高", "校时", "推算时间",
                 "待切航路", "待切航点", "任务起点", "程控选择", "遥控状态", "油门反馈",
                 "pitchAngle", "rollAngle", "courseAngle", "pitchRate", "rollRate", "yawRate", "flightTime",
                 "elevatorFeedback", "leftAileronFeedback", "rightAileronFeedback", "rudderFeedback", "powerVoltage",
                 "flightControllerVersion", "cpuVoltage", "elevatorOutput", "leftAileronOutput", "rightAileronOutput",
                 "bd1SatelliteCount", "gps1SatelliteCount",
                 "glonass1SatelliteCount", "bd2Gps1Hour", "bd2Gps1Minute", "bd2Gps1Second", "bd2Gps1Longitude",
                 "bd2Gps1Latitude", "bd2Gps1Altitude", "bd2Gps1GroundSpeed", "bd2Gps1AirSpeed", "bd2Gps1CourseAngle",
                 "pressureAltitude", "altitudeRateOfChange", "trueAirspeed", "indicatedAirspeed",
                 "rudderOutput", "throttleOutput", "pitchAngleSetpoint", "rollAngleSetpoint", "headingAngleSetpoint",
                 "altitudeSetpoint", "rollAngleIntegral", "magneticHeadingAngle", "distanceToGo",
                 "driftDistance", "flightPath", "waypoint", "gps2SatelliteCount", "bd2SatelliteCount",
                 "bd2Gps2Longitude","bd2Gps2Latitude", "bd2Gps2Altitude",
                 "bd2Gps2GroundSpeed", "bd2Gps2AirSpeed", "bd2Gps2CourseAngle",
                 "bd2Gps2Hour", "bd2Gps2Minute", "bd2Gps2Second", "lateralDelay", "longitudinalDelay", "currentAbsoluteAltitude",
                 "currentAirSpeed", "currentGroundSpeed", "currentRelativeAltitude", "flightPathHeading",
                 "fuselageTemperature", "currentLongitude", "currentLatitude", "atmosphericAltitude", "gpsAltitude",
                 "timeSynchronization", "estimatedTime", "pendingFlightPath", "pendingWaypoint", "missionStartPoint",
                 "autopilotSelection", "remoteControlStatus", "throttleFeedback",
                 "副翼舵输出","机动样式","离散输入","离散输出1","离散输出2","离散输出3","无线电高度","超低空给定","超低空高度",
                 "超低空高度变换率","加速度1","加速度2","加速度3",
                 "FYJ","GZJ","HJJ","FYJSD","GZJSD","PHJSD","FXSJ","SJDFK","ZFYDFK","YFYDFK","FXDFK","DYDY","FKBBH","CDY","SJDSC",
                 "ZFYDSC","YFYDSC","BD1XS","GPS1XS","G1XS","GPS1S","GPS1F","GPS1M","GPS1JD","GPS1WD","GPS1GD","GPS1DS","GPS1TS",
                 "GPS1HJJ","QYGD","GDBHL","ZKS","ZSKS","FXDSC","YMSC","FYJGD","GZJGD","HXJGD","GDGD","GZJJF","CHXJ","DFJ","CPJ",
                 "HL","HD","WXDGD","CDKGD1","CDKGD2","CDKGDL","GPS2XS","BD2XS","GPS2JD","GPS2WD","GPS2GD","GPS2DS","GPS2TS",
                 "GPS2HJJ","GPS2S","GPS2F","GPS2M","HXYC","ZXYC","DQJDGD","DQTS","DQDS","DQXDGD","HXHX","JXWD","DQJD","DQWD",
                 "DQJDG","GDG","XS","TSSJ","DQHL","DQHD","RWQD","CKXZ","YMFK","YKZT"};
    textColumn = {"BD2/GPS1定位模式","bd2Gps1PositioningMode","BD2/GPS1定位状态","bd2Gps1PositioningStatus",
                  "大气机状态","atmosphericCondition", "磁罗盘状态","magneticCompassStatus","飞控模态","flightControlMode",
                  "安控状态","safetyControlStatus","GPS识别","gpsIdentification","安控信号","safetyControlSignal",
                  "BD2/GPS2定位状态","bd2Gps2PositioningStatus","油门档位","throttlePosition",
                  "接收指令回报","状态字1","状态字2","状态字3","执行指令回报","靶机工作模式",
                  "GPS1DWMS","GPS1DWZT","DQJZT","CLPZT","FKMT","JDYS","AKZT","GPSSB","AKXH","GPS2DWZT","YMDW","BJGZMS"};

    //UI布局
    ui->setupUi(this);
    //    setupPlot();
    //设置标题

    setWindowTitle("FTsViz");

    setWindowIcon(QIcon(":/visual"));
    ui->speedlabel->setAlignment(Qt::AlignCenter);  // 设置居中对齐
    ui->horizontalSlider->setRange(1, 100);  // 设置范围
    ui->horizontalSlider->setValue(1);
    //    ui->stackedWidget->setFixedSize(2100,800);
    //    // 设置ui->scrollArea_line布局
    //    QWidget* lineChartContainer = new QWidget(ui->scrollArea_line); // 创建一个容器窗口小部件
    //    lineChartLayout= new QVBoxLayout(lineChartContainer); // 在容器中创建垂直布局
    //    ui->scrollArea_line->setWidget(lineChartContainer); // 将容器设置为滚动区域的窗口小部件
    //    lineChartContainer->setLayout(lineChartLayout);

    //    // 设置全局样式，好丑
    //    QFile file(":/QtTheme/Integrid/Integrid.qss");
    //    file.open(QFile::ReadOnly);
    //    setStyleSheet(file.readAll());

    //设置toolbar样式
    QIcon lineIcon(":/line");  // 创建QIcon 对象表示toolbar中折线图的图标
    QIcon scatterIcon(":/scatter");
    QIcon pieIcon(":/pie");
    QIcon threeDscatterIcon(":/3dscatter");
    QIcon boxplotIcon(":/boxplot");
    QIcon adIcon(":/AD");
    QIcon tableIcon(":/table");
    QIcon representativeIcon(":/representative");
    QIcon mlineIcon(":/mline");

    ui->tab->tabBar()->setTabIcon(0, lineIcon);
    ui->tab->tabBar()->setTabIcon(1,mlineIcon);
    ui->tab->tabBar()->setTabIcon(2, scatterIcon);
    ui->tab->tabBar()->setTabIcon(3, threeDscatterIcon);
    ui->tab->tabBar()->setTabIcon(4, pieIcon);
    ui->tab->tabBar()->setTabIcon(5, boxplotIcon);


    ui->action_line->setIcon(lineIcon);  // 设置 QAction 的图标
    ui->action_line->setIconText("Basic Viz");  // 设置文本
    ui->action_line->setText("基础可视化");  // 设置工具提示文本

    ui->action_ad->setIcon(adIcon);
    ui->action_ad->setIconText("Anomaly Detection");  // 设置文本
    ui->action_ad->setText("异常检测");  // 设置工具提示文本
    ui->action_table->setIcon(tableIcon);
    ui->action_table->setIconText("Table");
    ui->action_table->setText("概况表");
    ui->action_representative->setIcon(representativeIcon);
    ui->action_representative->setIconText("Representative");
    ui->action_representative->setText("代表数据选择");

    // 初始状态在表格页
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(ui->pageTable));
    //滑杆绑定
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::handleSpeedValueChanged);
    connect(ui->horizontalSlider_k, &QSlider::sliderReleased, this, &MainWindow::handleSelectKValueChanged);
    connect(ui->horizontalSlider_alpha, &QSlider::sliderReleased, this, &MainWindow::handleSelectAlphaValueChanged);


    // 只在第一次点击使调用的功能
    connect(ui->action_large,&QAction::triggered,this,[=]() {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(ui->pageLarge));
        if (!lineDrawn) {
            drawLargeLine();
            lineDrawn = true;
        }
    });
    connect(ui->action_ad, &QAction::triggered, this, [=]() {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(ui->pageAd));
        if (!adDrawn) {
            drawAnomalyDetection();
            adDrawn = true;
        }
    });
    // 绑定按钮和图栈
    connect(ui->action_table,&QAction::triggered,this,[=]() {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(ui->pageTable));
    });
    connect(ui->action_line, &QAction::triggered, this, [=]() {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(ui->pageLine));
    });
    connect(ui->action_representative,&QAction::triggered,this,[=]() {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(ui->pageRepresentative));
    });
    //直接绘制异常图片
    //    tracerXText = new QCPItemText(ui->pageAd);
    //    tracerXText->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);//文字布局：顶、左对齐
    //    tracerXText->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例为单位/实际坐标为单位）
    //    tracerXText->position->setCoords(0.5, 0); //把文字框放在X轴的中间，Y轴的最顶部
    //    tracerXText->setFont(QFont(font().family(), 16)); //字体大小
    //    tracerXText->setPen(QPen(Qt::black)); //字体颜色
    //    tracerXText->setPadding(QMargins(2,2,2,2));//文字距离边框几个像素

    tracer = new QCPItemTracer(ui->pageAd);
    tracer->setInterpolating(false);//游标禁用插值
    tracer->setGraph(ui->pageAd->graph(0));// 设置游标吸附在ui->pageAd->graph(0)这条曲线上

    initDatasetComboBox();// 代表性选择功能的初始点



    // 更换下拉框内容
    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(onComboBoxIndexChanged()));
    connect(ui->comboBox_datasets, SIGNAL(activated(int)), this, SLOT(onComboBoxDatasetChanged()));

    //动态展示
    // 初始化播放状态和定时器
    isPlaying = false;
    // 初始化定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateLineChart);

    // 开始停止按钮
    QIcon playIcon(":/play");
    QSize playButtonSize = ui->playPauseButton->size(); // 获取按钮的大小
    QPixmap playPixmap = playIcon.pixmap(playButtonSize);
    ui->playPauseButton->setIcon(playPixmap);
    ui->playPauseButton->setIconSize(playButtonSize);
    connect(ui->playPauseButton, &QPushButton::clicked, this, &MainWindow::onPlayPauseButtonClicked);

    // 重新开始按钮
    QIcon restartIcon(":/restart");
    QSize restartButtonSize = ui->restartButton->size(); // 获取按钮的大小
    QPixmap restartPixmap = restartIcon.pixmap(restartButtonSize);
    ui->restartButton->setIcon(restartPixmap);
    ui->restartButton->setIconSize(restartButtonSize);
    QObject::connect(ui->restartButton, &QPushButton::clicked, this, &MainWindow::onRestartButtonClicked);

    // 文件从excel导入
    QObject::connect(ui->action_Excel, &QAction::triggered, this, &MainWindow::handleActionExcelTriggered);
    QObject::connect(ui->action_Csv, &QAction::triggered, this, &MainWindow::handleActionCsvTriggered);
    QObject::connect(ui->action_OpenGauss,&QAction::triggered, this, &MainWindow::handleActionDatabaseTriggered);
    QObject::connect(ui->action_Txt,&QAction::triggered, this, &MainWindow::handleActionTxtTriggered);

    // 数据分列功能绑定
    QObject::connect(ui->action_Column_Type_Splitter, &QAction::triggered, this, &MainWindow::handleColumnTypeSplitterTriggered);

    // 帮助按钮
    QObject::connect(ui->actionhttps_doc_qt_io, &QAction::triggered, this,[]() { // Lambda 表达式
        QUrl url("https://doc.qt.io/");
        if (!QDesktopServices::openUrl(url)) {
            DEBUG_LOG("Could not open URL");
        }
    });

    // 退出
    QObject::connect(ui->action_quit,&QAction::triggered, this, &QApplication::quit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<float> doubleQvectortoFloatQvector(const QVector<double>& doubleData) {
    QVector<float> floatData;
    for (const double &value : doubleData) {
        floatData.append(static_cast<float>(value));
    }
    return floatData;
}

void MainWindow::updateLineChart()
{
    if(ui->comboBox->currentText().isEmpty()&&checkedCheckBoxList.size()==0)
    {
        ui->statusbar->showMessage("Selection Box is Empty");

        return;
    }
    else
    {
        for(QCheckBox* checkedCheckBox : checkedCheckBoxList)
        {
            if(inputstatus == 1 || inputstatus==2)
            {
                QVector<QString> xdata,ydata;
                // "飞行时间" 列
                xdata = getColumnDataByColumnName(timeColumnName);
                // 当前选择列
                ydata = getColumnDataByColumnName(checkedCheckBox->text());
                QCustomPlot* customPlot = findChild<QCustomPlot*>(QString("customPlot_") + checkedCheckBox->text());
                //             DEBUG_LOG(msg)"更新"<<customPlot->objectName();
                if (customPlot)
                {
                    //                 DEBUG_LOG(msg)"绘图更新"<<customPlot->objectName();
                    // 准备绘图数据
                    ui->statusbar->showMessage("Drawing Dynamic Line Chart  " + QString::number(timeStart/5) + " seconds");

                    QVector<double> x, y;
                    double epsilon=0.1;
                    //数据处理
                    ProcessData::processData(xdata,ydata,epsilon,x,y);
                    if (timeStart < x.size() - timeSize)
                    {
                        QVector<double> x_temp(timeSize), y_temp(timeSize);
                        if(timeStart < timeSize)
                        {
                            x_temp = x.mid(0, timeSize);
                            // 使用 NaN 填充前部分
                            for (int i = 0; i < timeSize - timeStart; ++i) {
                                y_temp[i] = std::numeric_limits<double>::quiet_NaN();
                            }
                            // 填充顺序值在后部分
                            // 使用 y[0] 到 y[timeSize] 填充指定范围
                            std::copy(y.begin(), y.begin() + timeStart, y_temp.begin() + timeSize - timeStart);
                        }
                        else
                        {
                            x_temp = x.mid(timeStart, timeSize);
                            y_temp = y.mid(timeStart, timeSize);
                        }
                        customPlot->addGraph();
                        customPlot->graph(0)->setData(x_temp, y_temp);
                        // 设置坐标轴标签:
                        customPlot->xAxis->setLabel("Time");
                        customPlot->yAxis->setLabel(ydata[0]);
                        // 设置坐标轴范围:
                        customPlot->xAxis->setRange(*std::min_element(x_temp.constBegin(), x_temp.constEnd()), *std::max_element(x_temp.constBegin(), x_temp.constEnd())); // 使用 x_temp 向量中的最小和最大值作为范围
                        //不显示x坐标
                        customPlot->xAxis->setTickLabels(false); // 设置刻度标签不可见
                        customPlot->yAxis->setRange(*std::min_element(y_temp.constBegin(), y_temp.constEnd()), *std::max_element(y_temp.constBegin(), y_temp.constEnd())); // 使用 y_temp 向量中的最小和最大值作为范围
                        // 重新绘制折线图:
                        customPlot->replot();
                    }
                    else
                    {
                        timer->stop();
                    }
                }
            }
            else if(inputstatus == 4)
            {
                QVector<double> x = handler.getDataByColumn("飞行时间");
                QVector<double> y= handler.getDataByColumn(checkedCheckBox->text());
                QCustomPlot* customPlot = findChild<QCustomPlot*>(QString("customPlot_") + checkedCheckBox->text());
                if (customPlot)
                {
                    ui->statusbar->showMessage("Drawing Dynamic Line Chart  " + QString::number(timeStart/5) + " seconds");
                    if (timeStart < x.size() - timeSize)
                    {
                        QVector<double> x_temp(timeSize), y_temp(timeSize);
                        if(timeStart < timeSize)
                        {
                            x_temp = x.mid(0, timeSize);
                            for (int i = 0; i < timeSize - timeStart; ++i) {
                                y_temp[i] = std::numeric_limits<double>::quiet_NaN();
                            }
                            std::copy(y.begin(), y.begin() + timeStart, y_temp.begin() + timeSize - timeStart);
                        }
                        else
                        {
                            x_temp = x.mid(timeStart, timeSize);
                            y_temp = y.mid(timeStart, timeSize);
                        }
                        customPlot->addGraph();
                        customPlot->graph(0)->setData(x_temp, y_temp);
                        customPlot->xAxis->setLabel("Time");
                        customPlot->yAxis->setLabel(checkedCheckBox->text());
                        customPlot->xAxis->setRange(*std::min_element(x_temp.constBegin(), x_temp.constEnd()), *std::max_element(x_temp.constBegin(), x_temp.constEnd()));
                        customPlot->xAxis->setTickLabels(false);
                        customPlot->yAxis->setRange(*std::min_element(y_temp.constBegin(), y_temp.constEnd()), *std::max_element(y_temp.constBegin(), y_temp.constEnd()));
                        customPlot->replot();
                    }
                    else
                    {
                        timer->stop();
                    }
                }
            }
            else
            {
                DEBUG_LOG("updateLineChart no inputstatus == 3 or anyothers");
            }
        }
        if(inputstatus==1||inputstatus==2)
        {
            QVector<QString> xdata,ydata;
            xdata = getColumnDataByColumnName(timeColumnName);
            ydata = getColumnDataByColumnName(ui->comboBox->currentText());
            QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(ui->single_line);
            if (customPlot)
            {
                ui->statusbar->showMessage("Drawing Dynamic Line Chart  " + QString::number(timeStart/5) + " seconds");
                QVector<double> x, y;
                double epsilon=0.1;
                ProcessData::processData(xdata,ydata,epsilon,x,y);
                if (timeStart < x.size() - timeSize)
                {
                    QVector<double> x_temp(timeSize), y_temp(timeSize);
                    if(timeStart < timeSize)
                    {
                        x_temp = x.mid(0, timeSize);
                        for (int i = 0; i < timeSize - timeStart; ++i) {
                            y_temp[i] = std::numeric_limits<double>::quiet_NaN();
                        }
                        std::copy(y.begin(), y.begin() + timeStart, y_temp.begin() + timeSize - timeStart);
                    }
                    else
                    {
                        x_temp = x.mid(timeStart, timeSize);
                        y_temp = y.mid(timeStart, timeSize);
                    }
                    customPlot->addGraph();
                    customPlot->graph(0)->setData(x_temp, y_temp);
                    customPlot->xAxis->setLabel("Time");
                    customPlot->yAxis->setLabel(ydata[0]);
                    customPlot->xAxis->setRange(*std::min_element(x_temp.constBegin(), x_temp.constEnd()), *std::max_element(x_temp.constBegin(), x_temp.constEnd()));                     customPlot->xAxis->setTickLabels(false); // 设置刻度标签不可见
                    customPlot->yAxis->setRange(*std::min_element(y_temp.constBegin(), y_temp.constEnd()), *std::max_element(y_temp.constBegin(), y_temp.constEnd()));
                    customPlot->replot();
                    timeStart = timeStart + playSpeed;
                }
                else
                {
                    timer->stop();
                }
            }
        }
        else if(inputstatus == 4)
        {
            QVector<double> x = handler.getDataByColumn("飞行时间");
            QVector<double> y= handler.getDataByColumn(ui->comboBox->currentText());
            QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(ui->single_line);
            if (customPlot)
            {
                ui->statusbar->showMessage("Drawing Dynamic Line Chart  " + QString::number(timeStart/5) + " seconds");
                if (timeStart < x.size() - timeSize)
                {
                    QVector<double> x_temp(timeSize), y_temp(timeSize);
                    if(timeStart < timeSize)
                    {
                        x_temp = x.mid(0, timeSize);
                        for (int i = 0; i < timeSize - timeStart; ++i) {
                            y_temp[i] = std::numeric_limits<double>::quiet_NaN();
                        }
                        std::copy(y.begin(), y.begin() + timeStart, y_temp.begin() + timeSize - timeStart);
                    }
                    else
                    {
                        x_temp = x.mid(timeStart, timeSize);
                        y_temp = y.mid(timeStart, timeSize);
                    }
                    customPlot->addGraph();
                    customPlot->graph(0)->setData(x_temp, y_temp);
                    customPlot->xAxis->setLabel("Time");
                    customPlot->yAxis->setLabel(ui->comboBox->currentText());
                    customPlot->xAxis->setRange(*std::min_element(x_temp.constBegin(), x_temp.constEnd()), *std::max_element(x_temp.constBegin(), x_temp.constEnd()));
                    customPlot->xAxis->setTickLabels(false);
                    customPlot->yAxis->setRange(*std::min_element(y_temp.constBegin(), y_temp.constEnd()), *std::max_element(y_temp.constBegin(), y_temp.constEnd()));
                    customPlot->replot();
                    timeStart = timeStart + playSpeed;
                }
                else
                {
                    timer->stop();
                }
            }
        }
        else
        {
            DEBUG_LOG("updateLineChart no inputstatus == 3 or anyothers");
        }

    }
}
void MainWindow::handleSpeedValueChanged()
{
    playSpeed = ui->horizontalSlider->value();
    ui->speedlabel->setText(QString::number(playSpeed)+" times speed");
}

void MainWindow::handleSelectKValueChanged()
{
    selectedK = ui->horizontalSlider_k->value();
    ui->label_k_num->setText(QString::number(selectedK));

    if (!rawData.isEmpty()) {
        QTimer::singleShot(200, this, [this]() {
            selectedData = Util::M4GreedySelectColumns(rawData, selectedK, selectedAlpha);
            updateColumnScrollArea(ui->scrollArea_selected_column, selectedData, columnColors);
            DrawPlot::drawSelectedLineChartByM4Sample(ui->plot_representative, selectedData, columnColors);
        });
    }
}

void MainWindow::handleSelectAlphaValueChanged()
{
    selectedAlpha = ui->horizontalSlider_alpha->value() / 100.0;
    ui->label_alpha_num->setText(QString::number(selectedAlpha, 'f', 2));

    if (!rawData.isEmpty()) {
        QTimer::singleShot(200, this, [this]() {
            selectedData = Util::M4GreedySelectColumns(rawData, selectedK, selectedAlpha);
            updateColumnScrollArea(ui->scrollArea_selected_column, selectedData, columnColors);
            DrawPlot::drawSelectedLineChartByM4Sample(ui->plot_representative, selectedData, columnColors);
        });
    }
}


void MainWindow::handleCheckBoxStateChanged(int state)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender()); // 获取发送信号的复选框对象
    if (checkBox)
    {
        QString checkBoxText = checkBox->text();
        if (state == Qt::Checked)
        {
            DEBUG_LOG(checkBoxText << "复选框 被选中");
            checkedCheckBoxList.append(checkBox);
            drawMultiLineChart(checkedCheckBoxList);
        }
        else if (state == Qt::Unchecked)
        {
            DEBUG_LOG(checkBoxText << "复选框 取消选中");
            checkedCheckBoxList.removeOne(checkBox);
            drawMultiLineChart(checkedCheckBoxList);
        }
    }
}
void clearWidgetLayout(QWidget* widget) {
    QLayout* layout = widget->layout(); // 获取指定 QWidget 的布局
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            QWidget* childWidget = item->widget();
            delete item;
            delete childWidget;
        }
    }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    //     DEBUG_LOG(msg)obj->objectName();
    if(obj->objectName().contains("AnomalyDetectionPlot"))
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        // 获取鼠标点击的像素坐标
        double pixelX = mouseEvent->pos().x();

        // 使用 xAxis 的 pixelToCoord 方法将像素坐标转换为横坐标
        double plotX = ui->pageAd->xAxis->pixelToCoord(pixelX);


        tracer->setGraphKey(plotX); // 设置游标的X值（这就是游标随动的关键代码）

        // 在线上的点的(x,y)坐标值
        double traceX = tracer->position->key();
        double traceY = tracer->position->value();



        //         DEBUG_LOG(msg) QString::number(traceX)<<","<<QString::number(traceY);
        ui->statusbar->showMessage("x:"+QString::number(traceX)+","+"y:"+QString::number(traceY));

        //        tracerXText->setText(QString::number(traceX)+","+QString::number(traceY));//


        ui->pageAd->replot();
    }

    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (obj->objectName().contains("draggableFrame") && mouseEvent->button() == Qt::LeftButton)
        {
            QFrame* frame = qobject_cast<QFrame*>(obj);
            if (frame)
            {
                m_dragging = true;
                m_dragStartPosition = mouseEvent->pos();
                frame->setCursor(Qt::ClosedHandCursor);
                frame->raise(); // 提升至顶部
                return true;
            }
        }
    }
    else if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (m_dragging && obj->objectName().contains("draggableFrame"))
        {
            QFrame* frame = qobject_cast<QFrame*>(obj);
            if (frame)
            {
                QPoint diff = mouseEvent->pos() - m_dragStartPosition;
                QPoint newFramePos = frame->pos() + diff;
                frame->move(newFramePos);
                return true;
            }
        }
        else if(!m_dragging && obj->objectName().contains("draggableFrame"))
        {
            QFrame* frame = qobject_cast<QFrame*>(obj);
            if (frame)
            {
                QPoint mousePos = frame->mapFromGlobal(QCursor::pos());
                if (mousePos.x() <= 4 || mousePos.x() >= frame->width() - 4 || mousePos.y() <= 4 || mousePos.y() >= frame->height() - 4)
                {
                    // 光标在边框上
                    frame->setCursor(Qt::SizeAllCursor);
                }
            }
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        if (obj->objectName().contains("draggableFrame"))
        {
            QFrame* frame = qobject_cast<QFrame*>(obj);
            if (frame)
            {
                m_dragging = false;
                frame->setCursor(Qt::OpenHandCursor);
                int distance = frame->pos().y() - ui->scrollArea_line->y();

                // 获取 ui->scrollArea_line 的布局
                QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollArea_line->widget()->layout());
                if (layout)
                {
                    // 根据 frame 的位置重新布局
                    int frameIndex = layout->indexOf(frame);
                    if (frameIndex != -1)
                    {
                        // 从布局中移除 frame
                        layout->removeWidget(frame);

                        // 获取其他元素的位置
                        QList<QWidget*> widgets;
                        for (int i = 0; i < layout->count(); ++i)
                        {
                            QWidget* widget = layout->itemAt(i)->widget();
                            if (widget != nullptr && widget != frame)
                                widgets.append(widget);
                        }

                        // 获取其他元素的位置
                        QVector<int> widgetPositions;
                        for (QWidget* widget : widgets)
                        {
                            widgetPositions.append(widget->pos().y()- ui->scrollArea_line->y());
                        }

                        // 根据其他元素的位置确定插入的位置
                        int insertIndex = -1;
                        for (int i = 0; i < widgetPositions.size(); i++)
                        {
                            if (distance <= widgetPositions[i])
                            {
                                insertIndex = i;
                                break;
                            }
                        }

                        // 将 frame 插入到指定位置
                        if (insertIndex != -1)
                            layout->insertWidget(insertIndex, frame);
                        else
                            layout->addWidget(frame);

                        // 更新布局
                        ui->scrollArea_line->widget()->setLayout(layout);
                    }
                }
                // 更新 ui->scrollArea_line 的内容
                ui->scrollArea_line->update();

                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}




/**
 * @brief MainWindow::handleActionExcelTriggered
 * 读取excel中的数据到columnData中
 */
void MainWindow::handleActionExcelTriggered()
{
    // 使用文件对话框让用户选择文件
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "Excel 文件 (*.xlsx)");

    // 获取当前时间
    QDateTime currentTime1 = QDateTime::currentDateTime();
    //  输出当前时间
    DEBUG_LOG("文件"<<filePath<<"已选择  "<<endl<<"当前时间："<< currentTime1.toString("yyyy-MM-dd hh:mm:ss")) ;

    // 检查是否选择了文件
    if (!filePath.isEmpty())
    {

        QDateTime currentTime2 = QDateTime::currentDateTime();
        DEBUG_LOG("文件不为空" <<endl<<"当前时间："<< currentTime2.toString("yyyy-MM-dd hh:mm:ss")) ;

        // 打开 Excel 文件
        QXlsx::Document xlsx(filePath);

        QDateTime currentTime3 = QDateTime::currentDateTime();
        DEBUG_LOG("打开 Excel 文件" <<endl<<"当前时间："<< currentTime3.toString("yyyy-MM-dd hh:mm:ss")) ;

        // 获取行数和列数
        int rowCount = xlsx.dimension().rowCount();
        int columnCount = xlsx.dimension().columnCount();
        // 获取当前时间
        QDateTime currentTime4 = QDateTime::currentDateTime();
        DEBUG_LOG("获取行数和列数 " <<endl<<"当前时间："<< currentTime4.toString("yyyy-MM-dd hh:mm:ss")) ;

        // 创建进度条
        QProgressBar* progressBar = new QProgressBar();
        statusBar()->addWidget(progressBar);
        progressBar->setRange(0, columnCount);  // 设置进度条范围

        // 获取列名
        QStringList columnNames;
        for (int column = 1; column <= columnCount; ++column)
        {
            QVariant columnName = xlsx.cellAt(1, column)->value();
            columnNames.append(columnName.toString());
        }

        // 清空 QComboBox
        ui->comboBox->clear();

        // 输出列名
        DEBUG_LOG("Excel 列名:");
        for (const QString& columnName : columnNames)
        {
            DEBUG_LOG(columnName) ;
            //设置选择框中的选项
            if (numColumn.contains(columnName) && columnName!="飞行时间")
            {
                ui->comboBox->addItem(columnName);
            }
        }

        // 读取数据并存储到 columnData 中
        Data.clear();
        DEBUG_LOG("正在存储数据到 columnData 中");
        for (int col = 1; col <= columnCount; ++col)
        {
            QVector<QString> column;

            for (int row = 1; row <= rowCount; ++row)
            {
                QVariant data = xlsx.read(row, col);
                column.append(data.toString());
            }
            // 更新进度条
            progressBar->setValue(col);
            QCoreApplication::processEvents();  // 处理事件，以确保进度条更新

            Data.append(column);
        }

        inputstatus = 1;
        // 获取当前时间
        QDateTime currentTime5 = QDateTime::currentDateTime();
        DEBUG_LOG("已经读取excel内容到Data中"<<endl<<"当前时间："<< currentTime5.toString("yyyy-MM-dd hh:mm:ss")) ;

        // 从状态栏中移除进度条
        statusBar()->removeWidget(progressBar);

        delete progressBar;
        initialPlot(columnNames);
    }
}
void MainWindow::handleActionDatabaseTriggered()
{
    //    // 实例化DatabaseHandler
    //    DatabaseHandler dbHandler;

    // 初始化数据库连接
    if (handler.initialize("192.168.127.131", "openGauss", "jdd", "123456@jdd", 7654))
    {
        // 获取rawdata表的列名
        QStringList columnNames = handler.getColumnNames("rawdata");

        if (!columnNames.isEmpty())
        {
            //更新复选框与高维界面布局
            setupCheckboxesForColumns(columnNames);

            inputstatus = 4;
            // 数据处理完毕，可以考虑初始化图表或其他后续操作
            initialPlot(columnNames); // 假设这是初始化图表的函数，需要确保Data已被正确填充
        }
        else
        {
            DEBUG_LOG("未能获取到列名") ;
        }
    }
    else
    {
        DEBUG_LOG("数据库连接失败") ;
    }
}
void MainWindow::handleActionCsvTriggered()
{
    // 使用文件对话框让用户选择文件
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "CSV 文件 (*.csv)");

    // 获取当前时间
    //    QDateTime currentTime1 = QDateTime::currentDateTime();
    //     DEBUG_LOG(msg) "文件"<<filePath<<"已选择  "<<endl<<"当前时间："<< currentTime1.toString("yyyy-MM-dd hh:mm:ss");

    // 检查是否选择了文件
    if (!filePath.isEmpty())
    {
        //        QDateTime currentTime2 = QDateTime::currentDateTime();
        //         DEBUG_LOG(msg) "文件不为空" <<endl<<"当前时间："<< currentTime2.toString("yyyy-MM-dd hh:mm:ss");

        QFile csvFile(filePath);
        if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            DEBUG_LOG("无法打开 CSV 文件：" << csvFile.errorString()) ;
            return;
        }

        QTextStream in(&csvFile);
        QString line;

        // 获取第一行作为列名
        QStringList columnNames;
        if (in.readLineInto(&line))
        {
            columnNames = line.split(',', QString::SkipEmptyParts);
        }
        else
        {
            DEBUG_LOG("CSV 文件为空或无法读取列名") ;
            csvFile.close();
            return;
        }
        DEBUG_LOG(filePath);
        DEBUG_LOG("列名：") ;
        for( QString columnName : columnNames){
            DEBUG_LOG(columnName) ;
        }

        //更新复选框与高维界面布局
        setupCheckboxesForColumns(columnNames);

        // 初始化数据存储容器
        Data.clear();

        // 读取数据并存储到 Data 中
        int currentRow = 0;
        int numRows = 0;
        int maxColumns = 0;
        while (in.readLineInto(&line))
        {
            numRows++;
            QStringList values = line.split(',', QString::SkipEmptyParts);
            maxColumns = std::max(maxColumns, static_cast<int>(values.size()));
        }

        Data.resize(maxColumns);
        for (auto& row : Data)
        {
            row.resize(numRows);
        }
         DEBUG_LOG("maxColumns "<<maxColumns);
         DEBUG_LOG("Data.size() "<<Data.size());
         DEBUG_LOG("Data[0].size() "<<Data[0].size());
        // 重置文件读取位置
        csvFile.seek(0);
        in.seek(0); // 对于 QTextStream，使用 in.seek(0) 来重新定位内部读取位置

        currentRow = 0;
        while (in.readLineInto(&line) && currentRow < numRows)
        {
            QStringList values = line.split(',', QString::SkipEmptyParts);
            for (int i = 0; i < values.size() && i < maxColumns; ++i)
            {
                Data[i][currentRow] = values[i];
            }
            currentRow++;
        }

        DEBUG_LOG("关闭文件" <<"currentRow: "<<currentRow);
        csvFile.close();
        inputstatus = 2;
        // 获取当前时间
        //        QDateTime currentTime5 = QDateTime::currentDateTime();
        //         DEBUG_LOG(msg) "已经读取CSV内容到Data中"<<endl<<"当前时间："<< currentTime5.toString("yyyy-MM-dd hh:mm:ss");

        initialPlot(columnNames); // 初始化绘图

    }
}
/**
 * @brief MainWindow::setupCheckboxesForColumns
 *
 * 此函数根据提供的列名在用户界面中设置复选框。它会清除现有的复选框和布局，
 * 然后为每个相关的列名创建新的复选框，将它们添加到可滚动区域内的垂直布局中，并将它们的状态更改。
 *
 * @param columnNames 包含应创建复选框的列名的QStringList。
 */
void MainWindow::setupCheckboxesForColumns(const QStringList& columnNames)
{
    // 清空comboBox
    ui ->comboBox->clear();
    // 输出列名
    DEBUG_LOG( "列名:");
    for (const QString& columnName : columnNames)
    {
        if (numColumn.contains(columnName) && columnName!= timeColumnName)
        {
            DEBUG_LOG(columnName) ;
            ui->comboBox->addItem(columnName);
        }
    }

    //清除原来的复选框与折线图布局
    clearWidgetLayout(ui->scrollArea_checkbox);
    clearWidgetLayout(ui->scrollArea_line);
    DEBUG_LOG("清除原来布局") ;
    checkBoxList.clear();//清除复选框
    checkedCheckBoxList.clear();
    draggableFrames.clear();
    DEBUG_LOG("清除复选框") ;
    // 添加复选框
    QWidget* checkBoxContainer = new QWidget(ui->scrollArea_checkbox); // 创建一个容器窗口小部件
    QVBoxLayout* layout = new QVBoxLayout(checkBoxContainer); // 在容器中创建垂直布局
    ui->scrollArea_checkbox->setWidget(checkBoxContainer); // 将容器设置为滚动区域的窗口小部件

    for (const QString& columnName : columnNames)
    {
        if (numColumn.contains(columnName) && columnName!=timeColumnName)
        {
            QCheckBox* checkBox = new QCheckBox(columnName, checkBoxContainer); // 将容器作为复选框的父控件
            checkBox->setFont(QFont("", 15)); // 设置字体大小为15
            layout->addWidget(checkBox);
            checkBoxList.append(checkBox); // 将复选框指针添加到容器中

            connect(checkBox, &QCheckBox::stateChanged, this, &MainWindow::handleCheckBoxStateChanged);
        }
    }
    //    checkBoxContainer->setLayout(layout); // 将垂直布局设置给容器

    // 添加折线图框，设置ui->scrollArea_line布局
    QWidget* lineChartContainer = new QWidget(ui->scrollArea_line); // 创建一个容器窗口小部件
    lineChartLayout= new QVBoxLayout(lineChartContainer); // 在容器中创建垂直布局
    ui->scrollArea_line->setWidget(lineChartContainer); // 将容器设置为滚动区域的窗口小部件
    //    lineChartContainer->setLayout(lineChartLayout);
}


void MainWindow::handleActionTxtTriggered()
{
    //目前使用的数据路径：“D:\研究生\科研\时序数据\数据集2023年12月3日\UCR_TimeSeriesAnomalyDatasets2021\AnomalyDatasets_2021\UCR_TimeSeriesAnomalyDatasets2021\FilesAreInHere\UCR_Anomaly_FullData"
    // 使用文件对话框让用户选择文件
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "文本文件 (*.txt)");

    // 获取当前时间
    QDateTime currentTime1 = QDateTime::currentDateTime();
    DEBUG_LOG("文件"<<filePath<<"已选择  "<<endl<<"当前时间："<< currentTime1.toString("yyyy-MM-dd hh:mm:ss")) ;

    // 检查是否选择了文件
    if (!filePath.isEmpty())
    {
        QDateTime currentTime2 = QDateTime::currentDateTime();
        DEBUG_LOG( "文件不为空" <<endl<<"当前时间："<< currentTime2.toString("yyyy-MM-dd hh:mm:ss"));

        QFile csvFile(filePath);
        if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            DEBUG_LOG("无法打开 CSV 文件：" << csvFile.errorString()) ;
            return;
        }

        QTextStream in(&csvFile);
        QString line;


        // 获取第一行作为列名
        QStringList columnNames;
        if (in.readLineInto(&line))
        {
            columnNames = line.split(QRegExp("\\s+"));
        }
        else
        {
            DEBUG_LOG("CSV 文件为空或无法读取列名") ;
            csvFile.close();
            return;
        }

        // 清空 QComboBox
        ui->comboBox->clear();

        // 输出列名并设置选择框中的选项
        DEBUG_LOG("CSV 列名:") ;
        for (const QString& columnName : columnNames)
        {
            DEBUG_LOG(columnName) ;
            if (numColumn.contains(columnName) && columnName!="飞行时间")
            {
                ui->comboBox->addItem(columnName);
            }
        }

        // 初始化数据存储容器
        Data.clear();

        // 读取数据并存储到 Data 中
        int currentRow = 0;
        int numRows = 0;
        int maxColumns = 0;
        while (in.readLineInto(&line))
        {
            numRows++;
            QStringList values = line.split(',', QString::SkipEmptyParts);
            maxColumns = std::max(maxColumns, static_cast<int>(values.size()));
        }

        Data.resize(maxColumns);
        for (auto& row : Data)
        {
            row.resize(numRows);
        }
        DEBUG_LOG("maxColumns "<<maxColumns);
        DEBUG_LOG("Data.size() "<<Data.size());
        DEBUG_LOG("Data[0].size() "<<Data[0].size());
        // 重置文件读取位置
        csvFile.seek(0);
        in.seek(0); // 对于 QTextStream，使用 in.seek(0) 来重新定位内部读取位置

        currentRow = 0;
        while (in.readLineInto(&line)&&currentRow<numRows)//
        {
            QStringList values = line.split(',', QString::SkipEmptyParts);
            //             DEBUG_LOG(msg)"values: "<<values;
            for (int i = 0; i < values.size() && i < maxColumns; ++i)
            {
                Data[i][currentRow] = values[i];
            }
            currentRow++;
            //             DEBUG_LOG(msg)currentRow;
        }
        DEBUG_LOG("关闭文件"<<endl<<"currentRow: "<<currentRow);
        csvFile.close();

        inputstatus = 3;
        // 获取当前时间
        QDateTime currentTime5 = QDateTime::currentDateTime();
        DEBUG_LOG("已经读取CSV内容到Data中"<<endl<<"当前时间："<< currentTime5.toString("yyyy-MM-dd hh:mm:ss")) ;


        QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(ui->single_line);
        if (customPlot)
        {
            // 准备绘图数据:
            ui->statusbar->showMessage("Line Chart");

            QVector<double> x, y;

            // 数据处理
            QVector<QString> y_inital = getColumnDataByColumnIndex(0);

            x.resize(y_inital.size()); // 设置x向量的大小与y_inital相同

            // 填充x向量
            for (int i = 0; i < y_inital.size(); ++i) {
                x[i] = i + 1; // 从1开始自增
            }

            // 遍历原始向量并进行转换
            for (const QString& str : y_inital) {
                bool ok;
                double value = str.toDouble(&ok);
                if (ok) {
                    y.append(value);
                }
            }

            // 创建折线图并设置数据:
            customPlot->addGraph();
            customPlot->graph(0)->setData(x, y);

            // 获取坐标轴并设置标签文本
            customPlot->xAxis->setLabel("time");
            customPlot->yAxis->setLabel("data");

            // 创建一个QFont对象并设置字体大小
            QFont labelFont;
            labelFont.setPointSize(15);

            // 设置X轴和Y轴标签的字体
            customPlot->xAxis->setLabelFont(labelFont);
            customPlot->yAxis->setLabelFont(labelFont);

            // 设置坐标轴范围:
            customPlot->yAxis->setRange(*std::min_element(y.constBegin(), y.constEnd()),
                                        *std::max_element(y.constBegin(), y.constEnd())); // 使用 y 向量中的最小和最大值作为范围

            // 设置Y轴范围后，调整Y轴刻度标签的字体大小
            customPlot->yAxis->setTickLabelFont(labelFont);
            customPlot->xAxis->setTickLabelFont(labelFont);

            // 设置一些基本的customPlot配置：
            customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
            customPlot->axisRect()->setupFullAxesBox();
            customPlot->rescaleAxes();
            customPlot->replot();
        }
    }
}

void MainWindow::handleColumnTypeSplitterTriggered()
{
    ui->statusbar->showMessage(Util::getText("StatusBar", "SplittingDataset"));

    QString filePath = QFileDialog::getOpenFileName(this, "选择 CSV 文件", "", "CSV 文件 (*.csv)");
    if (filePath.isEmpty())
        return;

    Util::splitCsvByColumnType(filePath);

    ui->statusbar->showMessage(Util::getText("StatusBar", "SplitDone"));

}


void MainWindow::onComboBoxIndexChanged()
{
    QComboBox* comboBox = ui->comboBox;
    QString selectedItem = comboBox->currentText();
    DEBUG_LOG("selectedItem:"<<selectedItem);
    if(inputstatus == 1 || inputstatus == 2)
    {
        QVector<QString> xdata,ydata;

        // "飞行时间" 列
        xdata = getColumnDataByColumnName(timeColumnName);
        // 指定列
        ydata = getColumnDataByColumnName(selectedItem);

        DrawPlot::drawScatterPlot(ui->pageScatter,xdata,ydata);
        DrawPlot::drawSingleLineChart(ui->single_line,xdata,ydata);
        DrawPlot::drawBoxPlot(ui->pageBox,xdata,ydata,ui->comboBox->currentText());
    }
    else if(inputstatus == 4)
    {
        // 图片默认展示第一列数据的图
        QString columnNamex = "飞行时间";
        QString columnNamey = selectedItem;

        QVector<double> xdata = handler.getDataByColumn(columnNamex);
        QVector<double> ydata = handler.getDataByColumn(columnNamey);

        // 调用绘制图函数，将数据传递给它
        DrawPlot::drawScatterPlotByDB(ui->pageScatter,xdata,ydata,columnNamex,columnNamey);
        DrawPlot::drawSingleLineChartByDB(ui->single_line,xdata,ydata,columnNamex,columnNamey);
        DrawPlot::drawBoxPlotByDB(ui->pageBox,ydata,ui->comboBox->currentText());

    }
    else
    {
        DEBUG_LOG("onComboBoxIndexChanged no inputstatus == 3");
    }
}

void MainWindow::onComboBoxDatasetChanged()
{
    try {
        ui->statusbar->showMessage("正在更新数据。。。");
        QString selectedFile = ui->comboBox_datasets->currentText();
        loadDatasetAndUpdateLine(selectedFile);
        ui->statusbar->showMessage("更新完成,数据集:"+selectedFile);
    } catch (const std::exception& e) {
        DEBUG_LOG("Error in onComboBoxDatasetChanged:" << e.what()) ;
        QMessageBox::critical(this, "Error", QString("Failed to process dataset: %1").arg(e.what()));
    } catch (...) {
        DEBUG_LOG( "Unknown error in onComboBoxDatasetChanged");
        QMessageBox::critical(this, "Error", "An unknown error occurred while processing dataset");
    }
}

void MainWindow::onPlayPauseButtonClicked()
{
    if (isPlaying) {
        // 当前状态是“播放”，切换为“暂停”
        ui->playPauseButton->setIcon(QIcon(":/play"));
        isPlaying = false;
        timer->stop();
    } else {
        // 当前状态是“暂停”，切换为“播放”
        ui->playPauseButton->setIcon(QIcon(":/pause"));
        isPlaying = true;
        timer->start(200);
    }
}

void MainWindow::onRestartButtonClicked()
{
    timeStart = 0;   //动态图从0开始运行
    onComboBoxIndexChanged();
    if(inputstatus == 1 || inputstatus == 2)
    {
        for(QCheckBox* checkedCheckBox : checkedCheckBoxList)
        {
            QVector<QString> xdata,ydata;
            // "飞行时间" 列
            xdata = getColumnDataByColumnName(timeColumnName);
            // 当前选择列
            ydata = getColumnDataByColumnName(checkedCheckBox->text());
            QCustomPlot* customPlot = findChild<QCustomPlot*>(QString("customPlot_") + checkedCheckBox->text());
            if (customPlot)
            {
                // 准备绘图数据
                QVector<double> x, y;
                double epsilon=0.1; //误差小于0.1说明0.1秒之内的数据，直接取平均

                //数据处理
                ProcessData::processData(xdata, ydata, epsilon, x, y);

                // 创建折线图并设置数据:
                customPlot->addGraph();
                customPlot->graph(0)->setData(x, y);

                // 设置坐标轴标签:
                customPlot->xAxis->setLabel("Time");
                customPlot->yAxis->setLabel(ydata[0]);

                // 设置坐标轴范围:
                //显示x坐标
                customPlot->xAxis->setTickLabels(true); // 设置刻度标签可见
                customPlot->xAxis->setRange(*std::min_element(x.constBegin(), x.constEnd()), *std::max_element(x.constBegin(), x.constEnd())); // 使用 x 向量中的最小和最大值作为范围
                customPlot->yAxis->setRange(*std::min_element(y.constBegin(), y.constEnd()), *std::max_element(y.constBegin(), y.constEnd())); // 使用 y 向量中的最小和最大值作为范围

                // 重新绘制折线图:
                // 设置一些基本的customPlot配置：
                customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                customPlot->axisRect()->setupFullAxesBox();
                customPlot->rescaleAxes();
                customPlot->replot();
            }
        }
    }
    else if(inputstatus == 4)
    {
        for(QCheckBox* checkedCheckBox : checkedCheckBoxList)
        {
            QVector<double> x = handler.getDataByColumn("飞行时间");
            QVector<double> y = handler.getDataByColumn(checkedCheckBox->text());

            QCustomPlot* customPlot = findChild<QCustomPlot*>(QString("customPlot_") + checkedCheckBox->text());
            if (customPlot)
            {
                // 创建折线图并设置数据:
                customPlot->addGraph();
                customPlot->graph(0)->setData(x, y);

                // 设置坐标轴标签:
                customPlot->xAxis->setLabel("Time");
                customPlot->yAxis->setLabel(checkedCheckBox->text());

                // 设置坐标轴范围:
                //显示x坐标
                customPlot->xAxis->setTickLabels(true); // 设置刻度标签可见
                customPlot->xAxis->setRange(*std::min_element(x.constBegin(), x.constEnd()), *std::max_element(x.constBegin(), x.constEnd())); // 使用 x 向量中的最小和最大值作为范围
                customPlot->yAxis->setRange(*std::min_element(y.constBegin(), y.constEnd()), *std::max_element(y.constBegin(), y.constEnd())); // 使用 y 向量中的最小和最大值作为范围

                // 重新绘制折线图:
                // 设置一些基本的customPlot配置：
                customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                customPlot->axisRect()->setupFullAxesBox();
                customPlot->rescaleAxes();
                customPlot->replot();
            }
        }
    }
    else
    {
        DEBUG_LOG("onRestartButtonClicked no inputstatus == 3");
    }
}


QCustomPlot *MainWindow::createLineChart(const QString& yText)
{
    QCustomPlot* customPlot = new QCustomPlot(this);
    if(inputstatus == 1 || inputstatus ==2)
    {
        QVector<QString> xdata, ydata;
        xdata = getColumnDataByColumnName(timeColumnName);
        // 指定列
        ydata = getColumnDataByColumnName(yText);
        if (customPlot)
        {

            QVector<double> x, y;
            double epsilon = 0.1; //误差小于0.1说明0.1秒之内的数据，直接取平均

            //数据处理
            ProcessData::processData(xdata, ydata, epsilon, x, y);

            // 创建折线图并设置数据:
            customPlot->addGraph();
            customPlot->graph(0)->setData(x, y);

            // 设置坐标轴标签:
            customPlot->xAxis->setLabel("Time");
            customPlot->yAxis->setLabel(ydata[0]);

            // 设置坐标轴范围:
            //显示x坐标
            customPlot->xAxis->setTickLabels(true); // 设置刻度标签可见
            customPlot->xAxis->setRange(*std::min_element(x.constBegin(), x.constEnd()), *std::max_element(x.constBegin(), x.constEnd())); // 使用 x 向量中的最小和最大值作为范围
            customPlot->yAxis->setRange(*std::min_element(y.constBegin(), y.constEnd()), *std::max_element(y.constBegin(), y.constEnd())); // 使用 y 向量中的最小和最大值作为范围

            // 重新绘制折线图:
            // 设置一些基本的customPlot配置：
            customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
            customPlot->axisRect()->setupFullAxesBox();
            customPlot->rescaleAxes();
            customPlot->replot();
        }
    }
    else if(inputstatus == 4)
    {
        QVector<double> x = handler.getDataByColumn("飞行时间");
        QVector<double> y = handler.getDataByColumn(yText);
        if (customPlot)
        {
            // 准备绘图数据:
            ui->statusbar->showMessage("Line Y-axis: " + yText);

            // 创建折线图并设置数据:
            customPlot->addGraph();
            customPlot->graph(0)->setData(x, y);

            // 设置坐标轴标签:
            customPlot->xAxis->setLabel("Time");
            customPlot->yAxis->setLabel(yText);

            // 设置坐标轴范围:
            //显示x坐标
            customPlot->xAxis->setTickLabels(true); // 设置刻度标签可见
            customPlot->xAxis->setRange(*std::min_element(x.constBegin(), x.constEnd()), *std::max_element(x.constBegin(), x.constEnd())); // 使用 x 向量中的最小和最大值作为范围
            customPlot->yAxis->setRange(*std::min_element(y.constBegin(), y.constEnd()), *std::max_element(y.constBegin(), y.constEnd())); // 使用 y 向量中的最小和最大值作为范围

            // 重新绘制折线图:
            // 设置一些基本的customPlot配置：
            customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
            customPlot->axisRect()->setupFullAxesBox();
            customPlot->rescaleAxes();
            customPlot->replot();
        }
    }

    QString objectName = QString("customPlot_%1").arg(yText);
    customPlot->setObjectName(objectName);
    customPlot->setCursor(Qt::ArrowCursor);
    customPlot->setMouseTracking(true);
    customPlot->installEventFilter(this);

    customPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    customPlot->setMinimumHeight(400); // 设置最小高度为400个像素点

    return customPlot;
}


void MainWindow::drawTablePlot(const QVector<QVector<QString>>& Data)
{
    if (!ui->tableWidget || !ui->comboBox) {
        qWarning("TableWidget or ComboBox is null!");
        return;
    }

    // 检查数据是否为空
    if (Data.isEmpty()) {
        qWarning("Data is empty!");
        return;
    }
    // 清除现有内容
    ui->tableWidget->clear();

    // 设置表格的默认字体
    QFont tableFont;
    tableFont.setPointSize(13); // 设置字体大小为13
    ui->tableWidget->setFont(tableFont);    // 为表格单元格设置字体
    ui->tableWidget->horizontalHeader()->setFont(tableFont);    // 为水平表头设置字体
    ui->tableWidget->verticalHeader()->setFont(tableFont);  // 为垂直表头（行号）设置字体

    // 设置表格的行数和列数
    int rowCount = Data[0].size() - 1; // 假设第一行是表头，不计入数据行, 全部数据是 Data[0].size() - 1
    int colCount = Data.size();

    // 创建一个二维数组来存储所有的 QTableWidgetItem
    QVector<QVector<QTableWidgetItem*>> items(rowCount, QVector<QTableWidgetItem*>(colCount));

    // 提前准备好所有的 QTableWidgetItem
    for (int col = 0; col < colCount; ++col) {
        for (int row = 1; row <= rowCount; ++row) { // 从第二行开始填充数据
            QString cellData = Data[col][row];
            items[row - 1][col] = new QTableWidgetItem(cellData);
        }
    }

    // 暂时禁用信号，避免每次 setItem 触发信号
    ui->tableWidget->blockSignals(true);

    // 设置表格的行数和列数
    ui->tableWidget->setRowCount(rowCount);
    ui->tableWidget->setColumnCount(colCount);

    // 设置表头（可选）
    QStringList headers;
    for (int i = 0; i < colCount; ++i) {
        headers << Data[i][0]; // 假设每一列的第一项是表头
    }
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 一次性将所有 QTableWidgetItem 设置到表格中
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            ui->tableWidget->setItem(row, col, items[row][col]);
        }
    }

    // 恢复信号
    ui->tableWidget->blockSignals(false);

}

void MainWindow::drawThreeD(const QVector<QString>& timeData)
{
    // === 关键：清理 pageThreeD 的旧内容 ===
    QLayout* oldLayout = ui->pageThreeD->layout();
    if (oldLayout) {
        // 删除布局中的所有子控件
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0))) {
            if (item->widget()) {
                delete item->widget();  // 删除 widget（如 createWindowContainer 创建的容器）
            }
            delete item;  // 删除 layout item
        }
        delete oldLayout;
    }

    // 现在 ui->pageThreeD 没有布局了，可以重新创建

    // 获取列名列表
    QComboBox *comboBox = ui->comboBox;
    QStringList columnNames;
    for (int i = 0; i < comboBox->count(); ++i) {
        columnNames << comboBox->itemText(i);
    }

    // 设置坐标轴对应字段名
    QString Latitude, Altitude, Longitude;
    if (columnNames.contains("俯仰角")) {
        Latitude  = "BD2/GPS2纬度";
        Altitude  = "BD2/GPS2高度";
        Longitude = "BD2/GPS2经度";
    }
    else if (columnNames.contains("pitchAngle")) {
        Latitude  = "bd2Gps2Latitude";
        Altitude  = "bd2Gps2Altitude";
        Longitude = "bd2Gps2Longitude";
    }
    else if (columnNames.contains("FYJ")) {
        Latitude  = "GPS2WD";
        Altitude  = "GPS2GD";
        Longitude = "GPS2JD";
    }else {
        return; // 没有匹配字段，不绘制
    }

    // 获取数据
    QVector<QString> Longitudedata = getColumnDataByColumnName(Latitude);
    QVector<QString> Latitudedata = getColumnDataByColumnName(Altitude);
    QVector<QString> Altitudedata = getColumnDataByColumnName(Longitude);

    QVector<double> t;
    QVector<float> x, y, z;
    double epsilon = 20;

    ProcessData::processThreeData(timeData, Longitudedata, Latitudedata, Altitudedata, epsilon, t, x, y, z);

    // 创建 3D 散点图
    QtDataVisualization::Q3DScatter *graph = new QtDataVisualization::Q3DScatter();
    graph->setTitle("3D Scatter Chart Example");
    graph->axisX()->setTitle("Latitude");
    graph->axisY()->setTitle("Altitude");
    graph->axisZ()->setTitle("Longitude");
    graph->axisX()->setTitleVisible(true);
    graph->axisY()->setTitleVisible(true);
    graph->axisZ()->setTitleVisible(true);

    // 准备数据
    QtDataVisualization::QScatterDataArray data;
    if (!x.isEmpty()) {
        for (int i = 0; i < x.size(); ++i) {
            data << QVector3D(x[i], y[i], z[i]);
        }
    }

//     设置数据代理和系列
    QtDataVisualization::QScatterDataProxy *proxy = new QtDataVisualization::QScatterDataProxy();
    proxy->addItems(data);

    QtDataVisualization::QScatter3DSeries *series = new QtDataVisualization::QScatter3DSeries(proxy);
    series->setItemSize(0.1f);
    graph->addSeries(series);

//    // === 准备数据并设置颜色渐变 ===
//    QtDataVisualization::QScatterDataArray data;
//    QVector<QColor> colorList;  // 每个点的颜色

//    if (!x.isEmpty()) {
//        QColor startColor(0, 0, 255);   // 蓝色 (起点)
//        QColor endColor(255, 0, 0);     // 红色 (终点)

//        for (int i = 0; i < x.size(); ++i) {
//            double ratio = double(i) / double(x.size() - 1); // 用 double，避免精度提升警告
//            QColor color = QColor::fromRgbF(
//                startColor.redF()   + ratio * (endColor.redF()   - startColor.redF()),
//                startColor.greenF() + ratio * (endColor.greenF() - startColor.greenF()),
//                startColor.blueF()  + ratio * (endColor.blueF()  - startColor.blueF())
//            );

//            // 明确使用 QScatterDataItem 包装位置
//            data << QtDataVisualization::QScatterDataItem(QVector3D(x[i], y[i], z[i]));
//            colorList.append(color);
//        }
//    }

//    // === 固定颜色数量实现整体渐变 ===
//    int totalPoints = data.size();
//    if (totalPoints == 0) return;
//    const int colorSteps = qMin(20, totalPoints); // 防止 series 数量爆炸

//    QColor startColor(0, 0, 255);
//    QColor endColor(255, 0, 0);

//    // 生成颜色列表
//    QVector<QColor> fixedColors;
//    for (int i = 0; i < colorSteps; ++i) {
//        double ratio = double(i) / double(colorSteps - 1);
//        QColor color = QColor::fromRgbF(
//            startColor.redF()   + ratio * (endColor.redF()   - startColor.redF()),
//            startColor.greenF() + ratio * (endColor.greenF() - startColor.greenF()),
//            startColor.blueF()  + ratio * (endColor.blueF()  - startColor.blueF())
//        );
//        fixedColors.append(color);
//    }

//    // 每段的点数（至少 1）
//    int pointsPerColor = qMax(1, totalPoints / colorSteps);

//    // === 生成多个小 series，每个颜色一段 ===
//    for (int c = 0; c < colorSteps; ++c) {
//        int startIdx = c * pointsPerColor;
//        int endIdx = (c == colorSteps - 1) ? totalPoints : (c + 1) * pointsPerColor;
//        if (startIdx >= totalPoints) break;

//        QtDataVisualization::QScatterDataArray segmentData;
//        for (int i = startIdx; i < endIdx && i < totalPoints; ++i) {
//            // 获取原始点（位置）
//            QtDataVisualization::QScatterDataItem item(data[i].position());
//            item.setColor(fixedColors[c]);  // ⭐⭐⭐ 必须手动设置颜色
//            segmentData << item;
//        }

//        // 创建代理并添加数据
//        QtDataVisualization::QScatterDataProxy *segmentProxy = new QtDataVisualization::QScatterDataProxy();
//        segmentProxy->addItems(segmentData);

//        // 创建系列
//        QtDataVisualization::QScatter3DSeries *segmentSeries = new QtDataVisualization::QScatter3DSeries(segmentProxy);
//        segmentSeries->setItemSize(0.08f);
//        // setBaseColor 已被覆盖，可省略
//        graph->addSeries(segmentSeries);
//    }

    // 调整坐标轴范围
    if (!x.isEmpty()) {
        graph->axisX()->setRange(*std::min_element(x.constBegin(), x.constEnd()),
                                 *std::max_element(x.constBegin(), x.constEnd()));
        graph->axisY()->setRange(*std::min_element(y.constBegin(), y.constEnd()),
                                 *std::max_element(y.constBegin(), y.constEnd()));
        graph->axisZ()->setRange(*std::min_element(z.constBegin(), z.constEnd()),
                                 *std::max_element(z.constBegin(), z.constEnd()));
    }

    // 创建窗口容器并添加到布局
    QWidget *container = QWidget::createWindowContainer(graph);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(container);
    ui->pageThreeD->setLayout(layout);
}

//void MainWindow::drawThreeD()
//{
//    // 创建一个 Q3DScatter 实例
//    QtDataVisualization::Q3DScatter *graph = new QtDataVisualization::Q3DScatter();

//    // 设置图表的标题
//    graph->setTitle("3D Scatter Chart Example");

//    // 设置 x、y、z 轴的标签
//    graph->axisX()->setTitle("Latitude");
//    graph->axisY()->setTitle("Altitude");
//    graph->axisZ()->setTitle("Longitude");
//    graph->axisX()->setTitleVisible(true);
//    graph->axisY()->setTitleVisible(true);
//    graph->axisZ()->setTitleVisible(true);

//    // 获取左侧选择框文本列表
//    QComboBox *comboBox = ui->comboBox;
//    QStringList columnNames;
//    for (int i = 0; i < comboBox->count(); ++i) {
//        columnNames << comboBox->itemText(i);
//    }

//    // 创建四个QString,如果有中文列名就设置为中文,否则设置为英文
//    QString Latitude,Altitude,Longitude;
//    if(columnNames.contains("俯仰角"))
//    {
//        Latitude = "BD2/GPS2纬度";
//        Altitude = "BD2/GPS2高度";
//        Longitude = "BD2/GPS2经度";
//    }
//    else if(columnNames.contains("pitchAngle")){
//        Latitude = "bd2Gps2Latitude";
//        Altitude = "bd2Gps2Altitude";
//        Longitude = "bd2Gps2Longitude";
//    }

//    // 创建散点图数据
//    // 准备绘图数据:
//    QVector<QString> time = getColumnDataByColumnName("飞行时间");
//    QVector<QString> Longitudedata = getColumnDataByColumnName(Latitude);
//    QVector<QString> Latitudedata = getColumnDataByColumnName(Altitude);
//    QVector<QString> Altitudedata = getColumnDataByColumnName( Longitude);
//    QVector<double> t;
//    QVector<float> x, y, z;
//    double epsilon = 20;

//    //数据处理
//    ProcessData::processThreeData(time,Longitudedata ,Latitudedata, Altitudedata,epsilon,t,x,y,z);

//    QtDataVisualization::QScatterDataArray data;

//    DEBUG_LOG("x.size():"<<x.size());
//    if(x.size()!=0)
//    {
//        for (int i = 0; i < x.size(); i++) {
//            QVector3D point(x[i], y[i], z[i]);
//            //             DEBUG_LOG(point);
//            data << point;
//        }
//    }
//    //    for (int i = 0; i < 10; i++) {
//    //        QVector3D point(i, i, i);
//    //         DEBUG_LOG(point);
//    //        data << point;
//    //    }
//    // 创建数据代理，并将数据添加到代理中
//    QtDataVisualization::QScatterDataProxy *proxy = new QtDataVisualization::QScatterDataProxy();
//    proxy->addItems(data);

//    // 创建系列，并将数据代理添加到系列中
//    QtDataVisualization::QScatter3DSeries *series = new QtDataVisualization::QScatter3DSeries(proxy);
//    series->setItemSize(0.1f);

//    // 将系列添加到图表中
//    graph->addSeries(series);

//    // 调整轴的范围以适应数据
//    DEBUG_LOG("x max:"<<*std::min_element(x.constBegin(), x.constEnd())<<"x min:"<<*std::max_element(x.constBegin(), x.constEnd()));
//    graph->axisX()->setRange(*std::min_element(x.constBegin(), x.constEnd()), *std::max_element(x.constBegin(), x.constEnd()));
//    graph->axisY()->setRange(*std::min_element(y.constBegin(), y.constEnd()), *std::max_element(y.constBegin(), y.constEnd()));
//    graph->axisZ()->setRange(*std::min_element(z.constBegin(), z.constEnd()), *std::max_element(z.constBegin(), z.constEnd()));


//    // 将图表添加到 ui->pageThreeD 中
//    QHBoxLayout *layout = new QHBoxLayout(ui->pageThreeD);
//    layout->addWidget(QWidget::createWindowContainer(graph));
//}

/**
 * @brief MainWindow::initialPlot
 * 初始化所有图
 */
void MainWindow::initialPlot(const QStringList& columnNames)
{
    if ((inputstatus == 1) || (inputstatus == 2))
    {
        QVector<QString> xdata, ydata;

        // --- 自动匹配“飞行时间”列名 ---
        QStringList timeColumnCandidates = {
            "飞行时间", "FXSJ", "FlightTime", "flightTime",
            "time", "Time", "timestamp", "Timestamp"
        };

        for (const QString &candidate : timeColumnCandidates)
        {

            if (columnNames.contains(candidate))
            {
                timeColumnName = candidate;
                break;
            }
        }

        if (timeColumnName.isEmpty())
        {
            qWarning() << "未找到‘飞行时间’相关列，使用第 0 列作为横轴。";
            if (!Data.isEmpty() && !Data[0].isEmpty())
            {
                xdata = Data[0];  // fallback
            }
        }
        else
        {
            xdata = getColumnDataByColumnName(timeColumnName);
            DEBUG_LOG(" 使用列名：" << timeColumnName << " 作为“飞行时间”横轴。") ;
        }

        // --- 默认展示第 1 列或第 0 列数据 ---
        if (Data.size() > 1)
            ydata = getColumnDataByColumnIndex(1);
        else if (Data.size() > 0)
            ydata = getColumnDataByColumnIndex(0);

        DEBUG_LOG("开始绘图。。。") ;
        // --- 调用绘图函数 ---
        drawTablePlot(Data);
        DEBUG_LOG( "drawTablePlot(Data) 完成");
        DrawPlot::drawScatterPlot(ui->pageScatter, xdata, ydata);
        DEBUG_LOG("DrawPlot::drawScatterPlot(ui->pageScatter, xdata, ydata) 完成") ;
        DrawPlot::drawSingleLineChart(ui->single_line, xdata, ydata);
        DEBUG_LOG("drawSingleLineChart(ui->single_line, xdata, ydata) 完成") ;
        drawPiePlot(columnNames);
        DEBUG_LOG( "drawPiePlot() 完成");
        DrawPlot::drawBoxPlot(ui->pageBox, xdata, ydata, ui->comboBox->currentText());
        DEBUG_LOG("DrawPlot::drawBoxPlot(ui->pageBox, xdata, ydata, ui->comboBox->currentText()) 完成");
        drawThreeD(xdata);
        DEBUG_LOG("drawThreeD() 完成") ;
    }
    else if (inputstatus == 4)
    {
        QString columnNamex = "飞行时间";
        QString columnNamey = "俯仰角";

        QVector<double> xdata = handler.getDataByColumn(columnNamex);
        QVector<double> ydata = handler.getDataByColumn(columnNamey);

        drawTablePlotByDB();
        DrawPlot::drawScatterPlotByDB(ui->pageScatter, xdata, ydata, columnNamex, columnNamey);
        DrawPlot::drawSingleLineChartByDB(ui->single_line, xdata, ydata, columnNamex, columnNamey);
        drawPiePlotByDB();
        DrawPlot::drawBoxPlotByDB(ui->pageBox, ydata, ui->comboBox->currentText());
        drawThreeDByDB();
    }
    else
    {
        DEBUG_LOG( "initialPlot: inputstatus != 1/2/4");
    }
}


/**
 * @brief MainWindow::drawPiePlot 绘制饼状图
 * @param xdata
 * @param ydata
 */
void MainWindow::drawPiePlot(const QStringList& columnNames)
{
    // === 关键：清理 pagePie 的旧内容 ===
    QLayout* oldLayout = ui->pagePie->layout();
    if (oldLayout) {
        // 删除布局中的所有子控件
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0))) {
            if (item->widget()) {
                delete item->widget();  // 删除 widget（如 QChartView）
            }
            delete item;  // 删除 layout item
        }
        delete oldLayout;
    }

    // 现在 pagePie 没有布局了，可以重新创建

    // 创建四个QString,如果有中文列名就设置为中文,否则设置为英文
    QString chartViewName1, chartViewName2, chartViewName3, chartViewName4;
    if (columnNames.contains("安控状态")) {
        chartViewName1 = "安控信号";
        chartViewName2 = "磁罗盘状态";
        chartViewName3 = "油门档位";
        chartViewName4 = "大气机状态";
    }
    else if (columnNames.contains("safetyControlStatus")) {
        chartViewName1 = "safetyControlSignal";
        chartViewName2 = "magneticCompassStatus";
        chartViewName3 = "throttlePosition";
        chartViewName4 = "atmosphericCondition";
    }else if (columnNames.contains("AKZT")) {
        chartViewName1 = "AKXH";
        chartViewName2 = "CLPZT";
        chartViewName3 = "YMDW";
        chartViewName4 = "DQJZT";
    } else {
        // 可选：处理不匹配的情况
        return; // 或设置默认值
    }

    // 创建四个饼状图系列（略，保持不变）
    QPieSeries *series1 = new QPieSeries();
    QMap<QString, int> aggregateddata1 = classifyDataByColumnName(chartViewName1);
    for (auto it = aggregateddata1.begin(); it != aggregateddata1.end(); ++it) {
        series1->append(it.key(), it.value());
    }

    QPieSeries *series2 = new QPieSeries();
    QMap<QString, int> aggregateddata2 = classifyDataByColumnName(chartViewName2);
    for (auto it = aggregateddata2.begin(); it != aggregateddata2.end(); ++it) {
        series2->append(it.key(), it.value());
    }

    QPieSeries *series3 = new QPieSeries();
    QMap<QString, int> aggregateddata3 = classifyDataByColumnName(chartViewName3);
    for (auto it = aggregateddata3.begin(); it != aggregateddata3.end(); ++it) {
        series3->append(it.key(), it.value());
    }

    QPieSeries *series4 = new QPieSeries();
    QMap<QString, int> aggregateddata4 = classifyDataByColumnName(chartViewName4);
    for (auto it = aggregateddata4.begin(); it != aggregateddata4.end(); ++it) {
        series4->append(it.key(), it.value());
    }

    // 创建四个饼状图视图
    QChartView *chartView1 = createChartView(series1, chartViewName1);
    QChartView *chartView2 = createChartView(series2, chartViewName2);
    QChartView *chartView3 = createChartView(series3, chartViewName3);
    QChartView *chartView4 = createChartView(series4, chartViewName4);

    chartView1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 创建新布局
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(chartView1, 0, 0);
    layout->addWidget(chartView2, 0, 1);
    layout->addWidget(chartView3, 1, 0);
    layout->addWidget(chartView4, 1, 1);

    // 设置新布局
    ui->pagePie->setLayout(layout);
}


void MainWindow::drawAnomalyDetection()
{
    // 文件地址
    QString filePath = ":/anomaly";

    // 检查是否选择了文件
    if (!filePath.isEmpty())
    {
        QFile csvFile(filePath);
        DEBUG_LOG(filePath<< "文件不为空" <<endl) ;
        if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            DEBUG_LOG("无法打开 CSV 文件：" << csvFile.errorString()) ;
            return;
        }

        QTextStream in(&csvFile);
        QString line;

        // 获取第一行作为列名
        QStringList columnNames;
        if (in.readLineInto(&line))
        {
            columnNames = line.split(',', QString::SkipEmptyParts);
        }
        else
        {
            DEBUG_LOG("CSV 文件为空或无法读取列名") ;
            csvFile.close();
            return;
        }

        // 输出列名并设置选择框中的选项
        DEBUG_LOG("CSV 列名:") ;
        for (const QString& columnName : columnNames)
        {
            DEBUG_LOG(columnName) ;
        }

        int scoreIndex = columnNames.indexOf("scores");
        int labelIndex = columnNames.indexOf("labels");
        int predIndex = columnNames.indexOf("preds");
        int thresholdIndex = columnNames.indexOf("threshold");
        DEBUG_LOG(scoreIndex);//11
        DEBUG_LOG(labelIndex);//12
        DEBUG_LOG(predIndex);//13
        DEBUG_LOG(thresholdIndex);//14

        //保存列名为scores的列的数据到QVector<double> scores,保存列名为labels的列的数据到QVector<double> labels
        QVector<double> scores,x;
        QVector<double> labels; // 假设labels也是数值类型，若非数值则需处理为逻辑判断条件
        QVector<int> preds;
        QVector<double> threshold;

        int lineNum = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList fields = line.split(',', QString::SkipEmptyParts);
            //             DEBUG_LOG(lineNum<<":"<<fields);

            // 确保当前行有足够多的列
            if (fields.size() >= columnNames.size()) {

                if (scoreIndex != -1 && labelIndex != -1 && predIndex != -1 && thresholdIndex != -1) {
                    bool okScores, okLabels,okpred,okthre;
                    double scoreValue = fields[scoreIndex+1].toDouble(&okScores);// 错1位的，+1
                    double labelValue = fields[labelIndex+1].toDouble(&okLabels); // 假定labels是整数类型的标签
                    int predValue = fields[predIndex+1].toInt(&okpred); // 假定labels是整数类型的标签
                    double thresholdValue = fields[thresholdIndex+1].toDouble(&okthre); // 假定labels是整数类型的标签


                    if (okScores) {
                        x.append(++lineNum);
                        scores.append(scoreValue);
                        labels.append(labelValue); // 将labels也存储起来，虽然本例未直接用于绘图
                        preds.append(predValue);
                        threshold.append(thresholdValue);
                        //                        DEBUG_LOG(lineNum<<":"<<scoreValue<<","<<labelValue<<","<<thresholdValue);
                    } else {
                        DEBUG_LOG("Error converting score value to double.") ;
                    }
                } else {
                    DEBUG_LOG("Column 'scores' or 'labels' not found.") ;
                }
            }
        }
        // 添加散点图数据
        QCPScatterStyle scatterRed(QCPScatterStyle::ssDisc, 3); // 红色圆点
        QPen penRed(Qt::darkRed);
        penRed.setWidthF(1.5); // 设置线宽，可选
        scatterRed.setPen(penRed);
        scatterRed.setBrush(Qt::red);

        QCPScatterStyle scatterBlue(QCPScatterStyle::ssDisc, 2); // 蓝色圆点
        QPen penBlue(Qt::blue);
        penBlue.setWidthF(1.5); // 设置线宽，可选
        scatterBlue.setPen(penBlue);
        scatterBlue.setBrush(Qt::blue);


        // 绘制折线图
        QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(ui->pageAd);

        customPlot->addGraph();
        customPlot->addGraph();
        customPlot->addGraph();
        customPlot->addGraph();

        customPlot->graph(0)->setScatterStyle(scatterBlue);
        customPlot->graph(0)->setName("Score");
        tracer->setGraph(ui->pageAd->graph(0));// 设置游标吸附在ui->pageAd->graph(0)这条曲线上
        customPlot->graph(1)->setScatterStyle(scatterRed);
        customPlot->graph(1)->setLineStyle((QCPGraph::lsNone));//阶梯线，左对齐
        customPlot->graph(1)->setName("True Anomaly");

        customPlot->graph(2)->setData(x, threshold);
        QPen pen3;
        pen3.setWidth(3);//线宽
        pen3.setStyle(Qt::DotLine);//虚线
        pen3.setColor(Qt::darkYellow);//黄色
        customPlot->graph(2)->setPen(pen3);
        customPlot->graph(2)->setName("Threshold");

        customPlot->graph(3)->setName("Anomaly Detected");
        QColor customcol("#ffe5d1");
        customPlot->graph(3)->setBrush(customcol);
        customPlot->graph(3)->setLineStyle((QCPGraph::lsNone));

        std::vector<double> anomalyStartXs, anomalyEndXs; // 用于存储异常点的x轴起始和结束坐标
        for(int i=0;i<preds.size();i++)
        {
            if(preds[i] == 1)
            {
                if(i==0)//从0开始
                {
                    anomalyStartXs.push_back(i+1);
                    DEBUG_LOG("anomalyStartXs:"<<i);

                }
                if(i!=0&&i!=preds.size()-1)
                {
                    if((preds[i-1]^preds[i])==1 ) //开始
                    {
                        anomalyStartXs.push_back(i+1);
                        DEBUG_LOG("anomalyStartXs:"<<i);
                    }
                    if((preds[i]^preds[i+1])==1) //结束
                    {
                        anomalyEndXs.push_back(i+1);
                        //                         DEBUG_LOG("anomalyEndXs:"<<i);
                    }
                }
                if(i==preds.size()-1)//最后一个结束
                {
                    anomalyEndXs.push_back(i+1);
                    //                      DEBUG_LOG("anomalyEndXs:"<<i);
                }
            }
        }

        // 准备数据和颜色列表
        std::vector<std::tuple<double, double, int>> dataColors; // 存储(x, score, label)

        for (int i = 0; i < scores.size(); ++i) {
            dataColors.emplace_back(x[i], scores[i], labels[i]);
        }

        int dataColorsIndex = 0;
        for (const auto& tupleEntry : dataColors) {
            double xVal, scoreVal;
            int label;
            std::tie(xVal, scoreVal, label) = tupleEntry;

            customPlot->graph(0)->addData(xVal, scoreVal);
            if(label == 1) {
                customPlot->graph(1)->addData(xVal, scoreVal);
            }
            dataColorsIndex++;
        }

        QString objectName = QString("AnomalyDetectionPlot");
        customPlot->setObjectName(objectName);
        customPlot->setCursor(Qt::ArrowCursor);
        customPlot->setMouseTracking(true);
        customPlot->installEventFilter(this);

        // 设置轴标签
        customPlot->xAxis->setLabel("Time");
        customPlot->yAxis->setLabel("Score");

        // 自动调整坐标轴范围
        customPlot->xAxis->setRange(*std::min_element(x.constBegin(), x.constEnd()), *std::max_element(x.constBegin(), x.constEnd())); // 使用 x 向量中的最小和最大值作为范围
        customPlot->yAxis->setRange(*std::min_element(threshold.constBegin(), threshold.constEnd()), *std::max_element(threshold.constBegin(), threshold.constEnd())); // 使用 threshold 向量中的最小和最大值作为范围
        // 设置一些基本的customPlot配置：
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        customPlot->axisRect()->setupFullAxesBox();
        customPlot->rescaleAxes();

        // 绘制灰色背景矩形覆盖所有异常点的x轴区间
        for (size_t i = 0; i < anomalyStartXs.size(); ++i) {
            QCPItemRect *grayBackground = new QCPItemRect(customPlot);
            QColor customRed("#ffe5d1");
            grayBackground->setBrush(customRed);
            grayBackground->setPen(Qt::NoPen);
            grayBackground->topLeft->setType(QCPItemPosition::ptPlotCoords);
            grayBackground->bottomRight->setType(QCPItemPosition::ptPlotCoords);


            //X轴范围 起始点anomalyStartXs[i]到结束点anomalyEndXs[i]
            //y轴范围是customPloty轴最小值到customPloty轴最大值
            // 设置x轴范围
            // 注意：确保anomalyStartXs和anomalyEndXs的长度匹配且逻辑正确
            grayBackground->topLeft->setCoords(anomalyStartXs[i], customPlot->yAxis->range().lower);
            grayBackground->bottomRight->setCoords(anomalyEndXs[i], customPlot->yAxis->range().upper);

            // 添加到图中并设置图层
            grayBackground->setLayer(customPlot->layer("grid")); // 或选择适合的层
        }

        customPlot->legend->setVisible(true);
        customPlot->legend->setFont(QFont("Helvetica", 9));
        // 重绘以显示图例
        customPlot->replot();

        csvFile.close();

    }
}

void MainWindow::drawLargeLine()
{
    ui->label->hide();
    QElapsedTimer timer; // 创建一个计时器实例

    // 文件地址
    QString filePath = ":/td100000";  // 确保这个路径是正确的

    // 检查是否选择了文件
    if (!filePath.isEmpty())
    {
        QFile csvFile(filePath);
        DEBUG_LOG(filePath << "文件不为空" << endl) ;
        if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            DEBUG_LOG("无法打开 CSV 文件：" << csvFile.errorString()) ;
            return;
        }

        QVector<double> timeSeconds; // 存储时间（作为自 Unix 纪元以来的秒数）
        QVector<double> value; // 只保留value向量
        bool firstLine = true; // 标记是否为第一行

        QTextStream in(&csvFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();  // 逐行读取文件内容
            if (firstLine) { // 跳过第一行
                firstLine = false;
                continue;
            }

            QStringList fields = line.split(",");  // 假设数据是以逗号分隔的

            // 确保每行有至少两个字段：timestamp 和 value
            if (fields.size() >= 2)
            {
                QDateTime timestamp = QDateTime::fromString(fields[0], "yyyy-MM-dd hh:mm:ss");
                bool ok;
                double val = fields[1].toDouble(&ok); // 直接获取第二个字段作为value

                if (ok && timestamp.isValid())  // 如果转换成功且时间戳有效
                {
                    // 将 QDateTime 转换为从 Unix 纪元以来的秒数
                    double secondsSinceEpoch = timestamp.toSecsSinceEpoch();
                    timeSeconds.append(secondsSinceEpoch);
                    value.append(val);
                }
            }
        }

        csvFile.close();

        timer.start(); // 开始计时
        //减少绘图数据到800个点

        // 间隔均匀采样
        //         DEBUG_LOG("间隔均匀采样") ;
        //        auto sampledData = ProcessData::intervalSample(timeSeconds, value, 1800);
        //        timeSeconds = sampledData.first;
        //        value = sampledData.second;
        // 间隔平均采样
        //         DEBUG_LOG("间隔平均采样") ;
        //        auto sampledData = ProcessData::intervalAverageSample(timeSeconds, value, 1800);
        //        timeSeconds = sampledData.first;
        //        value = sampledData.second;
        // 最大最小采样
        //         DEBUG_LOG("最大最小采样") ;
        //        auto sampledData = ProcessData::maxMinSample(timeSeconds, value, 1800);
        //        timeSeconds = sampledData.first;
        //        value = sampledData.second;
        // M4 聚合 m4Sample
        //         DEBUG_LOG(" M4 聚合 ") ;
        //        auto sampledData = ProcessData::m4Sample(timeSeconds, value, 1800);
        //        timeSeconds = sampledData.first;
        //        value = sampledData.second;


        //         DEBUG_LOG(" 执行时间:" << timer.elapsed() << "毫秒") ;


        //        timer.restart(); // 计时重新开始
        // 创建折线图
        QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(ui->pageLarge);
        if (customPlot)
        {
            // 清除之前的图
            customPlot->clearGraphs();

            // 添加折线图并设置数据:
            customPlot->addGraph();
            customPlot->graph(0)->setData(timeSeconds, value);

            // 设置坐标轴标签:
            customPlot->xAxis->setLabel("Timestamp");
            customPlot->yAxis->setLabel("Value");

            // 使用 QCPAxisTickerDateTime 处理 X 轴的时间戳
            QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
            dateTimeTicker->setDateTimeFormat("yyyy-MM-dd hh:mm:ss"); // 设置你想要的时间格式
            customPlot->xAxis->setTicker(dateTimeTicker);

            // 设置散点图样式
            customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

            // 设置坐标轴范围:
            customPlot->xAxis->setRange(timeSeconds.first(), timeSeconds.last()); // 使用自动生成的时间范围
            customPlot->yAxis->setRange(*std::min_element(value.constBegin(), value.constEnd()), *std::max_element(value.constBegin(), value.constEnd())); // 使用 y 向量中的最小和最大值作为范围

            // 设置一些基本的customPlot配置：
            customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
            customPlot->axisRect()->setupFullAxesBox(); // 四边安装轴并显示
            //            customPlot->setFixedSize(1900, 700);
            customPlot->replot();//刷新图像
        }

        // 计算并输出经过的时间
        DEBUG_LOG(filePath << " 绘制图表 执行时间:" << timer.elapsed() << "毫秒");
    }
}
void MainWindow::updateColumnScrollArea(QScrollArea* scrollArea,
                                        const QVector<QVector<QString>>& data,
                                        const QMap<QString, QColor>& colorMap,
                                        const QString& emptyText)
{
    // 清理旧内容
    QWidget *oldWidget = scrollArea->widget();
    if (oldWidget) oldWidget->deleteLater();

    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(scrollWidget);

    if (!data.isEmpty()) {
        for (int c = 0; c < data.size(); ++c) {
            if (!data[c].isEmpty()) {
                QString columnName = data[c][0];
                QLabel *label = new QLabel(columnName, this);

                // 设置标签颜色
                if (colorMap.contains(columnName)) {
                    QPalette palette = label->palette();
                    palette.setColor(QPalette::WindowText, colorMap[columnName]);
                    label->setPalette(palette);
                }

                layout->addWidget(label);
            }
        }
    } else {
        QLabel *label = new QLabel(emptyText, this);
        layout->addWidget(label);
    }

    scrollWidget->setLayout(layout);
    scrollArea->setWidget(scrollWidget);
}

void MainWindow::loadDatasetAndUpdateLine(const QString &selectedFile)
{
    try {
        if (selectedFile.isEmpty()) return;

        currentFile = selectedFolder + "/" + selectedFile;
        rawData = Util::readCSVWithColumnNames(currentFile);

        // 检查数据是否为空
        if (rawData.isEmpty()) {
            throw std::runtime_error("Loaded dataset is empty");
        }

        // 移除"飞行时间"相关列
        for (int i = 0; i < rawData.size(); ) {
            if (rawData[i].isEmpty()) {
                i++;  // 空列直接跳过
                continue;
            }

            QString columnName = rawData[i][0];
            // 匹配需要移除的列名
            if (columnName == "FXSJ" || columnName == "飞行时间" || columnName == "flightTime") {
                rawData.remove(i);  // 移除后i不变，因为下一个元素会移到当前位置
            } else {
                i++;  // 不匹配则移动到下一列
            }
        }

        // === 1. 创建列名与颜色的映射（使用HSV色轮）===
        columnColors.clear();
        int totalColumns = rawData.size();
        const int HUE_STEP = 40; // 固定步长，保持颜色差异明显
        for (int i = 0; i < totalColumns; ++i) {
            if (!rawData[i].isEmpty()) {
                QString columnName = rawData[i][0];
                // 使用固定步长，循环使用360度色轮
                int hue = (i * HUE_STEP) % 360;
                QColor color;
                color.setHsv(hue, 200, 255); // 保持较高饱和度和明度，颜色更鲜艳

                // 可选优化：如果列数超过9，微调步长避免颜色重复过于明显
                if (totalColumns > 9) {
                    // 增加一个小的随机偏移，避免列数多时颜色循环过于规律
                    hue = (hue + (i * 13) % 20) % 360; // 13是质数，偏移更均匀
                    color.setHsv(hue, 180 + (i % 3) * 25, 255); // 饱和度略有变化
                }

                columnColors[columnName] = color;
                DEBUG_LOG("列名: "<<columnName<< "颜色: "<<color) ;
            }
        }
        ui->horizontalSlider_k->setRange(1,totalColumns);

        // 选择最多5列（如果总列数不足5则选全部）
        int selectCount = qMin(5, totalColumns);
        int initAlpha = 0;
        selectedData = Util::M4GreedySelectColumns(rawData, selectCount, initAlpha);
        //      selectedData = Util::randomSelectColumns(rawData, selectCount);

        updateColumnScrollArea(ui->scrollArea_selected_column, selectedData, columnColors);
        updateColumnScrollArea(ui->scrollArea_original_column, rawData, columnColors);

        // === 绘制图表（使用颜色映射）===
        DrawPlot::drawSelectedLineChartByM4Sample(ui->plot_representative, selectedData, columnColors);
        DrawPlot::drawSelectedLineChart(ui->plot_original, rawData, columnColors);

    } catch (const std::exception& e) {
        DEBUG_LOG("Error in loadDatasetAndUpdateLine:" << e.what()) ;
        throw; // 重新抛出给上层处理
    } catch (...) {
        DEBUG_LOG("Unknown error in loadDatasetAndUpdateLine") ;
        throw std::runtime_error("Unknown error occurred while loading dataset");
    }
}


void MainWindow::initDatasetComboBox()
{
    try {
        selectedFolder = R"(E:\QTProgram\QT-FlyDataVisual_AnomalyDetection\time_series_visualization_01\data\Processed_Flydata)";
        QDir dir(selectedFolder);

        // 检查目录是否存在
        if (!dir.exists()) {
            throw std::runtime_error("Selected folder does not exist: " + selectedFolder.toStdString());
        }

        QStringList csvFiles = dir.entryList(QStringList() << "*.csv", QDir::Files);

        ui->comboBox_datasets->clear();
        ui->comboBox_datasets->addItems(csvFiles);

        if (!csvFiles.isEmpty()) {
            // 默认显示第5个文件
            int defaultIndex = qMin(5, csvFiles.size() - 1);
            ui->comboBox_datasets->setCurrentIndex(defaultIndex);
            loadDatasetAndUpdateLine(csvFiles[defaultIndex]);
        } else {
            qWarning() << "No CSV files found in directory:" << selectedFolder;
            QMessageBox::warning(this, "Warning", "No CSV files found in the selected directory");
        }

    } catch (const std::exception& e) {
        DEBUG_LOG("Error in initDatasetComboBox:" << e.what()) ;
        QMessageBox::critical(this, "Error", QString("Failed to initialize dataset combo box: %1").arg(e.what()));
    } catch (...) {
        DEBUG_LOG("Unknown error in initDatasetComboBox") ;
        QMessageBox::critical(this, "Error", "An unknown error occurred while initializing dataset combo box");
    }
}
//void MainWindow::drawSelectedLine()
//{
//    // 绑定滑杆(ui->horizontalSlider_k、ui->horizontalSlider_alpha)和参数(k、alpha)，这个参数用于文本标签（ui->label_k_num、ui->label_alpha_num）与图像选择线条的数量（选择k个线条、平衡参数alpha）

//    // 设置数据集选择框(ui->comboBox_datasets中设置为E:\QTProgram\QT-FlyDataVisual_AnomalyDetection\time_series_visualization_01\data\Processed_Flydata文件夹下的csv名称供选择)

//    // 选择数据后，根据“文件名”导入数据（局部变量Qvector<Qvector<double>> rawdata）

//    // 使用随机选择数据算法函数 选数据到 selectedata（Qvector<Qvector<double>>）

//    // 将选到的数据的列名放进 ui->scrollArea_selected_column

//    // 绘图到 customPlot = qobject_cast<QCustomPlot*>(ui->plot_representative);和ui->plot_original

//}

QChartView* MainWindow::createChartView(QPieSeries *series, const QString& title)
{
    // 创建饼状图
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(title);

    // 创建饼状图视图
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

QMap<QString, int> MainWindow::classifyDataByColumnName(QString ColumnName)
{
    QVector<QString> xdata;
    xdata = getColumnDataByColumnName(ColumnName,1);//1而不是0，为了不将列名重复计数

    QMap<QString, int> aggregatedData;

    // 将 xdata 中的内容聚合并计数
    for (const QString& item : xdata) {
        aggregatedData[item]++;
    }

    // 输出聚合结果
    //     DEBUG_LOG(endl<<"列名："<<ColumnName);
    //    for (auto it = aggregatedData.begin(); it != aggregatedData.end(); ++it) {
    //         DEBUG_LOG("内容项:" << it.key() << " 数量:" << it.value()) ;
    //    } DEBUG_LOG(endl);
    return aggregatedData;
}


/**
 * @brief MainWindow::getColumnDataByColumnName
 * 根据列名找某一列数据(包括列名)
 * @param ColumnName
 * @return
 */
QVector<QString> MainWindow::getColumnDataByColumnName(QString ColumnName,int row)
{
    QVector<QString> columnData;
    if(!Data.empty())
    {
        // 找到 columnName 对应的列索引
        int columnIndex = -1;
        for (int i = 0; i < Data.size(); ++i) {
            if (Data[i][0] == ColumnName) {
                columnIndex = i;
                break;
            }
        }

        // 如果找到了对应的列索引
        if (columnIndex != -1) {
            // 确保行索引在有效范围内
            if (row >= 0 && row < Data[columnIndex].size()) {
                // 将该列的数据添加到 columnData 向量中
                for (int i = row; i < Data[columnIndex].size(); ++i) {
                    columnData.append(Data[columnIndex][i]);
                }
            }
            else{
                DEBUG_LOG("MainWindow::getColumnDataByColumnName: row is wrong!!");
            }
        }
        return columnData;
    }
    DEBUG_LOG(ColumnName<<" 数据列为空");
    return columnData;  // 返回空的列数据
}
/**
 * @brief MainWindow::getColumnDataByColumnIndex
 * 根据索引找某一列数据(包括列名)
 * @param index
 * @return
 */
QVector<QString> MainWindow::getColumnDataByColumnIndex(int index,int row)
{
    QVector<QString> columnData;

    // 如果给定的列索引有效
    if (index >= 0 && index < Data.size()) {
        // 将该列的数据添加到 columnData 向量中
        for (int i = row; i < Data[index].size(); ++i) {
            columnData.append(Data[index][i]);
        }
    }else{
        DEBUG_LOG("MainWindow::getColumnDataByColumnIndex: row is wrong!!");
    }

    return columnData;
}



/**
 * @brief 绘制多条折线图并显示在滚动区域中。
 * @param checkedCheckBoxList 包含选中的 QCheckBox 指针的 QVector
 *
 * 该函数根据选中的 QCheckBox 列表，为每个选中的项创建折线图，并将它们添加到滚动区域中显示。
 * 每个折线图对应一个选中的 QCheckBox，折线图的数据由所选 QCheckBox 的文本指定。
 */
void MainWindow::drawMultiLineChart(const QVector<QCheckBox*>& checkedCheckBoxList)
{

    // 查找需要添加的 CheckBox
    QVector<const QCheckBox*> newCheckBoxes;  // 修改数据类型为 QVector<const QCheckBox*>
    for (const QCheckBox* checkedCheckBox : checkedCheckBoxList) {
        bool found = false;
        for (const QFrame* draggableFrame : draggableFrames) {
            if (draggableFrame->objectName().endsWith(checkedCheckBox->text())) {
                found = true;
                break;
            }
        }
        if (!found) {
            newCheckBoxes.append(checkedCheckBox);
        }
    }

    // 删除取消选中的 CheckBox 对应的图形
    QVector<const QFrame*> framesToRemove;
    for (const QFrame* draggableFrame : draggableFrames) {
        bool found = false;
        for (const QCheckBox* checkedCheckBox : checkedCheckBoxList) {
            if (draggableFrame->objectName().endsWith(checkedCheckBox->text())) {
                found = true;
                break;
            }
        }
        if (!found) {
            framesToRemove.append(draggableFrame);
        }
    }
    for (const QFrame* frameToRemove : framesToRemove) {
        lineChartLayout->removeWidget(const_cast<QFrame*>(frameToRemove));
        draggableFrames.removeOne(const_cast<QFrame*>(frameToRemove));
        const_cast<QFrame*>(frameToRemove)->deleteLater();
    }

    // 添加新的 CheckBox 对应的图形并重新布局
    for (const QCheckBox* newCheckBox : newCheckBoxes) {
        QFrame* draggableFrame = new QFrame(this);
        QString objectName = QString("draggableFrame_%1").arg(newCheckBox->text());
        draggableFrame->setObjectName(objectName);
        draggableFrame->setCursor(Qt::ArrowCursor);
        draggableFrame->setMouseTracking(true);
        draggableFrame->installEventFilter(this);
        draggableFrame->setStyleSheet(QString("QFrame#%1 {border: 2px solid black;}").arg(objectName));
        draggableFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        draggableFrame->setMinimumHeight(408);

        // 创建 customPlot 和其他部件并设置布局
        QHBoxLayout* draggableLayout = new QHBoxLayout(draggableFrame);
        draggableLayout->setContentsMargins(0, 0, 0, 0);

        draggableLayout->addWidget(createLineChart(newCheckBox->text()));
        draggableFrame->setLayout(draggableLayout);

        lineChartLayout->addWidget(draggableFrame);
        draggableFrames.append(draggableFrame);
    }

    // 更新布局
    ui->scrollArea_line->widget()->setLayout(lineChartLayout);

    //    for (const QFrame* draggableFrame : draggableFrames) {
    //         DEBUG_LOG(draggableFrame->objectName()) ;
    //    }
}


/**
 * @brief MainWindow::drawTablePlotByDB
 * 绘制概况表
 */
void MainWindow::drawTablePlotByDB()
{
    if (!ui->tableWidget || !ui->comboBox) {
        qWarning("TableWidget or ComboBox is null!");
        return;
    }

    // 清除现有内容
    ui->tableWidget->clear();

    // 获取表头信息
    QStringList headers = handler.getColumnNames("rawdata");
    int colCount = headers.size();

    // 获取行数
    int rowCount = handler.getRowNum("rawdata");

    if (rowCount <= 0 || colCount <= 0) {
        qWarning("No data available in the database.");
        return;
    }

    // 创建一个二维数组来存储所有的 QTableWidgetItem
    QVector<QVector<QTableWidgetItem*>> items(rowCount, QVector<QTableWidgetItem*>(colCount));

    // 设置表格的默认字体
    QFont tableFont;
    tableFont.setPointSize(13); // 设置字体大小为13
    ui->tableWidget->setFont(tableFont);    // 为表格单元格设置字体
    ui->tableWidget->horizontalHeader()->setFont(tableFont);    // 为水平表头设置字体
    ui->tableWidget->verticalHeader()->setFont(tableFont);  // 为垂直表头（行号）设置字体

    // 提前准备好所有的 QTableWidgetItem
    for (int col = 0; col < colCount; ++col) {
        QString columnName = headers[col];
        QVector<double> columnData = handler.getDataByColumn(columnName);

        for (int row = 0; row < rowCount; ++row) {
            if (row < columnData.size()) { // 确保数据不越界
                QString cellData = QString::number(columnData[row], 'f', 2); // 格式化数字为字符串，保留两位小数
                QTableWidgetItem *item = new QTableWidgetItem(cellData);
                item->setFont(tableFont); // 为每个项设置字体
                items[row][col] = item;
            } else {
                items[row][col] = new QTableWidgetItem(""); // 如果数据不足，填充空字符串
            }
        }
    }

    // 暂时禁用信号，避免每次 setItem 触发信号
    ui->tableWidget->blockSignals(true);

    // 设置表格的行数和列数
    ui->tableWidget->setRowCount(rowCount);
    ui->tableWidget->setColumnCount(colCount);

    // 设置表头
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 一次性将所有 QTableWidgetItem 设置到表格中
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            ui->tableWidget->setItem(row, col, items[row][col]);
        }
    }

    // 恢复信号
    ui->tableWidget->blockSignals(false);
}


void MainWindow::drawThreeDByDB()
{
    // 创建一个 Q3DScatter 实例
    QtDataVisualization::Q3DScatter *graph = new QtDataVisualization::Q3DScatter();

    // 设置图表的标题
    graph->setTitle("3D Scatter Chart Example");

    // 设置 x、y、z 轴的标签
    graph->axisX()->setTitle("纬度");
    graph->axisY()->setTitle("高度");
    graph->axisZ()->setTitle("经度");
    graph->axisX()->setTitleVisible(true);
    graph->axisY()->setTitleVisible(true);
    graph->axisZ()->setTitleVisible(true);

    //    // 如果有中文列名就设置为中文,否则设置为英文
    //    QStringList columnNames = handler.getColumnNames("rawdata");
    //    QString Latitude,Altitude,Longitude;
    //    if(columnNames.contains("安控信号"))
    //    {
    //        Latitude = "BD2/GPS2纬度";
    //        Altitude = "BD2/GPS2高度";
    //        Longitude = "BD2/GPS2经度";
    //    }
    //    else if(columnNames.contains("safetyControlSignal")){
    //        Latitude = "bd2Gps2Latitude";
    //        Altitude = "bd2Gps2Altitude";
    //        Longitude = "bd2Gps2Longitude";
    //    }
    // 创建散点图数据
    // 准备绘图数据:
    //    QVector<double> time = handler.getDataByColumn("飞行时间");
    QVector<double> rawLongitudedata = handler.getDataByColumn("BD2/GPS2纬度");
    QVector<double> rawLatitudedata = handler.getDataByColumn("BD2/GPS2高度");
    QVector<double> rawAltitudedata = handler.getDataByColumn( "BD2/GPS2经度");

    QVector<float> Longitudedata,Latitudedata,Altitudedata;
    // 遍历double类型的向量并将每个元素转换为float类型添加到新的向量中
    Longitudedata = doubleQvectortoFloatQvector(rawLongitudedata);
    Latitudedata = doubleQvectortoFloatQvector(rawLatitudedata);
    Altitudedata = doubleQvectortoFloatQvector(rawAltitudedata);

    QtDataVisualization::QScatterDataArray data;

    DEBUG_LOG("Longitudedata.size():"<<Longitudedata.size());
    if(Longitudedata.size()!=0)
    {
        for (int i = 0; i < Longitudedata.size(); i=i+10) {
            QVector3D point(Longitudedata[i], Latitudedata[i], Altitudedata[i]);
            //             DEBUG_LOG(point);
            data << point;
        }
    }

    // 创建数据代理，并将数据添加到代理中
    QtDataVisualization::QScatterDataProxy *proxy = new QtDataVisualization::QScatterDataProxy();
    proxy->addItems(data);

    // 创建系列，并将数据代理添加到系列中
    QtDataVisualization::QScatter3DSeries *series = new QtDataVisualization::QScatter3DSeries(proxy);
    series->setItemSize(0.1f);

    // 将系列添加到图表中
    graph->addSeries(series);

    // 调整轴的范围以适应数据
    DEBUG_LOG("x max:"<<*std::min_element(Longitudedata.constBegin(), Longitudedata.constEnd())<<"x min:"<<*std::max_element(Longitudedata.constBegin(), Longitudedata.constEnd()));
    graph->axisX()->setRange(*std::min_element(Longitudedata.constBegin(), Longitudedata.constEnd()), *std::max_element(Longitudedata.constBegin(), Longitudedata.constEnd()));
    graph->axisY()->setRange(*std::min_element(Latitudedata.constBegin(), Latitudedata.constEnd()), *std::max_element(Latitudedata.constBegin(), Latitudedata.constEnd()));
    graph->axisZ()->setRange(*std::min_element(Altitudedata.constBegin(), Altitudedata.constEnd()), *std::max_element(Altitudedata.constBegin(), Altitudedata.constEnd()));

    // 将图表添加到 ui->pageThreeD 中
    QHBoxLayout *layout = new QHBoxLayout(ui->pageThreeD);
    layout->addWidget(QWidget::createWindowContainer(graph));
}


void MainWindow::drawPiePlotByDB()
{
    // 创建四个QString,如果有中文列名就设置为中文,否则设置为英文
    QStringList columnNames = handler.getColumnNames("rawdata");
    QString chartViewName1,chartViewName2,chartViewName3,chartViewName4;
    if(columnNames.contains("安控信号"))
    {
        chartViewName1 = "安控信号";
        chartViewName2 = "磁罗盘状态";
        chartViewName3 = "油门档位";
        chartViewName4 = "大气机状态";
    }
    else if(columnNames.contains("safetyControlSignal")){
        chartViewName1 = "safetyControlSignal";
        chartViewName2 = "magneticCompassStatus";
        chartViewName3 = "throttlePosition";
        chartViewName4 = "atmosphericCondition";
    }
    // 创建四个饼状图系列
    QPieSeries *series1 = new QPieSeries();
    QMap<QString, int> aggregateddata1 = handler.groupDataByColumnName( chartViewName1);
    for (auto it = aggregateddata1.begin(); it != aggregateddata1.end(); ++it) {
        series1->append(it.key(), it.value());
    }

    QPieSeries *series2 = new QPieSeries();
    QMap<QString, int> aggregateddata2 = handler.groupDataByColumnName( chartViewName2);
    for (auto it = aggregateddata2.begin(); it != aggregateddata2.end(); ++it) {
        series2->append(it.key(), it.value());
    }

    QPieSeries *series3 = new QPieSeries();
    QMap<QString, int> aggregateddata3 = handler.groupDataByColumnName( chartViewName3);
    for (auto it = aggregateddata3.begin(); it != aggregateddata3.end(); ++it) {
        series3->append(it.key(), it.value());
    }

    QPieSeries *series4 = new QPieSeries();
    QMap<QString, int> aggregateddata4 = handler.groupDataByColumnName( chartViewName4);
    for (auto it = aggregateddata4.begin(); it != aggregateddata4.end(); ++it) {
        series4->append(it.key(), it.value());
    }

    // 创建四个饼状图视图
    QChartView *chartView1 = createChartView(series1, chartViewName1);
    QChartView *chartView2 = createChartView(series2, chartViewName2);
    QChartView *chartView3 = createChartView(series3, chartViewName3);
    QChartView *chartView4 = createChartView(series4, chartViewName4);

    // 设置饼状图视图的大小策略为自适应大小
    chartView1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 创建布局，并将饼状图视图添加到布局中
    QGridLayout *layout = new QGridLayout(ui->pagePie);
    layout->addWidget(chartView1, 0, 0);
    layout->addWidget(chartView2, 0, 1);
    layout->addWidget(chartView3, 1, 0);
    layout->addWidget(chartView4, 1, 1);

    // 设置布局到页面
    ui->pagePie->setLayout(layout);
}
