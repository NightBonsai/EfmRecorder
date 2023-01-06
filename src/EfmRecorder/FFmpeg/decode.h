#ifndef DECODE_H
#define DECODE_H

extern "C"
{
    #include "libavcodec/avcodec.h"			//编解码库，最重要的模块之一
    #include "libavdevice/avdevice.h"			//输入输出设备库；读取摄像头的
    #include "libavfilter/avfilter.h"			//音视频滤镜库；进行音视频处理与编辑
    #include "libavformat/avformat.h"			//文件格式和协议库
    #include "libavutil/avutil.h"				//音视频处理
    #include "libswresample/swresample.h"	//音频重采样
    #include "libswscale/swscale.h"			//图像进行格式转换
}

#include <QImage>
#include <QString>
#include <QImage>
#include <QThread>  //线程
#include "FFmpeg/encode.h"

//视频解码线程：解码出像素数据
class Decode:public QThread
{
    Q_OBJECT
public:
    Decode(QString videoPath="",int decodeState=0);
    ~Decode();

    void run();         //线程执行函数--重定义
    void getCamera();   //获得当前主机摄像头

    Encode *encodeThread;               //编码线程
    int framePercentage;                //帧数百分比
private:
    AVFormatContext *pFormatContext;    //封装格式上下文结构体
    AVCodecContext *pCodecContext;      //编解码器上下文结构体
    AVCodec *decoder;                   //编解码器结构体指针
    AVPacket *pkt;                      //一帧码流数据
    int videoIndex;                     //视频流下标
    AVFrame *picture,*pictureRGB,*pictureYUV;   //存储解码数据
    uint8_t *bufferRGB,*bufferYUV;
    SwsContext *swsRGB,*swsYUV;                 //RGB图像转置规则
    QImage image;                       //图像

    QString cameraName;                 //当前主机摄像头

    int decodeState;                    //解码操作 0-摄像头解码 1-视频解码
    int playState;                      //播放状态 0-视频暂停 1-视频播放
    int speedRate;                      //倍速状态

    int logState;                       //登录状态 0-未登录，不将YUV放入队列 1-登录后，开始将YUV放入队列
    int saveCover;                      //保存首帧状态 0-不保存 1-保存
signals:
    void sendImgSIGNAL(QImage);         //发送图像信号
public slots:
    void logStateChange();              //改变登录状态
    void saveCoverChange();             //改变保存首帧状态
    void playStateChange();             //改变视频播放状态
    void speedRateChange(int index);    //倍速功能实现
};

#endif // DECODE_H
