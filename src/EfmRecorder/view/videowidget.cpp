#include "videowidget.h"
#include <QMessageBox>
#include <QPalette>
#include <QPainter>
#include "controller/controller.h"

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent)
{
    this->initWin();
    this->initData();
    this->initControl();
    this->initQss();

    //信号与槽事件
    connect(this->back,SIGNAL(clicked()),this,SLOT(backToMain()));
    connect(this->search,SIGNAL(clicked()),this,SLOT(getVideo()));
    connect(this->videoList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(toPlayVideo(QListWidgetItem*)));
    connect(this->nextPage,SIGNAL(clicked()),this,SLOT(toNextPage()));
    connect(this->lastPage,SIGNAL(clicked()),this,SLOT(toLastPage()));
    connect(this->playVideoWin,SIGNAL(backToVideoSIGNAL()),this,SLOT(showVideoWidget()));
}

VideoWidget::~VideoWidget()
{

}

void VideoWidget::initWin()
{
    //设置窗口固定大小
    this->setFixedSize(500,800);
    this->setWindowTitle("HHS执法记录仪");
    this->setWindowIcon(QIcon(":/data/Image/enforcementRecorder.png"));
    //设置背景图片
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/data/Image/background.png")));
    setPalette(pal);

    this->playVideoWin = new PlayVideoWidget();
}

void VideoWidget::initData()
{
    this->videoCount = 0;
    this->videoPage = 0;
    this->nowPage = 1;
}

void VideoWidget::initControl()
{
    //标题
    this->title = new QLabel("视频回放",this);
    this->title->setObjectName("title");
    this->title->setGeometry(180,60,200,100);

    //返回
    this->back = new QPushButton(this);
    this->back->setObjectName("button");
    this->back->setIcon(QIcon(":/data/Image/videoImage/return.png"));
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
    this->search->setIcon(QIcon(":/data/Image/videoImage/search.png"));
    this->search->setGeometry(425,160,30,30);

    //视频墙
    this->videoList = new QListWidget(this);
    this->videoList->setObjectName("videoList");
    this->videoList->setViewMode(QListWidget::IconMode);    //显示模式
    this->videoList->setIconSize(QSize(175,175));           //图片大小
    this->videoList->setSpacing(1);                         //间距
    this->videoList->setResizeMode(QListView::Adjust);      //适应布局调整
    this->videoList->setMovement(QListView::Static);        //item无法移动
    this->videoList->setGeometry(40,210,420,500);

    //翻页
    this->lastPage = new QPushButton("上一页",this);
    this->lastPage->setObjectName("button");
    this->lastPage->setGeometry(50,720,100,50);
    this->nextPage = new QPushButton("下一页",this);
    this->nextPage->setObjectName("button");
    this->nextPage->setGeometry(350,720,100,50);
}

