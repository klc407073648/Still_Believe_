#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include"setdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QAction *openAction;
    QAction *saveAction;
    QAction *closeAction;

    //QTextEdit *textEdit;
    SetDialog *mySetDialog;

private slots:
    void openFile();
    void saveFile();
    void closeFile();
    void enterExplorer();

    void initMenbar();

    void newTask();
    void initProject();
    void startProject();
    void stopProject();
    void resetProject();
    void initDataBase();
    void productExplain();


    void on_dataSearch_clicked();
    void on_dataAdd_clicked();
    void on_dataDelete_clicked();
    void on_dataSave_clicked();
    void on_dataEmpty_clicked();

};

#endif // MAINWINDOW_H
