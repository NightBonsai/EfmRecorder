#include "logwidget.h"
#include <QFile>
#include <QIcon>
#include <QTime>
#include <QRegExpValidator> //正则表达式，实现输入字符限制
#include <QMessageBox>
#include <QDateTime>
#include <QPalette>
#include <QPainter>
#include "controller/controller.h"

LogWidget::LogWidget(QWidget *parent) : QWidget(parent)
{
    this->initWin();
    this->initControl();
    this->initQss();

    //信号与槽事件
    connect(this->no,SIGNAL(clicked()),this,SLOT(backToMain()));
    connect(this->Code,SIGNAL(clicked()),this,SLOT(getCode()));
    connect(this->pwdStateChange,SIGNAL(clicked()),this,SLOT(changePwdState()));
    connect(this->yes,SIGNAL(clicked()),this,SLOT(logJudge()));
    connect(this,SIGNAL(getCodeSIGNAL()),this,SLOT(getCode()));
}
LogWidget::~LogWidget()
{

}

void LogWidget::initWin()
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

void LogWidget::initControl()
{
    //标题
    this->title = new QLabel("警员登录",this);
    this->title->setObjectName("title");
    this->title->setGeometry(180,60,200,100);

    //标签
    this->deviceNameL = new QLabel("设备编号",this);
    this->deviceNameL->setObjectName("label");
    this->deviceNameL->setGeometry(100,150,100,100);
    this->policeIdentifierL = new QLabel("警员编号",this);
    this->policeIdentifierL->setObjectName("label");
    this->policeIdentifierL->setGeometry(100,230,100,100);
    this->passwordL = new QLabel("密码",this);
    this->passwordL->setObjectName("label");
    this->passwordL->setGeometry(100,310,100,100);
    this->codeL = new QLabel("验证码",this);
    this->codeL->setObjectName("label");
    this->codeL->setGeometry(100,390,100,100);

    //编辑框
    //限制输入  字母数字
    QRegExp input("[a-zA-Z0-9]+$");

    this->deviceNameE = new QLineEdit(this);
    this->deviceNameE->setGeometry(200,185,200,30);
    this->deviceNameE->setEnabled(false);
    this->policeIdentifierE = new QLineEdit(this);
    this->policeIdentifierE->setGeometry(200,265,200,30);
    this->policeIdentifierE->setValidator(new QRegExpValidator(input,this));    //设置验证规则
    this->passwordE = new QLineEdit(this);
    this->passwordE->setGeometry(200,345,200,30);
    this->passwordE->setValidator(new QRegExpValidator(input,this));            //设置验证规则
    this->passwordE->setEchoMode(QLineEdit::Password);
    this->codeE = new QLineEdit(this);
    this->codeE->setGeometry(200,425,120,30);
    this->codeE->setValidator(new QRegExpValidator(input,this));                //设置验证规则

    //按钮
    this->pwdStateChange = new QPushButton(this);
//    this->pwdStateChange->setObjectName("button");
    this->pwdStateChange->setIcon(QIcon(":/data/Image/logImage/pwd_invisible.png"));
    this->pwdStateChange->setGeometry(370,345,30,30);
    this->pwdState = 0;
    this->Code = new QPushButton("获取验证码",this);
    this->Code->setObjectName("button");
    this->Code->setGeometry(320,425,80,30);
    this->yes = new QPushButton("确定",this);
    this->yes->setObjectName("button");
    this->yes->setGeometry(100,520,100,40);
    this->no = new QPushButton("取消",this);
    this->no->setObjectName("button");
    this->no->setGeometry(300,520,100,40);
}

void LogWidget::initQss()
{
    QFile file(":/data/qss/logWidget.qss");
    if(file.open(QFile::ReadOnly))
    {
        //设置给整个窗口
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void LogWidget::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap(":/data/Image/background.png").scaled(this->size()));
}

void LogWidget::closeEvent(QCloseEvent *event)
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