void VideoWidget::initQss()
{
    QFile file(":/data/qss/videoWidget.qss");
    if(file.open(QFile::ReadOnly))
    {
        //设置给整个窗口
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/data/Image/background.png").scaled(this->size()));
}

void VideoWidget::closeEvent(QCloseEvent *event)
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

void VideoWidget::getVideo()
{
    //清空照片墙&数据信息
    this->videoList->clear();
    this->videoName.clear();
    this->videoCoverPath.clear();
    this->videoCount = 0;
    this->videoPage = 0;
    this->nowPage = 1;
    this->playVideoWin->videoNames.clear();

    int res = Controller::getInstance()->searchVideo(this->inputDateE->text(),videoCoverPath,videoName);
    if(res == -1)   //无数据
    {
        QMessageBox::information(this,"提示","未检索到对应视频信息");
        return;
    }
    else if(res == 0)   //sql语句执行失败
    {
        QMessageBox::information(this,"提示","检索视频信息失败");
        return;
    }
    else
    {
        //若查回的信息不为空
        if(!(videoName.isEmpty() && videoCoverPath.isEmpty()))
        {
            //计算视频总页数，视频总个数
            this->videoCount = videoName.count();
            if(videoCount % 4 == 0)
            {
                this->videoPage = videoCount/4;
            }
            else
            {
                this->videoPage = (videoCount/4) + 1;
            }

            //固定先显示4个内容
            //若查询视频数量大于4
            if(videoCount>4)
            {
                for(int i=0;i<4;i++)
                {
                    this->videoItem = new QListWidgetItem();
                    this->videoItem->setText(videoName[i]);                 //设置视频item文本
                    this->videoItem->setIcon(QIcon(videoCoverPath[i]));     //设置视频item图标

                    this->videoList->addItem(videoItem);            //item添加进照片墙中
                    this->playVideoWin->videoNames.push_back(videoName[i]); //获取查询到的视频名称
                }
            }
            //若查询的图片数量不足4个
            else
            {
                for(int i=0;i<videoName.count();i++)
                {
                    this->videoItem = new QListWidgetItem();
                    this->videoItem->setText(videoName[i]);                 //设置视频item文本
                    this->videoItem->setIcon(QIcon(videoCoverPath[i]));     //设置视频item图标

                    this->videoList->addItem(videoItem);            //item添加进照片墙中

                    this->playVideoWin->videoNames.push_back(videoName[i]); //获取查询到的视频名称
                }
            }
        }
        else
        {
            QMessageBox::information(this,"提示","未检索到视频信息");
            return;
        }
    }
}

void VideoWidget::toLastPage()
{
    //若查询信息不为空
    if((!(videoName.isEmpty() && videoCoverPath.isEmpty())) && videoName.count()>4)
    {
        //清空图片墙
        this->videoList->clear();

        //上一页
        if((nowPage-1)*4 >= 4)
        {
            for(int i=(nowPage-2)*4;i<(nowPage-1)*4;i++)
            {
                this->videoItem = new QListWidgetItem();
                this->videoItem->setText(videoName[i]);                 //设置视频item文本
                this->videoItem->setIcon(QIcon(videoCoverPath[i]));     //设置视频item图标

                this->videoList->addItem(videoItem);            //item添加进照片墙中
            }
            this->nowPage--;                              //当前页数自减
        }
        //若为最后一页
        else if(nowPage == 1)
        {
            for(int i=0;i<4;i++)
            {
                this->videoItem = new QListWidgetItem();
                this->videoItem->setText(videoName[i]);                 //设置视频item文本
                this->videoItem->setIcon(QIcon(videoCoverPath[i]));     //设置视频item图标

                this->videoList->addItem(videoItem);            //item添加进照片墙中
            }
            QMessageBox::information(this,"提示","当前已是第一页");
            return;
        }
    }
    else
    {
        QMessageBox::information(this,"提示","请先检索视频信息");
        return;
    }
}

void VideoWidget::toNextPage()
{
    //若查询信息不为空
    if((!(videoName.isEmpty() && videoCoverPath.isEmpty())) && videoName.count()>4)
    {
        //清空图片墙
        this->videoList->clear();

        //下一页
        if((nowPage+1)*4 <= videoCount)
        {
            for(int i=nowPage*4;i<(nowPage+1)*4;i++)
            {
                this->videoItem = new QListWidgetItem();
                this->videoItem->setText(videoName[i]);                 //设置视频item文本
                this->videoItem->setIcon(QIcon(videoCoverPath[i]));     //设置视频item图标

                this->videoList->addItem(videoItem);            //item添加进照片墙中
            }
            this->nowPage++;                              //当前页数自增
        }
        else if(videoCount-(nowPage*4)<4 && videoCount-(nowPage*4)>0)
        {
            for(int i=nowPage*4;i<videoCount;i++)
            {
                this->videoItem = new QListWidgetItem();
                this->videoItem->setText(videoName[i]);                 //设置视频item文本
                this->videoItem->setIcon(QIcon(videoCoverPath[i]));     //设置视频item图标

                this->videoList->addItem(videoItem);                    //item添加进照片墙中
            }
            this->nowPage++;                              //当前页数自增
        }
        //若当是最后一页
        else if(nowPage == videoPage)
        {
            for(int i=(nowPage-1)*4;i<videoCount;i++)
            {
                this->videoItem = new QListWidgetItem();
                this->videoItem->setText(videoName[i]);                 //设置视频item文本
                this->videoItem->setIcon(QIcon(videoCoverPath[i]));     //设置视频item图标

                this->videoList->addItem(videoItem);                    //item添加进照片墙中
            }
            QMessageBox::information(this,"提示","当前已是最后一页");
            return;
        }
    }
    else
    {
        QMessageBox::information(this,"提示","请先检索视频信息");
        return;
    }
}

void VideoWidget::backToMain()
{
    emit backToMainSIGNAL();
}

void VideoWidget::toPlayVideo(QListWidgetItem *item)
{
    this->hide();
    this->playVideoWin->item = item;
    this->playVideoWin->getVideo();
    this->playVideoWin->show();
}

void VideoWidget::showVideoWidget()
{
    this->playVideoWin->hide();
    this->show();
}
