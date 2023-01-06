#ifndef CSINGLEDB_H
#define CSINGLEDB_H

#include <sqlite3.h>
#include <QString>

//数据库单例
class CSingleDB
{
public:
    static CSingleDB *getInstance();                                //外部接口--获取该类唯一实例
    int execSql(QString sqlCode,char **&result,int &row,int &col);  //执行sql语句   返回1-sql语句执行成功   返回0-sql语句执行失败
private:
    CSingleDB();                        //私有构造函数--保证该类只有一个实例
    ~CSingleDB();

    static CSingleDB *dataBase;         //该类唯一实例
    sqlite3 *sqlDB;                     //数据库指针
};

#endif // CSINGLEDB_H
