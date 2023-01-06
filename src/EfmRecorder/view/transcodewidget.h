#ifndef TRANSCODEWIDGET_H
#define TRANSCODEWIDGET_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QProgressBar>
#include <QCloseEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include "FFmpeg/transcode.h"

//转码界面
class TranscodeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranscodeWidget(QWidget *parent = nullptr);
    ~TranscodeWidget();

    void initWin();
    void initData();
    void initControl();
    void initQss();

    void paintEvent(QPaintEvent *event);        //重绘函数重定义
    void closeEvent(QCloseEvent *event);        //右上角x退出事件重定义
private:
    QLabel *title,*inputDateL,*transcodeSelectL,*savePathL,*transcodeStateL;
    QDateTimeEdit *inputDateE;
    QListWidget *videoList;
    QListWidgetItem *videoItem;
    QComboBox *transcodeSelectE;
    QLineEdit *savePathE;
    QProgressBar *transcodeProgressBar;
    QPushButton *transcodeStart,*back,*search,*selectPath;

    int transcodeState;                          //转码状态 0-未开始 1-开始

    QList<QString> videoName;           //当前要输出的视频名称
    QList<QString> videoCoverPath;      //当前要输出的视频路径

    QString videoPath;                  //当前要转码的视频路径
    QString videoSavePath;              //转码视频的保存路径
    QString videoSaveName;              //转码视频名称

    Transcode *transcodeThread;         //转码线程
signals:
    void backToMainSIGNAL();                    //返回主界面信号
    void transcodeStateChangeSIGNAL();          //转码状态切换信号
public slots:
    void getVideo();                            //查询视频
    void setTranscodePath();                    //选择视频存储地址
    void getVideoPath(QListWidgetItem *item);   //获取编码视频地址
    void transcodeJudge();                      //编码
    void backToMain();                          //返回主界面
    void transcodeStateChange();                //转码状态切换
};

#endif // TRANSCODEWIDGET_H
