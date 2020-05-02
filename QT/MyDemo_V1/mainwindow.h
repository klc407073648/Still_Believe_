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

    QTextEdit *textEdit;
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


    void on_procedureRun1_clicked();
    void on_procedureRun2_clicked();
    void on_procedureRun3_clicked();
    void on_procedureRun4_clicked();
    void on_procedureRun5_clicked();
    void on_procedureRun6_clicked();
    void on_procedureRun7_clicked();
    void on_procedureRun8_clicked();
    void on_procedureRun9_clicked();
    void on_procedureRun10_clicked();
    void on_procedureRun11_clicked();
    void on_procedureRun12_clicked();
    void on_procedureRun13_clicked();
    void on_procedureRun14_clicked();
    void on_procedureRun15_clicked();
    void on_procedureRun16_clicked();
    void on_procedureRun17_clicked();
    void on_procedureRun18_clicked();
    void on_motorRun1_clicked();
    void on_motorRun2_clicked();
    void on_motorRun3_clicked();
    void on_motorRun4_clicked();
    void on_motorRun5_clicked();
    void on_motorRun6_clicked();
    void on_motorRun7_clicked();
    void on_motorRun8_clicked();
    void on_motorRun9_clicked();
    void on_inductanceOne_clicked();
    void on_inductanceTwo_clicked();
    void on_robotRun1_clicked();
    void on_robotRun2_clicked();
    void on_robotRun3_clicked();
};

#endif // MAINWINDOW_H
