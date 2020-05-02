#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>
#include<QComboBox>
#include<QGridLayout>
#pragma execution_character_set("utf-8")

namespace Ui {
class SetDialog;
}

class SetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetDialog(QWidget *parent = 0);
    ~SetDialog();

private:


private slots:
    void ensure();
    void quit();

private:
    Ui::SetDialog *ui;
};

#endif // SETDIALOG_H
