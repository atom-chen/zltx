//
//  TerritoryBannerTile.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/8/19.
//
//

#include "TerritoryBannerTile.h"
#include "ScoutInfo.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "RoleInfoView.h"
#include "TipsView.h"
#include "SceneController.h"
#include "SoundController.h"
#include "EnemyInfoController.h"
#include "AllianceManager.h"
#include "GlobalData.h"
#include "TerritoryInformationView.h"
#include "TerritoryFunctionView.h"
#include "TerritoryInfoDetailView.h"
#include "TroopInformationView.h"
#include "YuanJunTipView.h"
bool TerritoryBannerTile::init() {
    NewBaseTileInfo::init();
    
    soldierAlready = 0;
    soldierMax = 0;
    dataBack = false;
    
    requestDetail();
    onDetailCallback(NULL);
    m_nameNode->removeAllChildren();
    auto bg = CCLoadSprite::createSprite("name_bg.png");
    m_nameNode->addChild(bg);
    
    updateView();
    m_isInUpdate = false;
    addToParent();
    
    return true;
}

void TerritoryBannerTile::onEnter(){
    NewBaseTileInfo::onEnter();
}

void TerritoryBannerTile::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    NewBaseTileInfo::onExit();
}

void TerritoryBannerTile::requestDetail() {
    m_isInUpdate = true;
    auto cmd = new WorldDetailCommand(m_cityInfo.cityIndex,m_cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryBannerTile::onDetailCallback), nullptr));
    cmd->sendAndRelease();
}

void TerritoryBannerTile::onDetailCallback(cocos2d::CCObject *obj) {
    m_isInUpdate = false;
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
        return;
    }
    if(!WorldController::getInstance()->isInWorld){
        return;
    }
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        return;
    }
    if(!this->getParent()){
        return;
    }
    
    if (info) {
        dataBack = true;
    }
    
    unsigned int index = m_cityInfo.cityIndex;
    auto cityIt = WorldController::getInstance()->m_cityInfo.find(index);
    if (cityIt != WorldController::getInstance()->m_cityInfo.end()) {
        if (info->objectForKey("maxSoldier")) {
            soldierMax = info->valueForKey("maxSoldier")->intValue();
        }
        else
            soldierMax = 0;
        
        if (info->objectForKey("alreadySoldier")) {
            soldierAlready = info->valueForKey("alreadySoldier")->intValue();
        }
        else
            soldierAlready = 0;
        
        if (info->objectForKey("territory")) {
            CCDictionary* territory = _dict(info->objectForKey("territory"));
            if (territory) {
                CCDictionary* owner = _dict(territory->objectForKey("owner"));
                if (owner) {
                    cityIt->second.m_allianceAreaOwnerInfo.ownerPic = owner->valueForKey("pic")->getCString();
                    cityIt->second.m_allianceAreaOwnerInfo.ownerName = owner->valueForKey("name")->getCString();
                    cityIt->second.m_allianceAreaOwnerInfo.allianceAbbr = owner->valueForKey("abbr")->getCString();
                    cityIt->second.m_allianceAreaOwnerInfo.allianceId = owner->valueForKey("allianceId")->getCString();
                    cityIt->second.m_allianceAreaOwnerInfo.ownerId = owner->valueForKey("uid")->getCString();
                    cityIt->second.m_allianceAreaOwnerInfo.ownerMarchUuid = owner->valueForKey("ownerMarchUuid")->getCString();
                }
                if (!owner) {
                    cityIt->second.m_allianceAreaOwnerInfo.ownerPic = "";
                    cityIt->second.m_allianceAreaOwnerInfo.ownerName = "";
                    cityIt->second.m_allianceAreaOwnerInfo.allianceAbbr = "";
                    cityIt->second.m_allianceAreaOwnerInfo.allianceId = "";
                    cityIt->second.m_allianceAreaOwnerInfo.ownerId = "";
                    cityIt->second.m_allianceAreaOwnerInfo.ownerMarchUuid = "";
                }
                if (territory->objectForKey("stat")) {
                    cityIt->second.m_bannerInfo.state = (AAreaState)territory->valueForKey("stat")->intValue();
                }
                if (territory->objectForKey("speed")){
                    cityIt->second.m_bannerInfo.buildSpeed = territory->valueForKey("speed")->doubleValue() * 1000;
                }
                if (territory->objectForKey("curDef")) {
                    cityIt->second.m_bannerInfo.defence = territory->valueForKey("curDef")->intValue();
                }
                if (territory->objectForKey("maxDef")) {
                    cityIt->second.m_bannerInfo.defenceMax = territory->valueForKey("maxDef")->intValue();
                }
                if (territory->objectForKey("updateTime")) {
                    cityIt->second.m_bannerInfo.updateTime = territory->valueForKey("updateTime")->doubleValue() / 1000;
                }
                if (territory->objectForKey("startTime")) {
                    cityIt->second.m_bannerInfo.startTime = territory->valueForKey("startTime")->doubleValue() / 1000;
                }
                if (territory->objectForKey("flag")) {
                    cityIt->second.m_bannerInfo.banner = territory->valueForKey("flag")->getCString();
                }
                if (territory->objectForKey("allianceAbbr")) {
                    cityIt->second.m_aArea_simpleName = territory->valueForKey("allianceAbbr")->getCString();
                }
                if (territory->objectForKey("name")) {
                    cityIt->second.m_aArea_nickname = territory->valueForKey("name")->getCString();
                }
                if (territory->objectForKey("allianceId")) {
                    cityIt->second.m_aArea_id = territory->valueForKey("allianceId")->getCString();
                }
                if (!territory->objectForKey("name")) {
                    cityIt->second.m_aArea_nickname = _lang("115534");
                }
            }
        }
    }
    
    updateView();
    if (dataBack) {
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    }
}


