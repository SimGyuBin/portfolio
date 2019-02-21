//
//  PixImageCreateManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 22..
//
//

#ifndef PixImageCreateManager_h
#define PixImageCreateManager_h

#include "cocos2d.h"
#include "../Util/PixNativeUtils.h"
#include "../Common.h"

#define IMAGECREATE_MANAGER     (PixImageCreateManager::sharedObject())

USING_NS_CC;

typedef enum
{
    IMAGECREATE_THUMBNAIL,
    IMAGECREATE_FULLSCREEN,
}
IMAGECREATE_TYPE;

class LocalImageInfo;

typedef const std::function<void(Data*, const std::string&, const std::string&, const std::string&, const std::string&)> imageCreate_CallBack;

class CC_DLL PixImageCreateManager : public Ref
{
public:

    static PixImageCreateManager* sharedObject();
    
    PixImageCreateManager();
    virtual ~PixImageCreateManager();

    void init();
    void clear();
    void imageCreate(const std::string &strUrl, int nWidth, int nhHight, IMAGECREATE_TYPE eType, imageCreate_CallBack callBack = nullptr);

    void imageCreateComplete(const std::string &strUrl, int nWidth, int nhHight, IMAGECREATE_TYPE eType);
    bool unbindImage(const std::string &strUrl, IMAGECREATE_TYPE eType);
    bool isImageCreate(const std::string &strUrl, IMAGECREATE_TYPE eType);
    
    void setLocalImages(LocalImageInfo &pInfo);
    
    void addThumbnailData(const std::string &strPath, Data *pData, const std::string &strWidth, const std::string &strHeight);
    void addFullScreenData(const std::string &strPath, Data *pData, const std::string &strWidth, const std::string &strHeight);
    bool isTheradPool();
    
private:
    
    void startScheduler();
    void stopScheduler();
    
    void imageCreateSchedule(float dt);
      
protected :
    
    struct PixImageCreateStruct;
    
    std::deque<PixImageCreateStruct*> _requestQueue;
    std::deque<PixImageCreateStruct*> _responseQueue;
    
    Director *_director;
    Scheduler* _scheduler;
    
    int _threadRefCount;
    int _createCount;
    
    CC_SYNTHESIZE(int, mn_ThreadPoolSize, ThreadPoolSize)
};


#endif /* PixImageCreateManager_h */
