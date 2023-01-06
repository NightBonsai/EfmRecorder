#include "transcodewidget.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPalette>
#include <QPainter>
#include "controller/controller.h"

TranscodeWidget::TranscodeWidget(QWidget *parent) : QWidget(parent)
{
    this->initWin();
    this->initData();
    this->initControl();
    this->initQss();

    //信号与槽事件
    connect(this->search,SIGNAL(clicked()),this,SLOT(getVideo()));
    connect(this->videoList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(getVideoPath(QListWidgetItem*)));
    connect(this->selectPath,SIGNAL(clicked()),this,SLOT(setTranscodePath()));
    connect(this->back,SIGNAL(clicked()),this,SLOT(backToMain()));
    connect(this->transcodeStart,SIGNAL(clicked()),this,SLOT(transcodeJudge()));
    connect(this,SIGNAL(transcodeStateChangeSIGNAL()),this,SLOT(transcodeStateChange()));
}

TranscodeWidget::~TranscodeWidget()
{

}

void TranscodeWidget::initWin()
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

void TranscodeWidget::initData()
{
    this->transcodeState = 0;
}

void TranscodeWidget::initControl()
{
    //标题
    this->title = new QLabel("视频导出",this);
    this->title->setObjectName("title");
    this->title->setGeometry(180,60,200,100);

    //返回
    this->back = new QPushButton(this);
    this->back->setObjectName("button");
    this->back->setIcon(QIcon(":/data/Image/transcodeImage/return.png"));
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
    this->search->setIcon(QIcon(":/data/Image/transcodeImage/search.png"));
    this->search->setGeometry(425,160,30,30);

    //视频墙
    this->videoList = new QListWidget(this);
    this->videoList->setObjectName("videoList");
    this->videoList->setFlow(QListView::TopToBottom);       //元素从左到右横向排列
    this->videoList->setViewMode(QListWidget::IconMode);    //显示模式
    this->videoList->setIconSize(QSize(175,175));           //图片大小
    this->videoList->setSpacing(1);                         //间距
    this->videoList->setResizeMode(QListView::Adjust);      //适应布局调整
    this->videoList->setMovement(QListView::Static);        //item无法移动
    this->videoList->setGeometry(40,210,420,150);

    //转码格式选择
    this->transcodeSelectL = new QLabel("转码格式",this);
    this->transcodeSelectL->setObjectName("label");
    this->transcodeSelectL->setGeometry(90,380,100,30);
    this->transcodeSelectE = new QComboBox(this);
    this->transcodeSelectE->addItem("封装格式");
    this->transcodeSelectE->addItem("mp4");
    this->transcodeSelectE->addItem("wmv");
    this->transcodeSelectE->addItem("avi");
    this->transcodeSelectE->setGeometry(200,380,200,30);

    //路径选择
    this->savePathL = new QLabel("存储路径",this);
    this->savePathL->setObjectName("label");
    this->savePathL->setGeometry(90,440,100,30);
    this->savePathE = new QLineEdit(this);
    this->savePathE->setEnabled(false);
    this->savePathE->setGeometry(200,440,170,30);
    this->selectPath = new QPushButton("..",this);
    this->selectPath->setObjectName("button");
    this->selectPath->setGeometry(370,440,30,30);

    //转码进度条
    this->transcodeStateL = new QLabel("...",this);
    this->transcodeStateL->setObjectName("label");
    this->transcodeStateL->setGeometry(180,500,200,30);
    this->transcodeProgressBar = new QProgressBar(this);
    this->transcodeProgressBar->setValue(0);
    this->transcodeProgressBar->setGeometry(90,550,350,30);

    //开始转码按钮
    this->transcodeStart = new QPushButton("视频导出",this);
    this->transcodeStart->setObjectName("button");
    this->transcodeStart->setGeometry(90,650,310,30);
}