void TerritoryBannerTile::updateView() {
    this->m_iconNode->removeAllChildren();
    this->m_nameNode->removeAllChildren();
    auto sprite = CCLoadSprite::createSprite("territory_def.png");
    CCCommonUtils::setSpriteMaxSize(sprite, 30);
    this->m_iconNode->addChild(sprite);
    m_currentState = getState();
    
    //设置按钮
    unsigned int buttonCount = 0;
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        setButtonCount(1);
        setButtonName(1, _lang("115331"));
        setButtonState(1, ButtonInformation);
    }else{
        switch (m_currentState) {
            case TerritoryBannerUnfinished: {
                buttonCount = 2;
                setButtonCount(buttonCount);
                setButtonName(2, _lang("115331"));
                setButtonState(2, ButtonInformation);
                setButtonName(3, _lang("115332"));
                setButtonState(3, ButtonBuilding);
            }
                break;
            case TerritoryBannerBuilding: {//建造中
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.cityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                    setButtonName(1, _lang("115332"));
                    setButtonState(1, ButtonBuilding);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(1);
                    }
                }
                else{
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("115332"));
                    setButtonState(3, ButtonBuilding);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(3);
                    }
                }
            }
                break;
            case TerritoryBannerStation: {//驻扎
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.cityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                    setButtonName(1, _lang("115333"));
                    setButtonState(1, ButtonStation);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(1);
                    }
                }
                else{
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("115333"));
                    setButtonState(3, ButtonStation);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(3);
                    }
                }
            }
                break;
            case TerritoryBannerUnstation: {
                buttonCount = 2;
                setButtonCount(buttonCount);
                setButtonName(2, _lang("115331"));
                setButtonState(2, ButtonInformation);
                setButtonName(3, _lang("115333"));
                setButtonState(3, ButtonStation);
            }
                break;
            case TerritoryBannerRepair: {//修复
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.cityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                    setButtonName(1, _lang("115334"));
                    setButtonState(1, ButtonRepair);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(1);
                    }
                }
                else{
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("115334"));
                    setButtonState(3, ButtonRepair);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(3);
                    }
                }
            }
                break;
            case TerritoryBannerBroken: {
                buttonCount = 2;
                setButtonCount(buttonCount);
                setButtonName(2, _lang("115331"));
                setButtonState(2, ButtonInformation);
                setButtonName(3, _lang("115334"));
                setButtonState(3, ButtonRepair);
            }
                break;
            case TerritoryBannerMineOccupy: {//本联盟建筑被占领
                buttonCount = 3;
                setButtonCount(buttonCount);
                setButtonName(3, _lang("108723"));
                setButtonName(2, _lang("115331"));
                setButtonName(1, _lang("108726"));
                setButtonState(3, ButtonMarch);
                setButtonState(2, ButtonInformation);
                setButtonState(1, ButtonRally);
            }
                break;
            case TerritoryBannerOtherUnOccupy: {//非本联盟建筑未被占领
                buttonCount = 4;
                setButtonCount(buttonCount);
                setButtonName(3, _lang("108723"));
                setButtonName(2, _lang("115331"));
                setButtonName(4, _lang("108726"));
                setButtonName(5, _lang("108722"));
                setButtonState(3, ButtonMarch);
                setButtonState(2, ButtonInformation);
                setButtonState(4, ButtonRally);
                setButtonState(5, ButtonScout);
            }
                break;
            case TerritoryBannerOtherOccupy: {//非本联盟建筑被非本联盟占领
                buttonCount = 4;
                setButtonCount(buttonCount);
                setButtonName(3, _lang("108723"));
                setButtonName(2, _lang("115331"));
                setButtonName(4, _lang("108726"));
                setButtonName(5, _lang("108722"));
                setButtonState(3, ButtonMarch);
                setButtonState(2, ButtonInformation);
                setButtonState(4, ButtonRally);
                setButtonState(5, ButtonScout);
            }
                break;
            case TerritoryBannerOtherOccupyByMe: {//非本联盟建筑被本联盟占领
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.cityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                }
                else {
                    buttonCount = 1;
                    setButtonCount(buttonCount);
                    setButtonName(1, _lang("115331"));
                    setButtonState(1, ButtonInformation);
                }
            }
                break;
            default: {
                buttonCount = 1;
                setButtonCount(buttonCount);
                setButtonName(1, _lang("115331"));
                setButtonState(1, ButtonInformation);
            }
                break;
        }
    }
    
    for (int i=1; i<=TOTAL_BUTTON_NUM; ++i) {
        setButtonCallback(i, cccontrol_selector(TerritoryBannerTile::onClickButton));
    }
}

