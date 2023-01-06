#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>      //照片墙控件
#include <QListWidgetItem>
#include <QList>
#include <QString>
#include <QDateTimeEdit>
#include <QCloseEvent>
#include "view/enlargeimagewidget.h"

//查看照片界面
class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr);
    ~ImageWidget();

    void initWin();
    void initData();
    void initControl();
    void initQss();

    void paintEvent(QPaintEvent *event);        //重绘函数重定义
    void closeEvent(QCloseEvent *event);        //右上角x退出事件重定义

private:
    QLabel *title,*inputDateL;
    QDateTimeEdit *inputDateE;
    QListWidget *imageList;
    QListWidgetItem *imageItem;
    QPushButton *back,*search,*moreImage;

    int imageCount;             //当前照片墙内照片个数
    QList<QString> imageName;   //当前要输出的图片名称
    QList<QString> imagePath;   //当前要输出的图片路径

    EnlargeImageWidget *enlargeImage;   //显示放大照片界面
signals:
    void backToMainSIGNAL();    //返回主界面信号
public slots:
    void getImage();        //查询照片
    void getMoreImage();    //获取更多照片
    void backToMain();      //返回主界面
    void toEnlargeImage(QListWidgetItem *item);  //显示放大照片界面
    void showImageWidget(); //显示查看照片界面
};

#endif // IMAGEWIDGET_H
