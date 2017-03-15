//
//  UseCKFSkillView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/10.
//
//

#include "UseCKFSkillView.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "ToolController.h"
#include "SceneController.h"
#include "YesNoDialog.h"
#include "MoveCityPopUpView.h"
#include "WorldMapView.h"
#include "BuildQueueView.h"
#include "WinPointsUseCommand.h"
#include "EnemyInfoController.h"
#include "ActivityController.h"
#include "ParticleController.h"

#define cellW (115)
#define CKF_CELL_CLICK  "ckf_cell_click"

UseCKFSkillView *UseCKFSkillView::create(){
    UseCKFSkillView *ret = new UseCKFSkillView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void UseCKFSkillView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseCKFSkillView::cellClick), CKF_CELL_CLICK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseCKFSkillView::refreshQueue), MSG_QUEUE_REFRESH_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseCKFSkillView::refreshData), MSG_WIN_POINT_CHANGE, NULL);
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}

void UseCKFSkillView::onExit(){
    setTouchEnabled(false);
    GameController::getInstance()->removeWaitInterface();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CKF_CELL_CLICK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUEUE_REFRESH_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WIN_POINT_CHANGE);
    PopupBaseView::onExit();
}

void UseCKFSkillView::refreshQueue(CCObject* param){
    if (m_speedNode->isVisible()) {
        initQueueData();
    }
}

void UseCKFSkillView::refreshData(CCObject* param){
    m_numTxt->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.winPoint));
    refreshQueue(NULL);
    m_tabView->reloadData();
}

void UseCKFSkillView::cellClick(CCObject* param){
    m_speedNode->setVisible(false);
    CCString* skillId = dynamic_cast<CCString*>(param);
    if (skillId) {
        string sid = skillId->getCString();
        if (sid == "610003") {
            m_speedNode->setVisible(true);
            initQueueData();
        }
    }
}

bool UseCKFSkillView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void UseCKFSkillView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30){
        return ;
    }
    if (m_speedNode->isVisible()) {
        if (!isTouchInside(m_speedArea, pTouch)) {
            PopupViewController::getInstance()->removePopupView(this);
        }
    }else{
        if(!isTouchInside(m_clickArea, pTouch)){
            PopupViewController::getInstance()->removePopupView(this);
        }
    }
}

bool UseCKFSkillView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(105, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(105, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
        });
        CCBLoadFile("UseCKFSkillView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_data = CCArray::create();
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionHorizontal);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(Touch_Default);
        m_listContainer->addChild(m_tabView);
        
        m_scrollView = CCScrollView::create(m_list->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(Touch_Default);
        m_list->addChild(m_scrollView);
        
        m_skillId = "";
        
        m_speedNode->setVisible(false);
        CCDictElement *element;
        auto dictSK = LocalController::shared()->DBXMLManager()->getGroupByKey("wb_skill");
        CCDICT_FOREACH(dictSK, element){
            std::string skillId = element->getStrKey();
            m_data->addObject(CCString::create(skillId.c_str()));
        }
        m_skillTitle->setString(_lang("138125"));
        m_speedTxt->setString(_lang("138144"));
        m_numTxt->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.winPoint));
        m_tabView->reloadData();
        
        ret = true;
        
        auto* cmd = new WinPointsOpenCommand();
        if(cmd){
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(UseCKFSkillView::getSkillTimeCallBack), NULL));
            cmd->sendAndRelease();
        }
        
    }
    return ret;
}


void UseCKFSkillView::getSkillTimeCallBack(CCObject* param)
{
    NetResult* result = dynamic_cast<NetResult*>(param);
    if (result==NULL)
        return ;
    auto dic = _dict(result->getData());
    if(dic){
        DictElement *ele;
        CCDICT_FOREACH(dic, ele)
        {
            std::string key = (std::string)ele->getStrKey();
            std::string value = std::string(dic->valueForKey(key)->getCString());
            double finalTime=atof(value.c_str());
            finalTime=GlobalData::shared()->changeTime(finalTime);
            ActivityController::getInstance()->abf_skillCD_map[key]=finalTime;
        }
    }
}

void UseCKFSkillView::onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->removePopupView(this);
}

bool UseCKFSkillView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_skillTitle", CCLabelIF*, this->m_skillTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sprL", CCSprite*, this->m_sprL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sprR", CCSprite*, this->m_sprR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speedNode", CCNode*, this->m_speedNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speedArea", CCNode*, this->m_speedArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_list", CCNode*, this->m_list);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speedTxt", CCLabelIF*, this->m_speedTxt);
    return false;
}

