#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>
#include <QGraphicsView>    //视图类
#include <QGraphicsScene>   //场景类
#include <QTimer>           //计时器
#include "welcomeitem.h"
#include "view/setwidget.h"

class WelcomeWidget:public QGraphicsView
{
    Q_OBJECT
public:
    WelcomeWidget();
    ~WelcomeWidget();

    void initWin();     //初始化窗口
    void initScene();   //初始化场景
    void initView();    //初始化图元
    void initTimer();   //初始化计时器
    void initConnect(); //初始化信号与槽事件
private:
    QGraphicsScene *scene;
    WelcomeItem *loading,*dest;
    QTimer *timer;                  //定时器

    SetWidget *setWin;
public slots:
    void stopTimer();               //停止计时器
};

#endif // WELCOMEWIDGET_H
