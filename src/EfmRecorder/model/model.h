#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include "model/csingledb.h"

//模型层单例
class Model
{
public:
    static Model *getInstance();

    //模型层业务
    int getDeviceNameInfo(char **&qres,int &row,int &col);                  //模型层获取设备编号功能
    int getDeviceInfo(QString deviceName,char **&qres,int &row,int &col);   //模型层获取设备信息功能
    int setDeviceVideoPath(QString deviceVideoPath,QString deviceName);     //模型层修改设备视频存储路径
    int getDeviceVideoPath(char **&qres,int &row,int &col);                 //模型层获取设备视频存储路径
    int getDeviceImagePath(char **&qres,int &row,int &col);                 //模型层获取设备截图存储路径
    int setDeviceImagePath(QString deviceImagePath,QString deviceName);     //模型层修改设备图片存储路径
    int setDeviceState(QString deviceName);                                 //模型层修改设备使用状态
    int getUsingDeviceName(char **&qres,int &row,int &col);                 //模型层获取使用中的设备名
    int getDeviceID(char **&qres,int &row,int &col);                        //模型层获取当前使用的设备id

    int getPoliceByNumberAndPwd(QString policeNumber,QString password);     //模型层判断警员登录
    int getLoginPoliceName(char **&qres,int &row,int &col);                 //模型层获取登录中的警员姓名
    int getPoliceID(char **&qres,int &row,int &col);                        //模型层获取当前登录的警员id

    int addNewVideo(QString videoName,int deviceID,QString coverPath,QString creatTime);    //模型层将新视频信息写入视频表
    int addNewImage(QString imageName,int deviceID,QString creatTime);                      //模型层将新截屏信息写入截屏表
    int addNewDaily(int deviceID,int policeID,QString dailyTime,QString dailyOP);           //模型层将新日志信息写入日志表

    int searchImage(QString searchTime,QList<QString> &paths,QList<QString> &names);        //模型层图片模糊查询
    int searchVideo(QString searchTime,QList<QString> &paths,QList<QString> &names);        //模型层视频模糊查询
    int searchDaily(char **&qres,int &row,int &col);                                        //模型层日志查询

    int getVideoSavePath(QString videoName,char **&qres,int &row,int &col);                 //模型层获取视频存储地址
    int getVideoSaveTime(QString videoName,char **&qres,int &row,int &col);                 //模型层获取视频保存时间

    int endApplication();                                                   //模型层结束程序
private:
    Model();
    static Model *model;
};

#endif // MODEL_H
