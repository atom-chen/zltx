//
//  ActivityController.h
//  IF
//
//  Created by 李锐奇 on 14-4-3.
//
//

#ifndef __IF__ActivityController__
#define __IF__ActivityController__

#include "CommonInclude.h"
#include "ActivityInfo.h"
#include "ActivityEventInfo.h"
#include "AllianceActivityInfo.h"
#include "ActivityEventObj.h"
#include "DragonRankInfo.h"
#include "DragonAllianceRankInfo.h"

#define ACTIVITY_NUM_CHANGE "activityNumChange"
#define ACTIVITY_SCORE_CHANGE "activityScoreChange"

#define CHRISTMAS_SCOKS_EXCHANGE "christmas.scoks.exchange"
#define CHRISTMAS_LOGINRD_END "christmas.loginRd.end"
#define CHRISTMAS_SNOW_END "christmas.snow.end"
#define CHRISTMAS_DATA_INIT "christmas.data.init"

#define PORT_VIP_LOGINRD_END "port.vip.loginRd.end"
#define PORT_LOGINRD_END "port.loginRd.end"
#define PORT_TIME_EXCHANGE "port.time.exchange"
#define PORT_DATA_INIT "port.data.init"
#define MSG_NEW_TIME_RWD_END "msg.new.time.rwd.end"
#define MSG_REPAY_INFO_INIT "repay.info.init"
#define MSG_NEW_PLAYER_RD "msg.new.player.rd"
#define MSG_NEW_PLAYER_RD_SHOW "msg.new.player.rd.show"
#define MSG_GET_ACT6_DATA_SUCCESS "msg.get.act6.data.success"
#define MSG_GET_ACT6_DATA_FAIL "msg.get.act6.data.fail"

#define MSG_ACT6_EXC_SUCCESS "msg.act6.exc.success"
#define MSG_ACT6_EXC_FAIL "msg.act6.exc.fail"

#define MSG_ACT6_START_REFRESH "msg.act6.start.refresh"
#define MSG_ACT6_REFRESH_SUCCESS "msg.act6.refresh.success"
#define MSG_ACT6_REFRESH_FAIL "msg.act6.refresh.fail"

#define MSG_ACT6_SHOWRWD_END "msg.act6.showrwd.end"

#define MSG_GET_ATK_DATA "msg.get.atk.data"
#define MSG_TRIAL_START_SUC "msg.trial.start.suc"
#define MSG_TRIAL_FINISH_SUC "msg.trial.finish.suc"
#define MSG_TRIAL_BUY_SUC "msg.trial.buy.suc"
#define MSG_TRIAL_RWD_SUC "msg.trial.rwd.suc"

#define SHOULD_FORCE_REBEGIN "should_force_rebegin"
#define MSG_TRIAL_REFRESH "msg.trial.refresh"

#define FRESH_RECHARGE_REWARD_END "FRESH_RECHARGE_REWARD_END"

#define FRESH_RECHARGE_ICON_TOUCHED "FRESH_RECHARGE_ICON_TOUCHED"

#define FRESH_RECHARGE_ICON_TOUCH_BEGAN "FRESH_RECHARGE_ICON_TOUCH_BEGAN"
#define FRESH_RECHARGE_ICON_TOUCH_CANCELED "FRESH_RECHARGE_ICON_TOUCH_CANCELED"
#define FRESH_RECHARGE_ICON_TOUCH_MOVED "FRESH_RECHARGE_ICON_TOUCH_MOVED"
#define FRESH_RECHARGE_ICON_TOUCH_END "FRESH_RECHARGE_ICON_TOUCH_END"

#define MSG_FRESH_RECHARGE_TOTAL_CHANGED "MSG_FRESH_RECHARGE_TOTAL_CHANGED"

