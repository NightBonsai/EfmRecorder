#ifndef ENCODE_H
#define ENCODE_H

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

#include <QThread>  //线程
#include <QQueue>   //队列
#include <QList>
#include <QImage>
#include <QString>
#include "controller/controller.h"

//视频编码线程：编码为H264文件
class Encode:public QThread
{
    Q_OBJECT
public:
    Encode(AVCodecContext *pCodecContext);
    ~Encode();

    void run();                             //线程执行函数—重定义
    void encodeInit();
    void encodeFrame(AVFrame *pictureYUV);
    void writeTailer();                     //写入尾帧
    void setHeightWidth(int w,int h);

    QQueue<AVFrame*> YUVqueue;              //YUV420p队列—实现多线程
    QImage RGBcover;                        //存储视频封面
private:
    AVFormatContext *eFormatContext;    //封装格式上下文结构体指针--解码&编码
    AVCodecContext *pCodecContext,*eCodecContext;      //编解码器上下文结构体指针--编码
    AVCodec *encoder;                   //编解码器--编码器
    AVPacket *epkt;                     //一帧压缩的码流数据--编码               
    int epktIndex;                      //记录帧的顺序
    int encoderWidth,encoderHeight;     //编码器宽高

    QString videoSavePath,coverSavePath,videoName,creatTime;        //视频存储信息
    int deviceID;
signals:
    void saveCoverChangeSIGNAL();       //保存视频首帧
public slots:
    void getVideoSavePath();            //获取保存h264的路径和保存.png封面的路径
    void startEncodeThread();           //开启编码线程
};

#endif // ENCODE_H
