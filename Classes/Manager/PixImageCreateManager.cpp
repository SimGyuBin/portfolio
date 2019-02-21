//
//  PixImageCreatieManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 22..
//
//

#include "PixImageCreateManager.h"
#include "../Scene/PixApp.h"
#include "../Info/LocalImageInfo.h"
#include "../Info/CategoryInfo.h"
#include "../Util/PixStringUtils.h"
#include "PixImageCacheManager.h"

static PixImageCreateManager *_sharedObject = nullptr;

PixImageCreateManager* PixImageCreateManager::sharedObject()
{
    if (_sharedObject == nullptr)
    {
        _sharedObject = new (std::nothrow) PixImageCreateManager();
        _sharedObject->init();
    }
    
    return _sharedObject;
}

PixImageCreateManager::PixImageCreateManager()
: _threadRefCount(0)
, _createCount(0)
, mn_ThreadPoolSize(THREAD_POOL_SIZE)
{
    _director = Director::getInstance();
    _scheduler = _director->getScheduler();
    
    _requestQueue.clear();
    _responseQueue.clear();
}

PixImageCreateManager::~PixImageCreateManager()
{

}

struct PixImageCreateManager::PixImageCreateStruct
{
public:
    
    PixImageCreateStruct(const std::string &strUrl, int nWidth, int nhHight, IMAGECREATE_TYPE eType, imageCreate_CallBack f)
    : url(strUrl)
    , width(nWidth)
    , height(nhHight)
    , type(eType)
    , callBack(f)
    {
        
    }
    
    std::string url;
    int width;
    int height;
    IMAGECREATE_TYPE type;
    imageCreate_CallBack callBack;
};

void PixImageCreateManager::init()
{
    
}

