#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#pragma execution_character_set("utf-8")//保证中文有效显示

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_loginBtn_clicked();
    void on_quitBtn_clicked();
    void resizeEvent();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
