#ifndef ENLARGEIMAGEWIDGET_H
#define ENLARGEIMAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QListWidgetItem>

//照片放大界面
class EnlargeImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EnlargeImageWidget(QWidget *parent = nullptr);
    ~EnlargeImageWidget();

    void initWin();
    void initControl();

    bool event(QEvent *eve);    //事件重定义—实现点击窗口外部分关闭窗口
    void getEnlargeImage();     //获取图片并输出

    QListWidgetItem *item;      //接收查看照片界面传来的图片
private:
    QLabel *enlargeImage;
signals:
    void backToImageSIGNAL();     //返回至查看照片界面信号
public slots:

};

#endif // ENLARGEIMAGEWIDGET_H
