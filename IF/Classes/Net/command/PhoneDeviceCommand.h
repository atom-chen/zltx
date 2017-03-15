//
//  PhoneDeviceCommand.h
//  IF
//
//  Created by liruiqi on 15/8/10.
//
//

#ifndef __IF__PhoneDeviceCommand__
#define __IF__PhoneDeviceCommand__

#include "CommandBase.h"

#define PHONE_DEVICE_CMD "save.phone.device"

class PhoneDeviceCommand: public CommandBase {
public:
    PhoneDeviceCommand(std::string deviceId,std::string qemuBkpt = "",std::string batteryStates = "") : CommandBase(PHONE_DEVICE_CMD){
        putParam("phoneDevice", CCString::create(deviceId));
        
        if(qemuBkpt != "")
        {
            putParam("qemuBkpt", CCString::create(qemuBkpt));
        }
        
        if(batteryStates != "")
        {
            putParam("batteryStates", CCString::create(batteryStates));
        }
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__PhoneDeviceCommand__) */
