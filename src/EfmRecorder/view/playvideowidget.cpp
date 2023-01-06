#include "playvideowidget.h"
#include <QFile>
#include <QIcon>
#include <QDateTime>
#include <QMessageBox>
#include <QPalette>
#include <QPainter>

PlayVideoWidget::PlayVideoWidget(QWidget *parent) : QWidget(parent)
{
    this->initWin();
    this->initData();
    this->initControl();
    this->initQss();

    //信号与槽事件
    connect(this->back,SIGNAL(clicked()),this,SLOT(backToVideo()));
    connect(this->play,SIGNAL(clicked()),this,SLOT(playIconChange()));
    connect(this->catchImage,SIGNAL(clicked()),this,SLOT(catchImageJudge()));
}

PlayVideoWidget::~PlayVideoWidget()
{

}

void PlayVideoWidget::initWin()
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

void PlayVideoWidget::initData()
{
    this->item = nullptr;
    this->playState = 0;
}

void PlayVideoWidget::initControl()
{
    //标题
    this->title = new QLabel("视频回放",this);
    this->title->setObjectName("title");
    this->title->setGeometry(180,60,200,100);

    //返回
    this->back = new QPushButton(this);
    this->back->setObjectName("button");
    this->back->setIcon(QIcon(":/data/Image/playVideoImage/return.png"));
    this->back->setGeometry(395,95,30,30);

    //视频播放
    this->showImage = new QLabel(this);
    this->showImage->setObjectName("showImage");
    this->showImage->setGeometry(40,180,425,350);

    //进度条
    this->videoProgressBar = new QProgressBar(this);
    this->videoProgressBar->setValue(0);
    this->videoProgressBar->setGeometry(40,550,460,30);

    //操作按钮
    this->play = new QPushButton(this);
    this->play->setObjectName("button");
    this->play->setIcon(QIcon(":/data/Image/playVideoImage/play.png"));
    this->playState = 0;
    this->play->setGeometry(40,590,30,30);
    this->lastVideo = new QPushButton(this);
    this->lastVideo->setObjectName("button");
    this->lastVideo->setIcon(QIcon(":/data/Image/playVideoImage/lastVideo.png"));
    this->lastVideo->setGeometry(80,590,30,30);
    this->nextVideo = new QPushButton(this);
    this->nextVideo->setObjectName("button");
    this->nextVideo->setIcon(QIcon(":/data/Image/playVideoImage/nextVideo.png"));
    this->nextVideo->setGeometry(110,590,30,30);
    this->speedRate = new QComboBox(this);
    this->speedRate->addItem("倍速");
    this->speedRate->addItem("x0.5");
    this->speedRate->addItem("x1.0");
    this->speedRate->addItem("x1.5");
    this->speedRate->addItem("x2.0");
    this->speedRate->setGeometry(320,590,100,30);
    this->catchImage = new QPushButton(this);
    this->catchImage->setObjectName("button");
    this->catchImage->setIcon(QIcon(":/data/Image/playVideoImage/catchImage.png"));
    this->catchImage->setGeometry(430,590,30,30);
}

void PlayVideoWidget::initQss()
{
    QFile file(":/data/qss/playVideoWidget.qss");
    if(file.open(QFile::ReadOnly))
    {
        //设置给整个窗口
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void PlayVideoWidget::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/data/Image/background.png").scaled(this->size()));
    //输出图像
    this->showImage->setPixmap(QPixmap::fromImage(this->image).scaled(this->showImage->width(),this->showImage->height()));
    //输出进度条
    this->videoProgressBar->setValue(this->playThread->framePercentage);
}

void PlayVideoWidget::closeEvent(QCloseEvent *event)
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
        //结束线程
        this->playThread->terminate();

        event->accept(); // 接受退出信号，程序退出
    }
}

void PlayVideoWidget::getVideo()
{
    //获取当前视频保存路径
    QString videoSavePath = Controller::getInstance()->getVideoSavePath(this->item->text());
    videoSavePath = videoSavePath +"/"+ this->item->text();

    //初始化&开启解码线程
    this->playThread = new Decode(videoSavePath,1);
    connect(this->playThread,SIGNAL(sendImgSIGNAL(QImage)),this,SLOT(receiveImg(QImage)));
    connect(this->play,SIGNAL(clicked()),this->playThread,SLOT(playStateChange()));
    connect(this->speedRate,SIGNAL(currentIndexChanged(int)),this->playThread,SLOT(speedRateChange(int)));
}

void PlayVideoWidget::receiveImg(QImage image)
{
    if(!image.isNull())
    {
        this->image = image;
        this->update();         //自动执行paintEvent
    }
}

void PlayVideoWidget::backToVideo()
{
    //停止线程
    this->playThread->terminate();
    //修改播放状态
    this->playState = 0;
    this->play->setIcon(QIcon(":/data/Image/playVideoImage/play.png"));
    //发送返回信号
    emit backToVideoSIGNAL();
}

void PlayVideoWidget::playIconChange()
{
    if(playState == 0)
    {
        this->play->setIcon(QIcon(":/data/Image/playVideoImage/stop.png"));
        this->playState = 1;
    }
    else if(playState == 1)
    {
        this->play->setIcon(QIcon(":/data/Image/playVideoImage/play.png"));
        this->playState = 0;
    }
}

void PlayVideoWidget::catchImageJudge()
{
    //获取当下时间
    QDateTime time = QDateTime::currentDateTime();

    //拼接截屏保存信息
    QString creatTime = time.toString("yyyy-MM-dd-hh-mm-ss");                   //创建时间
    QString imageSavePath = Controller::getInstance()->getDeviceImagePath();
    imageSavePath = imageSavePath+"/"+creatTime+".png";                         //保存地址
    QString imageName = creatTime + ".png";                                     //截屏名称
    int deviceID = (Controller::getInstance()->getDeviceID()).toInt();          //设备id

    //保存图片
    this->image.save(imageSavePath);

    //写入数据库
    int res = Controller::getInstance()->addNewImage(imageName,deviceID,creatTime);
    if(res == 1)
    {
        QMessageBox::information(this,"提示","截屏保存成功");
        return;
    }
    return;
}

void PlayVideoWidget::toLastVideo()
{

}

void PlayVideoWidget::toNextVideo()
{

}
