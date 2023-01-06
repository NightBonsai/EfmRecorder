#include "mainwidget.h"
#include <QIcon>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>
#include <QPalette>
#include <QPainter>
#include "controller/controller.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    this->logState = 0;

    this->initWin();
    this->initControl();
    this->initQss();

    //信号与槽事件
    connect(this->login,SIGNAL(clicked()),this,SLOT(toLogWin()));
    connect(this->login,SIGNAL(clicked()),this->logWin,SLOT(initUsingDeviceName()));
    connect(this->logWin,SIGNAL(backToMainSIGNAL()),this,SLOT(showMain()));
    connect(this->logWin,SIGNAL(initLoginPoliceSIGNAL()),this,SLOT(initLoginPolice()));
    connect(this->logWin,SIGNAL(initVideoSavePathSIGNAL()),this->playThread->encodeThread,SLOT(getVideoSavePath()));
    connect(this->logWin,SIGNAL(startEncodeThreadSIGNAL()),this->playThread,SLOT(logStateChange()));
    connect(this->logWin,SIGNAL(startEncodeThreadSIGNAL()),this->playThread->encodeThread,SLOT(startEncodeThread()));
    connect(this->catchImage,SIGNAL(clicked()),this,SLOT(catchImageJudge()));
    connect(this->imageRecord,SIGNAL(clicked()),this,SLOT(toImageWin()));
    connect(this->imageWin,SIGNAL(backToMainSIGNAL()),this,SLOT(showMain()));
    connect(this->videoRecord,SIGNAL(clicked()),this,SLOT(toVideoWin()));
    connect(this->videoWin,SIGNAL(backToMainSIGNAL()),this,SLOT(showMain()));
    connect(this->catchVideo,SIGNAL(clicked()),this,SLOT(toTranscodeWin()));
    connect(this->transcodeWin,SIGNAL(backToMainSIGNAL()),this,SLOT(showMain()));
    connect(this->setting,SIGNAL(clicked()),this,SLOT(toSetWin()));
    connect(this->dailyRecord,SIGNAL(clicked()),this,SLOT(toDailyWin()));
    connect(this->dailyWin,SIGNAL(backToMainSIGNAL()),this,SLOT(showMain()));
    connect(this->exit,SIGNAL(clicked()),this,SLOT(end()));
}

MainWidget::~MainWidget()
{

}

void MainWidget::initWin()
{
    //设置窗口固定大小
    this->setFixedSize(500,800);
    this->setWindowTitle("HHS执法记录仪");
    this->setWindowIcon(QIcon(":/data/Image/enforcementRecorder.png"));
    //设置背景图片
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/data/Image/background.png")));
    setPalette(pal);

    this->logWin = new LogWidget();
    this->imageWin = new ImageWidget();
    this->videoWin = new VideoWidget();
    this->transcodeWin = new TranscodeWidget();
    this->dailyWin = new DailyWidget();
}

void MainWidget::initControl()
{
    //标题
    this->title = new QLabel("HHS执法记录仪",this);
    this->title->setObjectName("title");
    this->title->setGeometry(140,60,300,100);
    this->loginPoliceName.push_back("欢迎，未登录");
    this->welcome = new QLabel(loginPoliceName,this);
    this->welcome->setObjectName("welcome");
    this->welcome->setGeometry(180,135,150,30);

    //登录按钮
    this->login = new QPushButton(this);
    this->login->setObjectName("button");
    this->login->setIcon(QIcon(":/data/Image/mainImage/login.png"));
    this->login->setGeometry(400,95,30,30);

    //实时录像
    this->showImage = new QLabel(this);
    this->showImage->setObjectName("showImage");
    this->showImage->setGeometry(40,180,425,350);

    //功能按键
    this->catchImage = new QPushButton("现场拍照",this);
    this->catchImage->setObjectName("button");
    this->catchImage->setGeometry(80,575,80,40);
    this->videoRecord = new QPushButton("查看录像",this);
    this->videoRecord->setObjectName("button");
    this->videoRecord->setGeometry(210,575,80,40);
    this->imageRecord = new QPushButton("查看图片",this);
    this->imageRecord->setObjectName("button");
    this->imageRecord->setGeometry(340,575,80,40);
    this->dailyRecord = new QPushButton("查看日志",this);
    this->dailyRecord->setObjectName("button");
    this->dailyRecord->setGeometry(210,640,80,40);
    this->catchVideo = new QPushButton("录像导出",this);
    this->catchVideo->setObjectName("button");
    this->catchVideo->setGeometry(80,705,80,40);
    this->setting = new QPushButton("系统设置",this);
    this->setting->setObjectName("button");
    this->setting->setGeometry(210,705,80,40);
    this->exit = new QPushButton("退出",this);
    this->exit->setObjectName("button");
    this->exit->setGeometry(340,705,80,40);

    //播放线程
    this->playThread = new Decode();
    this->playThread->start();
    connect(this->playThread,SIGNAL(sendImgSIGNAL(QImage)),this,SLOT(receiveImg(QImage)));
}

void MainWidget::initQss()
{
    QFile file(":/data/qss/mainWidget.qss");
    if(file.open(QFile::ReadOnly))
    {
        //设置给整个窗口
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void MainWidget::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/data/Image/background.png").scaled(this->size()));

    //输出解码画面
    this->showImage->setPixmap(QPixmap::fromImage(this->image).scaled(this->showImage->width(),this->showImage->height()));
}

void MainWidget::closeEvent(QCloseEvent *event)
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
        this->playThread->encodeThread->terminate();
        this->playThread->terminate();

        event->accept(); // 接受退出信号，程序退出
    }
}

