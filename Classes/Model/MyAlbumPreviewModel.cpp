//
//  MyAlbumPreviewModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 14..
//
//

#include "MyAlbumPreviewModel.h"

MyAlbumPreviewModel::MyAlbumPreviewModel()
: mstr_Id("")
, mn_Page_no(0)
, mstr_Thumb_url("")
, imageLoader(nullptr)
, _previewData(nullptr)
{
    
}

MyAlbumPreviewModel::~MyAlbumPreviewModel()
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    
    if(scheduler->isScheduled(schedule_selector(MyAlbumPreviewModel::checkImageLoaderResponseQueue), this))
    {
        scheduler->unschedule(schedule_selector(MyAlbumPreviewModel::checkImageLoaderResponseQueue), this);
    }
    
    clear();
}

MyAlbumPreviewModel* MyAlbumPreviewModel::clone()
{
    MyAlbumPreviewModel *clone = new MyAlbumPreviewModel();
    
    clone->mstr_Id = mstr_Id;
    clone->mn_Page_no = mn_Page_no;
    clone->mstr_Thumb_url = mstr_Thumb_url;
//    clone->imageLoader = imageLoader;
    
    if(_previewData != nullptr && !_previewData->isNull())
    {
        Data *cloneData = new Data();
        cloneData->copy(_previewData->getBytes(), _previewData->getSize());
        clone->_previewData = cloneData;
    }
    
    return clone;
}


void MyAlbumPreviewModel::clear()
{
    unbindAllImageAsync();
    destroyPreviewData();
    deleteImageLoader();
}

void MyAlbumPreviewModel::destroyPreviewData()
{
    CC_SAFE_DELETE(_previewData);
}

void MyAlbumPreviewModel::createImageLoader()
{
    imageLoader = new PixImageLoader();
}

void MyAlbumPreviewModel::deleteImageLoader()
{
    CC_SAFE_DELETE(imageLoader);
}

void MyAlbumPreviewModel::addImageAsync(Ref *target, SEL_ImageLoader selector, void *pUserData)
{
    if(imageLoader == nullptr)
    {
        createImageLoader();
    }
    
    imageLoader->addImageAsync(mstr_Thumb_url, _previewData, target, selector, pUserData);
}

void MyAlbumPreviewModel::addImageAsync(Ref *target, SEL_ImageLoader selector, const std::string &filepath, void *pUserData)
{
    if(imageLoader == nullptr)
    {
        createImageLoader();
    }
    
    imageLoader->addImageAsync(filepath, target, selector, pUserData);
}

void MyAlbumPreviewModel::unbindAllImageAsync()
{
    if(imageLoader)
    {
        imageLoader->unbindAllImageAsync();
    }
}

void MyAlbumPreviewModel::deleteThreadSafeImageLoader()
{
    if(imageLoader)
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->schedule(schedule_selector(MyAlbumPreviewModel::checkImageLoaderResponseQueue), this, 0, false);
    }
}

void MyAlbumPreviewModel::checkImageLoaderResponseQueue(float dt)
{
    if(imageLoader && imageLoader->isLoadingimage() == false)
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->unschedule(schedule_selector(MyAlbumPreviewModel::checkImageLoaderResponseQueue), this);
        
        clear();
    }
}