void TerritoryBannerTile::update(float sec) {
    if (m_isInUpdate) {
        return;
    }
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
        return;
    }
    if(!WorldController::getInstance()->isInWorld){
        return;
    }
    if(!this->getParent()){
        return;
    }
    
    bool flag = false;
    if (WorldController::getInstance()->forceUpdateTerritoryBannerTile == true) {
        flag = true;
        WorldController::getInstance()->forceUpdateTerritoryBannerTile = false;
    }
    
    unsigned int index = m_cityInfo.cityIndex;
    auto cityIt = WorldController::getInstance()->m_cityInfo.find(index);
    if (cityIt != WorldController::getInstance()->m_cityInfo.end()) {
        double now = GlobalData::shared()->getTimeStamp();
        //设置联盟建筑当前城防值
        switch (cityIt->second.m_bannerInfo.state) {
            case 1: {//未建造
                auto currentDef = cityIt->second.m_bannerInfo.defence;
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({220, 176, 0});
            }
                break;
            case 0: {//建造中
                double timePass = now - cityIt->second.m_bannerInfo.startTime;
                timePass = MAX(timePass, 0.0f);
                double defTemp = cityIt->second.m_bannerInfo.buildSpeed * timePass;
                auto currentDef = cityIt->second.m_bannerInfo.defence + defTemp;
                if (currentDef>=cityIt->second.m_bannerInfo.defenceMax) {
                    currentDef = cityIt->second.m_bannerInfo.defenceMax;
                }
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({0, 190, 13});
            }
                break;
            case 4: {
                auto currentDef = cityIt->second.m_bannerInfo.defence;
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({0, 190, 13});
            }
                break;
            case 5: {//修理中
                double timePass = now - cityIt->second.m_bannerInfo.startTime;
                timePass = MAX(timePass, 0.0f);
                double defTemp = cityIt->second.m_bannerInfo.buildSpeed * timePass;
                auto currentDef = cityIt->second.m_bannerInfo.defence + defTemp;
                if (currentDef>=cityIt->second.m_bannerInfo.defenceMax) {
                    currentDef = cityIt->second.m_bannerInfo.defenceMax;
                }
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({0, 190, 13});
            }
                break;
            case 2:
            case 3: {//满值
                auto currentDef = cityIt->second.m_bannerInfo.defenceMax;
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({190, 0, 0});
            }
                break;
            case 6: {//摧毁中
                double timePass = now - cityIt->second.m_bannerInfo.startTime;
                timePass = MAX(timePass, 0.0f);
                double defTemp = cityIt->second.m_bannerInfo.buildSpeed * timePass;
                auto currentDef = cityIt->second.m_bannerInfo.defence - defTemp;
                if (currentDef<=0) {
                    currentDef = 0;
                }
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({220, 176, 0});
            }
                break;
            default:
                break;
        }
    }
    if(flag){
        updateView();
    }
}