void PixImageCreateManager::clear()
{
    stopScheduler();
    
    _threadRefCount = 0;
    _createCount = 0;
    mn_ThreadPoolSize = THREAD_POOL_SIZE;
    
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

void PixImageCreateManager::imageCreate(const std::string &strUrl, int nWidth, int nhHight, IMAGECREATE_TYPE eType, imageCreate_CallBack callBack)
{
    if (0 == _createCount)
    {
        startScheduler();
    }
    
    PixImageCreateStruct *creatieStruct = new (std::nothrow) PixImageCreateStruct(strUrl, nWidth, nhHight, eType, callBack);
    
    _requestQueue.push_back(creatieStruct);
    
    ++_createCount;
}

void PixImageCreateManager::imageCreateComplete(const std::string &strUrl, int nWidth, int nhHight, IMAGECREATE_TYPE eType)
{
    if(_responseQueue.empty())
        return;
    
    for (auto it = _responseQueue.begin(); it != _responseQueue.end(); ++it)
    {
        if((*it)->url.compare(strUrl) == 0 && (*it)->type == eType)
        {
            PixImageCreateStruct *creatieStruct = (*it);
            _responseQueue.erase(it);
            delete creatieStruct;
            --_threadRefCount;
            --_createCount;
            break;
        }
    }
}

bool PixImageCreateManager::unbindImage(const std::string &strUrl, IMAGECREATE_TYPE eType)
{
    bool IsUnbindImage = false;
    
    for (auto it = _requestQueue.begin(); it != _requestQueue.end(); ++it)
    {
        if((*it)->url.compare(strUrl) == 0 && (*it)->type == eType)
        {
            PixImageCreateStruct *createStruct = (*it);
            createStruct->url.clear();
            IsUnbindImage = true;
            break;
        }
    }
    
    if(IsUnbindImage)
    {
        imageCreateComplete(strUrl, 0, 0, eType);
    }
    
    return IsUnbindImage;
}

bool PixImageCreateManager::isImageCreate(const std::string &strUrl, IMAGECREATE_TYPE eType)
{
    if(_requestQueue.empty())
        return false;
    
    if(_responseQueue.empty())
        return false;
    
    bool isRequest = false;
    bool isResponse = false;
    
    for (auto it = _requestQueue.begin(); it != _requestQueue.end(); ++it)
    {
        if((*it)->url.compare(strUrl) == 0 && (*it)->type == eType)
        {
            isRequest = true;
            break;
        }
    }
    
    for (auto it = _responseQueue.begin(); it != _responseQueue.end(); ++it)
    {
        if((*it)->url.compare(strUrl) == 0 && (*it)->type == eType)
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

void PixImageCreateManager::setLocalImages(LocalImageInfo &pInfo)
{
    if(PIXAPP->userInfo->getLocalImageInfoByPath(pInfo.getPath()))
        return;
    
    LocalImageInfo *info = new LocalImageInfo(pInfo);
    
    std::string strDate = info->getFullDate();
    

    strDate = PixStringUtils::replaceAll(strDate, ".", "");
    
#if (ALBUM_MAKE_ONE_PIC == true)
    // 앨범 사진 1장씩 들어가게 만드는 테스트 코드
    int nlastIndex = PIXAPP->userInfo->getLocalImageCount() - 1;
    
    LocalImageInfo *pLastinfo = PIXAPP->userInfo->getLocalImageInfo(nlastIndex);
    
    if(pLastinfo)
    {
        long long test = atoll(pLastinfo->getFullDate().c_str());
        test += 10000000;
        strDate = StringUtils::format("%lld",test);
    }
#endif
    
    info->setFullDate(strDate);
    
    PIXAPP->userInfo->pushLocalImageInfo(info);
    
//    CCLOG("simsimsim setLocalImages path : %s", info->getPath().c_str());
//    CCLOG("simsimsim setLocalImages date_modified : %s", info->getDateModified().c_str());
//    CCLOG("simsimsim setLocalImages date_added : %s", info->getDateAdded().c_str());
//    CCLOG("simsimsim setLocalImages fulldate : %s", info->getFullDate().c_str());
//    CCLOG("simsimsim setLocalImages size : %s", info->getFileSize().c_str());
//    CCLOG("simsimsim setLocalImages width : %s", info->getWidth().c_str());
//    CCLOG("simsimsim setLocalImages height : %s", info->getHeight().c_str());
}

void PixImageCreateManager::addThumbnailData(const std::string &strPath, Data *pData, const std::string &strWidth, const std::string &strHeight)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {

        PIXAPP->userInfo->addThumbnailData(strPath, pData, strWidth, strHeight);
        
        for (auto it = _responseQueue.begin(); it != _responseQueue.end(); ++it)
        {
            if((*it)->url.compare(strPath) == 0 && (*it)->type == IMAGECREATE_THUMBNAIL)
            {
                PixImageCreateStruct *creatieStruct = (*it);
                
                if(creatieStruct->callBack)
                {
                    (creatieStruct->callBack)(pData, strPath, strWidth, strHeight, "");
                    break;
                }
            }
        }
        
        imageCreateComplete(strPath, 0, 0, IMAGECREATE_THUMBNAIL);
        
    });
}

void PixImageCreateManager::addFullScreenData(const std::string &strPath, Data *pData, const std::string &strWidth, const std::string &strHeight)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        PIXAPP->userInfo->addFullScreenData(strPath, pData, strWidth, strHeight);
        
        for (auto it = _responseQueue.begin(); it != _responseQueue.end(); ++it)
        {
            if((*it)->url.compare(strPath) == 0 && (*it)->type == IMAGECREATE_FULLSCREEN)
            {
                PixImageCreateStruct *creatieStruct = (*it);
                
                if(creatieStruct->callBack)
                {
                    (creatieStruct->callBack)(pData, strPath, strWidth, strHeight, "");
                    break;
                }
            }
        }
        
        imageCreateComplete(strPath, 0, 0, IMAGECREATE_FULLSCREEN);
        
    });
}

bool PixImageCreateManager::isTheradPool()
{
    if(_threadRefCount < getThreadPoolSize())
    {
        return true;
    }
    
    return false;
}

void PixImageCreateManager::startScheduler()
{
    if(!_scheduler->isScheduled(schedule_selector(PixImageCreateManager::imageCreateSchedule), this))
    {
        _scheduler->schedule(schedule_selector(PixImageCreateManager::imageCreateSchedule), this, 0, false);
    }
}

void PixImageCreateManager::stopScheduler()
{
    if(_scheduler->isScheduled(schedule_selector(PixImageCreateManager::imageCreateSchedule), this))
    {
        _scheduler->unschedule(schedule_selector(PixImageCreateManager::imageCreateSchedule),this);
    }
}

void PixImageCreateManager::imageCreateSchedule(float dt)
{
    while (true)
    {
        if(_threadRefCount < getThreadPoolSize())
        {
            PixImageCreateStruct *createStruct = nullptr;
            
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
            
            if(createStruct->type == IMAGECREATE_THUMBNAIL)
            {
                PixNativeUtils::nativeCallGetThumbnailImage(createStruct->url, createStruct->width, createStruct->height);
            }
            else if(createStruct->type == IMAGECREATE_FULLSCREEN)
            {
                PixNativeUtils::nativeCallGetFullScreenImage(createStruct->url, createStruct->width, createStruct->height);
            }
       
            _responseQueue.push_back(createStruct);
            
            ++_threadRefCount;
        }
        else
        {
            break;
        }
    }
}