#define MSG_FRESH_SINGLE_SCORE_VIEW "MSG_FRESH_SINGLE_SCORE_VIEW"
#define MSG_FRESH_SINGLE_SCORE_RANK_VIEW "MSG_FRESH_SINGLE_SCORE_RANK_VIEW"
#define MSG_SCORE_RANK_HISTORY_VIEW "MSG_SCORE_RANK_HISTORY_VIEW"

enum ActivityType{
    ACTIVITY_SOLO = 1,
    ACTIVITY_ALLIANCE,
};

enum ActivityGetScoreType{
    KILL_ARMY,
    TRAINING_ARMY,
    DESTROY_TRAP,
    PRODUCT_TRAP,
};

class ActExc2Info
{
public:
    void update(CCDictionary*);
    ActExc2Info(CCDictionary*);
    ActExc2Info(): m_type(0), m_multiple(0), m_reward(nullptr){};
    int m_type;
    int m_multiple;
    vector<string> m_ids;
    CCSafeObject<CCArray> m_reward;
    vector<string> m_rwdIds;
};

class Act2RefrshInfo
{
public:
    vector<string> m_excIds;
    int m_refresh;
    int m_freeNum;
    int m_refreshCost;
    int m_refreshTime;
};

class RewardClass: public Ref
{
public:
    static RewardClass* create(Ref*);
    void update(Ref*);
    RewardClass(): type(0), itemId(""), num(0){};
    
    int type;
    string itemId;
    int num;
};

class Act1ExcItem: public Ref
{
public:
    static Act1ExcItem* create(Ref*);
    void update(Ref*);
    Act1ExcItem(): needNum(0), level(0), color(0), isRecieved(true)
    {
        reward = __Array::create();
    }
    int needNum;
    int level;
    int color;
    bool isRecieved;
    CCSafeObject<__Array> reward;
    int getRewardCnt(){
        return reward->count();
    }
    RewardClass* getRewardByIdx(int idx){
        if(idx<reward->count()){
            CCObject *ccobj = reward->objectAtIndex(idx);
            if (ccobj){
                return dynamic_cast<RewardClass*>(ccobj);
            }
            return NULL;
        }
        return NULL;
    }
};

class Act1Data: public Ref
{
public:
    static Act1Data* create(string actId);
    void update(Ref* dict);
    Act1Data():actId(""), level(0), refreshTime(0), flushTimes(-1), itemId(""), freeFreshNum(0), lastFreshTime(0), position(0, 0), isDataInit(false)
    {
        data.clear();
        recievedLevels.clear();
    }
    
    string itemId;
    string actId;
    int level;
    int refreshTime;
    int flushTimes;
    map< int, CCSafeObject<Act1ExcItem> > data;
    int getDataSize(){
        return data.size();
    }
    int getAct1ExcKeyByIdx(int idx){
        auto it = data.begin();
        for (int i = 0; i < idx; ++i)
        {
            it++;
            if (it == data.end())
            {
                return -1;
            }
        }
        return (*it).first;
    }
    Act1ExcItem* getAct1ExcItemByKey(int key){
        auto it = data.find(key);
        if(it != data.end()){
            return (*it).second;
        }
        return NULL;
    }
    map<int, bool> recievedLevels;
    
    vector<int> freshCost;
    int freeFreshNum;
    int lastFreshTime;
    Vec2 position;
    Vec2 getPosition(){
        return position;
    }
    bool isDataInit;
};


const int prepareTime = 30 * 60;

class ActivityController : public CCObject{
public:
    static ActivityController *getInstance();
    static void purgeData();
    ActivityController();
    virtual ~ActivityController();
    void addActivity(CCArray *arr);
    void addActivity(CCDictionary *dict);
    void removeActivity(ActivityInfo *info);
    void removeActivity(int type);
    void onEnterFrame(float dt);
    bool hasActivity(int type);
    
    bool isActivityTimeOver(ActivityInfo *info);
    void doWhenTimeOver(float t = 0.0);
    void updateCurrentScore(int type, int score);
    void updateCurrentEvent(CCDictionary *dict);
    void openActivityView();
    void loginEventData();
    void paraseEventObj(CCArray *arr);
    void enterGameOpenView(int type = 0);
    
