#include "csingledb.h"
#include <QDebug>
#include <sqlite3.h>

//静态成员定义
CSingleDB *CSingleDB::dataBase=nullptr;

CSingleDB *CSingleDB::getInstance()
{
    if(nullptr == CSingleDB::dataBase)
    {
        CSingleDB::dataBase = new CSingleDB();
    }
    return CSingleDB::dataBase;
}

int CSingleDB::execSql(QString sqlCode, char **&result, int &row, int &col)
{
    char *errmsg = nullptr;

    int res = sqlite3_get_table(sqlDB,sqlCode.toUtf8(),&result,&row,&col,&errmsg);
    if(res == SQLITE_OK)
    {
        return 1;       //返回1-sql语句执行成功
    }
    else
    {
        qDebug()<<sqlite3_errmsg(sqlDB);
        qDebug()<<sqlite3_errcode(sqlDB);
        return 0;       //返回0-sql语句执行失败
    }
}

CSingleDB::CSingleDB()
{
    int res = sqlite3_open("../data/db/efmRecorderDB.db",&sqlDB);
    if(res == SQLITE_OK)
    {
        qDebug()<<"open dataBase success";
    }
    else
    {
        qDebug()<<sqlite3_errmsg(sqlDB);
        qDebug()<<sqlite3_errcode(sqlDB);
    }
}

CSingleDB::~CSingleDB()
{
    sqlite3_close(sqlDB);
}
