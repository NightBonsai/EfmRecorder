#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QCloseEvent>
#include "FFmpeg/decode.h"
#include "view/logwidget.h"
#include "view/imagewidget.h"
#include "view/videowidget.h"
#include "view/transcodewidget.h"
#include "view/dailywidget.h"

//主界面
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void initWin();
    void initControl();
    void initQss();

    void paintEvent(QPaintEvent *event);        //重绘函数重定义
    void closeEvent(QCloseEvent *event);        //右上角x退出事件重定义
private:
    QLabel *title,*welcome,*showImage;                           //show控件输出摄像头获取的图像
    QPushButton *catchImage,*videoRecord,*imageRecord,
                *dailyRecord,*catchVideo,*setting,*login,*exit;

    int logState;                       //登录状态 0-未登录 1-已登录

    Decode *playThread;                 //播放线程
    QImage image;                       //播放图片

    LogWidget *logWin;                  //登录界面
    ImageWidget *imageWin;              //照片查看界面
    VideoWidget *videoWin;              //视频查看界面
    TranscodeWidget *transcodeWin;      //转码界面
    DailyWidget *dailyWin;              //操作日志界面

    QString loginPoliceName;            //登录后的警员名称
    QString imageSavePath,imageName,creatTime;     //图片信息
    int deviceID;
signals:
    void toSetWinSIGNAL();              //跳转至设置界面
public slots:
    void receiveImg(QImage image);      //获取图像
    void toLogWin();                    //跳转至登录界面
    void toImageWin();                  //跳转至照片查看界面
    void toVideoWin();                  //跳转至视频查看界面
    void toTranscodeWin();              //跳转至转码界面
    void toSetWin();                    //跳转至设置界面
    void toDailyWin();                  //跳转至日志界面
    void showMain();                    //显示主界面
    void initLoginPolice();
    void catchImageJudge();             //拍照功能实现
    void end();                         //退出执法记录仪
};

#endif // MAINWIDGET_H