void MainWidget::receiveImg(QImage image)
{
    if(!image.isNull())
    {
        this->image = image;
        this->update();         //自动执行paintEvent
    }
}

void MainWidget::toLogWin()
{
    //若未登录，才能进入登录界面
    if(logState == 0)
    {
        this->hide();
        this->logWin->show();
    }
}

void MainWidget::toImageWin()
{
    //登录后才能进入照片查看界面
    if(logState == 1)
    {
        //写入操作日志
        int deviceID = (Controller::getInstance()->getDeviceID()).toInt();  //QString转int
        int policeID = (Controller::getInstance()->getPoliceID()).toInt();  //QString转int
        QDateTime dailyTime = QDateTime::currentDateTime();
        QString dailyOP = "查看照片";
        Controller::getInstance()->addNewDaily(deviceID,policeID,dailyTime.toString("yyyy-MM-dd-hh-mm-ss"),dailyOP);

        this->hide();
        this->imageWin->show();
    }
    else
    {
        QMessageBox::information(this,"提示","请先登录");
        return;
    }
}

void MainWidget::toVideoWin()
{
    //登录后才能进入视频查看界面
    if(logState == 1)
    {
        //写入操作日志
        int deviceID = (Controller::getInstance()->getDeviceID()).toInt();  //QString转int
        int policeID = (Controller::getInstance()->getPoliceID()).toInt();  //QString转int
        QDateTime dailyTime = QDateTime::currentDateTime();
        QString dailyOP = "视频回放";
        Controller::getInstance()->addNewDaily(deviceID,policeID,dailyTime.toString("yyyy-MM-dd-hh-mm-ss"),dailyOP);

        this->hide();
        this->videoWin->show();
    }
    else
    {
        QMessageBox::information(this,"提示","请先登录");
        return;
    }
}

void MainWidget::toTranscodeWin()
{
    //登录后才能进入转码界面
    if(logState == 1)
    {
        //写入操作日志
        int deviceID = (Controller::getInstance()->getDeviceID()).toInt();  //QString转int
        int policeID = (Controller::getInstance()->getPoliceID()).toInt();  //QString转int
        QDateTime dailyTime = QDateTime::currentDateTime();
        QString dailyOP = "视频导出";
        Controller::getInstance()->addNewDaily(deviceID,policeID,dailyTime.toString("yyyy-MM-dd-hh-mm-ss"),dailyOP);

        this->hide();
        this->transcodeWin->show();
    }
    else
    {
        QMessageBox::information(this,"提示","请先登录");
        return;
    }
}

void MainWidget::toSetWin()
{
    emit toSetWinSIGNAL();
}

void MainWidget::toDailyWin()
{
    //登录后才能进入日志界面
    if(logState == 1)
    {
        //写入操作日志
        int deviceID = (Controller::getInstance()->getDeviceID()).toInt();  //QString转int
        int policeID = (Controller::getInstance()->getPoliceID()).toInt();  //QString转int
        QDateTime dailyTime = QDateTime::currentDateTime();
        QString dailyOP = "查看日志";
        Controller::getInstance()->addNewDaily(deviceID,policeID,dailyTime.toString("yyyy-MM-dd-hh-mm-ss"),dailyOP);

        //更新操作日志数据信息
        this->dailyWin->renewDaily();

        this->hide();
        this->dailyWin->show();
    }
    else
    {
        QMessageBox::information(this,"提示","请先登录");
        return;
    }
}

void MainWidget::showMain()
{
    this->logWin->hide();
    this->imageWin->hide();
    this->videoWin->hide();
    this->transcodeWin->hide();
    this->dailyWin->hide();

    this->show();
}

void MainWidget::initLoginPolice()
{
    //获取当前登录警员名
    this->loginPoliceName.clear();
    this->loginPoliceName = Controller::getInstance()->getLoginPoliceName();
    this->welcome->setText("欢迎,"+loginPoliceName);

    //标记登录状态为已登录
    this->logState = 1;
}

void MainWidget::catchImageJudge()
{
    //登录后才能进行操作
    if(logState == 1)
    {
        //获取当下时间
        QDateTime time = QDateTime::currentDateTime();

        //拼接截屏保存信息
        this->creatTime = time.toString("yyyy-MM-dd-hh-mm-ss");                 //创建时间
        this->imageSavePath = Controller::getInstance()->getDeviceImagePath();
        this->imageSavePath = this->imageSavePath+"/"+creatTime+".png";         //保存地址
        this->imageName = creatTime + ".png";                                   //截屏名称
        this->deviceID = (Controller::getInstance()->getDeviceID()).toInt();    //设备id

        //保存图片
        this->image.save(imageSavePath);

        //写入数据库
        int res = Controller::getInstance()->addNewImage(imageName,deviceID,creatTime);
        if(res == 1)
        {
            QMessageBox::information(this,"提示","截屏保存成功");
        }

        //写入操作日志
        int deviceID = (Controller::getInstance()->getDeviceID()).toInt();  //QString转int
        int policeID = (Controller::getInstance()->getPoliceID()).toInt();  //QString转int
        QDateTime dailyTime = QDateTime::currentDateTime();
        QString dailyOP = "截图";
        Controller::getInstance()->addNewDaily(deviceID,policeID,dailyTime.toString("yyyy-MM-dd-hh-mm-ss"),dailyOP);

        return;
    }
    else
    {
        QMessageBox::information(this,"提示","请先登录");
        return;
    }
}

void MainWidget::end()
{
    this->close();
}
