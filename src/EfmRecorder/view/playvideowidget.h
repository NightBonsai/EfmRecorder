#ifndef PLAYVIDEOWIDGET_H
#define PLAYVIDEOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QProgressBar> //进度条
#include <QCloseEvent>
#include <QListWidgetItem>
#include "FFmpeg/decode.h"

//视频播放界面
class PlayVideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayVideoWidget(QWidget *parent = nullptr);
    ~PlayVideoWidget();

    void initWin();
    void initData();
    void initControl();
    void initQss();

    void paintEvent(QPaintEvent *event);        //重绘函数重定义
    void closeEvent(QCloseEvent *event);        //右上角x退出事件重定义
    void getVideo();                            //获取视频并输出

    QListWidgetItem *item;                      //接收查看视频界面传来的视频信息
    QList<QString> videoNames;                  //从查看视频界面获取检索到的播放视频信息
private:
    QLabel *title,*showImage;
    QProgressBar *videoProgressBar;
    QComboBox *speedRate;
    QPushButton *back,*play,*lastVideo,*nextVideo,*catchImage;

    int playState;          //播放状态 0-暂停 1-播放
    QImage image;           //播放图片

    Decode *playThread;     //解码播放线程
signals:
    void backToVideoSIGNAL();           //返回至查看视频界面信号
public slots:
    void receiveImg(QImage image);      //获取图像
    void backToVideo();                 //返回至查看视频界面
    void playIconChange();              //切换播放图标
    void catchImageJudge();             //截屏功能实现
    void toLastVideo();                 //切换上一视频
    void toNextVideo();                 //切换下一视频
};

#endif // PLAYVIDEOWIDGET_H
