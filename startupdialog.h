#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class StartUpDialog;
}

class StartUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartUpDialog(QWidget *parent = nullptr);
    ~StartUpDialog();



private:
    Ui::StartUpDialog *ui;

private slots:
    void onButtonClicked();
};

#endif // STARTUPDIALOG_H
