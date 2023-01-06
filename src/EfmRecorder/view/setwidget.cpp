#include "setwidget.h"
#include <QIcon>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>              //文件对话框—实现选择文件保存路径
#include <QDir>                     //获取关于目录路径及文件的相关信息—实现判断文件保存路径是否存在
#include <QPalette>
#include <QPainter>
#include "controller/controller.h"
#include "data/deviceinfo.h"

SetWidget::SetWidget(QWidget *parent) : QWidget(parent)
{
    this->initWin();
    this->initData();
    this->initControl();
    this->initQss();

    //信号与槽事件
    connect(this->deviceName,SIGNAL(currentIndexChanged(int)),this,SLOT(setInfo()));
    connect(this->videoPath,SIGNAL(clicked()),this,SLOT(selectVideoPath()));
    connect(this->imagePath,SIGNAL(clicked()),this,SLOT(selectImagePath()));
    connect(this->yes,SIGNAL(clicked()),this,SLOT(setJudge()));
    connect(this->no,SIGNAL(clicked()),this,SLOT(renewSet()));
    connect(this->mainWin,SIGNAL(toSetWinSIGNAL()),this,SLOT(showSet()));
}

SetWidget::~SetWidget()
{

}

void SetWidget::initWin()
{
    //设置窗口固定大小
    this->setFixedSize(500,800);
    this->setWindowTitle("HHS执法记录仪");
    this->setWindowIcon(QIcon(":/data/Image/enforcementRecorder.png"));
    //设置背景图片
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/data/Image/background.png")));
    setPalette(pal);

    //初始化主界面
    this->mainWin = new MainWidget();
}

void SetWidget::initData()
{
    //初始化界面数据
    this->devices = Controller::getInstance()->getDeviceNameInfo();
    this->setState = 0;
}

void SetWidget::initControl()
{
    //标题
    this->title = new QLabel("设备设置",this);
    this->title->setObjectName("title");
    this->title->setGeometry(180,60,200,100);

    //标签
    this->deviceNameL = new QLabel("设备编号",this);
    this->deviceNameL->setObjectName("label");
    this->deviceNameL->setGeometry(100,150,100,100);
    this->memoryL = new QLabel("内存空间",this);
    this->memoryL->setObjectName("label");
    this->memoryL->setGeometry(100,230,100,100);
    this->memoryLeaveL = new QLabel("内存剩余",this);
    this->memoryLeaveL->setObjectName("label");
    this->memoryLeaveL->setGeometry(100,310,100,100);
    this->videoSavePathL = new QLabel("录像存储",this);
    this->videoSavePathL->setObjectName("label");
    this->videoSavePathL->setGeometry(100,390,100,100);
    this->imageSavePathL = new QLabel("图片存储",this);
    this->imageSavePathL->setObjectName("label");
    this->imageSavePathL->setGeometry(100,470,100,100);

    //编辑框
    this->deviceName = new QComboBox(this);
    this->deviceName->setGeometry(200,185,200,30);
    for(int i=0;i<devices.count();i++)
    {
        this->deviceName->addItem(devices[i]);
    }
    this->memoryE = new QLineEdit(this);
    this->memoryE->setGeometry(200,265,200,30);
    this->memoryE->setEnabled(false);           //编辑框内容无法编辑
    this->memoryLeaveE = new QLineEdit(this);
    this->memoryLeaveE->setGeometry(200,345,200,30);
    this->memoryLeaveE->setEnabled(false);
    this->videoSavePathE = new QLineEdit(this);
    this->videoSavePathE->setGeometry(200,425,150,30);
    this->videoSavePathE->setEnabled(false);
    this->imageSavePathE = new QLineEdit(this);
    this->imageSavePathE->setGeometry(200,505,150,30);
    this->imageSavePathE->setEnabled(false);

    //按钮
    this->videoPath = new QPushButton("..",this);
    this->videoPath->setObjectName("button");
    this->videoPath->setGeometry(370,425,30,30);
    this->imagePath = new QPushButton("..",this);
    this->imagePath->setObjectName("button");
    this->imagePath->setGeometry(370,505,30,30);
    this->yes = new QPushButton("确定",this);
    this->yes->setObjectName("button");
    this->yes->setGeometry(100,600,100,40);
    this->no = new QPushButton("取消",this);
    this->no->setObjectName("button");
    this->no->setGeometry(300,600,100,40);
}

