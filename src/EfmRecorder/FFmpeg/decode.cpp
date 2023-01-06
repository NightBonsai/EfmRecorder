#include "decode.h"
#include <QDebug>
#include <QCameraInfo>  //需要.pro内 QT += multimedia
#include <QQueue>

Decode::Decode(QString videoPath,int decodeState)
{
    this->logState = 0;
    this->saveCover = 0;
    this->decodeState = decodeState;
    this->playState = 0;
    this->speedRate = 0;
    this->framePercentage = 0;

    //1.注册组件：所有支持的封装格式和对应的编解码器
    av_register_all();
    avdevice_register_all();

    //2.打开摄像头or打开视频
    if(decodeState == 0)
    {
        //打开摄像头
        this->getCamera();
        this->pFormatContext = avformat_alloc_context();
        AVInputFormat *fmt=av_find_input_format("dshow");
        qDebug()<<"camera";
        int res = avformat_open_input(&pFormatContext,cameraName.toStdString().c_str(),fmt,nullptr);
        if(res!=0)
        {
            qDebug()<<"open camera error";
            return;
        }
    }
    else if(decodeState == 1)
    {
        //打开视频
        this->pFormatContext = avformat_alloc_context();
        int res = avformat_open_input(&pFormatContext,videoPath.toStdString().c_str(),nullptr,nullptr);
        if(res == 0)
        {
            qDebug()<<"open success";
        }
        else
        {
            qDebug()<<"open error";
            return;
        }
    }

    //3.查找流媒体数据
    int res = avformat_find_stream_info(pFormatContext,nullptr);
    if(res >= 0)
    {
        qDebug()<<"find stream success";
    }
    else
    {
        qDebug()<<"find stream error";
        return;
    }

    //4.查找视频流下标
    videoIndex = -1;
    for(int i=0;i<pFormatContext->nb_streams;i++)
    {
        if(pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoIndex = i;
            break;
        }
    }
    if(videoIndex == -1)
    {
        qDebug()<<"find video stream error";
        return;
    }
    else
    {
        qDebug()<<"find video stream success";
    }

    //5.查找解码器
    this->pCodecContext = pFormatContext->streams[videoIndex]->codec;
    this->decoder = avcodec_find_decoder(pCodecContext->codec_id);
    if(this->decoder == nullptr)
    {
        qDebug()<<"find decoder error";
        return;
    }
    else
    {
        qDebug()<<"find decoder success "<<videoIndex;
    }

    //6.打开解码器
    res = avcodec_open2(pCodecContext,decoder,nullptr);
    if(res == 0)
    {
        qDebug()<<"open decoder success "<<videoIndex;
    }
    else
    {
        qDebug()<<"open decoder error";
        return;
    }

    //编码初始化
    this->encodeThread = new Encode(pCodecContext);

    //--------------------------------------解码----------------------------------
    //7.1.解码数据准备
    this->pkt = (AVPacket*)malloc(sizeof (AVPacket));
    int size = pCodecContext->width*pCodecContext->height;
    res = av_new_packet(pkt,size);    //新建包
    if(res == 0)
    {
        qDebug()<<"new packet success";
    }
    else
    {
        qDebug()<<"new packet error";
        return;
    }

    //7.2.AVFrame数据准备
    picture = av_frame_alloc();
    pictureRGB = av_frame_alloc();
    pictureYUV = av_frame_alloc();
    picture->width = pCodecContext->width;
    picture->height = pCodecContext->height;
    picture->format = pCodecContext->pix_fmt;
    pictureRGB->width = pCodecContext->width;
    pictureRGB->height = pCodecContext->height;
    pictureRGB->format = pCodecContext->pix_fmt;
    pictureYUV->width = pCodecContext->width;
    pictureYUV->height = pCodecContext->height;
    pictureYUV->format = pCodecContext->pix_fmt;

    //压缩数据RGB大小
    int numByteRGB = avpicture_get_size(AV_PIX_FMT_RGB32,pCodecContext->width,pCodecContext->height);
    bufferRGB = (uint8_t*)malloc(numByteRGB*sizeof(uint8_t));
    //填充数据到pictureRGB
    avpicture_fill((AVPicture*)pictureRGB,bufferRGB,AV_PIX_FMT_RGB32,pCodecContext->width,pCodecContext->height);
    //7.3.创建转置规则
    this->swsRGB = sws_getContext(pCodecContext->width,pCodecContext->height,pCodecContext->pix_fmt
                                  ,pCodecContext->width,pCodecContext->height,AV_PIX_FMT_RGB32,
                                  SWS_BICUBIC,nullptr,nullptr,nullptr);
    if(nullptr == swsRGB)
    {
        qDebug()<<"new swsRGB error";
        return;
    }
    else
    {
        qDebug()<<"new swsRGB success";
    }

    //压缩数据YUV大小
    int numByteYUV = avpicture_get_size(AV_PIX_FMT_YUV420P,pCodecContext->width,pCodecContext->height);
    bufferYUV = (uint8_t*)malloc(numByteYUV*sizeof (uint8_t));
    //填充数据到pictureYUV
    avpicture_fill((AVPicture*)pictureYUV,bufferYUV,AV_PIX_FMT_YUV420P,pCodecContext->width,pCodecContext->height);
    //7.3.创建转置规则
    this->swsYUV = sws_getContext(pCodecContext->width,pCodecContext->height,pCodecContext->pix_fmt
                                  ,pCodecContext->width,pCodecContext->height,AV_PIX_FMT_YUV420P,
                                  SWS_BICUBIC,nullptr,nullptr,nullptr);
    if(nullptr == swsYUV)
    {
        qDebug()<<"new swsYUV error";
        return;
    }
    else
    {
        qDebug()<<"new swsYUV success";
    }

    //信号与槽事件
    connect(this->encodeThread,SIGNAL(saveCoverChangeSIGNAL()),this,SLOT(saveCoverChange()));
}

