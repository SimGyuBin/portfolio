//
//  PixOneSignalManager.hpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 19..
//
//

#ifndef PixOneSignalManager_h
#define PixOneSignalManager_h


#include "cocos2d.h"

#define ONESIGNAL_MANAGER     (PixOneSignalManager::sharedObject())

USING_NS_CC;

class PixOneSignalAdditionalData
{
    
public:
    PixOneSignalAdditionalData(const std::string &strMessage, const std::string &strTitle, const std::string &strPush_evevt, bool blnActive)
    : mstr_Message(strMessage)
    , mstr_Title(strTitle)
    , mstr_Push_evevt(strPush_evevt)
    , mbln_Active(blnActive)
    {
        
    }
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Message, Message)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Push_evevt, Push_evevt)
    CC_SYNTHESIZE(bool, mbln_Active, Active)
};

class PixOneSignalManager
{
public:
    static PixOneSignalManager* sharedObject();
    
    PixOneSignalManager();
    virtual ~PixOneSignalManager();
    
    void init(const std::string &strUserID, const std::string &strPushToken);
    void setSubscription(bool enable);
    void setOneSignalHandleNotification(const std::string &strMessage, const std::string &strTitle, const std::string &strPush_event, bool blnActive);
    void pushEventExecution();
    void pushEventExecution(const std::string &strMessage, const std::string &strTitle, const std::string &strPush_event, bool blnActive);
    void moveEventPage(const std::string &strPush_event);
    std::string getUserID();
    std::string getPushToken();
    bool isAdditionalData();

    void setIntroFinish(bool var);
    bool getIntroFinish();
    
private:

    std::string mstr_UserID;
    std::string mstr_PushToken;

    bool mbln_IntroFinish;
    
    std::deque<PixOneSignalAdditionalData*> additionalDataQueue;
};

#endif /* PixOneSignalManager_h */
