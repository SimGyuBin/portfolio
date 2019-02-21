//
//  PixServerUrlManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 25..
//
//

#include "PixServerUrlManager.h"
#include "../Network/PixHttpClient.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

static PixServerUrlManager* _sharedObject = nullptr;

PixServerUrlManager* PixServerUrlManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixServerUrlManager();
    }
    
    return _sharedObject;
}

PixServerUrlManager::PixServerUrlManager()
: dictionary(nullptr)
, serverConfig(nullptr)
, mbln_IsDev(false)
{
    
}

PixServerUrlManager::~PixServerUrlManager()
{
    clear();
}

void PixServerUrlManager::clear()
{
    CC_SAFE_RELEASE_NULL(dictionary);
    CC_SAFE_DELETE(serverConfig);
}

void PixServerUrlManager::init()
{
    clear();
    
    std::string fullPath = std::string(SERVER_INFO_PATH).append(SERVER_INFO_FILENAME);
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    contentStr = PixStringUtils::replaceAll(contentStr,".","+");
    contentStr = PixStringUtils::replaceAll(contentStr,"-","=");
    contentStr = PixStringUtils::replaceAll(contentStr,"_","/");
    contentStr = PixNativeUtils::nativeCallAES256Decrypt(contentStr);
    
    rapidjson::Document document;
    document.Parse<0>(contentStr.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        dictionary = PixHttpClient::sharedObject()->getDictionary(document);
        dictionary->retain();
        
        if(dictionary->stringForKey("TYPE").compare("dev") == 0)
        {
            mbln_IsDev = true;
        }
        
        PixDictionary *config = dictionary->dictionaryForKey("CONFIG");
        
        if(config)
        {
            serverConfig = new PixServerConfig();
            serverConfig->setImage_max_upload(config->intForKey("image_max_upload"));
            serverConfig->setResouce_download_timeout(config->doubleForKey("resouce_download_timeout"));
        }
    }
}

std::string PixServerUrlManager::getUrl(const std::string &key)
{
    if(dictionary)
    {
        PixDictionary *urlDic = dictionary->dictionaryForKey(key);
        
        if(urlDic)
        {
            return urlDic->stringForKey("url");
        }
    }
    
    return "error";
}

std::string PixServerUrlManager::getPhotoServerUrl()
{
    return getUrl("PHOTO_SERVER");
}

std::string PixServerUrlManager::getResourceServerUrl()
{
    return getUrl("RESOURCE_SERVER");
}

std::string PixServerUrlManager::getAlbumServerUrl()
{
    return getUrl("ALBUM_SERVER");
}

PixArray* PixServerUrlManager::getBank()
{
    if(dictionary)
    {
        return dictionary->arrayForKey("BANK");
    }
    
    return nullptr;
}

PixDictionary* PixServerUrlManager::getWebViewDictionary()
{
    if(dictionary)
    {
        return dictionary->dictionaryForKey("WEBVIEW");
    }
    
    return nullptr;
}

PixDictionary* PixServerUrlManager::getUpdateInfo()
{
    if(dictionary)
    {
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return dictionary->dictionaryForKey("AOS_UPDATE");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        return dictionary->dictionaryForKey("IOS_UPDATE");
#endif
        
    }
    
    return nullptr;
}

PixDictionary* PixServerUrlManager::getServerNoticeInfo()
{
    if(dictionary)
    {
        return dictionary->dictionaryForKey("SERVER_NOTICE");
    }
    
    return nullptr;
}

std::string PixServerUrlManager::getWebViewUrl(const std::string &key)
{
    PixDictionary *webView = getWebViewDictionary();
    
    if(webView)
    {
        PixDictionary *dic = webView->dictionaryForKey(key);
        
        if(dic)
        {
            return dic->stringForKey("url");
        }
    }
    
    return "";
}

std::string PixServerUrlManager::getBoardWebViewUrl(BOARD_TYPE type, int nDetail_ID)
{
    std::string strUrl = getWebViewUrl("BOARD_DETAIL");
    
    if(!strUrl.empty())
    {
        strUrl.append(StringUtils::format("%d",(int)type));
        strUrl.append("/");
        strUrl.append(StringUtils::format("%d",(int)nDetail_ID));
        
        return strUrl;
    }
    
    return "";
}

std::string PixServerUrlManager::getAppUrl()
{
 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return std::string(AOS_APP_URL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return std::string(IOS_APP_URL);
#endif
    
    return "";
}

int PixServerUrlManager::getImageMaxUploadCount()
{
    if(serverConfig)
    {
        return serverConfig->getImage_max_upload();
    }
    
    return 0;
}

float PixServerUrlManager::getResouceDownloadTimeout()
{
    if(serverConfig)
    {
        return serverConfig->getResouce_download_timeout();
    }
    
    return 0.0f;
}
