#include "controller.h"
#include <QDebug>
#include <QCryptographicHash>   //MD5加密库

//静态数据成员声明
Controller *Controller::controller = nullptr;

Controller *Controller::getInstance()
{
    if(nullptr == Controller::controller)
    {
        Controller::controller = new Controller();
    }
    return Controller::controller;
}

QList<QString> Controller::getDeviceNameInfo()
{
    QList<QString> devices;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getDeviceNameInfo(qres,row,col);
    qDebug()<<"controller_getDeviceNameInfo";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return devices;
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return devices;
    }
    else
    {
        qDebug()<<"get deviceName info success";
        for(int i=col;i<=row;i++)
        {
            devices.push_back(qres[i]);
        }
        return devices;
    }
}

DeviceInfo Controller::getDeviceInfo(QString deviceName)
{
    QList<DeviceInfo> devices;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getDeviceInfo(deviceName,qres,row,col);
    qDebug()<<"controller_getDeviceInfo";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return devices[0];
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return devices[0];
    }
    else
    {
        qDebug()<<"get deviceName info success";
        for(int i=1;i<=row;i++)
        {
            devices.push_back(DeviceInfo(atoi(qres[i*col]),qres[i*col+1],qres[i*col+2]));
        }
        return devices[0];
    }
}

int Controller::setDeviceVideoPath(QString deviceVideoPath, QString deviceName)
{
    int res = Model::getInstance()->setDeviceVideoPath(deviceVideoPath,deviceName);
    qDebug()<<"controller_setDeviceVideoPath";
    if(res == 1)
    {
        qDebug()<<"set deviceVideoPath success";
        return 1;
    }
    else
    {
        qDebug()<<"set deviceVideoPath error";
        return 0;
    }
}

QString Controller::getDeviceVideoPath()
{
    QList<QString> deviceVideoPath;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getDeviceVideoPath(qres,row,col);
    qDebug()<<"controller_getDeviceVideoPath";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return deviceVideoPath[0];
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return deviceVideoPath[0];
    }
    else
    {
        qDebug()<<"get deviceVideoPath success";
        for(int i=col;i<(row+1)*col;i++)
        {
            deviceVideoPath.push_back(qres[i]);
        }
        return deviceVideoPath[0];
    }
}

QString Controller::getDeviceImagePath()
{
    QList<QString> deviceImagePath;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getDeviceImagePath(qres,row,col);
    qDebug()<<"controller_getDeviceImagePath";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return deviceImagePath[0];
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return deviceImagePath[0];
    }
    else
    {
        qDebug()<<"get deviceImagePath success";
        for(int i=col;i<(row+1)*col;i++)
        {
            deviceImagePath.push_back(qres[i]);
        }
        return deviceImagePath[0];
    }
}

int Controller::setDeviceImagePath(QString deviceImagePath, QString deviceName)
{
    int res = Model::getInstance()->setDeviceVideoPath(deviceImagePath,deviceName);
    qDebug()<<"controller_setDeviceImagePath";
    if(res == 1)
    {
        qDebug()<<"set deviceImagePath success";
        return 1;
    }
    else
    {
        qDebug()<<"set deviceImagePath error";
        return 0;
    }
}

int Controller::setDeviceState(QString deviceName)
{
    int res = Model::getInstance()->setDeviceState(deviceName);
    qDebug()<<"controller_setDeviceState";
    if(res == 1)
    {
        qDebug()<<"set deviceState success";
        return 1;
    }
    else
    {
        qDebug()<<"set deviceState error";
        return 0;
    }
}

QString Controller::getDeviceID()
{
    QList<QString> deviceID;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getDeviceID(qres,row,col);
    qDebug()<<"controller_getDeviceID";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return deviceID[0];
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return deviceID[0];
    }
    else
    {
        qDebug()<<"get usingDeviceName success";
        for(int i=col;i<(row+1)*col;i++)
        {
            deviceID.push_back(qres[i]);
        }
        return deviceID[0];
    }
}

QList<QString> Controller::getUsingDeviceName()
{
    QList<QString> usingDeviceName;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getUsingDeviceName(qres,row,col);
    qDebug()<<"controller_getUsingDeviceName";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return usingDeviceName;
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return usingDeviceName;
    }
    else
    {
        qDebug()<<"get usingDeviceName success";
        for(int i=col;i<(row+1)*col;i++)
        {
            usingDeviceName.push_back(qres[i]);
        }
        return usingDeviceName;
    }
}

