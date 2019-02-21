//
//  PixImageCacheManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 25..
//
//

#ifndef PixImageCacheManager_h
#define PixImageCacheManager_h

#include "cocos2d.h"
#include "../Common.h"

#define IMAGECACHE_MANAGER     (PixImageCacheManager::sharedObject())

USING_NS_CC;

class PixImageModel;

class CC_DLL PixImageCacheManager : public Ref
{
 
public :
    
    static PixImageCacheManager* sharedObject();
    
    PixImageCacheManager();
    virtual ~PixImageCacheManager();
    
    void clear();
    
    void addImageModel(PixImageModel *pModel);
    void removeImageModel(PixImageModel *pModel);
    void removeImageModel();
    void removeAllImageModel();
    bool isRemoveImageModel(PixImageModel *pModel);
    
    CC_SYNTHESIZE(int, mn_maxCacheCount, MaxCacheCount)
    CC_SYNTHESIZE(int, mn_CacheCount, CacheCount)
    
private:
    
    std::list<PixImageModel*> _imageCacheList;
};

#endif /* PixImageCacheManager_h */
