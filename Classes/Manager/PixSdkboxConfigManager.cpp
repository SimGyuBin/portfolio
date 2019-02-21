//
//  PixSdkboxConfigManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 19..
//
//

#include "PixSdkboxConfigManager.h"
#include "../Network//PixHttpClient.h"
#include "../Util/PixStringUtils.h"

static PixSdkboxConfigManager* _sharedObject = nullptr;
static std::string filename = "sdkbox_config.json";

PixSdkboxConfigManager* PixSdkboxConfigManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixSdkboxConfigManager();
        _sharedObject->init();
    }
    
    return _sharedObject;
}

PixSdkboxConfigManager::PixSdkboxConfigManager()
: configDic(nullptr)
, mstr_Platform("")
{
    CC_SAFE_RELEASE(configDic);
}

PixSdkboxConfigManager::~PixSdkboxConfigManager()
{
    
}

std::string PixSdkboxConfigManager::getOneSignalID()
{
    if(configDic)
    {
        PixDictionary *platformDic = configDic->dictionaryForKey(mstr_Platform);
        
        if(platformDic)
        {
            PixDictionary *oneSignalDic = platformDic->dictionaryForKey("OneSignal");
            
            if(oneSignalDic)
            {
                return oneSignalDic->stringForKey("id");
            }
        }
    }
    
    return "";
}

#pragma mark - private

void PixSdkboxConfigManager::init()
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    rapidjson::Document document;
    document.Parse<0>(contentStr.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        configDic = PixHttpClient::sharedObject()->getDictionary(document);
        configDic->retain();
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    mstr_Platform = "android";
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    mstr_Platform = "ios";
#endif
}




