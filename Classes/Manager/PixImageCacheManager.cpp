//
//  PixImageCacheManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 25..
//
//

#include "PixImageCacheManager.h"
#include "../Model/PixImageModel.h"

static PixImageCacheManager* _sharedObject = nullptr;

PixImageCacheManager* PixImageCacheManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixImageCacheManager();
        _sharedObject->clear();
    }
    
    return _sharedObject;
}

PixImageCacheManager::PixImageCacheManager()
: mn_maxCacheCount(MAX_CACHE_COUNT)
, mn_CacheCount(0)
{
    clear();
}

PixImageCacheManager::~PixImageCacheManager()
{
    clear();
}

void PixImageCacheManager::clear()
{
    setMaxCacheCount(MAX_CACHE_COUNT);
    setCacheCount(0);
    _imageCacheList.clear();
}

void PixImageCacheManager::addImageModel(PixImageModel *pModel)
{
    removeImageModel();
    _imageCacheList.push_back(pModel);
    ++mn_CacheCount;
}

void PixImageCacheManager::removeImageModel(PixImageModel *pModel)
{
    for (auto iter = _imageCacheList.begin(); iter != _imageCacheList.end(); )
    {
        if((*iter) == pModel)
        {
            iter = _imageCacheList.erase(iter);
            --mn_CacheCount;
        }
        else
        {
            ++iter;
        }
    }
}

void PixImageCacheManager::removeImageModel()
{
    for (auto iter = _imageCacheList.begin(); iter != _imageCacheList.end(); )
    {
        if(getCacheCount() > getMaxCacheCount())
        {
            if((*iter)->getCell() && (*iter)->getCell()->isVisible() == false)
            {
                (*iter)->removeThumbnailImage();
                --mn_CacheCount;
                iter = _imageCacheList.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
        else
        {
            break;
        }
    }
}

void PixImageCacheManager::removeAllImageModel()
{
    for (auto iter = _imageCacheList.begin(); iter != _imageCacheList.end(); ++iter)
    {
        (*iter)->removeThumbnailImage();
    }

    clear();
}

bool PixImageCacheManager::isRemoveImageModel(PixImageModel *pModel)
{
    if(getCacheCount() > getMaxCacheCount())
    {
        removeImageModel(pModel);
        return true;
    }
    
    return false;
}

