#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QToolButton>
#include<QImage>
#include<QPixmap>
#include<QSpinBox>
#include<QSlider>
#include<QHBoxLayout>
#include "setdialog.h"

extern QString productName;
extern QString productType;
extern QString productNumber;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initMenbar();
    ui->procedureRun1->setCheckable(true);
    ui->procedureRun2->setCheckable(true);
    ui->procedureRun3->setCheckable(true);
    ui->procedureRun4->setCheckable(true);
    ui->procedureRun5->setCheckable(true);
    ui->procedureRun6->setCheckable(true);
    ui->procedureRun7->setCheckable(true);
    ui->procedureRun8->setCheckable(true);
    ui->procedureRun9->setCheckable(true);
    ui->procedureRun10->setCheckable(true);
    ui->procedureRun11->setCheckable(true);
    ui->procedureRun12->setCheckable(true);

    //——————————————————————————————各个按钮槽函数连接————————————————————————————————————
    connect(ui->action_init, &QAction::triggered, this, &MainWindow::initProject);
    connect(ui->action_newprj, &QAction::triggered, this, &MainWindow::newTask);
    connect(ui->action_start, &QAction::triggered, this, &MainWindow::startProject);
    connect(ui->action_stop, &QAction::triggered, this, &MainWindow::stopProject);
    connect(ui->action_reset, &QAction::triggered, this, &MainWindow::resetProject);
    connect(ui->action_database,&QAction::triggered, this,&MainWindow::initDataBase);

   //——————————————————————————————————————————————————————————————————————————————————


}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::initMenbar()
{
    //——————————————————————————————菜单栏的文件————————————————————————————————————
    ui->action_open->setShortcuts(QKeySequence::Open);
    ui->action_open->setStatusTip(tr("打开已经存在的文件"));
    connect(ui->action_open, &QAction::triggered, this, &MainWindow::openFile);

    ui->action_save->setShortcuts(QKeySequence::Save);
    ui->action_save->setStatusTip(tr("保存当前的文件"));
    connect(ui->action_save, &QAction::triggered, this, &MainWindow::saveFile);

    ui->action_quit->setShortcuts(QKeySequence::Close);
    ui->action_quit->setStatusTip(tr("关闭当前的文件"));
    connect(ui->action_quit, &QAction::triggered, this, &MainWindow::closeFile);

    ui->File->addAction(ui->action_open);
    ui->File->addAction(ui->action_save);
    ui->File->addAction(ui->action_quit);

   //——————————————————————————————开发者选项————————————————————————————————————
   connect(ui->action_enter,&QAction::triggered, this,&MainWindow::enterExplorer);

  //———————————————————————————————帮助说明——————————————————————————————————————————
   connect(ui->action_aboutproduct,&QAction::triggered, this,&MainWindow::productExplain);
}

 //——————————————————————————————————打开文件——————————————————————————————————————————
void MainWindow::openFile()
{
   QString path = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", tr("Text Files(*.txt)"));
   if(!path.isEmpty()) {
       QFile file(path);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           QMessageBox::warning(this, tr("读取文件"), tr("无法打开文件:\n%1").arg(path));
           return;
       }
       QTextStream in(&file);
       textEdit->setText(in.readAll());
       file.close();
   } else {
       QMessageBox::warning(this, tr("路径"), tr("你没有选择任何文件"));
   }
}

 //——————————————————————————————————保存文件——————————————————————————————————————————
void MainWindow::saveFile()
{
   QString path = QFileDialog::getSaveFileName(this, tr("保存文件"), ".", tr("Text Files(*.txt)"));
   if(!path.isEmpty()) {
       QFile file(path);
       if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
           QMessageBox::warning(this, tr("写出文件"), tr("无法打开文件:\n%1").arg(path));
           return;
       }
       QTextStream out(&file);
       out << textEdit->toPlainText();
       file.close();
   } else {
       QMessageBox::warning(this, tr("路径"), tr("你没有选择任何文件"));
   }
}

 //——————————————————————————————————关闭文件——————————————————————————————————————————
void MainWindow::closeFile()
{
   this->close();
}

//———————————————————————————————————开发者模式——————————————————————————————————————————
void MainWindow::enterExplorer()
{
   ui->stackedWidget->setCurrentIndex(2);
}

//———————————————————————————————————帮助说明——————————————————————————————————————————
void MainWindow::productExplain()
{
    QMessageBox::information(this, tr("产品说明"), tr("本设备利用机器视觉及精密测量方法，"
     "通过机器手实现零件的抓取、放置、加压、锁紧，测量等操作，最终实现变压器的全自动装配过程。"));
}

//——————————————————————————————————初始化——————————————————————————————————————————
void MainWindow::initProject()
{
    QMessageBox::information(this, tr("current"), tr("initProject"));
    ui->stackedWidget->setCurrentIndex(0);
}

//——————————————————————————————————新建任务——————————————————————————————————————————
void MainWindow::newTask()
{

    //新建一个窗口对象，其中的this指定了新的窗口的父对象是MainWindow，在销毁Mainwindow时也会销毁子对象，即该窗口对象
    SetDialog *dialog = new SetDialog(this);
    dialog->setWindowTitle(QString("产品设置"));
    dialog->setModal(true); //此处ture为模态，false为非模态
    dialog->show();
    // ui->stackedWidget->setCurrentIndex(0);
    // QPixmap *pixmap = new QPixmap("D:\\QT5.8.0\\klc\\MyDemo\\MyDemo\\photo\\1.jpeg");
    // pixmap->scaled(ui->Imageone->size(), Qt::KeepAspectRatio);
    // ui->Imageone->setScaledContents(true);
     //ui->Imageone->setPixmap(*pixmap);
}

