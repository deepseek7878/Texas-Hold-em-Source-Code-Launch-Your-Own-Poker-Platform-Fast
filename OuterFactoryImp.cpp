#include <sstream>
#include "OuterFactoryImp.h"
#include "LogComm.h"
#include "GameRecordServer.h"

//
using namespace wbl;

/**
 *
*/
OuterFactoryImp::OuterFactoryImp() : _pFileConf(NULL)
{
    createAllObject();
}


/**
 *
*/
OuterFactoryImp::~OuterFactoryImp()
{
    deleteAllObject();
}

/**
 *
*/
void OuterFactoryImp::deleteAllObject()
{
    if (_pFileConf)
    {
        delete _pFileConf;
        _pFileConf = NULL;
    }
}

/**
 *
*/
void OuterFactoryImp::createAllObject()
{
    try
    {
        //
        deleteAllObject();

        //本地配置文件
        _pFileConf = new tars::TC_Config();
        if (!_pFileConf)
        {
            ROLLLOG_ERROR << "create config parser fail, ptr null." << endl;
            terminate();
        }

        //tars代理Factory,访问其他tars接口时使用
        _pProxyFactory = new OuterProxyFactory();
        if (!_pProxyFactory)
        {
            ROLLLOG_ERROR << "create outer proxy factory fail, ptr null." << endl;
            terminate();
        }

        LOG_DEBUG << "init proxy factory succ." << endl;
        load();
    }
    catch (TC_Exception &ex)
    {
        LOG->error() << ex.what() << endl;
    }
    catch (exception &e)
    {
        LOG->error() << e.what() << endl;
    }
    catch (...)
    {
        LOG->error() << "unknown exception." << endl;
    }
}

//读取所有配置
void OuterFactoryImp::load()
{
    __TRY__

    //拉取远程配置
    g_app.addConfig(ServerConfig::ServerName + ".conf");

    wbl::WriteLocker lock(m_rwlock);

    _pFileConf->parseFile(ServerConfig::BasePath + ServerConfig::ServerName + ".conf");
    LOG_DEBUG << "init config file succ : " << ServerConfig::BasePath + ServerConfig::ServerName + ".conf" << endl;

    //代理配置
    readPrxConfig();
    printPrxConfig();

    //数据库配置
    loadDBConfig();

    if(getConfigServantPrx()->listProps(mapPropsConfig) != 0)
    {
        LOG_ERROR << "read props err."<< endl;
        return;
    }

    __CATCH__
}

//域名解析
string OuterFactoryImp::getIp(const string &domain)
{
    if(domain.length() == 0)
    {
        return "";
    }

    struct hostent host = *gethostbyname(domain.c_str());
    for (int i = 0; host.h_addr_list[i]; i++)
    {
        string ip = inet_ntoa(*(struct in_addr *)host.h_addr_list[i]);
        return ip;
    }

    return "";
}

// 读取db配置
void OuterFactoryImp::loadDBConfig()
{
    dbConf.Domain = (*_pFileConf).get("/Main/db<domain>", "");
    dbConf.Host = (*_pFileConf).get("/Main/db<host>", "");
    dbConf.port = (*_pFileConf).get("/Main/db<port>", "3306");
    dbConf.user = (*_pFileConf).get("/Main/db<user>", "tars");
    dbConf.password = (*_pFileConf).get("/Main/db<password>", "tars2015");
    dbConf.charset = (*_pFileConf).get("/Main/db<charset>", "utf8mb4");
    dbConf.dbname = (*_pFileConf).get("/Main/db<dbname>", "");

    //域名
    if (dbConf.Domain.length() > 0)
    {
        string szHost = getIp(dbConf.Domain);
        if (szHost.length() > 0)
        {
            dbConf.Host = szHost;
            ROLLLOG_DEBUG << "get host by domain, Domain: " << dbConf.Domain << ", szHost: " << szHost << endl;
        }
    }
}

//代理配置
void OuterFactoryImp::readPrxConfig()
{
    _ConfigServantObj = (*_pFileConf).get("/Main/Interface/ConfigServer<ProxyObj>", "");
    _DBAgentServantObj = (*_pFileConf).get("/Main/Interface/DBAgentServer<ProxyObj>", "");
    _HallServantObj = (*_pFileConf).get("/Main/Interface/HallServer<ProxyObj>", "");
}

