#include "model.h"
#include <QString>
#include <QDebug>
#include <QList>

//静态数据成员声明
Model *Model::model = nullptr;

Model *Model::getInstance()
{
    if(nullptr == Model::model)
    {
        Model::model = new Model();
    }
    return Model::model;
}

int Model::getDeviceNameInfo(char **&qres, int &row, int &col)
{
    //执行查询sql语句
    QString sqlCode = "select device_name from tbl_device";

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_getDeviceNameInfo";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 1)
    {
        qDebug()<<"get deviceName success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;         //0 执行sql语句失败
}

int Model::getDeviceInfo(QString deviceName, char **&qres, int &row, int &col)
{
    QString sqlCode = QString("select device_memory,device_video_path,device_image_path from tbl_device where device_name='%1'").arg(deviceName);

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_getDeviceInfo";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 1)
    {
        qDebug()<<"get devices info success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;         //0 执行sql语句失败
}

int Model::setDeviceVideoPath(QString deviceVideoPath,QString deviceName)
{
    QString sqlCode = QString("update tbl_device set device_video_path='%1' where device_name='%2'").arg(deviceVideoPath).arg(deviceName);

    char **qres = nullptr;
    int row = 0,col = 0;
    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_setDeviceVideoPath";
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

int Model::getDeviceVideoPath(char **&qres, int &row, int &col)
{
    QString sqlCode = "select device_video_path from tbl_device where device_state='1'";

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_getDeviceVideoPath";
    if(res == 1)
    {
        qDebug()<<"get deviceVideoPath success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;         //0 执行sql语句失败
}

int Model::getDeviceImagePath(char **&qres, int &row, int &col)
{
    QString sqlCode = "select device_image_path from tbl_device where device_state='1'";

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_getDeviceImagePath";
    if(res == 1)
    {
        qDebug()<<"get deviceImagePath success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;         //0 执行sql语句失败
}

int Model::setDeviceImagePath(QString deviceImagePath, QString deviceName)
{
    QString sqlCode = QString("update tbl_device set device_image_path='%1' where device_name='%2'").arg(deviceImagePath).arg(deviceName);

    char **qres = nullptr;
    int row = 0,col = 0;
    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_setDeviceImagePath";
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

int Model::setDeviceState(QString deviceName)
{
    QString sqlCode = QString("update tbl_device set device_state=1 where device_name='%1'").arg(deviceName);

    char **qres = nullptr;
    int row = 0,col = 0;
    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_setDeviceState";
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

int Model::getUsingDeviceName(char **&qres, int &row, int &col)
{
    QString sqlCode = "select device_name from tbl_device where device_state=1";

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_getUsingDeviceName";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 1)
    {
        qDebug()<<"get UsingDeviceName success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;         //0 执行sql语句失败
}

int Model::getDeviceID(char **&qres, int &row, int &col)
{
    QString sqlCode = "select device_id from tbl_device where device_state='1'";

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_getDeviceID";
    if(res == 1)
    {
        qDebug()<<"get deviceID success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;     //0 执行sql语句失败
}

int Model::getPoliceByNumberAndPwd(QString policeNumber, QString password)
{
    QString sqlCode1 = QString("select police_name from tbl_police where police_identifier='%1' and police_password='%2'")
            .arg(policeNumber).arg(password);

    char **qres = nullptr;
    int row = 0,col = 0;
    int res = CSingleDB::getInstance()->execSql(sqlCode1,qres,row,col);
    qDebug()<<"model_getPoliceByNumberAndPwd_res="<<res;
    if(res == 1)
    {
        if(row == 1)
        {
            //更新警员登录状态
            QString sqlCode2 = QString("update tbl_police set police_state=1 where police_identifier='%1'").arg(policeNumber);
            qDebug()<<sqlCode2;
            char **qres2 = nullptr;
            int row2 = 0,col2 = 0;
            CSingleDB::getInstance()->execSql(sqlCode2,qres2,row2,col2);
        }
        return row; //0-登录失败  1-登录成功
    }
    else
    {
        return -1;
    }
}

int Model::getLoginPoliceName(char **&qres, int &row, int &col)
{
    QString sqlCode = "select police_name from tbl_police where police_state=1";

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_getLoginPoliceName";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 1)
    {
        qDebug()<<"get loginPoliceName success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;         //0 执行sql语句失败
}

int Model::getPoliceID(char **&qres, int &row, int &col)
{
    QString sqlCode = "select police_id from tbl_police where police_state='1'";

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_getPoliceID";
    if(res == 1)
    {
        qDebug()<<"get policeID success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;     //0 执行sql语句失败
}

int Model::addNewVideo(QString videoName, int deviceID, QString coverPath, QString creatTime)
{
    QString sqlCode = QString("insert into tbl_video(video_name,device_id,video_cover_path,video_create_time) "
                              "values('%1',%2,'%3','%4')").arg(videoName).arg(deviceID).arg(coverPath).arg(creatTime);
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_addNewVideo";
    if(res == 1)
    {
        qDebug()<<"add newVideo success";
        return 1;
    }
    else
    {
        qDebug()<<"add newVideo error";
        return 0;
    }
}

int Model::addNewImage(QString imageName, int deviceID, QString creatTime)
{
    QString sqlCode = QString("insert into tbl_image(image_name,device_id,image_create_time) values('%1',%2,'%3')")
            .arg(imageName).arg(deviceID).arg(creatTime);
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_addNewImage";
    if(res == 1)
    {
        qDebug()<<"add newImage success";
        return 1;
    }
    else
    {
        qDebug()<<"add newImage error";
        return 0;
    }
}

int Model::addNewDaily(int deviceID, int policeID, QString dailyTime, QString dailyOP)
{
    QString sqlCode = QString("insert into tbl_daily(device_id,police_id,daily_time,daily_operation) values(%1,%2,'%3','%4')")
            .arg(deviceID).arg(policeID).arg(dailyTime).arg(dailyOP);
    char **qres = nullptr;
    int row = 0,col = 0;

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_addNewDaily";
    if(res == 1)
    {
        qDebug()<<"add newDaily success";
        return 1;
    }
    else
    {
        qDebug()<<"add newDaily error";
        return 0;
    }
}

int Model::searchImage(QString searchTime,QList<QString> &paths,QList<QString> &names)
{
    //获取所有时间
    QString sqlCode1 = "select image_create_time from tbl_image";
    QList<QString> times;
    char **timeQres = nullptr;
    int timeRow = 0,timeCol = 0;
    char **pathQres = nullptr;
    int pathRow = 0,pathCol = 0;
    char **nameQres = nullptr;
    int nameRow = 0,nameCol = 0;

    int res = CSingleDB::getInstance()->execSql(sqlCode1,timeQres,timeRow,timeCol);
    qDebug()<<"model_searchImage";
    if(res == 1)
    {
        qDebug()<<"search imageCreateTime success";
        if(timeRow == 0)
        {
            return -1;  //-1 无数据
        }
        //返回查询到的所有图片路径&图片名
        else
        {
            for(int i=timeCol;i<(timeRow+1)*timeCol;i++)
            {
                times.push_back(timeQres[i]);
            }
            for (int i=0;i<times.count();i++)
            {
                //若查询时间存在——模糊查询
                if(strstr(times[i].toStdString().c_str(),searchTime.toStdString().c_str()) != nullptr)
                {
                    //获取截屏保存地址
                    QString sqlCode2 = QString("select a.device_image_path "
                                               "from tbl_device a left join tbl_image b on a.device_id=b.device_id "
                                               "where b.image_create_time='%1'").arg(times[i]);
                    pathQres = nullptr;
                    pathRow = 0;
                    pathCol = 0;
                    res = CSingleDB::getInstance()->execSql(sqlCode2,pathQres,pathRow,pathCol);
                    if(res == 1)
                    {
                        qDebug()<<"search imageSavePath success";
                        if(pathRow == 0)
                        {
                            return -1;  //-1 无数据
                        }
                        else if(pathRow > 0)
                        {
                            paths.push_back(pathQres[1]);
                        }
                    }
                    //获取截屏名称
                    QString sqlCode3 = QString("select image_name from tbl_image where image_create_time='%1'").arg(times[i]);
                    nameQres = nullptr;
                    nameRow = 0;
                    nameCol = 0;
                    res = CSingleDB::getInstance()->execSql(sqlCode3,nameQres,nameRow,nameCol);
                    if(res == 1)
                    {
                        qDebug()<<"search imageName success";
                        if(pathRow == 0)
                        {
                            return -1;  //-1 无数据
                        }
                        else if(pathRow > 0)
                        {
                            names.push_back(nameQres[1]);
                        }
                    }
                }
            }
        }
    }
    return res;         //0 执行sql语句失败
}

int Model::searchVideo(QString searchTime, QList<QString> &paths, QList<QString> &names)
{
    //获取所有时间
    QString sqlCode1 = "select video_create_time from tbl_video";
    QList<QString> times;
    char **timeQres = nullptr;
    int timeRow = 0,timeCol = 0;
    char **pathQres = nullptr;
    int pathRow = 0,pathCol = 0;
    char **nameQres = nullptr;
    int nameRow = 0,nameCol = 0;

    int res = CSingleDB::getInstance()->execSql(sqlCode1,timeQres,timeRow,timeCol);
    qDebug()<<"model_searchVideo";
    if(res == 1)
    {
        qDebug()<<"search videoCreateTime success";
        if(timeRow == 0)
        {
            return -1;  //-1 无数据
        }
        //返回查询到的所有视频封面路径&视频名
        else
        {
            for(int i=timeCol;i<(timeRow+1)*timeCol;i++)
            {
                times.push_back(timeQres[i]);
            }
            for (int i=0;i<times.count();i++)
            {
                //若查询时间存在——模糊查询
                if(strstr(times[i].toStdString().c_str(),searchTime.toStdString().c_str()) != nullptr)
                {
                    //获取视频封面保存地址
                    QString sqlCode2 = QString("select video_cover_path from tbl_video where video_create_time='%1'").arg(times[i]);
                    pathQres = nullptr;
                    pathRow = 0;
                    pathCol = 0;
                    res = CSingleDB::getInstance()->execSql(sqlCode2,pathQres,pathRow,pathCol);
                    if(res == 1)
                    {
                        qDebug()<<"search videoSavePath success";
                        if(pathRow == 0)
                        {
                            return -1;  //-1 无数据
                        }
                        else if(pathRow > 0)
                        {
                            paths.push_back(pathQres[1]);
                        }
                    }
                    //获取视频名称
                    QString sqlCode3 = QString("select video_name from tbl_video where video_create_time='%1'").arg(times[i]);
                    nameQres = nullptr;
                    nameRow = 0;
                    nameCol = 0;
                    res = CSingleDB::getInstance()->execSql(sqlCode3,nameQres,nameRow,nameCol);
                    if(res == 1)
                    {
                        qDebug()<<"search imageName success";
                        if(pathRow == 0)
                        {
                            return -1;  //-1 无数据
                        }
                        else if(pathRow > 0)
                        {
                            names.push_back(nameQres[1]);
                        }
                    }
                }
            }
        }
    }
    return res;         //0 执行sql语句失败
}

int Model::searchDaily(char **&qres, int &row, int &col)
{
    QString sqlCode = "select device_id,police_id,daily_time,daily_operation from tbl_daily";

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_searchDaily";
    if(res == 1)
    {
        qDebug()<<"search daily success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;     //0 执行sql语句失败
}

int Model::getVideoSavePath(QString videoName, char **&qres, int &row, int &col)
{
    //根据视频名称查找使用的设备的id
    QString sqlCode1 = QString("select device_id from tbl_video where video_name='%1'").arg(videoName);
    char **IDqres = nullptr;
    int IDrow = 0,IDcol = 0;
    int res = CSingleDB::getInstance()->execSql(sqlCode1,IDqres,IDrow,IDcol);
    qDebug()<<"model_getVideoSavePath";
    if(res == 1)
    {
        qDebug()<<"search deviceID success";
        if(IDrow == 0)
        {
            return -1;  //无数据
        }
        else
        {
            //根据设备id查找视频存储路径
            QString sqlCode2 = QString("select device_video_path from tbl_device where device_id='%1'").arg(IDqres[1]);

            res = CSingleDB::getInstance()->execSql(sqlCode2,qres,row,col);
            if(res == 1)
            {
                qDebug()<<"search videoSavePath success";
                if(row == 0)
                {
                    return -1;  //无数据
                }
                else
                {
                    return row; //有数据
                }
            }
            return res;         //0—执行语句失败
        }
    }
}

int Model::getVideoSaveTime(QString videoName, char **&qres, int &row, int &col)
{
    QString sqlCode = QString("select video_create_time from tbl_video where video_name='%1'").arg(videoName);

    int res = CSingleDB::getInstance()->execSql(sqlCode,qres,row,col);
    qDebug()<<"model_getVideoSaveTime";
    qDebug()<<"row="<<row<<" col="<<col;
    if(res == 1)
    {
        qDebug()<<"get videoSaveTime success";
        if(row == 0)
        {
            return -1;  //-1 无数据
        }
        else
        {
            return row; //有数据
        }
    }
    return res;         //0 执行sql语句失败
}

int Model::endApplication()
{
    QString sqlCode1 = "update tbl_police set police_state='0'";
    QString sqlCode2 = "update tbl_device set device_state='0'";

    char **qres = nullptr;
    int row = 0,col = 0;
    int res1 = CSingleDB::getInstance()->execSql(sqlCode1,qres,row,col);
    int res2 = CSingleDB::getInstance()->execSql(sqlCode2,qres,row,col);
    qDebug()<<"model_endApplication";
    if(res1 == 1 && res2 == 1)
    {
        qDebug()<<"end application success";
        return 1;
    }
    else
    {
        qDebug()<<"end application error";
        return 0;
    }

}

Model::Model()
{

}