//——————————————————————————————————运行——————————————————————————————————————————
void MainWindow::startProject()
{
    QMessageBox::information(this, tr("current"), tr("startProject"));
}

//——————————————————————————————————停止——————————————————————————————————————————
void MainWindow::stopProject()
{
    QMessageBox::information(this, tr("current"), tr("stopProject"));
}

//——————————————————————————————————复位——————————————————————————————————————————
void MainWindow::resetProject()
{
    QMessageBox::information(this, tr("current"), tr("resetProject"));

}

//——————————————————————————————————数据库——————————————————————————————————————————
void MainWindow::initDataBase()
{
   ui->stackedWidget->setCurrentIndex(1);

   //设置列数和列宽
   int width = qApp->desktop()->availableGeometry().width() - 120;
   ui->tableWidget->setColumnCount(5);
   ui->tableWidget->setColumnWidth(0, width * 0.06);
   ui->tableWidget->setColumnWidth(1, width * 0.10);
   ui->tableWidget->setColumnWidth(2, width * 0.06);
   ui->tableWidget->setColumnWidth(3, width * 0.10);
   ui->tableWidget->setColumnWidth(4, width * 0.15);
   ui->tableWidget->verticalHeader()->setDefaultSectionSize(25);

   QStringList headText;
   headText << "产品编号" << "产品名称" << "产品信息" << "装配状态" << "完成时间";
   ui->tableWidget->setHorizontalHeaderLabels(headText);
   ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
   ui->tableWidget->setAlternatingRowColors(true);
   ui->tableWidget->verticalHeader()->setVisible(false);
   ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

   //设置行高
   ui->tableWidget->setRowCount(300);

   for (int i = 0; i < 300; i++) {
       ui->tableWidget->setRowHeight(i, 24);

       QTableWidgetItem *itemDeviceID = new QTableWidgetItem(QString::number(i + 1));
       QTableWidgetItem *itemDeviceName = new QTableWidgetItem(QString("测试设备%1").arg(i + 1));
       QTableWidgetItem *itemDeviceAddr = new QTableWidgetItem(QString::number(i + 1));
       QTableWidgetItem *itemContent = new QTableWidgetItem("合格/不合格");
       QTableWidgetItem *itemTime = new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

       ui->tableWidget->setItem(i, 0, itemDeviceID);
       ui->tableWidget->setItem(i, 1, itemDeviceName);
       ui->tableWidget->setItem(i, 2, itemDeviceAddr);
       ui->tableWidget->setItem(i, 3, itemContent);
       ui->tableWidget->setItem(i, 4, itemTime);
   }
}

//——————————————————————————————————数据库查询、添加、删除、保存和清除操作——————————————————————————————————————————
void MainWindow::on_dataSearch_clicked()
{
    //ui->stackedWidget->setCurrentIndex(1);
    ui->searchEdit->setText("Search");
//    QString str=productName+productType+productNumber;
//    ui->searchEdit->setText(str);
}

void MainWindow::on_dataAdd_clicked()
{
    ui->searchEdit->setText("Add");
}

void MainWindow::on_dataDelete_clicked()
{
    ui->searchEdit->setText("Delete");
}

void MainWindow::on_dataSave_clicked()
{
    ui->searchEdit->setText("Save");
}

void MainWindow::on_dataEmpty_clicked()
{
    ui->searchEdit->setText("Empty");
}

void MainWindow::on_procedureRun1_clicked()
{

}

void MainWindow::on_procedureRun2_clicked()
{

}

void MainWindow::on_procedureRun3_clicked()
{

}

void MainWindow::on_procedureRun4_clicked()
{

}

void MainWindow::on_procedureRun5_clicked()
{

}

void MainWindow::on_procedureRun6_clicked()
{

}

void MainWindow::on_procedureRun7_clicked()
{

}

void MainWindow::on_procedureRun8_clicked()
{

}

void MainWindow::on_procedureRun9_clicked()
{

}

void MainWindow::on_procedureRun10_clicked()
{

}

void MainWindow::on_procedureRun11_clicked()
{

}

void MainWindow::on_procedureRun12_clicked()
{

}

void MainWindow::on_procedureRun13_clicked()
{

}

void MainWindow::on_procedureRun14_clicked()
{

}

void MainWindow::on_procedureRun15_clicked()
{

}

void MainWindow::on_procedureRun16_clicked()
{

}

void MainWindow::on_procedureRun17_clicked()
{

}

void MainWindow::on_procedureRun18_clicked()
{

}

void MainWindow::on_motorRun1_clicked()
{

}

void MainWindow::on_motorRun2_clicked()
{

}

void MainWindow::on_motorRun3_clicked()
{

}

void MainWindow::on_motorRun4_clicked()
{

}

void MainWindow::on_motorRun5_clicked()
{

}

void MainWindow::on_motorRun6_clicked()
{

}

void MainWindow::on_motorRun7_clicked()
{

}

void MainWindow::on_motorRun8_clicked()
{

}

void MainWindow::on_motorRun9_clicked()
{

}

void MainWindow::on_inductanceOne_clicked()
{

}

void MainWindow::on_inductanceTwo_clicked()
{

}

void MainWindow::on_robotRun1_clicked()
{

}

void MainWindow::on_robotRun2_clicked()
{

}

void MainWindow::on_robotRun3_clicked()
{

}
