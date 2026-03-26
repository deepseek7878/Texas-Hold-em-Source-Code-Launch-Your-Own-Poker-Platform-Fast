#ifndef _GameRecordServantImp_H_
#define _GameRecordServantImp_H_

#include "servant/Application.h"
#include "GameRecordServant.h"
#include "XGameComm.pb.h"
#include "CommonCode.pb.h"
#include "CommonStruct.pb.h"
#include "GameRecord.pb.h"

#include "UserInfoProto.h"

#include "third.pb.h"

//
using namespace gamerecord;

/**
 *登录服务逻辑处理接口
 *
 */
class GameRecordServantImp : public gamerecord::GameRecordServant
{
public:
    /**
     *
     */
    virtual ~GameRecordServantImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

public:
    //http请求处理接口
    virtual tars::Int32 doRequest(const vector<tars::Char> &reqBuf, const map<std::string, std::string> &extraInfo, vector<tars::Char> &rspBuf, tars::TarsCurrentPtr current);
    //tcp请求处理接口
    virtual tars::Int32 onRequest(tars::Int64 lUin, const std::string &sMsgPack, const std::string &sCurServrantAddr, const JFGame::TClientParam &stClientParam, const JFGame::UserBaseInfoExt &stUserBaseInfo, tars::TarsCurrentPtr current);
    //获取玩家牌局收藏信息
    virtual tars::Int32 getCollectGame(const CollectGameListReq& req, CollectGameListResp& resp, tars::TarsCurrentPtr current);
    //收藏牌局
    virtual tars::Int32 reportCollectGame(const gamerecord::CollectGame& collectGame, tars::TarsCurrentPtr current);
    //上报牌谱
    virtual tars::Int32 reportGameDetail(const gamerecord::GameDetail& gameDetail, tars::TarsCurrentPtr current);
    //上报牌局
    virtual tars::Int32 reportGameBrief(const gamerecord::GameBrief& gameBrief, tars::TarsCurrentPtr current);
    //上报作弊信息
    virtual tars::Int32 reportCheat(const string &sCheatIndex, const int iCheatType, const int iOption, const vector<string> &vecRelateId, tars::TarsCurrentPtr current);
    //获取玩家玩牌统计信息
    virtual string getUserStat(tars::Int64 lUin, tars::TarsCurrentPtr current);

public:
    //
    int queryHonourRankRecord(const GameRecordProto::QueryHonourRankReq &req, GameRecordProto::QueryHonourRankResp &resp);

    int queryUserHonourRecord(const GameRecordProto::QueryUserHonourReq &req, GameRecordProto::QueryUserHonourResp &resp);

    int queryCheatRecord(const GameRecordProto::QueryCheatReq &req, GameRecordProto::QueryCheatResp &resp);

    int queryCheatGameRecord(const GameRecordProto::QueryCheatGameReq &req, GameRecordProto::QueryCheatGameResp &resp);

public:
    int onQueryCollectGameRecord(const XGameComm::TPackage &pkg, const GameRecordProto::QueryCollectGameReq &req, const std::string &sCurServrantAddr);

    //
    int onDeleteCollectGameRecord(const XGameComm::TPackage &pkg, const GameRecordProto::DeleteCollectGameReq &req, const std::string &sCurServrantAddr);

    //
    int onQueryBriefGameRecord(const XGameComm::TPackage &pkg, const GameRecordProto::QueryGameBriefReq &req, const std::string &sCurServrantAddr);

    //
    int onQueryGameInfoRecord(const XGameComm::TPackage &pkg, const GameRecordProto::QueryGameInfoReq &req, const std::string &sCurServrantAddr);

    //
    int onQueryUserStatRecord(const XGameComm::TPackage &pkg, const GameRecordProto::QueryUserStatReq &req, const std::string &sCurServrantAddr);

    //
    int onQueryHonourRankRecord(const XGameComm::TPackage &pkg, const GameRecordProto::QueryHonourRankReq &req, const std::string &sCurServrantAddr);

    //
    int onQueryUserHonourRecord(const XGameComm::TPackage &pkg, const GameRecordProto::QueryUserHonourReq &req, const std::string &sCurServrantAddr);

    //
    int onQueryCheatRecord(const XGameComm::TPackage &pkg, const GameRecordProto::QueryCheatReq &req,const std::string &sCurServrantAddr);

    //
    int onQueryCheatGameRecord(const XGameComm::TPackage &pkg, const GameRecordProto::QueryCheatGameReq &req, const std::string &sCurServrantAddr);

    //
    int onQueryUserOwnRecord(const XGameComm::TPackage &pkg, const GameRecordProto::QueryUserOwnReq &req, const std::string &sCurServrantAddr);

public:
    //
    int getUserInfo(const long lPlayerID, GameRecordProto::UserInfo* userinfo);
    //
    int getUserRankInfo(const long lPlayerID, const string &sRewardInfo, Pb::UserInfo* userinfo);
    //
    string getUserStat(const long lPlayerID);

private:
    //发送消息到客户端
    template<typename T>
    int toClientPb(const XGameComm::TPackage &tPackage, const std::string &sCurServrantAddr, XGameProto::ActionName actionName, const T &t);
};
/////////////////////////////////////////////////////
#endif
