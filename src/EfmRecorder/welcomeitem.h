#ifndef WELCOMEITEM_H
#define WELCOMEITEM_H

#define STATIC 0
#define ACTION 1
#define LEFT 2
#define RIGHT 3
#define UP 4
#define DOWN 5

#include <QGraphicsItem>    //图元类
#include <QPixmap>          //绘图类
#include <QString>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QObject>

class WelcomeItem:public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    WelcomeItem(QString path,int state,int direct);
    ~WelcomeItem();

    //公有接口
    int getItemState() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
          QWidget *widget = Q_NULLPTR); //绘制图元
    QRectF boundingRect() const;        //返回当前图元位置 虚函数重定义
    void advance(int phase);            //图元移动        虚函数重定义

private:
    QPixmap image;              //图片
    int itemState;              //图片状态  0-静态 1-动态
    int itemDirect;             //图片移动方向 2-向左移 3-向右移 4-向上移 5-向下移

signals:
    void stopTimerSIGNAL();      //停止计时器信号
};

#endif // WelcomeItem_H
