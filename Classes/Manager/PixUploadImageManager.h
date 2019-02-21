//
//  PixUploadImageManager.h
//  Pzle
//
//  Created by 심규빈 on 2017. 1. 19..
//
//

#ifndef PixUploadImageManager_h
#define PixUploadImageManager_h

#include "cocos2d.h"
#include "../Util/PixNativeUtils.h"
#include "../Common.h"

#define UPLOADIMAGE_MANAGER     (PixUploadImageManager::sharedObject())

class LocalImageInfo;

typedef const std::function<void(Data*, const std::string&, const std::string&, const std::string&, const std::string&)> uploadImage_CallBack;

class CC_DLL PixUploadImageManager : public Ref
{
public:
    
    static PixUploadImageManager* sharedObject();
    
    PixUploadImageManager();
    virtual ~PixUploadImageManager();
    
    void init();
    void clear();
    void imageCreate(const std::string &strUrl, int nWidth, int nhHight, uploadImage_CallBack callBack = nullptr);
    
    void imageCreateComplete(const std::string &strUrl, int nWidth, int nhHight);
    bool unbindImage(const std::string &strUrl);
    bool isImageCreate(const std::string &strUrl);
    
    void uploadImageComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate);
    bool isTheradPool();
    
private:
    
    void startScheduler();
    void stopScheduler();
    
    void imageCreateSchedule(float dt);
    
    protected :
    
    struct PixImageUploadStruct;
    
    std::deque<PixImageUploadStruct*> _requestQueue;
    std::deque<PixImageUploadStruct*> _responseQueue;
    
    Director *_director;
    Scheduler* _scheduler;
    
    int _threadRefCount;
    int _createCount;
    
    CC_SYNTHESIZE(int, mn_ThreadPoolSize, ThreadPoolSize)
};


#endif /* PixUploadImageManager_h */
