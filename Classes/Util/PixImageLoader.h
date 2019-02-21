//
//  PixImageLoader.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 28..
//
//

// 이미지를 Async 방식으로 로드하는 ImageLoader 클래스

#ifndef PixImageLoader_h
#define PixImageLoader_h

#include "cocos2d.h"
#include "../Common.h"
#include "../Manager/PixImageCacheManager.h"

USING_NS_CC;

typedef void (Ref::*SEL_ImageLoader)(Texture2D*, void*);
#define imageloader_selector(_SELECTOR) static_cast<SEL_ImageLoader>(&_SELECTOR)

class CC_DLL PixImageLoader : public Ref
{
public :
    
    PixImageLoader();
    virtual ~PixImageLoader();

    void clear();
    
    void addImageAsync(const std::string &filepath, Ref *target, SEL_ImageLoader selector, void* pUserData = nullptr);
    void addImageAsync(const std::string &key, Data *pData, Ref *target, SEL_ImageLoader selector, void* pUserData = nullptr);
    
    bool unbindImageAsync(const std::string &filename);
    void unbindAllImageAsync();
    
    void waitForQuit();
    bool isLoadingimage();
    
private :
   
    void startScheduler();
    void stopScheduler();
    
    void initloadingThread();
    void imageLoaderSchedule(float dt);
    void loadImage();

protected :
    
    struct PixImageLoaderStruct;
    
    std::thread* _loadingThread;
    std::deque<PixImageLoaderStruct*> _imageLoaderQueue;
    std::deque<PixImageLoaderStruct*> _requestQueue;
    std::deque<PixImageLoaderStruct*> _responseQueue;
    
    std::mutex _requestMutex;
    std::mutex _responseMutex;
    std::mutex _imageFileThreadMutex;
    
    std::condition_variable _sleepCondition;
    
    int _asyncRefCount;
    bool _needQuit;
};



#endif /* PixImageLoader_h */