    bool activityIsOpen();
    void activityRecord(int type);
    void rewardRecordHandle();
    std::string UrlDecode(const std::string& szToDecode);
    void facebookPostMethod(string key,CCArray* param=NULL); // also for lua
    void openCrossServerActivity(); // also for lua
    void leaveCKFServer(CCDictionary *dict); // also for lua
    bool checkShengDianOpen(); // also for lua
    bool checkLianShengOpen(); // also for lua
    bool checkUseCKFMassTime(); // also for lua
    
    ActivityEventObj* getActObj(string itemId);  // also for lua
    
    map<int, ActivityInfo*> activity;
    
    ActivityEventInfo eventInfo;
    CCSafeObject<CCArray> activityArr;//ActivityEventObj
    CCArray* getActivityArr(){ //for lua use -- add by js
        return activityArr;
    }
    
    CCSafeObject<CCArray> speActivityArr;//ActivityEventObj
    CCSafeObject<CCArray> allianceKill;
    CCSafeObject<CCArray> gole;
    CCSafeObject<CCArray> memberKill;
    AllianceActivityInfo  aActivityInfo;//联盟活动信息
    CCSafeObject<CCDictionary> ckfServerInfo;//ckf离开的服务信息
    CCSafeObject<CCArray> baozangHelpArr;//宝藏可帮好友的信息
    
    string getActivityInfo(); // also for lua
    void sortActivityArr(); // also for lua
    
    //type = 6的活动
    void createRwdViewByActId(string actId);
    
    bool checkIsToday(time_t time); // also for lua
    bool checkCanExchange2(string actId, int idx); // also for lua
    
    bool checkHaveActivityByType(int type); // also for lua
    
    void startGetAct6Data(); // also for lua
    void endGetAct6Data(CCDictionary*);
    bool startAct6Exchange(string, int); // also for lua
    void endAct6Exchange(CCDictionary*);
    void startAct6Refresh(string);  // also for lua
    void endAct6Refresh(CCDictionary*);
    
    void createGoodsIconWithGray(CCNode* parent, string itemId, float size); // also for lua
    void createGoodsIcon(CCNode* parent, string itemId, float size); // also for lua
    void createEquipIcon(Node* node, string itemId, float size); // also for lua
    string createRandomGoodsId(string actId, int type);//type == 2  是碎片活动 ； type == 1 是普通的兑换活动   // also for lua
    
    bool isHasAct(string actId); // also for lua
    
    
    
    //进游戏时活动弹窗相关
    string getRunningPopEvent(); // also for lua
    string getRandomPopActId(); // also for lua
    void showRandomActPop();
    void parseActConfig(CCDictionary* dict);//包括试炼场相关配置
    void parseActConfig2(CCDictionary* dict);//配置试炼场
    
    //每天活动弹窗的最大次数
    int popCntLimit;
    
    //type = 6, exchange = 1 普通兑换相关数据
    CCSafeObject<__Dictionary> m_exc1Data;
    Act1Data* getExc1DataByKey(string key){   // for lua use
        if(m_exc1Data->objectForKey(key)!=NULL){
            return dynamic_cast<Act1Data*>(m_exc1Data->objectForKey(key));
        }
        return NULL;
    }
    //type = 6 exchange = 1 相关方法
    bool isAct1DataInit(string actId); // also for lua
    bool isAct1DataShouldRefresh(string actId); // also for lua
    bool isAct1ItemHasExchange(string actId, int level); // also for lua
    bool checkAct1ReFreshTimes(string actId); // also for lua
    bool isCanRefreshByhand(string actId); // also for lua
    
    //type = 6 的碎片兑换相关数据
    map<string, Act2RefrshInfo> m_exc2RefInfo;
    map<string, map<int, ActExc2Info> > m_exc2Data;
    
    int getExc2DataSize(string aid); // for lua use
    bool isInExc2Data(string actid); // for lua use
    Act2RefrshInfo* getAct2RefrshInfoByKey(string actid); // for lua use
    
