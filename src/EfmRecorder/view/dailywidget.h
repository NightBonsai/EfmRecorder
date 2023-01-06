#ifndef DAILYWIDGET_H
#define DAILYWIDGET_H

#include <QWidget>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>     //表格类
#include <QTableWidgetItem>
#include <QCloseEvent>
#include "data/dailyinfo.h"

class DailyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DailyWidget(QWidget *parent = nullptr);
    ~DailyWidget();

    void initWin();
    void initData();
    void initControl();
    void initQss();

    void paintEvent(QPaintEvent *event);        //重绘函数重定义
    void closeEvent(QCloseEvent *event);        //右上角x退出事件重定义
    void renewDaily();                          //更新操作日志界面数据
private:
    QLabel *title;
    QTableWidget *dailyList;
    QTableWidgetItem *daily;
    QPushButton *back;

    QList<DailyInfo> dailys;    //存储从数据库中查回的操作日志数据
signals:
    void backToMainSIGNAL();    //返回主界面信号
public slots:
    void backToMain();          //返回主界面
};

#endif // DAILYWIDGET_H