void LogWidget::backToMain()
{
    //表单清空
    this->policeIdentifierE->clear();
    this->passwordE->clear();
    this->codeE->clear();
    this->Code->setText("获取验证码");

    //发送返回信号
    emit backToMainSIGNAL();
}

void LogWidget::initUsingDeviceName()
{
    this->deviceName = Controller::getInstance()->getUsingDeviceName();
    this->deviceNameE->setText(deviceName[0]);
}

void LogWidget::changePwdState()
{
    //密文显示改明文显示
    if(pwdState == 0)
    {
        this->pwdState = 1;
        this->pwdStateChange->setIcon(QIcon(":/data/Image/logImage/pwd_visible.png"));
        this->passwordE->setEchoMode(QLineEdit::Normal);                        //设置明文显示
    }
    //明文显示改密文显示
    else if(pwdState == 1)
    {
        this->pwdState = 0;
        this->pwdStateChange->setIcon(QIcon(":/data/Image/logImage/pwd_invisible.png"));
        this->passwordE->setEchoMode(QLineEdit::Password);                      //设置密文显示
    }
}

void LogWidget::getCode()
{
    //设置随机种子
    QTime randtime = QTime::currentTime();
    qsrand(randtime.msec()+randtime.second()*1000);

    //根据种子生成随机验证码
    int codes = qrand()%9000+1000;                              //获取1000~9000之间的随机数

    //改变按钮控件名
    this->Code->setText(QString::number(codes,10));             //(int)codes转QStirng字符串
}

void LogWidget::logJudge()
{
    //获取表单信息
    QString policeNumber = this->policeIdentifierE->text().trimmed();   //trimmed() 剔除字符串左右两边空格
    QString password = this->passwordE->text().trimmed();
    QString code = this->codeE->text().trimmed();

    //表单验证
    if(policeNumber.isEmpty() || password.isEmpty() || code.isEmpty())
    {
        //弹窗提示
        QMessageBox::information(this,"登录提示","表单不能为空");
        this->Code->setText("重新获取");
        return;
    }
    else if(policeNumber.count()<6 || policeNumber.count()>10 )
    {
        QMessageBox::information(this,"登录提示","警员编号长度不合法(6-10个字符)");
        this->Code->setText("重新获取");
        return;
    }
    else if(password.count()<6 || password.count()>10)
    {
        QMessageBox::information(this,"登录提示","密码长度不合法(6-10个字符)");
        this->Code->setText("重新获取");
        return;
    }
    //验证验证码
    else if(code != this->Code->text())
    {
        QMessageBox::information(this,"登录提示","验证码有误");
        emit getCodeSIGNAL();
        return;
    }
    //账号密码验证
    int res = Controller::getInstance()->getPoliceByNumberAndPwd(this->policeIdentifierE->text(),this->passwordE->text());
    if (res == 1)
    {
        QMessageBox::information(this,"登录提示","登录成功");
        this->policeIdentifierE->clear();
        this->passwordE->clear();
        this->codeE->clear();
        this->Code->setText("重新获取");

        emit backToMainSIGNAL();                //返回主界面信号
        emit initLoginPoliceSIGNAL();           //初始化警员信息
        emit initVideoSavePathSIGNAL();         //初始化视频保存路径信息
        emit startEncodeThreadSIGNAL();         //开启编码录制线程

        //写入操作日志
        int deviceID = (Controller::getInstance()->getDeviceID()).toInt();  //QString转int
        int policeID = (Controller::getInstance()->getPoliceID()).toInt();  //QString转int
        QDateTime time = QDateTime::currentDateTime();
        QString dailyOP = "登录";
        Controller::getInstance()->addNewDaily(deviceID,policeID,time.toString("yyyy-MM-dd-hh-mm-ss"),dailyOP);

        return;
    }
    else if(res == 0)
    {
        QMessageBox::information(this,"登录提示","用户名或密码错误");
        this->Code->setText("重新获取");
        return;
    }
    else
    {
        QMessageBox::information(this,"登录提示","网路异常，请稍后重试");
        this->Code->setText("重新获取");
        return;
    }
}