    int wb_condition_k1;//跨服控制进入条件 玩家大本等级
    int wb_condition_k2;//跨服控制进入条件 需要玩家道具的id
    int wb_condition_k3;//跨服控制进入条件 表示所需道具的数量
    string wb_info_k2;//开放功能的版本
    int wb_info_k6;//总时间
    int wb_info_k7;//表示活动每个阶段的时间，以小时为单位，当前值=3
    int wb_info_k9;//跨服伤兵速度加倍那个
    float wb_info_k10;//跨服伤兵速度加倍,乘方因子
    int wb_info_k11;//跨服是从多少连胜开始显示
    
    int wb_timing_k1;//活动结束前，开始进入下一个阶段的提示
    int wb_timing_k2;//每间一段时间提示一次
    int ckfNeedLeave;//已经时间到，要离开中立服，但还没有离开的标志
    double ckfProtectTime;//跨服保护时间
    int wb_switch_k1;//跨服连胜的开关
    string wb_switch_k2;//跨服连胜开放的版本
    int wb_switch_k3;//跨服技能的开关
    string wb_switch_k4;//跨服技能开放的版本
    double abfEnterCDTime;//远古的CD时间
    
    
    map<std::string, double> abf_skillCD_map;//远古技能CD
    
    
    
    int wb_switch_k6;//集结时间的开关
    string wb_switch_k7;//集结时间开放的版本
    
    double m_userScore;//跨服用户积分
    int m_userRank;//跨服用户排名
    double m_kingdomScore;//跨服王国积分
    int m_kingdomRank;//跨服王国排名
    
