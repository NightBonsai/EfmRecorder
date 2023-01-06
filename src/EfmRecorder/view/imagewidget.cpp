#include "imagewidget.h"
#include <QIcon>
#include <QFile>
#include <QMessageBox>
#include <QPalette>
#include <QPainter>
#include "controller/controller.h"

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{
    this->initWin();
    this->initData();
    this->initControl();
    this->initQss();

    //信号与槽事件
    connect(this->back,SIGNAL(clicked()),this,SLOT(backToMain()));
    connect(this->search,SIGNAL(clicked()),this,SLOT(getImage()));
    connect(this->moreImage,SIGNAL(clicked()),this,SLOT(getMoreImage()));
    connect(this->imageList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(toEnlargeImage(QListWidgetItem*)));
    connect(this->enlargeImage,SIGNAL(backToImageSIGNAL()),this,SLOT(showImageWidget()));
}

ImageWidget::~ImageWidget()
{

}

void ImageWidget::initWin()
{
    //设置窗口固定大小
    this->setFixedSize(500,800);
    this->setWindowTitle("HHS执法记录仪");
    this->setWindowIcon(QIcon(":/data/Image/enforcementRecorder.png"));
    //设置背景图片
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/data/Image/background.png")));
    setPalette(pal);

    //初始化放大界面
    this->enlargeImage = new EnlargeImageWidget();
}

void ImageWidget::initData()
{
    this->imageCount = 0;
}

void ImageWidget::initControl()
{
    //标题
    this->title = new QLabel("查看照片",this);
    this->title->setObjectName("title");
    this->title->setGeometry(180,60,200,100);

    //返回
    this->back = new QPushButton(this);
    this->back->setObjectName("button");
    this->back->setIcon(QIcon(":/data/Image/imageImage/return.png"));
    this->back->setGeometry(395,95,30,30);

    //时间检索
    this->inputDateL = new QLabel("输入检索日期",this);
    this->inputDateL->setObjectName("label");
    this->inputDateL->setGeometry(75,125,150,100);
    this->inputDateE = new QDateTimeEdit(QDate::currentDate(),this);
    this->inputDateE->setMinimumDate(QDate::currentDate().addDays(-365)); // -365天
    this->inputDateE->setMaximumDate(QDate::currentDate().addDays(365));  // +365天
    this->inputDateE->setCalendarPopup(true);                             // 日历弹出
    this->inputDateE->setDisplayFormat("yyyy-MM-dd");
    this->inputDateE->setGeometry(225,160,200,30);
    this->inputDateE->setCalendarPopup(true);
    this->search = new QPushButton(this);
    this->search->setObjectName("button");
    this->search->setIcon(QIcon(":/data/Image/imageImage/search.png"));
    this->search->setGeometry(425,160,30,30);

    //照片墙
    this->imageList = new QListWidget(this);
    this->imageList->setObjectName("imageList");
    this->imageList->setViewMode(QListWidget::IconMode);    //显示模式
    this->imageList->setIconSize(QSize(175,175));           //图片大小
    this->imageList->setSpacing(1);                         //间距
    this->imageList->setResizeMode(QListView::Adjust);      //适应布局调整
    this->imageList->setMovement(QListView::Static);        //item无法移动
    this->imageList->setGeometry(40,210,420,500);

    //加载更多
    this->moreImage = new QPushButton("加载更多",this);
    this->moreImage->setObjectName("button");
    this->moreImage->setGeometry(50,720,400,50);
}

void ImageWidget::initQss()
{
    QFile file(":/data/qss/imageWidget.qss");
    if(file.open(QFile::ReadOnly))
    {
        //设置给整个窗口
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/data/Image/background.png").scaled(this->size()));
}

void ImageWidget::closeEvent(QCloseEvent *event)
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

void ImageWidget::getImage()
{
    //清空照片墙&数据信息
    this->imageList->clear();
    this->imageName.clear();
    this->imagePath.clear();
    this->imageCount = 0;

    int res = Controller::getInstance()->searchImage(this->inputDateE->text(),imagePath,imageName);
    if(res == -1)   //无数据
    {
        QMessageBox::information(this,"提示","未检索到对应图片信息");
        return;
    }
    else if(res == 0)   //sql语句执行失败
    {
        QMessageBox::information(this,"提示","检索图片信息失败");
        return;
    }
    else
    {
        //若查回的信息不为空
        if(!(imageName.isEmpty() && imagePath.isEmpty()))
        {
            //固定先显示6个内容—实现懒加载
            //若查询图片数量大于6
            if(imageName.count()>6)
            {
                for(int i=0;i<6;i++)
                {
                    QString photoPath = imagePath[i]+"/"+imageName[i];

                    this->imageItem = new QListWidgetItem();
                    this->imageItem->setText(imageName[i]);         //设置照片item文本
                    this->imageItem->setIcon(QIcon(photoPath));     //设置照片item图标

                    this->imageList->addItem(imageItem);            //item添加进照片墙中

                    this->imageCount++;                             //图片数量累加
                }
            }
            //若查询的图片数量不足6个
            else
            {
                for(int i=0;i<imageName.count();i++)
                {
                    QString photoPath = imagePath[i]+"/"+imageName[i];

                    this->imageItem = new QListWidgetItem();
                    this->imageItem->setText(imageName[i]);         //设置照片item文本
                    this->imageItem->setIcon(QIcon(photoPath));     //设置照片item图标

                    this->imageList->addItem(imageItem);            //item添加进照片墙中

                    this->imageCount++;                             //图片数量累加
                }
            }
        }
        else
        {
            QMessageBox::information(this,"提示","未检索到对应图片信息");
            return;
        }
    }
}

void ImageWidget::getMoreImage()
{
    //若查询信息不为空
    if((!(imageName.isEmpty() && imagePath.isEmpty())) && imageName.count()>6)
    {
        //显示后最多6个内容—实现懒加载
        //若多于的图片数量大于6
        if(imageName.count()>imageCount+6)
        {
            int count = imageCount+6;
            for(int i=imageCount;i<count;i++)
            {
                QString photoPath = imagePath[i]+"/"+imageName[i];

                this->imageItem = new QListWidgetItem();
                this->imageItem->setText(imageName[i]);         //设置照片item文本
                this->imageItem->setIcon(QIcon(photoPath));     //设置照片item图标

                this->imageList->addItem(imageItem);            //item添加进照片墙中

                this->imageCount++;                             //图片数量累加
            }
        }
        //若多于的图片数量不足6个
        else
        {
            for(int i=imageCount;i<imageName.count();i++)
            {
                QString photoPath = imagePath[i]+"/"+imageName[i];

                this->imageItem = new QListWidgetItem();
                this->imageItem->setText(imageName[i]);         //设置照片item文本
                this->imageItem->setIcon(QIcon(photoPath));     //设置照片item图标

                this->imageList->addItem(imageItem);            //item添加进照片墙中

                this->imageCount++;                             //图片数量累加
            }
        }
    }
}

void ImageWidget::backToMain()
{
    emit backToMainSIGNAL();
}

void ImageWidget::toEnlargeImage(QListWidgetItem *item)
{
    this->enlargeImage->item = item;
    this->enlargeImage->getEnlargeImage();
    this->enlargeImage->show();
}

void ImageWidget::showImageWidget()
{
    this->enlargeImage->close();
}
