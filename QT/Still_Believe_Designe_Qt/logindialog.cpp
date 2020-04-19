#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    ui->usrLineEdit->setPlaceholderText("请输入用户的账号");
    ui->pwdLineEdit->setPlaceholderText("请输入用户的密码");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
    //if(ui->usrLineEdit->text()==tr("klc")&&ui->pwdLineEdit->text()==tr("123456"))
    //判断用户名和密码是否正确
    if(ui->usrLineEdit->text()==tr("")&&ui->pwdLineEdit->text()==tr(""))
    accept();
    else{
    QMessageBox::warning(this,tr("Warning"),tr("user name or password error!"),QMessageBox::Yes);
    //如果不正确，弹出警告对话框
    //如果不正确，弹出警告对话框
    ui->usrLineEdit->clear();//清空用户名输入框
    ui->pwdLineEdit->clear();//清空密码输入框
    ui->usrLineEdit->setFocus();//将光标转到用户名输入框
  }
}

void LoginDialog::on_quitBtn_clicked()
{
    this->close();
}
