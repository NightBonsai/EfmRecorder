#include "enlargeimagewidget.h"
#include <QPixmap>

EnlargeImageWidget::EnlargeImageWidget(QWidget *parent) : QWidget(parent)
{
    this->initWin();
    this->initControl();
}

EnlargeImageWidget::~EnlargeImageWidget()
{

}

void EnlargeImageWidget::initWin()
{
    //无边框窗口
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setFixedSize(500,280);
}

void EnlargeImageWidget::initControl()
{
    this->enlargeImage = new QLabel(this);
    this->enlargeImage->setGeometry(0,0,500,280);
}

bool EnlargeImageWidget::event(QEvent *eve)
{
    if (QEvent::WindowDeactivate == eve->type())
    {
        emit backToImageSIGNAL();
    }
    return QWidget::event(eve);
}

void EnlargeImageWidget::getEnlargeImage()
{
    this->enlargeImage->clear();
    this->enlargeImage->setPixmap(item->icon().
                                  pixmap(QSize(this->width(),this->height())).
                                  scaled(enlargeImage->width(),enlargeImage->height()));
}
