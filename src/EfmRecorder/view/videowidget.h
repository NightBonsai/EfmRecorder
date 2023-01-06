#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QCloseEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include "view/playvideowidget.h"

//查看视频界面
class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget();

    void initWin();
    void initData();
    void initControl();
    void initQss();

    void paintEvent(QPaintEvent *event);        //重绘函数重定义
    void closeEvent(QCloseEvent *event);        //右上角x退出事件重定义
private:
    QLabel *title,*inputDateL;
    QDateTimeEdit *inputDateE;
    QListWidget *videoList;
    QListWidgetItem *videoItem;
    QPushButton *back,*search,*lastPage,*nextPage;

    QList<QString> videoName;           //当前要输出的视频名称
    QList<QString> videoCoverPath;      //当前要输出的视频路径

    int videoCount;             //视频总个数
    int videoPage;              //视频总页数
    int nowPage;                //视频当前页数

    PlayVideoWidget *playVideoWin;  //视频播放界面
signals:
    void backToMainSIGNAL();    //返回主界面信号
public slots:
    void getVideo();        //查询视频
    void toLastPage();      //上一页
    void toNextPage();      //下一页
    void backToMain();      //返回主界面
    void toPlayVideo(QListWidgetItem *item);//显示视频播放界面
    void showVideoWidget();                 //显示查看视频界面
};

#endif // VIDEOWIDGET_H