bool TerritoryBannerTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

TerritoryBannerTileState TerritoryBannerTile::getState() {
    int type = -1;
    TerritoryBannerTileState m_tempState;
    unsigned int index = m_cityInfo.cityIndex;
    auto cityIt = WorldController::getInstance()->m_cityInfo.find(index);
    if (cityIt != WorldController::getInstance()->m_cityInfo.end()) {
        type = cityIt->second.m_bannerInfo.state;
        if (cityIt->second.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
            if (type == 0) {
                m_tempState = TerritoryBannerBuilding;
            }
            else if (type == 1) {
                m_tempState = TerritoryBannerUnfinished;
            }
            else if (type == 2) {
                m_tempState = TerritoryBannerStation;
            }
            else if (type == 3) {
                m_tempState = TerritoryBannerUnstation;
            }
            else if (type == 4) {
                m_tempState = TerritoryBannerBroken;
            }
            else if (type == 5) {
                m_tempState = TerritoryBannerRepair;
            }
            else if (type == 6) {
                m_tempState = TerritoryBannerMineOccupy;
            }
        }
        else {
            if (type != 6) {
                m_tempState = TerritoryBannerOtherUnOccupy;
            }
            else {
                std::string m_allianceId = GlobalData::shared()->playerInfo.allianceInfo.uid;
                std::string m_uId = GlobalData::shared()->playerInfo.uid;
                if ((m_allianceId != "" && cityIt->second.m_allianceAreaOwnerInfo.allianceId == m_allianceId) || (m_allianceId == "" && cityIt->second.m_allianceAreaOwnerInfo.ownerId == m_uId)) {//非本联盟建筑被本联盟占领判断条件，待加
                    m_tempState = TerritoryBannerOtherOccupyByMe;
                }
                else
                    m_tempState = TerritoryBannerOtherOccupy;
            }
        }
    }
    return m_tempState;
}

SEL_CCControlHandler TerritoryBannerTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

BaseTileInfo* TerritoryBannerTile::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.cityIndex,m_title->getString());
//    return AddFavorite::create(m_cityInfo.cityIndex,m_cityInfo.m_bannerInfo.uid);
}