int Controller::getPoliceByNumberAndPwd(QString policeNumber, QString password)
{
    //密码MD5加密
    QByteArray MD5 = QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Md5);
    QString passwordMD5;
    passwordMD5.append(MD5.toHex());

    int res = Model::getInstance()->getPoliceByNumberAndPwd(policeNumber,passwordMD5);
    qDebug()<<"controller_getPoliceByNumberAndPwd";
    return res;
}

QString Controller::getLoginPoliceName()
{
    QList<QString> loginPoliceName;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getLoginPoliceName(qres,row,col);
    qDebug()<<"controller_getLoginPoliceName";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return loginPoliceName[0];
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return loginPoliceName[0];
    }
    else
    {
        qDebug()<<"get loginPoliceName success";
        for(int i=col;i<(row+1)*col;i++)
        {
            loginPoliceName.push_back(qres[i]);
        }
        return loginPoliceName[0];
    }
}

QString Controller::getPoliceID()
{
    QList<QString> policeID;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getPoliceID(qres,row,col);
    qDebug()<<"controller_getPoliceID";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return policeID[0];
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return policeID[0];
    }
    else
    {
        qDebug()<<"get usingPoliceID success";
        for(int i=col;i<(row+1)*col;i++)
        {
            policeID.push_back(qres[i]);
        }
        return policeID[0];
    }
}

int Controller::addNewVideo(QString videoName, int deviceID, QString coverPath, QString creatTime)
{
    int res = Model::getInstance()->addNewVideo(videoName,deviceID,coverPath,creatTime);
    return res;
}

int Controller::addNewImage(QString imageName, int deviceID, QString creatTime)
{
    int res = Model::getInstance()->addNewImage(imageName,deviceID,creatTime);
    return res;
}

int Controller::addNewDaily(int deviceID, int policeID, QString dailyTime, QString dailyOP)
{
    int res = Model::getInstance()->addNewDaily(deviceID,policeID,dailyTime,dailyOP);
    return res;
}

int Controller::searchImage(QString searchTime, QList<QString> &paths, QList<QString> &names)
{
    int res = Model::getInstance()->searchImage(searchTime,paths,names);
    return res;
}

int Controller::searchVideo(QString searchTime, QList<QString> &paths, QList<QString> &names)
{
    int res = Model::getInstance()->searchVideo(searchTime,paths,names);
    return res;
}

QList<DailyInfo> Controller::searchDaily()
{
    QList<DailyInfo> dailys;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->searchDaily(qres,row,col);
    qDebug()<<"controller_searchDaily";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return dailys;
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return dailys;
    }
    else
    {
        qDebug()<<"get videoSavePath success";
        for(int i=1;i<=row;i++)
        {
            dailys.push_back(DailyInfo(atoi(qres[i*col]),atoi(qres[i*col+1]),qres[i*col+2],qres[i*col+3]));
        }
        return dailys;
    }
}

QString Controller::getVideoSavePath(QString videoName)
{
    QList<QString> videoSavePath;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getVideoSavePath(videoName,qres,row,col);
    qDebug()<<"controller_getVideoSavePath";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return videoSavePath[0];
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return videoSavePath[0];
    }
    else
    {
        qDebug()<<"get videoSavePath success";
        for(int i=col;i<(row+1)*col;i++)
        {
            videoSavePath.push_back(qres[i]);
        }
        return videoSavePath[0];
    }
}

QString Controller::getVideoSaveTime(QString videoName)
{
    QList<QString> videoSaveTime;
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = Model::getInstance()->getVideoSaveTime(videoName,qres,row,col);
    qDebug()<<"controller_getVideoSaveTime";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 0)
    {
        qDebug()<<"sql error";
        return videoSaveTime[0];
    }
    else if(res == -1)
    {
        qDebug()<<"no data";
        return videoSaveTime[0];
    }
    else
    {
        qDebug()<<"get videoSaveTime success";
        for(int i=col;i<(row+1)*col;i++)
        {
            videoSaveTime.push_back(qres[i]);
        }
        return videoSaveTime[0];
    }
}

int Controller::endApplication()
{
    int res = Model::getInstance()->endApplication();
    return res;
}

Controller::Controller()
{

}