    map<string, string> wbScoreMap;//跨服获得积分计算
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_ckfMemberReward, CkfMemberReward);//跨服个人奖励
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_ckfKingdomReward, CkfKingdomReward);//跨服王国奖励

    //进入游戏的弹窗
    void initViewConfig(CCDictionary *ccDict);
    void showEnterView();
    void showDailyRwd(CCObject* ccObj);
    
    //-------试炼场活动-------start-------
    void parseTrialInfo(CCDictionary* );
    void endAtkCmd(CCDictionary*);
    void endStartTrialCmd(CCDictionary*);
    void endTrialFinishCmd(CCDictionary*);
    void endTrialBuyCmd(CCDictionary*);
    void endTrialRwdCmd(CCDictionary*);
    void showRwdParticle();
    void removeParticleNodes(CCObject* obj);
    void showTrainingView();
    int getMyTrialState();//得到当前的活动进行程度  1 可以进行全新一轮的试炼 2 继续进行上一轮的攻击不需要领奖  3 继续进行上一轮的攻击但是需要领奖 4所有的攻击次数用完但是当前怪没有打死 5 所有的攻击次数用完正好打死当前的怪 但不是最后一只怪 6 打死最后一只怪
    int getKilledMonsterCnt();
    int getAllMonsterCnt();
    
    int m_freeAtk;//free攻击次数
    vector<int> m_screen;//0 1 2三种攻击在16个格子中的位置
    int m_blood;//怪物当前血量
    string m_monster;//怪物ID
    int m_maxBlood;//怪物最大血量
    int m_freeTrial;//免费挑战次数
    int m_maxAtkCnt;//每局玩家可攻击怪兽的次数
    vector<int> m_atkHarm;//三种攻击对怪物的伤害
    int m_isTrialOpen;//试炼场活动开关
    int m_bigStrengthGold;//蓄力攻击花费金币数量~~~
    int m_moreAtkGold;//增加攻击次数花费金币数量~~~~
    int m_addedAtkCnt;//花金币增加的攻击次数数量
    int m_newTrialGold;//新挑战花费的金币数量~~~~~
    bool m_isInAtkCmdSending;//正在发送攻击命令
    int m_bigStrenMul; // 蓄力时的攻击倍数
    vector<int> m_monsterDiv; //怪物的级别划分
    time_t m_trialRefTime;//数据刷新时间
    
    int m_buyTrialTimes;//花金币购买试炼的次数
    int m_buyAtkTimes;//花金币购买攻击的次数
    int m_chargeAtkTimes;//蓄力攻击的次数
    string m_rewardMonster;//兑换奖励的怪物
    vector<int> m_newTrialGoldVec;//新挑战花费的金币数量的vector
    vector<int> m_moreAtkGoldVec;//增加攻击次数花费的金币数量的vector;
    vector<int> m_bigStrengthGoldVec;//蓄力攻击花费金币数量的vector;
    
    int getBigStrengthGold();
    int getNewTrialGold();
    int getMoredAtkGold();
    //--------试炼场活动--------end-------
    
    // -------巨龙战场
    int dragonBattleState; //1 报名 2 准备 3 战斗 4排行榜
    int dragonTeamApplyState; //联盟是否报名 0 没报名 1 已报名
    double fightBeginTime;//战斗开始时间，如果战斗已经结束就传-1
    double applyEndTime;//报名结束时点，如果报了，就是-1
    double enterFightCdEndTime; //巨龙战场 重新进入cd
    
    double nextApplyBeginTime;//下一轮，战斗开始时间，如果战斗已经结束就传-1
    
    CCSafeObject<CCDictionary> dragonEnemy;//敌人信息
    map<std::string, DragonRankInfo> dragonCatalogMap;// key 为rank_level
    int wa_switch_k1; //巨龙战场开关
    std::string wa_switch_k2;//开关的版本
    map<std::string, DragonAllianceRankInfo*> dragonRankMap;// allianceId 为
    int wa_army_k1;//伤兵速度加倍那个
    float wa_army_k2;//伤兵速度加倍,乘方因子
    int wa_army_k3;//表示金币秒伤兵等待的cd时间，单位是秒
    int wa_army_k4;//表示集结时间的选项
    int canFight; //1 可进入战斗 0 不可进入  (只有在state是3的时候传)
    int bye; //1 本次轮空
    int wa_fire_k1; //燃烧速度
    double wa_score_k10;//表示积分最大值
    int wa_switch_k3; //巨龙战场邀请迁城开关
    std::string wa_switch_k4;//表示巨龙是否开始
    bool isMoveToDragonBuild;
    
    void initCatalog();
    bool checkOpenDragonBattle();
    void parssDragonData(CCDictionary *dict);
    bool checkHaveDragonActivity();
    CCArray* getDataByRankAndLevel(int rank,int level);
    std::string getTitileByRank(int rank);
    bool dragonBattleNeedTip();
    
    int wa_info_k11; //迁城次数上限
    int wa_info_k9; //创建联盟的天数
    int fightNum;//出战人数
    int maxFightNum;//出战人数最大值
    bool isShowLine;//是否隐藏中产行军路线
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_dragonRewards, DragonRewards);//巨龙排名奖励
    
    //巨龙即将结束，系统公告时间（距离结束还有XX分钟。。）
    std::vector<int>wa_time_list;
    
     // --------- end 巨龙战场
    
    // -------单人积分活动
    map<string, ActivityInfo*> m_singleScoreRwdMap;
    map<string, CCSafeObject<CCArray>> m_RankRwdMap;
    map<string, string> m_CurRankRwdMap;
    vector<string> m_CurRankKeys;
    void getSingleScoreData(string activityId);
    void retSingleScoreData(CCDictionary* dict);
    void pushSingleScoreValue(CCDictionary* dict);
    
    void getCurRankRwd(string activityId, int type=0);
    void retCurRankRwd(CCDictionary* dict, int type=0);
    void addRwdToMap(string rewardId, CCArray* rwdArr);
    int SingleScoreRank;
    
    void getRank(string activityId);
    void retRank(CCDictionary* dict);
    
    void getHistroyRank(int type);
    void retHistroyRank(CCDictionary* dict, int type);
    map<int, CCSafeObject<CCArray>> m_historyRankMap;
    vector<int> m_historyTimeVec;
    // --------- end 单人积分活动
    // -------联盟积分活动
    map<string, ActivityInfo*> m_allScoreRwdMap;
    map<string, string> m_allRankRwdMap;
    vector<string> m_allRankKeys;
    int AllianceScoreRank;
    map<int, CCSafeObject<CCArray>> m_historyAllRankMap;
    vector<int> m_historyAllTimeVec;
    
    void getAllianceConRank(string activityId, int conType=0);
    void retAllianceConRank(CCDictionary* dict);
    CCSafeObject<CCArray> m_allConUsers;
    void CleanAllianceScoreData();
    // --------- end 联盟积分活动
    // -------王国积分活动
    map<string, ActivityInfo*> m_kingScoreRwdMap;
    map<string, string> m_kingAlRankRwdMap;//王国 联盟排行奖励
    vector<string> m_kingAlRankKeys;
    map<string, string> m_kingPlRankRwdMap;//王国 个人排行奖励
    vector<string> m_kingPlRankKeys;
    int KingAlScoreRank;//王国 联盟活动排名
    int KingPlScoreRank;//王国 单人活动排名
    map<int, CCSafeObject<CCArray>> m_historyKingPlRankMap; //王国 单人 历史排行
    map<int, CCSafeObject<CCArray>> m_historyKingRankMap; //王国 历史排行
    vector<int> m_historyKingPlTimeVec;
    
    map<int, CCSafeObject<CCArray>> m_historyKingAlRankMap; //王国 联盟 历史排行
    vector<int> m_historyKingAlTimeVec;
    CCSafeObject<CCArray> m_kingAlConUsers;
    CCSafeObject<CCArray> m_vsCountry;
    int m_ConType;
    // --------- end 王国积分活动
    
    // -------王国纷争活动
    std::string wk_info_k1; //开放的版本
    std::string wk_info_k2; //开放的服
    int wk_info_k3; //开放的等级
    
    void parseWKActivityConfig(CCDictionary *dict);
    bool checkWKactivityOpen();
    bool checkServerInOpenList(std::string openServer);
    bool canJoinWKActivity();
    bool checkServerCanBattle(int serverId);
    bool checkServerCanJoin(int serverId);
    bool checkHaveWK();
    bool checkShowRank();
    // --------- end 王国纷争
    
    void changeServerByInfo(CCDictionary* info);//跨服回来切换
    
    
    //for lua
    CCSafeObject<Dictionary> m_dataForLua;
    Dictionary* getDataForLua();
    void setDataForLua(Ref* ref, string name);