SEL_CCControlHandler UseCKFSkillView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", UseCKFSkillView::onCloseBtnClick);
    return NULL;
}



void UseCKFSkillView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize UseCKFSkillView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(cellW, 120);
}

CCTableViewCell* UseCKFSkillView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    UseCKFSkillCell* cell = (UseCKFSkillCell*)table->dequeueGrid();
    CCString* info = (CCString*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info->getCString());
        }else{
            cell = UseCKFSkillCell::create(info->getCString());
        }
    }
    return cell;
}

ssize_t UseCKFSkillView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_data->count();
}

ssize_t UseCKFSkillView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

void UseCKFSkillView::scrollViewDidScroll(CCScrollView* view){
    float dx = view->getContentOffset().x;
    //    CCLOG("%f",dx);
    if (dx>=0) {
        m_sprL->setVisible(false);
        m_sprR->setVisible(true);
    }else if (dx<=m_listContainer->getContentSize().width-(m_data->count()*cellW)) {
        m_sprL->setVisible(true);
        m_sprR->setVisible(false);
    }else{
        m_sprL->setVisible(true);
        m_sprR->setVisible(true);
    }
}

void UseCKFSkillView::initQueueData(){
    m_scrollView->getContainer()->removeAllChildrenWithCleanup(true);
    int queueCount = 0;
    map<int, QueueInfo>::iterator it;
    map<int, QueueInfo> tempQueuesInfo;
    for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
        
        if((it->second).finishTime >= WorldController::getInstance()->getTime() + 1000 && ((it->second).type == TYPE_MARCH
                                                                                           
                                                                                           || (it->second).type == TYPE_OCCUPY_RESOURCE
                                                                                           
                                                                                           || (it->second).type == TYPE_OCCUPY_ALLIANCE
                                                                                           
                                                                                           || (it->second).type == TYPE_OCCUPY_MAZE
                                                                                           || (it->second).type == TYPE_ARMY_MASS
                                                                                           ||(it->second).type == TYPE_OCCUPY_CAMP
                                                                                           || (it->second).type == TYPE_BUILDING_TERRITORY
                                                                                           || (it->second).type == TYPE_REPAIR_TERRITORY
                                                                                           || (it->second).type == TYPE_STATION_TERRITORY
                                                                                           || (it->second).type == TYPE_DESTROY_TERRITORY
                                                                                           || (it->second).type == TYPE_OCCUPY_TERRITORY_RESOURCE))
            
        {
            if (WorldController::getInstance()->m_marchInfo.find(it->second.uuid) != WorldController::getInstance()->m_marchInfo.end()) {
                if (WorldController::getInstance()->m_marchInfo[it->second.uuid].marchType == MethodHeiqishi) {
                    continue;
                }
            }
            tempQueuesInfo[it->first] = (it->second);
        }
    }
    
    int totalH = tempQueuesInfo.size()*67;
    int tmpY = totalH -14;
    for (it = tempQueuesInfo.begin(); it != tempQueuesInfo.end(); it++) {
        queueCount++;
        auto queueCell = BuildQueueView::create((it->second).key, (it->second).qid, (it->second).type,false,1);
        queueCell->setTag((it->second).qid);
        queueCell->setPosition(ccp(10,tmpY));
        tmpY -= 67;
        m_scrollView->addChild(queueCell);
    }
    m_scrollView->setContentSize(CCSize(m_list->getContentSize().width,totalH));
    m_scrollView->setContentOffset(ccp(0, m_list->getContentSize().height - totalH));
}


