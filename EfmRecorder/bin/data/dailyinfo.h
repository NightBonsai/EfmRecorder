#ifndef DAILYINFO_H
#define DAILYINFO_H

#include <QString>

//日志数据：存储日志数据
class DailyInfo
{
public:
    DailyInfo(int deviceID=2001,int policeID=1001,QString dailyTime="",QString dailyOP="");
    ~DailyInfo();

    int getDeviceID() const;
    void setDeviceID(int value);

    int getPoliceID() const;
    void setPoliceID(int value);

    QString getDailyTime() const;
    void setDailyTime(const QString &value);

    QString getDailyOP() const;
    void setDailyOP(const QString &value);

private:
    int deviceID,policeID;
    QString dailyTime,dailyOP;
};

#endif // DAILYINFO_H
