//
//  PixImageAsyncManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 28..
//
//

#include "PixImageLoader.h"

PixImageLoader::PixImageLoader()
: _loadingThread(nullptr)
, _asyncRefCount(0)
, _needQuit(false)
{
    
}

PixImageLoader::~PixImageLoader()
{
    clear();
}

struct PixImageLoader::PixImageLoaderStruct
{
public:
    
    PixImageLoaderStruct(const std::string& fn, Data *d, Ref *t,  SEL_ImageLoader s, void* pUserData)
    : filename(fn)
    , target(t)
    , selector(s)
    , image(nullptr)
    , loadSuccess(false)
    , userData(pUserData)
    {
        if(d != nullptr && !d->isNull())
        {
            data.copy(d->getBytes(), d->getSize());
        }
    }
    
    ~PixImageLoaderStruct()
    {
        data.clear();
    }
    
    bool initWithImageFileThreadSafe()
    {
        bool ret = false;
        
        if (!data.isNull())
        {
            ssize_t size = data.getSize();
            unsigned char *bytes = (unsigned char*)malloc(sizeof(unsigned char) * size);
            memcpy(bytes, data.getBytes(), size);
            
            image = new (std::nothrow) Image();
            ret = image->initWithImageData(bytes, size);
            free(bytes);
        }
        else
        {
            std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
            Data dataFromFile = FileUtils::getInstance()->getDataFromFile(fullPath);
            
            image = new (std::nothrow) Image();
            ret = image->initWithImageData(dataFromFile.getBytes(), dataFromFile.getSize());
        }
        
        return ret;
    }
    
    std::string filename;
    Data data;
    Ref *target;
    SEL_ImageLoader selector;
    Image *image;
    bool loadSuccess;
    void *userData;
};


void PixImageLoader::clear()
{
    for (auto it = _imageLoaderQueue.begin(); it != _imageLoaderQueue.end(); ++it)
    {
        (*it)->target = nullptr;
        (*it)->selector = nullptr;
        CC_SAFE_DELETE((*it));
    }
    
    waitForQuit();
    CC_SAFE_DELETE(_loadingThread);
    
    _asyncRefCount = 0;
           
    _imageLoaderQueue.clear();
    _requestQueue.clear();
    _responseQueue.clear();
    
    stopScheduler();
    
}

void PixImageLoader::startScheduler()
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    
    if(scheduler->isScheduled(schedule_selector(PixImageLoader::imageLoaderSchedule), this) == false)
    {
        scheduler->schedule(schedule_selector(PixImageLoader::imageLoaderSchedule), this, 0, false);
    }
}

void PixImageLoader::stopScheduler()
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(PixImageLoader::imageLoaderSchedule),this);
}

void PixImageLoader::initloadingThread()
{
    if (_loadingThread == nullptr)
    {
        _loadingThread = new std::thread(&PixImageLoader::loadImage, this);
        _needQuit = false;
    }
}

void PixImageLoader::addImageAsync(const std::string &filepath, Ref *target, SEL_ImageLoader selector, void* pUserData)
{
    Texture2D *texture = nullptr;
    
    texture = Director::getInstance()->getTextureCache()->getTextureForKey(filepath);
    
    if(nullptr != texture)
    {
        if (target != nullptr && selector != nullptr)
        {
            (target->*selector)(texture, pUserData);
        }
        
        return;
    }
    
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(filepath);

    if (fullpath.empty() || ! FileUtils::getInstance()->isFileExist(fullpath))
    {
        if (target != nullptr && selector != nullptr)
        {
            (target->*selector)(nullptr, pUserData);
        }
        
        return;
    }

    initloadingThread();
    
    if (0 == _asyncRefCount)
    {
        startScheduler();
    }
    
    ++_asyncRefCount;
    
    PixImageLoaderStruct *LoaderStruct = new (std::nothrow) PixImageLoaderStruct(fullpath, nullptr, target, selector, pUserData);
    
    _imageLoaderQueue.push_back(LoaderStruct);
    _requestMutex.lock();
    _requestQueue.push_back(LoaderStruct);
    _requestMutex.unlock();
    
    _sleepCondition.notify_one();
    
}