void SetWidget::initQss()
{
    QFile file(":/data/qss/setWidget.qss");
    if(file.open(QFile::ReadOnly))
    {
        //设置给整个窗口
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void SetWidget::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/data/Image/background.png").scaled(this->size()));
}

void SetWidget::closeEvent(QCloseEvent *event)
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

quint64 SetWidget::getDirSize(const QString filePath)
{
    //传入的文件路径格式如: "D:/SoftPage/SVN"
    //"D:\SoftPage\SVN" 无效  需转义为"D:\\SoftPage\\SVN"可正常使用
    QDir tmpDir(filePath);
    quint64 size = 0;

    //获取文件列表  统计文件大小
    foreach(QFileInfo fileInfo, tmpDir.entryInfoList(QDir::Files))
    {
        size += fileInfo.size();
    }

    //获取文件夹  并且过滤掉.和..文件夹 统计各个文件夹的文件大小
    foreach(QString subDir, tmpDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        size += getDirSize(filePath + QDir::separator() + subDir); //递归进行  统计所有子目录
    }

    //返回单位：MB
    return size/1024/1024;
}

void SetWidget::setInfo()
{
    //获取设备信息
    DeviceInfo device = Controller::getInstance()->getDeviceInfo(this->deviceName->currentText());

    //输出设备信息
    this->memoryE->setText(QString::number(device.getDeviceMemory())+"GB");  //int 转 QString
    this->videoSavePathE->setText(device.getDeviceVideoPath());
    this->imageSavePathE->setText(device.getDeviceImagePath());

    //计算设备空间剩余大小
    QDir dir;
    if(dir.exists(videoSavePathE->text()) && dir.exists(imageSavePathE->text()))
    {
        quint64 videoSize = getDirSize(device.getDeviceVideoPath());
        quint64 imageSize = getDirSize(device.getDeviceImagePath());

        qDebug()<<"videoSize="<<videoSize<<"MB "<<"imageSize="<<imageSize<<"MB";
        this->memoryLeaveE->setText(QString::number(20-videoSize/1024-imageSize/1024)+"GB");    //int 转 QString
    }
}

void SetWidget::selectVideoPath()
{
    //获取自定义路径
    QString path = QFileDialog::getExistingDirectory(this,tr("选择文件夹"),"../data",QFileDialog::ShowDirsOnly);
    if(!path.isEmpty())
    {
        this->videoSavePathE->setText(path);
    }
    //自定义路径写入数据库
    int res = Controller::getInstance()->setDeviceVideoPath(path,this->deviceName->currentText());
    if(res == 1)
    {
        qDebug()<<"set deviceVideoPath success";
    }
    else
    {
        this->videoSavePathE->setText("路径设置失败");
    }
}

void SetWidget::selectImagePath()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("选择文件夹"),"../data",QFileDialog::ShowDirsOnly);
    if(!path.isEmpty())
    {
        this->imageSavePathE->setText(path);
    }

    int res = Controller::getInstance()->setDeviceImagePath(path,this->deviceName->currentText());
    if(res == 1)
    {
        qDebug()<<"set deviceImagePath success";
    }
    else
    {
        this->imageSavePathE->setText("路径设置失败");
    }
}

void SetWidget::setJudge()
{
    QDir dir;
    QString imgSavePath = imageSavePathE->text();
    QString vidSavePath = videoSavePathE->text();

    //表单验证
    if(this->memoryE->text() == "" || this->videoSavePathE->text() == "" || this->imageSavePathE->text() == "")
    {
        QMessageBox::information(this,"提示","表单不能为空");
        return;
    }
    //判断文件保存路径是否存在
    else if(!dir.exists(vidSavePath) || !dir.exists(imgSavePath))
    {
        QMessageBox::information(this,"提示","保存路径不存在");
        return;
    }
    //判断文件保存路径剩余存储空间是否符合符合要求
    else if( (20-getDirSize(vidSavePath)/1024-getDirSize(imgSavePath)/1024) <= 1)
    {
        QMessageBox::information(this,"提示","剩余存储空间不足，请选择其他路径");
        return;
    }
    else
    {
        QMessageBox::information(this,"提示","设置成功，点击跳转至主界面");
        Controller::getInstance()->setDeviceState(this->deviceName->currentText()); //修改设备使用状态
        this->setState = 1;
        this->hide();
        this->mainWin->show();
    }
}

void SetWidget::renewSet()
{
    if(setState == 0)
    {
        this->videoSavePathE->clear();
        this->imageSavePathE->clear();
    }
    else if(setState == 1)
    {
        this->hide();
        this->mainWin->show();
    }
}

void SetWidget::showSet()
{
    this->mainWin->hide();

    this->deviceName->setEnabled(false);
    this->videoPath->setEnabled(false);
    this->imagePath->setEnabled(false);
    this->yes->setEnabled(false);
    this->show();
}