//打印代理配置
void OuterFactoryImp::printPrxConfig()
{
    FDLOG_CONFIG_INFO << "_ConfigServantObj ProxyObj : " << _ConfigServantObj << endl;
    FDLOG_CONFIG_INFO << "_DBAgentServantObj ProxyObj : " << _DBAgentServantObj << endl;
    FDLOG_CONFIG_INFO << "_HallServantObj ProxyObj : " << _HallServantObj << endl;
}

//游戏配置服务代理
const ConfigServantPrx OuterFactoryImp::getConfigServantPrx()
{
    if (!_ConfigServerPrx)
    {
        _ConfigServerPrx = Application::getCommunicator()->stringToProxy<ConfigServantPrx>(_ConfigServantObj);
        ROLLLOG_DEBUG << "Init _ConfigServantObj succ, _ConfigServantObj : " << _ConfigServantObj << endl;
    }

    return _ConfigServerPrx;
}

//数据库代理服务代理
const DBAgentServantPrx OuterFactoryImp::getDBAgentServantPrx(const long uid)
{
    if (!_DBAgentServerPrx)
    {
        _DBAgentServerPrx = Application::getCommunicator()->stringToProxy<dbagent::DBAgentServantPrx>(_DBAgentServantObj);
        ROLLLOG_DEBUG << "Init _DBAgentServantObj succ, _DBAgentServantObj : " << _DBAgentServantObj << endl;
    }

    if (_DBAgentServerPrx)
    {
        return _DBAgentServerPrx->tars_hash(uid);
    }

    return NULL;
}

//数据库代理服务代理
const DBAgentServantPrx OuterFactoryImp::getDBAgentServantPrx(const string key)
{
    if (!_DBAgentServerPrx)
    {
        _DBAgentServerPrx = Application::getCommunicator()->stringToProxy<dbagent::DBAgentServantPrx>(_DBAgentServantObj);
        ROLLLOG_DEBUG << "Init _DBAgentServantObj succ, _DBAgentServantObj : " << _DBAgentServantObj << endl;
    }

    if (_DBAgentServerPrx)
    {
        return _DBAgentServerPrx->tars_hash(tars::hash<string>()(key));
    }

    return NULL;
}

//活动代理服务代理
const HallServantPrx OuterFactoryImp::getHallServantPrx(const long uid)
{
    if (!_HallServantPrx)
    {
        _HallServantPrx = Application::getCommunicator()->stringToProxy<hall::HallServantPrx>(_HallServantObj);
        ROLLLOG_DEBUG << "Init _HallServantObj succ, _HallServantObj : " << _HallServantObj << endl;
    }

    if (_HallServantPrx)
    {
        return _HallServantPrx->tars_hash(uid);
    }

    return NULL;
}

int OuterFactoryImp::parseProps(string szSrc, map<string, string>& result)
{
    vector<string> vecItem = SEPSTR(szSrc, ";");
    for(auto item : vecItem)
    {
        vector<string> vecValue = SEPSTR(item, ":");
        if(vecValue.size() != 2)
        {
            continue;
        }
        result.insert(std::make_pair(vecValue[0], vecValue[1]));
    }
    return 0;
}

//
string OuterFactoryImp::getPorpsSmallIconByID(const int iPropsID)
{
    auto it = mapPropsConfig.data.find(iPropsID);
    if(it == mapPropsConfig.data.end())
    {
        return "";
    }
    return it->second.propsIconSmall;
}

string OuterFactoryImp::getPorpsBigIconByID(const int iPropsID)
{
    auto it = mapPropsConfig.data.find(iPropsID);
    if(it == mapPropsConfig.data.end())
    {
        return "";
    }
    return it->second.propsIconBig;
}

string OuterFactoryImp::getPorpsNameByID(const int iPropsID)
{
    auto it = mapPropsConfig.data.find(iPropsID);
    if(it == mapPropsConfig.data.end())
    {
        return "";
    }
    return it->second.propsName;
}


////////////////////////////////////////////////////////////////////////////////