void PixImageLoader::addImageAsync(const std::string &key, Data *pData, Ref *target, SEL_ImageLoader selector, void* pUserData)
{    
    Texture2D *texture = nullptr;
    
    texture = Director::getInstance()->getTextureCache()->getTextureForKey(key);
    
    if(nullptr != texture)
    {
        if (target != nullptr && selector != nullptr)
        {
            (target->*selector)(texture, pUserData);
        }
        
        return;
    }
    
    if (pData && pData->isNull())
    {
        if (target != nullptr && selector != nullptr)
        {
            (target->*selector)(nullptr, pUserData);
        }
        
        return;
    }
    
    initloadingThread();
    
    if (0 == _asyncRefCount)
    {
        startScheduler();
    }
    
    ++_asyncRefCount;
    
    PixImageLoaderStruct *LoaderStruct = new (std::nothrow) PixImageLoaderStruct(key, pData, target, selector, pUserData);
    
    _imageLoaderQueue.push_back(LoaderStruct);
    _requestMutex.lock();
    _requestQueue.push_back(LoaderStruct);
    _requestMutex.unlock();
    
    _sleepCondition.notify_one();
}

bool PixImageLoader::unbindImageAsync(const std::string &filename)
{
    if (_imageLoaderQueue.empty())
    {
        return false;
    }
    
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(filename);
 
    for (auto it = _imageLoaderQueue.begin(); it != _imageLoaderQueue.end(); ++it)
    {
        if ((*it)->filename == fullpath)
        {
            (*it)->target = nullptr;
            (*it)->selector = nullptr;
            return true;
        }
    }
    
    return false;
}

void PixImageLoader::unbindAllImageAsync()
{
    if (_imageLoaderQueue.empty())
    {
        return;
    }
    
    for (auto it = _imageLoaderQueue.begin(); it != _imageLoaderQueue.end(); ++it)
    {
        (*it)->target = nullptr;
        (*it)->selector = nullptr;
    }
}

void PixImageLoader::waitForQuit()
{
    _needQuit = true;
    _sleepCondition.notify_one();
    if (_loadingThread)
    {
        if (_loadingThread->joinable())
        {
            _loadingThread->join();
        }
    }
}

bool PixImageLoader::isLoadingimage()
{
    if(_requestQueue.empty() && _responseQueue.empty())
    {
        return false;
    }
    
    return false;
}

void PixImageLoader::imageLoaderSchedule(float dt)
{
    Texture2D *texture = nullptr;
    PixImageLoaderStruct *loaderStruct = nullptr;
    while (true)
    {
        _responseMutex.lock();
        if(_responseQueue.empty())
        {
            loaderStruct = nullptr;
        }
        else
        {
            loaderStruct = _responseQueue.front();
            _responseQueue.pop_front();
            CC_ASSERT(loaderStruct == _imageLoaderQueue.front());
            _imageLoaderQueue.pop_front();
        }
        _responseMutex.unlock();
        
        CC_BREAK_IF(nullptr == loaderStruct);
        
        texture = Director::getInstance()->getTextureCache()->getTextureForKey(loaderStruct->filename);
       
        if(nullptr == texture)
        {
            if (loaderStruct->loadSuccess)
            {                
                Image* image = loaderStruct->image;
                CC_BREAK_IF(nullptr == image);
                texture = Director::getInstance()->getTextureCache()->addImage(image,loaderStruct->filename);
                CC_SAFE_RELEASE(image);
            }
            else
            {
                texture = nullptr;
            }
        }
        
        if (loaderStruct->target != nullptr && loaderStruct->selector != nullptr)
        {
            (loaderStruct->target->*loaderStruct->selector)(texture, loaderStruct->userData);
        }
        
        delete loaderStruct;
        --_asyncRefCount;
    }
    
    if (0 == _asyncRefCount)
    {
        stopScheduler();

        waitForQuit();
        CC_SAFE_DELETE(_loadingThread);
    }
}

void PixImageLoader::loadImage()
{
    PixImageLoaderStruct *loaderStruct = nullptr;
    std::mutex signalMutex;
    std::unique_lock<std::mutex> signal(signalMutex);
    while (!_needQuit)
    {
        // pop an AsyncStruct from request queue
        _requestMutex.lock();
        if(_requestQueue.empty())
        {
            loaderStruct = nullptr;
        }
        else
        {
            loaderStruct = _requestQueue.front();
            _requestQueue.pop_front();
        }
        _requestMutex.unlock();
        
        if (nullptr == loaderStruct) {
            _sleepCondition.wait(signal);
            continue;
        }
        
        // load image
        
        _imageFileThreadMutex.lock();
        loaderStruct->loadSuccess = loaderStruct->initWithImageFileThreadSafe();
        _imageFileThreadMutex.unlock();
        
        // push the asyncStruct to response queue
        _responseMutex.lock();
        _responseQueue.push_back(loaderStruct);
        _responseMutex.unlock();
    }
}



