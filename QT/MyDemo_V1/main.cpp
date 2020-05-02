#include "mainwindow.h"
#include <QApplication>
//#include "palette.h"
#include <QBoxLayout>
#include<QWidget>
#include"logindialog.h"

#include <QDebug>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setFont(QFont("Microsoft Yahei", 9));

    //——————————————————————————————界面设计风格载入————————————————————————————————————
    QFile qss("D:\\QT5.8.0\\klc\\MyDemo\\MyDemo\\qss\\xieti11.qss");//"D:\\QT5.8.0\\klc\\MyDemo\\MyDemo\\qss\\xieti.qss"
    if( qss.open(QFile::ReadOnly))
    {
        qDebug("open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug("Open failed");
    }
    //——————————————————————————————————————————————————————————————————————————————————

   LoginDialog login;
   login.setWindowTitle(QString("登陆界面"));
   if(login.exec()==QDialog::Accepted)
      {
          MainWindow w;
          w.setWindowTitle(QString("变压器智能装配"));
          w.show();

          return a.exec();
      }
      else return 0;



 }





