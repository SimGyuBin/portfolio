//
//  PixSdkboxConfigManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 19..
//
//

#ifndef PixSdkboxConfigManager_h
#define PixSdkboxConfigManager_h

#include "cocos2d.h"

#define SDKBOXCONFING_MANAGER     (PixSdkboxConfigManager::sharedObject())

USING_NS_CC;

class PixDictionary;

class PixSdkboxConfigManager
{
public:
    static PixSdkboxConfigManager* sharedObject();
    
    PixSdkboxConfigManager();
    virtual ~PixSdkboxConfigManager();
    
    std::string getOneSignalID();
    
private:
    
    PixDictionary *configDic;
    std::string mstr_Platform;
    
    void init();
};


#endif /* PixSdkboxConfigManager_h */
