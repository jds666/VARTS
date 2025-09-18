#include "startupdialog.h"
#include "ui_startupdialog.h"

StartUpDialog::StartUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartUpDialog)
{
    ui->setupUi(this);
    // 连接按钮的 clicked 信号到槽函数
    connect(ui->startUpPushButton, &QPushButton::clicked, this, &StartUpDialog::onButtonClicked);
}

StartUpDialog::~StartUpDialog()
{
    delete ui;
}

void StartUpDialog::onButtonClicked()
{
    // 按钮点击事件的处理
    // 创建主界面对象
    MainWindow *mainWindow = new MainWindow();

    // 隐藏启动界面
    hide();

    // 显示主界面
    mainWindow->show();

}
