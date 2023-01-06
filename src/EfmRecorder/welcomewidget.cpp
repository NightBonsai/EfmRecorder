#include "welcomewidget.h"
#include <QIcon>
#include <windows.h>
#include <QPalette>
#include <QPainter>

WelcomeWidget::WelcomeWidget()
{
    this->initWin();
    this->initScene();
    this->initView();
    this->initTimer();
    this->initConnect();
}
WelcomeWidget::~WelcomeWidget()
{

}

void WelcomeWidget::initWin()
{
    //设置窗口固定大小
    this->setFixedSize(500,800);
    this->setWindowTitle("HHS执法记录仪");
    this->setWindowIcon(QIcon(":/data/Image/enforcementRecorder.png"));

    this->setWin = new SetWidget();
}

void WelcomeWidget::initScene()
{
    this->scene = new QGraphicsScene();
    //给当前窗口设置场景
    this->setScene(scene);
    //设置场景坐标与视图匹配
    this->scene->setSceneRect(0,0,this->width()-2,this->height()-2);
}

void WelcomeWidget::initView()
{
    this->loading = new WelcomeItem(":/data/Image/welcomeImage/loading.png",ACTION,UP);
    this->dest = new WelcomeItem(":/data/Image/welcomeImage/dest.png",ACTION,DOWN);

    //场景中添加图元
    this->scene->addItem(loading);
    this->scene->addItem(dest);
    //设置图元位置
    this->loading->setPos(this->width()/2,this->height()/1.3);
    this->dest->setPos(this->width()/2,this->height()/4);
    //设置图元背景
    this->setBackgroundBrush(QBrush(QPixmap(":/data/Image/background-welcome.png").scaled(this->width(),this->height())));
}

void WelcomeWidget::initTimer()
{
    this->timer = new QTimer();
    //开启计时器
    this->timer->start(100);
}

void WelcomeWidget::initConnect()
{
    connect(this->timer,SIGNAL(timeout()),this->scene,SLOT(advance()));
    connect(this->loading,SIGNAL(stopTimerSIGNAL()),this,SLOT(stopTimer()));
    connect(this->dest,SIGNAL(stopTimerSIGNAL()),this,SLOT(stopTimer()));
}

void WelcomeWidget::stopTimer()
{
    //停止计时器
    this->timer->stop();

    //跳转至设置界面
    this->hide();
    this->setWin->show();
}
