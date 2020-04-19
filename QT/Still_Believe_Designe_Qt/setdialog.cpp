#include "setdialog.h"
#include "ui_setdialog.h"
#include<QString>
#include<QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QDialog>
#include<QGridLayout>

QString productName;
QString productType;
QString productNumber;

SetDialog::SetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);


    ui->comboBox_Type->addItem(tr("A类任务"));
    ui->comboBox_Type->addItem(tr("B类任务"));
    ui->comboBox_Type->addItem(tr("C类任务"));
    ui->comboBox_Type->addItem(tr("D类任务"));

    ui->comboBox_Number->addItem(tr("10001"));
    ui->comboBox_Number->addItem(tr("10002"));
    ui->comboBox_Number->addItem(tr("10003"));
    ui->comboBox_Number->addItem(tr("10004"));


    connect(ui->ensure, SIGNAL(clicked()), this, SLOT(ensure()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(quit()));

}

SetDialog::~SetDialog()
{
    delete ui;
}


void SetDialog::ensure()
{
    QString str;
    str = "任务创建：" +ui->comboBox_Type->currentText()+ui->comboBox_Number->currentText();
    productType = ui->comboBox_Type->currentText();
    productNumber = ui->comboBox_Number->currentText();
    QMessageBox::information(this, tr("Info"), str);
}

void SetDialog::quit()
{
    this->close();
}