Decode::~Decode()
{

}

void Decode::run()
{
    int num = 0;
    char path[100] = {0};

    //解码摄像头获取的数据
    if(decodeState == 0)
    {
        //7.4.解码函数_剔除损坏数据
        while(av_read_frame(pFormatContext,pkt) == 0)
        {
            //过滤出视频流
            if(videoIndex == pkt->stream_index)
            {
                int got_pic = -1;
                avcodec_decode_video2(pCodecContext,picture,&got_pic,pkt);      //解码函数
                if(got_pic != 0)
                {
                    //剔除损坏数据
                    sws_scale(swsRGB,picture->data,picture->linesize,0,picture->height
                              ,pictureRGB->data,pictureRGB->linesize);
                    sws_scale(swsYUV,picture->data,picture->linesize,0,picture->height
                              ,pictureYUV->data,pictureYUV->linesize);
                    this->image = QImage((uchar*)bufferRGB,pictureRGB->width,pictureRGB->height,QImage::Format_RGB32);   //创建图片

                    //发送RGB至主界面输出
                    emit sendImgSIGNAL(image);

                    //发送第一帧RGB至编码线程
                    if(saveCover == 1)
                    {
                        encodeThread->RGBcover = image;

                        saveCover = 0;
                    }
                    //发送YUV至队列
                    if(logState == 1)
                    {
                        //队列尾部添加元素
                        this->encodeThread->YUVqueue.enqueue(pictureYUV);
                    }
                }
            }
            av_packet_unref(pkt);
        }
    }
    //解码视频文件数据
    else if(decodeState == 1)
    {
        num = 0;

        while (true)
        {
            //若视频属于暂停状态
            if(playState == 0)
            {
                msleep(10);
                continue;
            }
            if(av_read_frame(pFormatContext,pkt) == 0)
            {
                //过滤出视频流
                if(videoIndex == pkt->stream_index)
                {
                    int got_pic = -1;
                    avcodec_decode_video2(pCodecContext,picture,&got_pic,pkt);      //解码函数
                    if(got_pic != 0)
                    {
                        //剔除损坏数据
                        sws_scale(swsRGB,picture->data,picture->linesize,0,picture->height
                                  ,pictureRGB->data,pictureRGB->linesize);
                        this->image = QImage((uchar*)bufferRGB,pictureRGB->width,pictureRGB->height,QImage::Format_RGB32);   //创建图片

                        //发送RGB至视频播放界面
                        emit sendImgSIGNAL(image);
                    }
                }
                av_packet_unref(pkt);

                //实现倍速切换
                switch(this->speedRate)
                {
                case 0:
                    msleep(30);
                    break;
                case 1:
                    msleep(50);
                    break;
                case 2:
                    msleep(30);
                    break;
                case 3:
                    msleep(14);
                    break;
                case 4:
                    msleep(10);
                    break;
                }
                num++;
            }
            //计算进度条百分比
            this->framePercentage = ((float)num/489)*100;
            qDebug()<<framePercentage;
        }
    }
}

void Decode::getCamera()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    int count = cameras.size();
    for(int i=0;i<count;i++)
    {
        qDebug()<<count<<cameras.at(i).description();
        this->cameraName = cameras.at(i).description();
        this->cameraName = "video="+this->cameraName;
    }
}

void Decode::logStateChange()
{
    if(logState == 0)
    {
        this->logState = 1;       
    }
    else if(logState == 1)
    {
        this->logState = 0;   
    }
}

void Decode::saveCoverChange()
{
    this->saveCover = 1;
}

void Decode::playStateChange()
{
    //暂停态变启动态
    if(playState == 0)
    {
        this->playState = 1;
        this->start();
    }
    //启动态变暂停态
    else if(playState == 1)
    {
        this->playState = 0;
//        this->terminate();
    }
}

void Decode::speedRateChange(int index)
{
    this->speedRate = index;
}