void TerritoryBannerTile::onClickButton(CCObject * pSender, CCControlEvent pCCControlEvent) {
    // todo
    auto button = dynamic_cast<CCControlButton*>(pSender);
    auto buttonState = getButtonState(button);
    WorldController::getInstance()->alertProectFlag = false;
    WorldController::getInstance()->m_alertStateFlag = -1;
    switch (buttonState) {
        case ButtonStation:
        case ButtonRepair:
        case ButtonBuilding:{
            if (soldierAlready >= soldierMax && soldierMax != 0) {
                CCCommonUtils::flyHint("", "", _lang("115403"), 3, 0, true);
            }
            else {
                if (dataBack) {
                    //WorldController::getInstance()->openMarchDeploy(m_cityInfo.cityIndex,0);
                    YuanJunTipView::openYuanYunView(m_cityInfo.cityIndex, tile_banner, 0);
                }
            }
        }
            break;
        case ButtonMarch: {
            // judge self protect state
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            
            unsigned int index = m_cityInfo.cityIndex;
            auto func = [&,index](){
                WorldController::getInstance()->openMarchDeploy(index,1);
            };
            
            if (now < selfProtect) {
                YesNoDialog::show(_lang("101438").c_str(),func);//E100050
            } else {
                func();
            }
        }
            break;
        case ButtonRally: {
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            if (now < selfProtect) {
                CCCommonUtils::flyHint("", "", _lang("108623"));
                closeThis();
                return;
            }
            if(!GlobalData::shared()->playerInfo.isInAlliance()){
                CCCommonUtils::flyHint("", "", _lang("115218"));
                closeThis();
                return;
            }
            unsigned int index = m_cityInfo.cityIndex;
            auto func = [&,index](){
                AllianceManager::getInstance()->openAllianceMassView(index);
                closeThis();
            };
            if (now < selfProtect) {
                YesNoDialog::show(_lang("101438").c_str(),func);
            } else {
                func();
                closeThis();
            }
            return;
        }
            break;
        case ButtonScout: {
            // judge self protect state
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            
            unsigned int index = m_cityInfo.cityIndex;
            auto func = [&,index](){
                auto world = WorldMapView::instance();
                if (world) {
                    world->addPopupView(ScoutInfo::create(WorldController::getInstance()->m_cityInfo[index]));
                }
            };
            
            if (now < selfProtect) {
                YesNoDialog::show(_lang("101438").c_str(),func);//E100050
            } else {
                func();
            }
        }
            break;
        case ButtonInformation: {
            unsigned int index = m_cityInfo.cityIndex;
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            PopupViewController::getInstance()->addPopupInView(TerritoryInfoDetailView::create(WorldController::getInstance()->m_cityInfo[index]));
        }
            break;
        case ButtonGoHome: {
            auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
            auto it = selfMarch.find(m_cityInfo.cityIndex);
            if (it == selfMarch.end()) {
                // todo : show error
                return;
            }
            string uuid = it->second;
            
            if (uuid.empty()) {
                // todo : show error
                return;
            }
            
            auto dict = CCDictionary::create();
            dict->setObject(CCString::create(uuid), "marchId");
            WorldMapView::instance()->afterMarchCancel(dict);
            
            vector<EnemyInfo>::iterator enemyIt = EnemyInfoController::getInstance()->m_enemyInfos.begin();
            for(auto &enemyInfo : EnemyInfoController::getInstance()->m_enemyInfos)
            {
                for(auto &marchInfo : WorldController::getInstance()->m_marchInfo)
                {
                    if((enemyInfo.type == ENEMY_TYPE_BATTLE || enemyInfo.type == ENEMY_TYPE_TEAM) && enemyInfo.uuid == marchInfo.second.uuid)
                    {
                        if(marchInfo.second.endPointIndex == m_cityInfo.cityIndex)
                        {
                            // 停止闪红
                            //                                enemyInfo.type = ENEMY_TYPE_TARGET_RETREAT;
                            EnemyInfoController::getInstance()->m_enemyInfos.erase(enemyIt);
                            break;
                        }
                    }
                }
                enemyIt++;
            }
        }
            break;
            
        default:
            break;
    }
    
    closeThis();
}