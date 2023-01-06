#include "welcomewidget.h"
#include <QApplication>
#include <QDebug>
#include "FFmpeg/decode.h"
#include "FFmpeg/encode.h"
#include "FFmpeg/transcode.h"
#include "view/setwidget.h"
#include "view/mainwidget.h"
#include "view/logwidget.h"
#include "view/imagewidget.h"
#include "view/enlargeimagewidget.h"
#include "view/videowidget.h"
#include "view/playvideowidget.h"
#include "view/transcodewidget.h"
#include "view/dailywidget.h"

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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeWidget w;
    w.show();
//    SetWidget sw;
//    sw.show();
//    MainWidget mw;
//    mw.show();
//    LogWidget lw;
//    lw.show();
//    ImageWidget iw;
//    iw.show();
//    VideoWidget vw;
//    vw.show();
//    PlayVideoWidget pvw;
//    pvw.show();
//    TranscodeWidget tw;
//    tw.show();
//    DailyWidget dw;
//    dw.show();

    return a.exec();
}
