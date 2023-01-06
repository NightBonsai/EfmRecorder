#include "WelcomeItem.h"

/**
 * @brief WelcomeItem::WelcomeItem
 * @param path   图片存储地址
 * @param state  图片状态  0-静态 1-动态
 * @param direct 图片移动方向 2-向左移 3-向右移 4-向上移 5-向下移
 */
WelcomeItem::WelcomeItem(QString path,int state,int direct)
{
    this->image=QPixmap(path);
    this->itemState=state;
    this->itemDirect=direct;
}
WelcomeItem::~WelcomeItem()
{
}

int WelcomeItem::getItemState() const
{
    return itemState;
}

void WelcomeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   painter->drawPixmap(-this->image.width()/2,-this->image.height()/2,
                       this->image.width(),this->image.height(),
                       this->image);
}

QRectF WelcomeItem::boundingRect() const
{
    return QRectF(-this->image.width()/2,-this->image.height()/2,
                  this->image.width(),this->image.height());
}

void WelcomeItem::advance(int phase)
{
    //若两图元碰撞
    if(collidingItems().count()>0)
    {
        emit stopTimerSIGNAL();
    }
    else
    {
        if(this->itemState == ACTION)
        {
            //图元向左移
            if(this->itemDirect == LEFT)
            {
                this->setPos(mapToScene(-2,0));
            }
            //图元向右移
            else if(this->itemDirect == RIGHT)
            {
                this->setPos(mapToScene(2,0));
            }
            //图元向上移
            else if(this->itemDirect == UP)
            {
                this->setPos(mapToScene(0,-2));
            }
            //图元向下移
            else if(this->itemDirect == DOWN)
            {
                this->setPos(mapToScene(0,2));
            }
        }
    }
}