UseCKFSkillCell *UseCKFSkillCell::create(std::string skillId){
    UseCKFSkillCell *ret = new UseCKFSkillCell(skillId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void UseCKFSkillCell::setData(std::string skillId){
    m_skillId = skillId;
    string itemId = CCCommonUtils::getPropById(m_skillId, "item_id");
    std::string head = CCCommonUtils::getIcon(itemId);
    m_clickNode->setScale(0.7);
    m_icon->removeAllChildrenWithCleanup(true);
    iconSpr = CCLoadSprite::createSprite(head.c_str());
    iconSpr->setScale(0.7);
    m_icon->addChild(iconSpr);
    string skill_point = CCCommonUtils::getPropById(m_skillId, "skill_point");
    m_numTxt->setString(skill_point);
    int point = atoi(skill_point.c_str());
    if (GlobalData::shared()->playerInfo.winPoint>=point) {
        m_numTxt->setColor({255,255,255});
    }else{
        m_numTxt->setColor({255,0,0});
    }
    skillCDTime=this->getCDTime();
    isGray=false;
    this->unschedule(schedule_selector(UseCKFSkillCell::updateTime));
    this->updateTime(0);
    this->schedule(schedule_selector(UseCKFSkillCell::updateTime), 1);
}


void UseCKFSkillCell::updateTime(float dt)
{
    if(skillCDTime<=0){
        m_timeTxt->setVisible(false);
        this->updateIconColor(false);
        return;
    }
    m_timeTxt->setVisible(true);
    m_timeTxt->setString(CC_SECTOA(skillCDTime));
    if (dt>0) {
        skillCDTime--;
    }
    this->updateIconColor(true);
    
}

void UseCKFSkillCell::updateIconColor(bool isGray)
{
    m_cdNode->setVisible(isGray);
    if (this->isGray!=isGray) {
        this->isGray=isGray;
        //CCCommonUtils::setSpriteGray(iconSpr, isGray);
        m_effectNode->removeAllChildrenWithCleanup(true);
        if(isGray){
            string parStr = "SkillGlowR_";
            for (int i=1; i<=4; i++) {//SkillGlowB_ //SkillGlowG_ //SkillGlowR_
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",parStr.c_str(),i)->getCString());
                particle->setPosition(53, 60);
                m_effectNode->addChild(particle);
            }
        }
    }
}


double UseCKFSkillCell::getCDTime()
{
    std::string key="cdEndTime_"+m_skillId;
    map<std::string, double>::iterator it = ActivityController::getInstance()->abf_skillCD_map.find(key);
    double gapTime=-1;
    double wTime = GlobalData::shared()->getWorldTime();
    if(it != ActivityController::getInstance()->abf_skillCD_map.end()){
        if(it->second>0.0001){
            gapTime = it->second - wTime;
        }
    }
    return gapTime;
}


void UseCKFSkillCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void UseCKFSkillCell::onExit(){
    setTouchEnabled(false);
    this->unschedule(schedule_selector(UseCKFSkillCell::updateTime));
    CCNode::onExit();
}

bool UseCKFSkillCell::init(){
    auto bg = CCBLoadFile("UseCKFSkillCell", this, this);
    this->setContentSize(bg->getContentSize());
    m_effectNode = CCNode::create();
    this->addChild(m_effectNode,-100);
    iconSpr=NULL;
    //m_timeTxt->setColor({255,0,0});
    setData(m_skillId);
    return true;
}

bool UseCKFSkillCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_clickNode, pTouch)){
        return true;
    }
    return false;
}

void UseCKFSkillCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    if (skillCDTime>0) {
        return;
    }
    
    
    if(isTouchInside(m_clickNode, pTouch)){
        int id =atoi(m_skillId.c_str());
        string skill_point = CCCommonUtils::getPropById(m_skillId, "skill_point");
        int point = atoi(skill_point.c_str());
        switch (id) {
            case 610001:   //迁城
            {
                if (GlobalData::shared()->playerInfo.winPoint>=point) {
                    string itemId = CCCommonUtils::getPropById(m_skillId, "item_id");
                    ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
                    YesNoDialog::show(_lang_1("104955", info.getName().c_str()), CCCallFunc::create(this, callfunc_selector(UseCKFSkillCell::sureToUsePoint)));
                }else{
                    YesNoDialog::show(_lang("138160").c_str(), CCCallFunc::create(this, callfunc_selector(UseCKFSkillCell::sureToUsePoint)));
                }

            }
                break;
            case 610002:
            {
                AutoSafeRef temp(this);
                
                PopupViewController::getInstance()->removeAllPopupView();
                PopupViewController::getInstance()->addPopupView(MoveCityPopUpView::create(MoveCity_Castle,ITEM_SKILL_MOVE));
            }
                break;
            case 610003:
            {
//                PopupViewController:: getInstance()->removeAllPopupView();
//                PopupViewController::getInstance()->addPopupInView(CKFSpeedUpView::create());
            }
                break;
            case 610004:
            {
                if (GlobalData::shared()->playerInfo.winPoint>=point) {
                    string itemId = CCCommonUtils::getPropById(m_skillId, "item_id");
                    ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
                    YesNoDialog::show(_lang_1("104955", info.getName().c_str()), CCCallFunc::create(this, callfunc_selector(UseCKFSkillCell::sureToUsePoint)));
                }else{
                    YesNoDialog::show(_lang("138165").c_str(), NULL);
                    //YesNoDialog::show(_lang("138160").c_str(), CCCallFunc::create(this, callfunc_selector(UseCKFSkillCell::sureToUsePoint)));
                }
            }
                break;
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CKF_CELL_CLICK,CCString::create(m_skillId));
    }
}

