#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>

//设备数据类：存储设备数据
class DeviceInfo
{
public:
    DeviceInfo(int deviceMemory=0,QString deviceVideoPath="",QString deviceImagePath="");
    ~DeviceInfo();

    int getDeviceMemory() const;
    void setDeviceMemory(int value);
    QString getDeviceVideoPath() const;
    void setDeviceVideoPath(const QString &value);
    QString getDeviceImagePath() const;
    void setDeviceImagePath(const QString &value);

private:
    int deviceMemory;
    QString deviceVideoPath,deviceImagePath;
};

#endif // DEVICEINFO_H