private:
    void sortSpeActivityArr();
    
    //进入游戏的弹窗
    vector<string> mOpenViewConfig;
    void showEnterViewByType(string popup);

};

class SocksExInfo;
class ChristmasActController : public CCObject{
public:
    static ChristmasActController *getInstance();
    static void purgeData();
    ChristmasActController();
    ~ChristmasActController();
    
    //袜子兑换数据
    int m_exchangeTime;//兑换刷新时间
    int m_level;//当前已经兑换了第几档
    string m_socksMsg;//获得袜子的信息
    map<int, SocksExInfo> m_socExMap;
    //end
    
    //14天登陆奖励数据
    int m_loginDay;
    int m_isRdLoginDay;//0 今天没领 1今天已经领了
    map<int, CCSafeObject<CCArray>> m_loginDayMap;
    int m_Today;
    bool m_isNewDay;
    //end
    
    //雪球数据
    double m_refreshTime;//恢复一个雪球的时间戳
    int m_maxThrowCount;//一天最多扔多少雪球
    int m_todayThrowCount;//当天已经扔过多少雪球
    int m_curLevel;//当前雪球等级
    int m_goldCostRecover;//恢复全部雪球需要的金币数量。
    int m_expArr[5];//资源扔雪球获得的相应的经验值
    int m_goldThrowMultiply;//用金币扔雪球获得的经验倍数
    int m_goldCostThrow;//花费金币扔雪球消耗的金币数
    int m_resourceType;//扔雪球所耗费的资源种类
    int m_resourceNum;//扔雪球所耗费的资源的数量
    long m_remainGold;//玩家金币数量
    int m_exp;//玩家当前经验值
    int m_isInit;
    int m_free;//1代表免费  0代表不免费
    //end 雪球数据
    
