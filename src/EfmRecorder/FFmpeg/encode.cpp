#include "encode.h"
#include <QDebug>
#include <QDateTime>

Encode::Encode(AVCodecContext *pCodecContext)
{
    this->pCodecContext = pCodecContext;
    epktIndex = 0;
}
Encode::~Encode()
{

}

void Encode::run()
{
    this->setHeightWidth(pCodecContext->width,pCodecContext->height);
    int count = 0;
    while(true)
    {
        if(count == 0)
        {
            //编码初始化
            this->encodeInit();

            //保存封面
            emit saveCoverChangeSIGNAL();
            this->RGBcover.save(coverSavePath);
        }
        if(!YUVqueue.isEmpty())
        {
            //20秒保存一个.h264文件
            this->encodeFrame(YUVqueue.dequeue());  //删除当前队列首部数据并返回该数据
            count++;

            //写尾帧
            if(count >= 500)
            {
                this->writeTailer();
                count = 0;

                //写入数据库
                Controller::getInstance()->addNewVideo(videoName,deviceID,coverSavePath,creatTime);
            }
        }
    }
}

void Encode::encodeInit()
{
    //1.指针开空间，初始化
    eFormatContext = avformat_alloc_context();
    epkt = av_packet_alloc();

    //2.注册组件
    av_register_all();

    //3.猜测输出格式
    AVOutputFormat *outFormat = av_guess_format(nullptr,videoSavePath.toStdString().c_str(),nullptr);
    if(nullptr == outFormat)
    {
        qDebug()<<"guess error";
        return;
    }
    qDebug()<<"guess success";

    //4.设置输出格式
    eFormatContext->oformat = outFormat;

    //获取视频保存信息
    this->getVideoSavePath();

    //5.打开文件流             输入输出上下文结构体      输出文件路径              打开方式：写入
    int res = avio_open(&eFormatContext->pb,videoSavePath.toStdString().c_str(),AVIO_FLAG_WRITE);
    if(res >= 0)
    {
        qDebug()<<"open success";
    }
    else
    {
        qDebug()<<"open error";
        return;
    }

    //6.新建视频流
    AVStream *newStream = avformat_new_stream(eFormatContext,nullptr);
    if(nullptr == newStream)
    {
        qDebug()<<"newStream error";
        return;
    }
    qDebug()<<"newStream success";

    //7.查找编码器上下文结构体
    eCodecContext = newStream->codec;

    //8.基础参数设置
    eCodecContext->width = this->encoderWidth;     //变成解码器的宽高
    eCodecContext->height = this->encoderHeight;
    eCodecContext->time_base = {1,25};  //时间基--1秒播放帧数
    eCodecContext->bit_rate = 400000;   //码率
    eCodecContext->framerate = {25,1};  //帧率
    eCodecContext->gop_size = 10;       //10帧一组
    //高级参数设置
    eCodecContext->qmax = 51;
    eCodecContext->qmin = 10;
    eCodecContext->max_b_frames = 0;    //没有b帧
    eCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;    //编码器格式
    eCodecContext->codec_type = AVMEDIA_TYPE_VIDEO; //视频流
    eCodecContext->codec_id = outFormat->video_codec;   //编码器id
    //9.查找编码器
    encoder = avcodec_find_encoder(eCodecContext->codec_id);
    if(nullptr == encoder)
    {
        qDebug()<<"find encoder error";
        return;
    }
    else
    {
        qDebug()<<"find encoder success";
    }

    //10.打开编码器
    res = avcodec_open2(eCodecContext,encoder,nullptr);
    if(res == 0)
    {
        qDebug()<<"encoder open success";
    }
    else
    {
        qDebug()<<"encoder open error";
        return;
    }

    //11.写入头部信息
    res = avformat_write_header(eFormatContext,nullptr);
    if(res < 0)
    {
        qDebug()<<"write header error";
        return;
    }
    qDebug()<<"write header success";
}

void Encode::encodeFrame(AVFrame *pictureYUV)
{
    //发送一帧像素数据给编码器
    int res = avcodec_send_frame(eCodecContext,pictureYUV);
    if(res < 0)
    {
        qDebug()<<"send frame error";
    }
    else
    {
        qDebug()<<"send frame success";
    }

    //编码的一帧像素数据给编码器，可能一个pkt放不下
    while(res >= 0)
    {
        pictureYUV->pts = epktIndex++;
        qDebug()<<epktIndex;
        res = avcodec_receive_packet(eCodecContext,epkt);
        if(res == AVERROR(EAGAIN) || res == AVERROR_EOF)
        {
            qDebug()<<"receive packet error";
            break;
        }

        //写入文件
        av_interleaved_write_frame(eFormatContext,epkt);
        qDebug()<<"encode one frame success";
    }

    //pkt内容重置
    av_packet_unref(epkt);
}

void Encode::writeTailer()
{
    //写入尾帧
    av_write_trailer(eFormatContext);

    //关闭输入流--编码
    avio_close(eFormatContext->pb);

    //释放视频信息
    avformat_free_context(eFormatContext);
}

void Encode::setHeightWidth(int w, int h)
{
    this->encoderWidth = w;
    this->encoderHeight = h;
}

void Encode::getVideoSavePath()
{
    //获取当下时间
    QDateTime time = QDateTime::currentDateTime();

    this->coverSavePath.clear();
    this->videoSavePath.clear();
    this->videoSavePath = Controller::getInstance()->getDeviceVideoPath();

    //拼接视频保存信息
    this->creatTime = time.toString("yyyy-MM-dd-hh-mm-ss");                 //创建时间
    this->coverSavePath = this->videoSavePath+"/"+creatTime+".png";         //截屏保存地址
    this->videoSavePath = this->videoSavePath+"/"+creatTime+".h264";        //视频保存地址
    this->videoName = creatTime +".h264";                                   //视频名称
    this->deviceID = (Controller::getInstance()->getDeviceID()).toInt();    //设备id  QString转换为int
}

void Encode::startEncodeThread()
{
    //开启编码线程
    this->start();
}
