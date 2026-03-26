#ifndef _Processor_H_
#define _Processor_H_

//
#include <util/tc_singleton.h>
#include "GameRecordServant.h"
#include "DataProxyProto.h"
#include "third.pb.h"
#include "GameRecord.pb.h"

//
using namespace tars;

/**
 *请求处理类
 *
 */
class Processor
{
public:
    /**
     *
    */
    Processor();

    /**
     *
    */
    ~Processor();

public:
    //
    int readDataFromDB(long uid, const string& table_name, const std::vector<string>& col_name, const std::vector<vector<string>>& whlist, const string& order_col, dbagent::TDBReadRsp &dataRsp);
    //收藏牌局
    int InsertCollectGame(const gamerecord::CollectGame& collectGame);
    //
    int SelectCollectGame(const long lUid, const string& sRoomIndex, vector<string>& vecDetailIndex);
    //
    int SelectCollectGameByUid(const long lUid, vector<string>& vecDetailIndex);
    //
    int InsertUserGameStat(const string& sRoomIndex, map<int, gamerecord::UserInfo> mapUserInfo);
    //
    int SelectUserGameStat(const long lUid, const string &sRoomIndex, vector<map<string, string>>& vecGameStat);
    //
    int SelectUserGameStatByColName(const map<string, string> &whilist, vector<map<string, string>>& vecGameStat);
    //
    int InsertGameDetail(const gamerecord::GameDetail& gameDetail);
    //
    int SelectGameDetail(const string &sRoomIndex, vector<map<string, string>>& vecGameDetail);
    //
    int SelectGameDetailByDetailIndex(const string &sDetailIndex, vector<map<string, string>>& vecGameDetail);
    //
    int InsertGameBrief(const gamerecord::GameBrief& gameBrief);
    //
    int SelectGameBrief(const string &sRoomIndex, vector<map<string, string>>& vecGameBrief);
    //
    int deleteCheat(const string &sCheatIndex, const string &sRelateID = "");
    //
    int InsertCheat(const string &sCheatIndex, const int iCheatType, const string &sRelateID);
    //
    int SelectCheat(const map<string, string> &whilist, vector<map<string, string>>& vecCheatGame);
    //
    int SelectUserGameOwn(const long lUid, const long lStartTime, const long lEndTime, vector<map<string, string>>& vecGameStat);
    //
    int InsertUserGameOwn(const string& sRoomIndex, map<int, gamerecord:: UserOwn> mapUserInfo);
};

//singleton
typedef TC_Singleton<Processor, CreateStatic, DefaultLifetime> ProcessorSingleton;

#endif

