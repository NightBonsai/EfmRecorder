#include "transcode.h"
#include <QDebug>

Transcode::Transcode(QString inPath,QString outPath)
{
    //注册组件
    av_register_all();

    //初始化
    pFormatContext = avformat_alloc_context();
    outFormatContext = avformat_alloc_context();
    videoIndex = -1;
    pkt = (AVPacket*)malloc(sizeof (AVPacket));
    this->progressBarPercentage = 0;

    this->openH264(inPath);
    this->prepare(outPath);
}
Transcode::~Transcode()
{

}

void Transcode::run()
{
    //循环读取码流数据
    int size = newStream->codecpar->width * newStream->codecpar->height;
    av_new_packet(pkt,size);
    int frameCount = 0;
    this->progressBarPercentage = 0;

    while(av_read_frame(pFormatContext,pkt) == 0)
    {
        //判断是不是视频流
        if(pkt->stream_index == videoIndex)
        {
            //判断是否设置显示时间基：若没设置
            if(pkt->pts == AV_NOPTS_VALUE)
            {
                //时间基转换
                AVRational timeBase1=pFormatContext->streams[videoIndex]->time_base;
                //计算两帧之间的长度：转换
                int64_t duration = (double)AV_TIME_BASE/
                        av_q2d(pFormatContext->streams[videoIndex]->r_frame_rate);
                //计算显示时间基：(当前帧数*两帧之间长度)/(输入时间基*AV_TIME_BASE)
                pkt->pts = (double)(frameCount*duration)/
                        (double)(av_q2d(timeBase1)*AV_TIME_BASE);
                //解码时间基=显示时间基
                pkt->dts = pkt->pts;

                pkt->duration =duration/(double)(av_q2d(timeBase1)*AV_TIME_BASE);
            }
            //若有设置
            else if(pkt->pts < pkt->dts)
            {
                continue;
            }

            //时间基转换
            pkt->pts = av_rescale_q_rnd(pkt->pts,pFormatContext->streams[videoIndex]->time_base,newStream->time_base,
                                        (AVRounding)(AV_ROUND_INF|AV_ROUND_PASS_MINMAX));
            pkt->dts = av_rescale_q_rnd(pkt->dts,pFormatContext->streams[videoIndex]->time_base,newStream->time_base,
                                        (AVRounding)(AV_ROUND_INF|AV_ROUND_PASS_MINMAX));
            pkt->duration = av_rescale_q(pkt->duration,pFormatContext->streams[videoIndex]->time_base,
                                         newStream->time_base);
            pkt->pos = -1;
            pkt->flags|=AV_PKT_FLAG_KEY;
            pkt->stream_index = 0;

            //写入数据到输出视频信息结构体
            av_interleaved_write_frame(outFormatContext,pkt);

            //计算进度条百分比
            frameCount++;
            this->progressBarPercentage = ((float)frameCount/489)*100;
            emit sendProgressBarSIGNAL();

            msleep(5);
        }
        av_packet_unref(pkt);
    }

    //写入尾巴帧
    av_write_trailer(outFormatContext);

    //关闭编码器
    avcodec_close(outFormatContext->streams[videoIndex]->codec);
    av_freep(&outFormatContext->streams[videoIndex]->codec);

    //关闭输出流
    avio_close(outFormatContext->pb);

    //释放输出视频信息结构体
    av_free(outFormatContext);

    //关闭输入流
    avformat_close_input(&pFormatContext);

    //释放输入视频信息结构体
    av_free(pFormatContext);

    //停止转码线程
    emit transcodeStateChangeSIGNAL();
}

void Transcode::openH264(QString path)
{
    //打开h264视频文件
    int res = avformat_open_input(&pFormatContext,path.toStdString().c_str(),nullptr,nullptr);
    if(res < 0)
    {
        qDebug()<<"open h264 error";
        return;
    }
    qDebug()<<"open h264 success";

    //查找是否有流媒体数据
    res = avformat_find_stream_info(pFormatContext,nullptr);
    if(res < 0)
    {
        qDebug()<<"find stream info error";
        return;
    }
    qDebug()<<"find stream info success";

    //查找视频流
    for(int i=0;i<pFormatContext->nb_streams;i++)
    {
        if(pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoIndex = i;
            qDebug()<<"find videoIndex success";
            break;
        }
    }
    if(videoIndex == -1)
    {
        qDebug()<<"find videoIndex error";
        return;
    }
}

void Transcode::prepare(QString path)
{
    //猜测输出的格式是否存在
    outFormat = av_guess_format(nullptr,path.toStdString().c_str(),nullptr);
    if(nullptr == outFormat)
    {
        qDebug()<<"guess format error";
        return;
    }
    qDebug()<<"guess format success";

    //输出封装格式文件的格式设置
    outFormatContext->oformat = outFormat;

    //打开目标文件流
    int res = avio_open(&outFormatContext->pb,path.toStdString().c_str(),AVIO_FLAG_WRITE);
    if(res < 0)
    {
        qDebug()<<"open output file error";
        return;
    }
    qDebug()<<"open output file success";

    //新建目标文件流
    newStream = avformat_new_stream(outFormatContext,nullptr);
    if(nullptr == newStream)
    {
        qDebug()<<"new stream error";
        return;
    }
    qDebug()<<"new stream success";

    //编码器参数设置
    res = avcodec_parameters_copy(newStream->codecpar,pFormatContext->streams[videoIndex]->codecpar);
    if(res < 0)
    {
        qDebug()<<"parameter setting error";
        return;
    }
    qDebug()<<"parameter setting success";
    newStream->codecpar->codec_tag = 0;
    newStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;

    //写入头部信息
    res = avformat_write_header(outFormatContext,nullptr);
    if(res < 0)
    {
        qDebug()<<"write header error";
        return;
    }
    qDebug()<<"write header success";
}
