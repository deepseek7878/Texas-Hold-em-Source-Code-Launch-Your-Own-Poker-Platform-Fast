#ifndef _DB_OPERATOR_H_
#define _DB_OPERATOR_H_

#include <string>
#include <map>
#include <vector>

#include <util/tc_config.h>
#include <util/tc_mysql.h>
#include <util/tc_singleton.h>
#include <util/tc_autoptr.h>

///
#include <wbl/pthread_util.h>

#include "GameRecordServer.h"
//
using namespace std;
using namespace tars;
using namespace wbl;

/**
*
* DB操作类
*/
class CDBOperator : public TC_HandleBase
{
public:
    /**
     *
    */
    CDBOperator();

    /**
     *
    */
    ~CDBOperator();

public:
    /**
    * 初始化
    */
    int init();
    int init(const TC_DBConf &dbConf);
    int init(const string &dbhost, const string &dbuser, const string &dbpass,
             const string &dbname, const string &charset, const string &dbport);

public:
    //加载配置数据
    int loadConfig();

    int SelectUserGameStatFromDB(const string sFilter, vector<map<string, string>>& vecGameStat);

    int SelectCheatFromDB(const int iCheatType, const string sFilter, vector<map<string, string>>& vecCheatGame);

private:
    wbl::ReadWriteLocker m_rwlock; //读写锁，防止数据脏读

private:
    TC_Mysql m_mysqlObj; //mysql操作对象
};

//singleton
typedef TC_Singleton<CDBOperator, CreateStatic, DefaultLifetime> DBOperatorSingleton;

//ptr
typedef TC_AutoPtr<CDBOperator> CDBOperatorPtr;

#endif


