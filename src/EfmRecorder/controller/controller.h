#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QList>
#include <QString>
#include "model/model.h"
#include "data/deviceinfo.h"
#include "data/dailyinfo.h"

//控制层单例
class Controller
{
public:
    static Controller *getInstance();

    //控制层业务
    QList<QString> getDeviceNameInfo();                                     //控制层获取设备编号功能
    DeviceInfo getDeviceInfo(QString deviceName);                           //控制层获取设备信息功能
    int setDeviceVideoPath(QString deviceVideoPath,QString deviceName);     //控制层修改设备视频存储路径
    QString getDeviceVideoPath();                                           //控制层获取设备视频存储路径
    QString getDeviceImagePath();                                           //控制层获取设备截图存储路径
    int setDeviceImagePath(QString deviceImagePath,QString deviceName);     //控制层修改设备图片存储路径
    int setDeviceState(QString deviceName);                                 //控制层修改设备使用状态
    QString getDeviceID();                                                  //控制层获取当前使用的设备id

    QList<QString> getUsingDeviceName();                                    //控制层获取使用中的设备名
    int getPoliceByNumberAndPwd(QString policeNumber,QString password);     //控制层判断警员登录
    QString getLoginPoliceName();                                           //控制层获取登录的警员名称
    QString getPoliceID();                                                  //控制层获取当前登录的警员id

    int addNewVideo(QString videoName,int deviceID,QString coverPath,QString creatTime);    //控制层将新h264视频写入视频表
    int addNewImage(QString imageName,int deviceID,QString creatTime);                      //控制层将新截屏信息写入截屏表
    int addNewDaily(int deviceID,int policeID,QString dailyTime,QString dailyOP);           //控制层将新日志信息写入日志表

    int searchImage(QString searchTime,QList<QString> &paths,QList<QString> &names);        //控制层图片模糊查询
    int searchVideo(QString searchTime,QList<QString> &paths,QList<QString> &names);        //控制层视频模糊查询
    QList<DailyInfo> searchDaily();                                                         //模型层日志查询

    QString getVideoSavePath(QString videoName);                            //控制层获取视频存储地址
    QString getVideoSaveTime(QString videoName);                            //控制层获取视频保存时间

    int endApplication();                                                   //控制层结束程序
private:
    Controller();
    static Controller *controller;
};

#endif // CONTROLLER_H