    map<string, int> m_actTime;
    
    int checkActState();
    int getStartTime();//获取活动开始时间
    int getSnowTime();//获取打雪人活动开始时间
    int getEndTime();//获取活动结束时间
    
    void startGetActTime();
    void endGetActTime(CCDictionary* param);

    void startGetExchangeData(int type=0);//初始化数据
    void endGetExchangeData(CCDictionary *dict, int type=0);
    
    void startExchange(int eid);//兑换
    void endExchange(CCDictionary *dict);
    
    void pushSocksMsg(CCDictionary *dict);//袜子信息更新
    
    void startGetLogin14Day();
    void endGetLogin14Day(CCDictionary *dict);
    void startGetLogin14RD();
    void endGetLogin14RD(CCDictionary *dict);
    
    void startGetSnowActData(int isInit=0);
    void endGetSnowActData(CCDictionary* param);
};

class SocksExInfo : public cocos2d::CCObject {
public:
    SocksExInfo(CCDictionary* dict);
    SocksExInfo():
    eId(0)
    ,needNum(0)
    ,rewardId("")
    ,rewardArr(NULL)
    {};
    
    int eId;
    int needNum;
    string rewardId;
    CCSafeObject<CCArray> rewardArr;
};

class TimeExInfo: public cocos2d::CCObject{
public:
    TimeExInfo(CCArray* arr, int i);
    TimeExInfo():
    eId(0)
    , rewardArr(NULL)
    {};
    
    int eId;
    CCSafeObject<CCArray> rewardArr;
    
};

class DailyRwdInfo: public cocos2d::CCObject{
public:
    DailyRwdInfo(CCDictionary* dict);
    DailyRwdInfo():
    day(0)
    , showDay(0)
    , multiple(0)
    , vipMultiple(0)
    , vipReward(NULL)
    , reward(NULL)
    {};
    
    int day;
    int showDay;
    int multiple;
    int vipMultiple;
    CCSafeObject<CCArray> reward;
    CCSafeObject<CCArray> vipReward;
};

class PortActController: public CCObject{
public:
    static PortActController *getInstance();
    static void purgeData();
    PortActController();
    ~PortActController();
    
    //在线时长兑换数据
    //等级的划分时间
    vector<int> m_levelTime;
    //上一次领奖的级别的时长最大值
    long m_lastRwdTime;
    //更新时大本等级
    int m_castleLevel;
    //当前在线时长
    long m_timeOnline;
    //前台开始计时时间
    long m_startTime;
    map<int, TimeExInfo> m_timeExMap;
    //end
    
    //新在线时长数据
    //下次可领奖时间
    time_t m_nextRewardTime;
    //奖励信息
    string m_rewardInfo;
    //开关
    bool m_isNewTimeRwd;
    
    //每日签到
    int m_loginDay;
    int m_isRdLoginDay;//0 今天没领 1今天已经领了
    int m_isVipRdLoginDay;//vip 0 今天没领 1今天已经领了
    int m_Today;
    bool m_isNewDay;
    int m_inCnt;//进页面次数
//    map<int, CCSafeObject<CCArray>> m_loginDayMap;
    map<int, DailyRwdInfo> m_loginDayMap;
    //end
    
    //新手7日累计签到
    int m_newLoginDay;//累计天数
    int m_isGetNewRD;//0 今天没领 1今天已经领了
    map<int, CCSafeObject<CCArray>> m_newPalyerRDMap;
    
