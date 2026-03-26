#include <sstream>
#include <util/tc_common.h>
#include "DBOperator.h"
#include "globe.h"
#include "LogComm.h"

//
using namespace wbl;

/**
 *
*/
CDBOperator::CDBOperator()
{

}

/**
 *
*/
CDBOperator::~CDBOperator()
{

}

//初始化
int CDBOperator::init()
{
    FUNC_ENTRY("");

    int iRet = 0;

    try
    {
        //for test
        map<string, string> mpParam;
        mpParam["dbhost"]  = "localhost";
        mpParam["dbuser"]  = "tars";
        mpParam["dbpass"]  = "tars2015";
        mpParam["dbname"]  = "config";
        mpParam["charset"] = "utf8";
        mpParam["dbport"]  = "3306";

        TC_DBConf dbConf;
        dbConf.loadFromMap(mpParam);

        //初始化数据库连接
        m_mysqlObj.init(dbConf);
    }
    catch (exception &e)
    {
        iRet = -1;
        ROLLLOG_ERROR << "Catch exception: " << e.what() << endl;
    }
    catch (...)
    {
        iRet = -2;
        ROLLLOG_ERROR << "Catch unknown exception." << endl;
    }

    FUNC_EXIT("", iRet);
    return iRet;
}

//初始化
int CDBOperator::init(const string &dbhost, const string &dbuser, const string &dbpass, const string &dbname, const string &charset, const string &dbport)
{
    FUNC_ENTRY("");

    int iRet = 0;

    try
    {
        map<string, string> mpParam;
        mpParam["dbhost"]  = dbhost;
        mpParam["dbuser"]  = dbuser;
        mpParam["dbpass"]  = dbpass;
        mpParam["dbname"]  = dbname;
        mpParam["charset"] = charset;
        mpParam["dbport"]  = dbport;

        TC_DBConf dbConf;
        dbConf.loadFromMap(mpParam);

        m_mysqlObj.init(dbConf);
    }
    catch (exception &e)
    {
        iRet = -1;
        ROLLLOG_ERROR << "Catch exception: " << e.what() << endl;
    }
    catch (...)
    {
        iRet = -2;
        ROLLLOG_ERROR << "Catch unknown exception." << endl;
    }

    FUNC_EXIT("", iRet);

    return iRet;
}

//初始化
int CDBOperator::init(const TC_DBConf &dbConf)
{
    FUNC_ENTRY("");

    int iRet = 0;

    try
    {
        //初始化数据库连接
        m_mysqlObj.init(dbConf);
    }
    catch (exception &e)
    {
        iRet = -1;
        ROLLLOG_ERROR << "Catch exception: " << e.what() << endl;
    }
    catch (...)
    {
        iRet = -2;
        ROLLLOG_ERROR << "Catch unknown exception." << endl;
    }

    FUNC_EXIT("", iRet);

    return iRet;
}

int CDBOperator::SelectUserGameStatFromDB(const string sFilter, vector<map<string, string>>& vecGameStat)
{
    FUNC_ENTRY("");

    WriteLocker lock(m_rwlock);

    int iRet = 0;

    try
    {
        string strSQL = "SELECT uid, room_index, reward_info, log_time from tb_game_stats where rank = '1' " \
                        "and uid in (select tb_userinfo.uid from tb_userinfo, tb_useraccount where "\
                        "tb_userinfo.uid = tb_useraccount.uid and ((idc_verify = 1 and realname LIKE '%" + sFilter + "%') "\
                        "or (idc_verify = 0 and nickname LIKE '%" + sFilter +  "%')))";
        TC_Mysql::MysqlData res = m_mysqlObj.queryRecord(strSQL);
        ROLLLOG_DEBUG << "Execute SQL: [" << strSQL << "], return " << res.size() << " records." << endl;
        if (res.size() <= 0)
        {
            ROLLLOG_WARN << " no data." << endl;
            return 0;
        }

        for (size_t i = 0; i < res.size(); ++i)
        {
            map<string, string> mapRecord;
            mapRecord.insert(std::make_pair("uid", res[i]["uid"]));
            mapRecord.insert(std::make_pair("room_index", res[i]["room_index"]));
            mapRecord.insert(std::make_pair("reward_info", res[i]["reward_info"]));
            mapRecord.insert(std::make_pair("log_time", res[i]["log_time"]));
            vecGameStat.push_back(mapRecord);
        }
    }
    catch (TC_Mysql_Exception &e)
    {
        ROLLLOG_DEBUG << "select operator catch mysql exception: " << e.what() << endl;
        iRet = -1;
    }
    catch (...)
    {
        ROLLLOG_DEBUG << "select operator catch unknown exception." << endl;
        iRet = -2;
    }

    FUNC_EXIT("", iRet);
    return iRet;
}

int CDBOperator::SelectCheatFromDB(const int iCheatType, const string sFilter, vector<map<string, string>>& vecCheatGame)
{
    FUNC_ENTRY("");

    WriteLocker lock(m_rwlock);

    int iRet = 0;

    try
    {
        string strSQL = "SELECT cheat_index, cheat_type, relate_id, game_time from tb_game_cheat where cheat_type = " + I2S(iCheatType);
        if(iCheatType == 0)
        {
            strSQL += " and cheat_index in (select tb_userinfo.uid from tb_userinfo, tb_useraccount where "\
                        "tb_userinfo.uid = tb_useraccount.uid and ((idc_verify = 1 and realname LIKE '%" + sFilter + "%') "\
                        "or (idc_verify = 0 and nickname LIKE '%" + sFilter +  "%')))";
        }
        else
        {
            strSQL += " and cheat_index in (select room_index from tb_game_brief WHERE room_name like '%" + sFilter +  "%')";
        }
        strSQL += " order by game_time desc;";

        TC_Mysql::MysqlData res = m_mysqlObj.queryRecord(strSQL);
        ROLLLOG_DEBUG << "Execute SQL: [" << strSQL << "], return " << res.size() << " records." << endl;
        if (res.size() <= 0)
        {
            ROLLLOG_WARN << " no data." << endl;
            return 0;
        }

        for (size_t i = 0; i < res.size(); ++i)
        {
            map<string, string> mapRecord;
            mapRecord.insert(std::make_pair("detail_index", res[i]["detail_index"]));
            mapRecord.insert(std::make_pair("cheat_type", res[i]["cheat_type"]));
            mapRecord.insert(std::make_pair("relate_id", res[i]["relate_id"]));
            mapRecord.insert(std::make_pair("game_time", res[i]["game_time"]));
            vecCheatGame.push_back(mapRecord);
        }
    }
    catch (TC_Mysql_Exception &e)
    {
        ROLLLOG_DEBUG << "select operator catch mysql exception: " << e.what() << endl;
        iRet = -1;
    }
    catch (...)
    {
        ROLLLOG_DEBUG << "select operator catch unknown exception." << endl;
        iRet = -2;
    }

    FUNC_EXIT("", iRet);
    return iRet;
}


//加载配置数据
int CDBOperator::loadConfig()
{
    FUNC_ENTRY("");

    int iRet = 0;

    FUNC_EXIT("", iRet);

    return iRet;
}

