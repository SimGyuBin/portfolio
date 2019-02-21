//
//  PixUploadImageManager.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 1. 19..
//
//

#include "PixUploadImageManager.h"
#include "../Scene/PixApp.h"
#include "../Info/LocalImageInfo.h"
#include "../Info/CategoryInfo.h"
#include "../Util/PixStringUtils.h"
#include "PixImageCacheManager.h"

static PixUploadImageManager *_sharedObject = nullptr;

PixUploadImageManager* PixUploadImageManager::sharedObject()
{
    if (_sharedObject == nullptr)
    {
        _sharedObject = new (std::nothrow) PixUploadImageManager();
        _sharedObject->init();
    }
    
    return _sharedObject;
}

PixUploadImageManager::PixUploadImageManager()
: _threadRefCount(0)
, _createCount(0)
, mn_ThreadPoolSize(ALBUM_SAVE_POOL_SIZE)
{
    _director = Director::getInstance();
    _scheduler = _director->getScheduler();
    
    _requestQueue.clear();
    _responseQueue.clear();
}

PixUploadImageManager::~PixUploadImageManager()
{
    
}

struct PixUploadImageManager::PixImageUploadStruct
{
public:
    
    PixImageUploadStruct(const std::string &strUrl, int nWidth, int nhHight, uploadImage_CallBack f)
    : url(strUrl)
    , width(nWidth)
    , height(nhHight)
    , callBack(f)
    {
        
    }
    
    std::string url;
    int width;
    int height;
    uploadImage_CallBack callBack;
};

void PixUploadImageManager::init()
{
    
}

void PixUploadImageManager::clear()
{
    stopScheduler();
    
    _threadRefCount = 0;
    _createCount = 0;
    mn_ThreadPoolSize = ALBUM_SAVE_POOL_SIZE;
    
    for (auto it = _requestQueue.begin(); it != _requestQueue.end(); ++it)
    {
        CC_SAFE_DELETE((*it));
    }
    
    for (auto it = _responseQueue.begin(); it != _responseQueue.end(); ++it)
    {
        CC_SAFE_DELETE((*it));
    }
    
    _requestQueue.clear();
    _responseQueue.clear();
}

void PixUploadImageManager::imageCreate(const std::string &strUrl, int nWidth, int nhHight, uploadImage_CallBack callBack)
{
    if (0 == _createCount)
    {
        startScheduler();
    }
    
    PixImageUploadStruct *creatieStruct = new (std::nothrow) PixImageUploadStruct(strUrl, nWidth, nhHight, callBack);
    
    _requestQueue.push_back(creatieStruct);
    
    ++_createCount;
}

void PixUploadImageManager::imageCreateComplete(const std::string &strUrl, int nWidth, int nhHight)
{
    if(_responseQueue.empty())
        return;
    
    for (auto it = _responseQueue.begin(); it != _responseQueue.end(); ++it)
    {
        if((*it)->url.compare(strUrl) == 0)
        {
            PixImageUploadStruct *creatieStruct = (*it);
            _responseQueue.erase(it);
            delete creatieStruct;
            --_threadRefCount;
            --_createCount;
            break;
        }
    }
}

bool PixUploadImageManager::unbindImage(const std::string &strUrl)
{
    bool IsUnbindImage = false;
    
    for (auto it = _requestQueue.begin(); it != _requestQueue.end(); ++it)
    {
        if((*it)->url.compare(strUrl) == 0)
        {
            PixImageUploadStruct *createStruct = (*it);
            createStruct->url.clear();
            IsUnbindImage = true;
            break;
        }
    }
    
    if(IsUnbindImage)
    {
        imageCreateComplete(strUrl, 0, 0);
    }
    
    return IsUnbindImage;
}

bool PixUploadImageManager::isImageCreate(const std::string &strUrl)
{
    if(_requestQueue.empty())
        return false;
    
    if(_responseQueue.empty())
        return false;
    
    bool isRequest = false;
    bool isResponse = false;
    
    for (auto it = _requestQueue.begin(); it != _requestQueue.end(); ++it)
    {
        if((*it)->url.compare(strUrl) == 0)
        {
            isRequest = true;
            break;
        }
    }
    
    for (auto it = _responseQueue.begin(); it != _responseQueue.end(); ++it)
    {
        if((*it)->url.compare(strUrl) == 0)
        {
            isRequest = true;
            break;
        }
    }
    
    if(isRequest == false && isResponse == false)
    {
        return false;
    }
    
    
    return true;
}

void PixUploadImageManager::uploadImageComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        for (auto it = _responseQueue.begin(); it != _responseQueue.end(); ++it)
        {
            if((*it)->url.compare(strUrl) == 0)
            {
                PixImageUploadStruct *creatieStruct = (*it);
                
                if(creatieStruct->callBack)
                {
                    (creatieStruct->callBack)(data, strUrl, strWidth, strHeight, strDate);
                    break;
                }
            }
        }
        
        imageCreateComplete(strUrl, 0, 0);
    });
}

bool PixUploadImageManager::isTheradPool()
{
    if(_threadRefCount < getThreadPoolSize())
    {
        return true;
    }
    
    return false;
}

void PixUploadImageManager::startScheduler()
{
    if(!_scheduler->isScheduled(schedule_selector(PixUploadImageManager::imageCreateSchedule), this))
    {
        _scheduler->schedule(schedule_selector(PixUploadImageManager::imageCreateSchedule), this, 0, false);
    }
}

void PixUploadImageManager::stopScheduler()
{
    if(_scheduler->isScheduled(schedule_selector(PixUploadImageManager::imageCreateSchedule), this))
    {
        _scheduler->unschedule(schedule_selector(PixUploadImageManager::imageCreateSchedule),this);
    }
}

void PixUploadImageManager::imageCreateSchedule(float dt)
{
    while (true)
    {
        if(_threadRefCount < getThreadPoolSize())
        {
            PixImageUploadStruct *createStruct = nullptr;
            
            if(_requestQueue.empty())
            {
                createStruct  = nullptr;
            }
            else
            {
                createStruct = _requestQueue.front();
                _requestQueue.pop_front();
            }
            
            if (nullptr == createStruct)
            {
                break;
            }
            
            if(createStruct->url.empty())
            {
                delete createStruct;
                --_createCount;
                continue;
            }
            
            PixNativeUtils::nativeCallGetUploadImage(createStruct->url);
            
            _responseQueue.push_back(createStruct);
            
            ++_threadRefCount;
        }
        else
        {
            break;
        }
    }
}
