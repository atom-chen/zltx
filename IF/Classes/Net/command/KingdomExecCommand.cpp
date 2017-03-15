//
//  KingdomExecCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-11.
//
//

#include "KingdomExecCommand.h"
#include "GameController.h"
#include "WorldController.h"

bool KingdomExecCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(KINGDOM_KING_EXEC) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        int type = -1;
        if (pStr->compare("111141")==0 && this->getData()!=NULL && this->getData()->objectForKey("type")) {
            type = this->getData()->valueForKey("type")->intValue();
        }
        double now = WorldController::getInstance()->getTime()/1000;
        double appointTime = params->valueForKey("appointTime")->doubleValue();
        double gapTime = appointTime - now;
        if (type==APPOINT && gapTime>0) {
            CCCommonUtils::flyText(_lang_1(pStr->getCString(),CC_SECTOA(gapTime)));
        }else{
            CCCommonUtils::flyText(_lang(pStr->getCString()));
        }
        CCLOG("KingdomExecCommand error=%s",pStr->getCString());
        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    GameController::getInstance()->removeWaitInterface();
    return true;
}