#include "dailywidget.h"
#include <QFile>
#include <QIcon>
#include <QStringList>
#include <QPalette>
#include <QPainter>
#include <QMessageBox>
#include "controller/controller.h"

DailyWidget::DailyWidget(QWidget *parent) : QWidget(parent)
{
    this->initWin();
    this->initData();
    this->initControl();
    this->initQss();

    //信号与槽事件
    connect(this->back,SIGNAL(clicked()),this,SLOT(backToMain()));
}

DailyWidget::~DailyWidget()
{

}

void DailyWidget::initWin()
{
    //设置窗口固定大小
    this->setFixedSize(500,800);
    this->setWindowTitle("HHS执法记录仪");
    this->setWindowIcon(QIcon(":/data/Image/enforcementRecorder.png"));
    //设置背景图片
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/data/Image/background.png")));
    setPalette(pal);

}

void DailyWidget::initData()
{
    //获取数据库操作日志信息
    this->dailys = Controller::getInstance()->searchDaily();
}

void DailyWidget::initControl()
{
    //标题
    this->title = new QLabel("操作日志",this);
    this->title->setObjectName("title");
    this->title->setGeometry(180,60,200,100);

    //返回
    this->back = new QPushButton(this);
    this->back->setObjectName("button");
    this->back->setIcon(QIcon(":/data/Image/dailyImage/return.png"));
    this->back->setGeometry(395,95,30,30);

    //操作日志界面
    this->dailyList = new QTableWidget(14,4,this);
    this->dailyList->setObjectName("dailyList");
    this->dailyList->setGeometry(40,180,425,580);
    QStringList rowLabels;
    rowLabels<<"时间"<<"设备编号"<<"警员编号"<<"操作类型";
    this->dailyList->setHorizontalHeaderLabels(rowLabels);    //设置表头
    int row = 14,col = 4;
    for(int i=0;i<row;i++)
    {
        int count = dailys.count()-1;
        if(i<dailys.count())
        {
            this->daily = new QTableWidgetItem();
            daily->setText(dailys[count-i].getDailyTime());
            dailyList->setItem(i,0,daily);

            this->daily = new QTableWidgetItem();
            daily->setText(QString::number(dailys[count-i].getDeviceID())); //int转QString
            dailyList->setItem(i,1,daily);

            this->daily = new QTableWidgetItem();
            daily->setText(QString::number(dailys[count-i].getPoliceID())); //int转QString
            dailyList->setItem(i,2,daily);

            this->daily = new QTableWidgetItem();
            daily->setText(dailys[count-i].getDailyOP());
            dailyList->setItem(i,3,daily);
        }
    }
}

void DailyWidget::initQss()
{
    QFile file(":/data/qss/dailyWidget.qss");
    if(file.open(QFile::ReadOnly))
    {
        //设置给整个窗口
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void DailyWidget::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/data/Image/background.png").scaled(this->size()));
}

void DailyWidget::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("退出程序"),QString(tr("确认退出程序")),QMessageBox::Yes|QMessageBox::No);
    if(button==QMessageBox::No)
    {
        event->ignore(); // 忽略退出信号，程序继续进行
    }
    else if(button==QMessageBox::Yes)
    {
        //修改状态
        Controller::getInstance()->endApplication();

        event->accept(); // 接受退出信号，程序退出
    }
}

void DailyWidget::renewDaily()
{
    //获取数据库操作日志信息
    this->dailys.clear();
    this->dailys = Controller::getInstance()->searchDaily();

    //操作日志表格增加数据
    int row = 14,col = 4;
    for(int i=0;i<row;i++)
    {
        int count = dailys.count()-1;
        if(i<dailys.count())
        {
            this->daily = new QTableWidgetItem();
            daily->setText(dailys[count-i].getDailyTime());
            dailyList->setItem(i,0,daily);

            this->daily = new QTableWidgetItem();
            daily->setText(QString::number(dailys[count-i].getDeviceID())); //int转QString
            dailyList->setItem(i,1,daily);

            this->daily = new QTableWidgetItem();
            daily->setText(QString::number(dailys[count-i].getPoliceID())); //int转QString
            dailyList->setItem(i,2,daily);

            this->daily = new QTableWidgetItem();
            daily->setText(dailys[count-i].getDailyOP());
            dailyList->setItem(i,3,daily);
        }
    }
}

void DailyWidget::backToMain()
{
    emit backToMainSIGNAL();
}
