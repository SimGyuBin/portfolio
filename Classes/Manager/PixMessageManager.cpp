//
//  PixMessageManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 18..
//
//

#include "PixMessageManager.h"
#include "../Util/PixStringUtils.h"
#include "../Network/PixHttpClient.h"
#include "../Util/PixNativeUtils.h"
#include "../Common.h"

static PixMessageManager* _sharedObject = nullptr;

PixMessageManager* PixMessageManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixMessageManager();
    }
    
    return _sharedObject;
}

PixMessageManager::PixMessageManager()
: m_ManagerMap(nullptr)
, mstr_LanguageCode("")
{
    mstr_LanguageCode = Application::getInstance()->getCurrentLanguageCode();
}

PixMessageManager::~PixMessageManager()
{
    clear();
}

void PixMessageManager::clear()
{
    CC_SAFE_RELEASE_NULL(m_ManagerMap);
}

void PixMessageManager::init()
{
    clear();
    
    std::string fullPath = std::string(MASSAGE_INFO_PATH).append(MASSAGE_INFO_FILENAME);
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    contentStr = PixStringUtils::replaceAll(contentStr,".","+");
    contentStr = PixStringUtils::replaceAll(contentStr,"-","=");
    contentStr = PixStringUtils::replaceAll(contentStr,"_","/");
    contentStr = PixNativeUtils::nativeCallAES256Decrypt(contentStr);
    
    rapidjson::Document document;
    document.Parse<0>(contentStr.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        PixDictionary *root =  PixHttpClient::sharedObject()->getDictionary(document);
        
        if(root)
        {
            m_ManagerMap = root->dictionaryForKey("data");
            
            if(m_ManagerMap)
            {
                m_ManagerMap->retain();
            }
        }
    }
}

std::string PixMessageManager::getCodeName(const std::string &cd)
{
    if(m_ManagerMap)
    {
        PixDictionary *cdDic = m_ManagerMap->dictionaryForKey(cd);
    
        if (cdDic)
        {
            std::string msg = cdDic->stringForKey(mstr_LanguageCode);
            
            if(msg.empty())
            {
                std::string defaultMsg = cdDic->stringForKey(DEFAULT_LANGUAGE).c_str();

                if(defaultMsg.empty())
                {
                    return "";
                }
                
                return replaceAllMsg(defaultMsg);
            }
            
            return replaceAllMsg(msg);
        }
    }
    
    return "";
}

std::string PixMessageManager::getNetworkErrorMessage(bool blnRetry)
{
    if(blnRetry)
    {
        if (mstr_LanguageCode.compare("ko") == 0) return TEXT_001_ko;
        if (mstr_LanguageCode.compare("en") == 0) return TEXT_001_en;
        if (mstr_LanguageCode.compare("ja") == 0) return TEXT_001_ja;
        if (mstr_LanguageCode.compare("zh") == 0) return TEXT_001_zh;
        
        return TEXT_001_en;
    }
    else
    {
        if (mstr_LanguageCode.compare("ko") == 0) return TEXT_002_ko;
        if (mstr_LanguageCode.compare("en") == 0) return TEXT_002_en;
        if (mstr_LanguageCode.compare("ja") == 0) return TEXT_002_ja;
        if (mstr_LanguageCode.compare("zh") == 0) return TEXT_002_zh;
        
        return TEXT_002_en;
    }
}

std::string PixMessageManager::replaceAllMsg(const std::string &str)
{
    std::string replaceMsg = str;
    
    replaceMsg = PixStringUtils::replaceAll(replaceMsg, "\\n", "\n");
    
    return replaceMsg;
}
