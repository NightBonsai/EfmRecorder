#include "dailyinfo.h"

DailyInfo::DailyInfo(int deviceID,int policeID,QString dailyTime,QString dailyOP)
{
    this->deviceID = deviceID;
    this->policeID = policeID;
    this->dailyTime = dailyTime;
    this->dailyOP = dailyOP;
}

DailyInfo::~DailyInfo()
{

}

int DailyInfo::getDeviceID() const
{
    return deviceID;
}

void DailyInfo::setDeviceID(int value)
{
    deviceID = value;
}

int DailyInfo::getPoliceID() const
{
    return policeID;
}

void DailyInfo::setPoliceID(int value)
{
    policeID = value;
}

QString DailyInfo::getDailyTime() const
{
    return dailyTime;
}

void DailyInfo::setDailyTime(const QString &value)
{
    dailyTime = value;
}

QString DailyInfo::getDailyOP() const
{
    return dailyOP;
}

void DailyInfo::setDailyOP(const QString &value)
{
    dailyOP = value;
}
