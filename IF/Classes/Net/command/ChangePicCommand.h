//
//  ChangePicCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-23.
//
//

#ifndef __IF__ChangePicCommand__
#define __IF__ChangePicCommand__

#include "CommandBase.h"

#define PIC_CHANGE "pic.change"
#define PIC_CHANGE_NEWICON "pic.time.change"

class ChangePicCommand: public CommandBase {
public:
    ChangePicCommand(std::string pic) : CommandBase(PIC_CHANGE),m_pic(pic){
        putParam("pic", CCString::create(m_pic.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_pic;
};

class ChangePicCommand_newIcon: public CommandBase {
public:
    ChangePicCommand_newIcon(std::string pic) : CommandBase(PIC_CHANGE_NEWICON),m_pic(pic){
        putParam("pic", CCString::create(m_pic.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_pic;
};
#endif /* defined(__IF__ChangePicCommand__) */