void TranscodeWidget::initQss()
{
    QFile file(":/data/qss/transcodeWidget.qss");
    if(file.open(QFile::ReadOnly))
    {
        //设置给整个窗口
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void TranscodeWidget::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/data/Image/background.png").scaled(this->size()));
}

void TranscodeWidget::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("退出程序"),QString(tr("转码界面无法退出界面")),QMessageBox::Yes|QMessageBox::No);
    if(button==QMessageBox::No)
    {
        event->ignore(); // 忽略退出信号，程序继续进行
    }
    else if(button==QMessageBox::Yes)
    {
        event->ignore(); // 忽略退出信号，程序继续进行
    }
}

void TranscodeWidget::getVideo()
{
    //清空照片墙&数据信息
    this->videoList->clear();
    this->videoName.clear();
    this->videoCoverPath.clear();
    this->videoPath.clear();
    this->videoSavePath.clear();

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
            //输出所有查询到的视频信息
            for(int i=0;i<videoName.count();i++)
            {
                this->videoItem = new QListWidgetItem();
                this->videoItem->setText(videoName[i]);                 //设置视频item文本
                this->videoItem->setIcon(QIcon(videoCoverPath[i]));     //设置视频item图标

                this->videoList->addItem(videoItem);            //item添加进照片墙中
            }
        }
        else
        {
            QMessageBox::information(this,"提示","未检索到视频信息");
            return;
        }
    }
}

void TranscodeWidget::setTranscodePath()
{
    //获取自定义路径
    QString path = QFileDialog::getExistingDirectory(this,tr("选择文件夹"),"../data/transcodeVideo",QFileDialog::ShowDirsOnly);
    if(!path.isEmpty())
    {
        this->savePathE->setText(path);
    }
}

void TranscodeWidget::getVideoPath(QListWidgetItem *item)
{
    //获取当前视频路径
    this->videoPath.clear();
    this->videoPath = Controller::getInstance()->getVideoSavePath(item->text());
    this->videoPath = videoPath +"/"+ item->text();

    //获取当前视频名称(无后缀)
    this->videoSaveName.clear();
    this->videoSaveName = Controller::getInstance()->getVideoSaveTime(item->text());
}

void TranscodeWidget::transcodeJudge()
{
    //获取表单信息
    QString savePath = this->savePathE->text();

    //表单验证
    if(savePath.isEmpty() || videoPath.isEmpty() || transcodeSelectE->currentText() == "封装格式")
    {
        QMessageBox::information(this,"提示","表单不能为空");
        return;
    }
    else
    {
        //进度条归0
        this->transcodeProgressBar->setValue(0);

        //拼接保存路径
        this->videoSaveName = videoSaveName + "." + transcodeSelectE->currentText();
        savePath = savePath + "/" + videoSaveName;

        //初始化转码线程
        this->transcodeThread = new Transcode(videoPath,savePath);
        emit transcodeStateChangeSIGNAL();
        connect(this->transcodeThread,SIGNAL(transcodeStateChangeSIGNAL()),this,SLOT(transcodeStateChange()));
    }
}

void TranscodeWidget::backToMain()
{
    emit backToMainSIGNAL();
}

void TranscodeWidget::transcodeStateChange()
{
    if(transcodeState == 0)
    {
        //开始转码
        this->transcodeState = 1;

        //进度条归0
        this->transcodeProgressBar->setValue(0);

        this->transcodeStart->setEnabled(false);
        this->back->setEnabled(false);
        this->search->setEnabled(false);
        this->videoList->setEnabled(false);
        this->selectPath->setEnabled(false);
        this->transcodeSelectE->setEnabled(false);

        this->transcodeStateL->setText("导出视频中");
        this->transcodeThread->start();
    }
    else if(transcodeState == 1)
    {
        //停止转码
        this->transcodeState = 0;

        //进度条变满
        this->transcodeProgressBar->setValue(100);

        this->transcodeStart->setEnabled(true);
        this->back->setEnabled(true);
        this->search->setEnabled(true);
        this->videoList->setEnabled(true);
        this->selectPath->setEnabled(true);
        this->transcodeSelectE->setEnabled(true);

        this->transcodeStateL->setText("..");
        this->transcodeThread->terminate();

        QMessageBox::information(this,"提示","视频导出完成");
    }
}
