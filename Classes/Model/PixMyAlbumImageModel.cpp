//
//  PixMyAlbumImageModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 23..
//
//

#include "PixMyAlbumImageModel.h"
#include "../Widget/PixAlbumCoverCell.h"

PixMyAlbumImageModel::PixMyAlbumImageModel()
: _albumImageData (nullptr)
, _id("")
, _url("")
, _width(0)
, _height(0)
, _albumCoverCell(nullptr)
, imageLoader(nullptr)
{
    
}

PixMyAlbumImageModel::~PixMyAlbumImageModel()
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    
    if(scheduler->isScheduled(schedule_selector(PixMyAlbumImageModel::checkImageLoaderResponseQueue), this))
    {
        scheduler->unschedule(schedule_selector(PixMyAlbumImageModel::checkImageLoaderResponseQueue), this);
    }
    
    clear();
}

PixMyAlbumImageModel* PixMyAlbumImageModel::clone()
{
    PixMyAlbumImageModel *clone = new PixMyAlbumImageModel();
    
    if(_albumImageData != nullptr && !_albumImageData->isNull())
    {
        Data *cloneData = new Data();
        cloneData->copy(_albumImageData->getBytes(), _albumImageData->getSize());
        clone->_albumImageData = cloneData;
    }
    
    clone->_id = _id;
    clone->_url = _url;
    clone->_width = _width;
    clone->_height = _height;
    clone->_albumCoverCell = _albumCoverCell;
//    clone->imageLoader = imageLoader;
    
    return clone;
}

void PixMyAlbumImageModel::clear()
{
    destroyAlbumImageData();
    
    deleteImageLoader();
}

void PixMyAlbumImageModel::destroyAlbumImageData()
{
    CC_SAFE_DELETE(_albumImageData);
}

void PixMyAlbumImageModel::createImageLoader()
{
    imageLoader = new PixImageLoader();
}

void PixMyAlbumImageModel::deleteImageLoader()
{
    CC_SAFE_DELETE(imageLoader);
}

void PixMyAlbumImageModel::addImageAsync(Ref *target, SEL_ImageLoader selector, void *pUserData)
{
    if(imageLoader == nullptr)
    {
        createImageLoader();
    }
    
    imageLoader->addImageAsync(_url, _albumImageData, target, selector, pUserData);
}

void PixMyAlbumImageModel::deleteThreadSafeImageLoader()
{
    if(imageLoader)
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->schedule(schedule_selector(PixMyAlbumImageModel::checkImageLoaderResponseQueue), this, 0, false);
    }
}

void PixMyAlbumImageModel::checkImageLoaderResponseQueue(float dt)
{
    if(imageLoader && imageLoader->isLoadingimage() == false)
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->unschedule(schedule_selector(PixMyAlbumImageModel::checkImageLoaderResponseQueue), this);
        
        clear();
    }
}
