#include "deviceinfo.h"


DeviceInfo::DeviceInfo(int deviceMemory, QString deviceVideoPath, QString deviceImagePath)
{
    this->deviceMemory = deviceMemory;
    this->deviceVideoPath = deviceVideoPath;
    this->deviceImagePath = deviceImagePath;
}

DeviceInfo::~DeviceInfo()
{

}

int DeviceInfo::getDeviceMemory() const
{
    return deviceMemory;
}

void DeviceInfo::setDeviceMemory(int value)
{
    deviceMemory = value;
}

QString DeviceInfo::getDeviceVideoPath() const
{
    return deviceVideoPath;
}

void DeviceInfo::setDeviceVideoPath(const QString &value)
{
    deviceVideoPath = value;
}

QString DeviceInfo::getDeviceImagePath() const
{
    return deviceImagePath;
}

void DeviceInfo::setDeviceImagePath(const QString &value)
{
    deviceImagePath = value;
}