    //根据时间获取等级
    int getLvByTime(long time);
    //获取当前最大可兑换等级
    int getMaxRwdLv();
    //判断是否是新的一天
    bool isNewDay();
    //解析奖励的时间点
    void parseTimeDiv(CCDictionary* dict);
    
    //初始化数据
    void startGetExchangeData(int type = 0);
    void endGetExchangeData(CCDictionary *dict, int type = 0);
    
    //兑换
    void startExchange(int eId);
    void endExchange(CCDictionary *dict);
    
    //每日签到初始化
    void startGetDailyRwdData();
    void endGetDailyRwdData(CCDictionary* dict);
    
    //新手7日签到初始化
    void initNewPlayerDailyRwdData(CCDictionary* dict);
    void startGetNewPlayerRD(string key="");
    void endGetNewPlayerRD(CCDictionary* dict);
    
    //签到
    void startGetCheckInRwd(bool isVip,int num=0,int newNum=0,int oldNum=0);
    void endGetCheckInRwd(CCDictionary* dict, bool isVip);
    
    //奖励领取特效
    void flyReward(CCArray* arr, bool isVip);
    void flyToolReward(CCArray* arr, bool isVip);
    void aniComplete(CCObject* node);
    void removeLayerColor(CCObject* layerColor);
    void showRwdParticle();
    
    //全部奖励领取完是否第一次进当前页面
    bool isLastDayFirstIn();
    
    //新的领取在线时长奖励方法
    void startTimeRwd();
    void endTimeRwd(CCDictionary*);
    //判断是否可以领取奖励
    bool isCanRwd();
    //增加推送
    void addPush();
    
    bool canPopNewRDView();
    
    void addParticle(CCObject* obj);
    void particleComplete(CCObject* obj);
    int rwdCount;
    
};



//
//累计充值回馈
//
struct RepayRewardStruct1{
    int type;
    std::string itemid;
    int count;
    int orderid;
};
struct RepayRewardStruct{
    int point;
    vector<RepayRewardStruct1> rewards;
};
class RepayController:public CCObject{
public:
    RepayController();
    ~RepayController();
    static RepayController* getInstance();
    static bool sortRepayItem(const RepayRewardStruct& itemId1,const RepayRewardStruct& itemId2);
    void testInfoInit();
    void initRepayInfo(CCDictionary *ccDict);
    vector<RepayRewardStruct> repayRewards;
    int getLVIndexByPoint(int point);
    bool isBegin();
    void addPoint(int addPoint);
    void resetChangeLV();
private:
    CC_SYNTHESIZE_READONLY(int, m_oldPayPoint, OldPayPoint);
    CC_SYNTHESIZE_READONLY(int, m_newPayPoint, NewPayPoint);
    CC_SYNTHESIZE_READONLY(double, m_startTime, StartTime);
    CC_SYNTHESIZE_READONLY(double, m_endTime, EndTime);
    CC_SYNTHESIZE_READONLY(bool, m_changeLV, ChangeLV);
};

struct FrReReward
{
    int type;
    string itemId;
    int count;
};

struct FreshRechargeItem
{
    string m_itemId;
    int m_node;
    vector<FrReReward> m_rwds;
};

class FreshRechargeController: public Ref
{
public:
    static FreshRechargeController* getInstance();
    static void purgeData();
    static bool sortByNode(const FreshRechargeItem& item1, const FreshRechargeItem& item2);
    bool hasRwdedAll();
    FreshRechargeController();
    bool isHasExchange(string itemId);
    bool isCanExchange(string itemId);
    
    
    void initFreshRecharge(Dictionary* dict);
    void initFreshRechargeRewardedIds(__Dictionary* dict);
    
    void startReward(int idx);
    void endReward(__Dictionary* dict);
    
    bool isCanRewardAll();
    int getLeftToNextStage();
    int getCanExchangeCnt();
    
    vector<FreshRechargeItem> m_freshRechargeVec;
    vector<string> m_rewardedIds;
};

#endif /* defined(__IF__ActivityController__) */