void UseCKFSkillCell::sureToUsePoint(){

    string id = CCCommonUtils::getPropById(m_skillId, "item_id");
    int itemId = atoi(id.c_str());
    string skill_point = CCCommonUtils::getPropById(m_skillId, "skill_point");
    int point = atoi(skill_point.c_str());
    
    
    
    if (GlobalData::shared()->playerInfo.winPoint>=point) {
        if (m_skillId=="610001") {
            unsigned int current = WorldController::getInstance()->getCurrentMarchCount();
            if (current >= 1) {
                CCCommonUtils::flyHint("", "", _lang("108744"));
                return;
            }
            if (EnemyInfoController::getInstance()->m_curType==ENEMY_TYPE_BATTLE || EnemyInfoController::getInstance()->m_curType==ENEMY_TYPE_TEAM) {
                CCCommonUtils::flyHint("", "", _lang("108745"));
                return;
            }
        }
        WinPointsUseCommand* cmd = new WinPointsUseCommand(m_skillId);
        if(m_skillId=="610001"){
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(UseCKFSkillCell::successCallBack), NULL));
        }
        else if (m_skillId=="610004"){
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(UseCKFSkillCell::protectUseSuccess), NULL));
        }
        cmd->sendAndRelease();
    }else{
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
        if(itemId == ITEM_RANDOM && info.getCNT() > 0){
            auto dict = CCDictionary::create();
            dict->setObject(CCInteger::create(0), "point");
            dict->setObject(CCString::create(info.uuid), "goodsId");
            WorldMapView::instance()->afterCityMove(dict);
            PopupViewController::getInstance()->removeAllPopupView();
        }else if(info.getCNT() <= 0){
            ToolController::getInstance()->buyTool(itemId, 1, CCCallFunc::create(this, callfunc_selector(UseCKFSkillCell::onUseTool)),0,true,false);
            GameController::getInstance()->showWaitInterface();
        }else{
            ToolController::getInstance()->useTool(itemId,1,true);
        }
    }
}

void UseCKFSkillCell::onUseTool()
{
    GameController::getInstance()->removeWaitInterface();
    if(!this->getParent() || m_skillId==""){
        return;
    }
    string id = CCCommonUtils::getPropById(m_skillId, "item_id");
    int itemId = atoi(id.c_str());
    ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
    if(itemId == ITEM_RANDOM && toolInfo.getCNT() > 0){
        auto dict = CCDictionary::create();
        dict->setObject(CCInteger::create(0), "point");
        dict->setObject(CCString::create(toolInfo.uuid), "goodsId");
        WorldMapView::instance()->afterCityMove(dict);
        PopupViewController::getInstance()->removeAllPopupView();
    }else if(toolInfo.getCNT()==1){
        ToolController::getInstance()->useTool(itemId,1,true);
    }
}

void UseCKFSkillCell::successCallBack(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if (result==NULL) return ;
    auto dic = _dict(result->getData());
    if (dic->objectForKey("point")) {
        unsigned int index = dic->valueForKey("point")->intValue();
        auto selfPoint = WorldController::getPointByIndex(index);
        WorldController::getInstance()->selfPoint = selfPoint;
        if(WorldMapView::instance()){
            WorldMapView::instance()->onCityMoveCallback(dic->objectForKey("point"));
        }
    }
}


void UseCKFSkillCell::protectUseSuccess(CCObject* param)
{
    NetResult* result = dynamic_cast<NetResult*>(param);
    if (result==NULL) {
        return;
    }
    auto dic = _dict(result->getData());
    std::string key="cdEndTime_"+m_skillId;
    if (dic->objectForKey(key.c_str())) {
        std::string cdTime = std::string(dic->valueForKey(key)->getCString());
        double finalTime=atof(cdTime.c_str());
        finalTime=GlobalData::shared()->changeTime(finalTime);
        ActivityController::getInstance()->abf_skillCD_map[key] = finalTime;
    }
    skillCDTime=this->getCDTime();
    this->updateTime(0);
}

void UseCKFSkillCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}


bool UseCKFSkillCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cdNode", CCNode*, this->m_cdNode);
    return false;
}

SEL_CCControlHandler UseCKFSkillCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}