#ifndef SETWIDGET_H
#define SETWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>
#include <QComboBox>
#include <QString>
#include <QList>
#include "view/mainwidget.h"

//设置界面
class SetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SetWidget(QWidget *parent = nullptr);
    ~SetWidget();

    void initWin();
    void initData();    //初始化数据
    void initControl(); //初始化控件
    void initQss();     //初始化样式表

    void paintEvent(QPaintEvent *event);        //重绘函数重定义
    void closeEvent(QCloseEvent *event);        //右上角x退出事件重定义
    quint64  getDirSize(const QString filePath);    //统计文件夹内数据大小
private:
    QLabel *title,*deviceNameL,*memoryL,*memoryLeaveL,*videoSavePathL,*imageSavePathL;
    QLineEdit *memoryE,*memoryLeaveE,*videoSavePathE,*imageSavePathE;
    QComboBox *deviceName;
    QPushButton *videoPath,*imagePath,*yes,*no;

    MainWidget *mainWin;
    int setState;               //设置状态 0-未设置 1-已设置

    QList<QString> devices;     //存储从数据库读出的设备编号
signals:

public slots:
    void setInfo();             //设置设备
    void selectVideoPath();     //自定义视频存储路径
    void selectImagePath();     //自定义截图存储路径
    void setJudge();            //判断设备设置是否合法 合法：跳转至主界面 不合法：提示
    void renewSet();            //刷新存储路径信息
    void showSet();             //显示设置界面
};

#endif // SETWIDGET_H
