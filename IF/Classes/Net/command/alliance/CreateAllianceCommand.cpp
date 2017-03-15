//
//  CreateAllianceCommand.cpp
//  IF
//
//  Created by chenliang on 14-4-2.
//
//

#include "CreateAllianceCommand.h"
#include "NetController.h"
#include "UIComponent.h"
#include "YesNoDialog.h"
#include "ChatServiceCocos2dx.h"

static string COMMAND_NAME = "al.create";

CreateAllianceCommand::CreateAllianceCommand()
: CommandBase(COMMAND_NAME)
{
    
}

bool CreateAllianceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_NAME.c_str()) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        if (pStr->compare("E100001") == 0) {
            YesNoDialog::gotoPayTips();
        }else if (pStr->compare("115506") == 0) {
            CCCommonUtils::flyText(_lang_1("115506", CC_ITOA(GlobalData::shared()->alliance_cost_k8)));
        }
        else{
            CCCommonUtils::flyText(_lang(pStr->getCString()));
        }
        callFail(NetResult::create(Error_OK, params));
    }else{
        if(params->objectForKey("gold")){
            GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        //GlobalData::shared()->playerInfo.isfirstJoin = 0;
        UIComponent::getInstance()->showFlygold();
        callSuccess(NetResult::create(Error_OK, (CCObject*)(params->objectForKey("alliance"))));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::setPlayerAllianceInfo(GlobalData::shared()->playerInfo.allianceInfo.shortName.c_str(),GlobalData::shared()->playerInfo.getAllianceId().c_str(),GlobalData::shared()->playerInfo.allianceInfo.rank ,GlobalData::shared()->playerInfo.isfirstJoin);
#endif
    }
    return true;
}