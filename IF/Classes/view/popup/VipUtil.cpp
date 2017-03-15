//
//  VipUtil.cpp
//  IF
//
//  Created by chenliang on 14-3-20.
//
//

#include "VipUtil.h"
#include "LocalController.h"
#include "CCTypesExt.h"
#include "GlobalData.h"
#include "VipActiveView.h"
#include "PopupViewController.h"

#define VIP_ACTIVE_KEY "vip.active.key"

int VipUtil::getVipLevel(long vipPoints)
{
    int vipLevel = -1;
    if(!LocalController::shared()->DBXMLManager()){
        return 0;
    }
    CCDictionary* vipGroup = LocalController::shared()->DBXMLManager()->getGroupByKey("vip");
    for (int i=VipConstants::VIP_DATA_ID_OFFSET+vipGroup->count()-1; i>=VipConstants::VIP_DATA_ID_OFFSET; i--)
    {
        std::string itemId = CC_ITOA(i);
        
        CCDictionary* itemAttributes = dynamic_cast<CCDictionary*>(vipGroup->objectForKey(itemId));
        if(itemAttributes){
            int vipPointsRequired = itemAttributes->valueForKey(KEY_VIP_POINTS_REQUIRED)->intValue();
            if (vipPointsRequired <= vipPoints) {
                vipLevel = i-VipConstants::VIP_DATA_ID_OFFSET;
                break;
            }
        }
    }
    
    if (vipLevel < 0) {
        vipLevel = vipGroup->count() - 1;
    }
    
    return vipLevel+1;
    
}
int VipUtil::getVipItemValueByKey(std::string itemId, std::string key){
   std:: map<int, CCDictionary*>::iterator it;
    int value = 0;
    for(it = GlobalData::shared()->vipListMap.begin(); it != GlobalData::shared()->vipListMap.end(); it++){
        CCDictionary* dict = it->second;
        string id = dict->valueForKey("id")->getCString();
        if(id==itemId){
            value = dict->valueForKey(key)->intValue();
            break;
        }

    }
    return value;
}
int VipUtil::getCurVipValueByKey(std::string key){
   int vipLv = VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints);
    int value = 0;
    if(GlobalData::shared()->playerInfo.vipEndTime>GlobalData::shared()->getWorldTime()){
        value  = getVipItemValueByKey(CC_ITOA(vipLv+7000-1),key);
    }
    
    return value;

}
bool VipUtil::showVIPActiveView(){
    if(  IsQuitGame() == true )
        return false;
    
    auto &playerInfo = GlobalData::shared()->playerInfo;
    int dTime = playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
    bool isActive = (dTime>0)?true:false;
    if(isActive){
        CCUserDefault::sharedUserDefault()->setDoubleForKey(VIP_ACTIVE_KEY, playerInfo.vipEndTime);
        CCUserDefault::sharedUserDefault()->flush();
    }else{
        double active = CCUserDefault::sharedUserDefault()->getDoubleForKey(VIP_ACTIVE_KEY);
        if(active>0){
            if(active < GlobalData::shared()->getWorldTime()){
                PopupViewController::getInstance()->addPopupView(VipActiveView::create());
                CCUserDefault::sharedUserDefault()->setDoubleForKey(VIP_ACTIVE_KEY, 0);
                CCUserDefault::sharedUserDefault()->flush();
                return true;
            }
        }
    }
    return false;
}

bool VipUtil::isSVIP(){
    return false;
//    return true;
}

int VipUtil::getMaxSpoint(){
    int svip_id = 7010+getSvipShowLevel();
    return atoi(CCCommonUtils::getPropById(CC_ITOA(svip_id), "spoint").c_str());
}

int VipUtil::getMaxPoint(){
    if(isSVIP()){
        return getSvip1Point();
    }
    return getVip10Point();
}

int VipUtil::getVip10Point(){
    return atoi(CCCommonUtils::getPropById("7009", "point").c_str());
}
int VipUtil::getSvip1Point(){
    return atoi(CCCommonUtils::getPropById("7010", "point").c_str());
}
int VipUtil::getSvipShowLevel(){
    string display = CCCommonUtils::getPropById("7100", "display").c_str();
    if(display!=""){
        int level = atoi(display.c_str());
        return level-1;
    }
    return 10-1;
}
bool VipUtil::isSvipPointId(int itemId){
    return (itemId >= 200920 && itemId <= 200926);
}

bool VipUtil::isVipPointId(int itemId){
    return (itemId >= 200350 && itemId <= 200353);
}

bool VipUtil::isSvipTimeId(int itemId){
    return (itemId >=200900  && itemId <=200904 );
}

bool VipUtil::isVipTimeId(int itemId){
    return (itemId >=200370  && itemId <=200374 );
}
