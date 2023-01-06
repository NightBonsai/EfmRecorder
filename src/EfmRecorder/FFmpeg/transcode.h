#ifndef TRANSCODE_H
#define TRANSCODE_H

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

#include <QString>
#include <QThread>

//视频转码线程：h264文件转码成常规视频封装格式(mp4，wav，avi...)
class Transcode:public QThread
{
    Q_OBJECT
public:
    Transcode(QString inPath,QString outPath);
    ~Transcode();

    void run();                         //线程执行函数—重定义
    void openH264(QString path);        //打开h264的压缩码流数据
    void prepare(QString path);         //转码得到对应封装格式

    int progressBarPercentage;          //进度条百分比
private:
    AVFormatContext *pFormatContext,*outFormatContext;  //视频信息结构体&目标视频信息结构体
    int videoIndex;                                     //码流下标
    AVOutputFormat *outFormat;
    AVStream *newStream;
    AVPacket *pkt;                                      //压缩码流数据
signals:
    void transcodeStateChangeSIGNAL();                  //转码状态切换信号
    void sendProgressBarSIGNAL();                       //进度条信号
public slots:
};

#endif // TRANSCODE_H
