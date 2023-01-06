#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>
#include <QString>
#include <QList>

//登录界面
class LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent = nullptr);
    ~LogWidget();

    void initWin();
    void initControl();
    void initQss();

    void paintEvent(QPaintEvent *event);        //重绘函数重定义
    void closeEvent(QCloseEvent *event);        //右上角x退出事件重定义
private:
    QLabel *title,*deviceNameL,*policeIdentifierL,*passwordL,*codeL;
    QLineEdit *deviceNameE,*policeIdentifierE,*passwordE,*codeE;
    QPushButton *pwdStateChange,*Code,*yes,*no;

    int pwdState;                       //密文显示 0-密文 1-明文
    QList<QString> deviceName;                 //当前使用设备名
signals:
    void backToMainSIGNAL();    //返回主界面信号
    void getCodeSIGNAL();       //获取验证码信号
    void initLoginPoliceSIGNAL();   //获取登录警员名信号
    void initVideoSavePathSIGNAL(); //初始化视频保存路径信息
    void startEncodeThreadSIGNAL(); //开启编码线程
public slots:
    void backToMain();          //返回主界面
    void initUsingDeviceName();
    void changePwdState();      //改变密码显示 0-密文 1-明文
    void getCode();             //获取验证码
    void logJudge();            //登录验证
};

#endif // LOGWIDGET_H
