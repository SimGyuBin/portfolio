//
//  MyAlbumUsedImagesModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 14..
//
//

#include "MyAlbumUsedImageModel.h"

MyAlbumUsedImageModel::MyAlbumUsedImageModel()
: mn_Index(-1)
, mstr_s_image("")
, mstr_m_image("")
, mn_Width(0)
, mn_Height(0)
, mstr_File_create_date("")
, _S_imageLoader(nullptr)
, _M_imageLoader(nullptr)
, _S_Data(nullptr)
, _M_Data(nullptr)
, mItem(nullptr)
{

}

MyAlbumUsedImageModel::~MyAlbumUsedImageModel()
{
    clear();
}

MyAlbumUsedImageModel* MyAlbumUsedImageModel::clone()
{
    MyAlbumUsedImageModel *clone = new MyAlbumUsedImageModel();
    
    clone->mn_Index = mn_Index;
    clone->mstr_s_image = mstr_s_image;
    clone->mstr_m_image = mstr_m_image;
    clone->mn_Width = mn_Width;
    clone->mn_Height = mn_Height;
    clone->mstr_File_create_date = mstr_File_create_date;
//    clone->_S_imageLoader = new PixImageLoader();
//    clone->_M_imageLoader = new PixImageLoader();
    
    if(_S_Data != nullptr && !_S_Data->isNull())
    {
        Data *cloneData = new Data();
        cloneData->copy(_S_Data->getBytes(), _S_Data->getSize());
        clone->_S_Data = cloneData;
    }
    
    if(_M_Data != nullptr && !_M_Data->isNull())
    {
        Data *cloneData = new Data();
        cloneData->copy(_M_Data->getBytes(), _M_Data->getSize());
        clone->_M_Data = cloneData;
    }
    
    clone->mItem = mItem;
    
    return clone;

}

void MyAlbumUsedImageModel::clear()
{
    destroy_S_Data();
    destroy_M_Data();
    
    delete_S_ImageLoader();
    delete_M_ImageLoader();
}

void MyAlbumUsedImageModel::destroy_S_Data()
{
    CC_SAFE_DELETE(_S_Data);
}

void MyAlbumUsedImageModel::destroy_M_Data()
{
    CC_SAFE_DELETE(_M_Data);
}

void MyAlbumUsedImageModel::create_S_ImageLoader()
{
    _S_imageLoader = new PixImageLoader();
}

void MyAlbumUsedImageModel::create_M_ImageLoader()
{
    _M_imageLoader = new PixImageLoader();
}

void MyAlbumUsedImageModel::delete_S_ImageLoader()
{
    CC_SAFE_DELETE(_S_imageLoader);
}

void MyAlbumUsedImageModel::delete_M_ImageLoader()
{
    CC_SAFE_DELETE(_M_imageLoader);
}

void MyAlbumUsedImageModel::add_S_ImageAsync(Ref *target, SEL_ImageLoader selector, void* pUserData)
{
    if(_S_imageLoader == nullptr)
    {
        create_S_ImageLoader();
    }
    
    _S_imageLoader->addImageAsync(mstr_s_image, _S_Data, target, selector, pUserData);
}

void MyAlbumUsedImageModel::add_M_ImageAsync(Ref *target, SEL_ImageLoader selector, void* pUserData)
{
    if(_M_imageLoader == nullptr)
    {
        create_M_ImageLoader();
    }
    
    _M_imageLoader->addImageAsync(mstr_m_image, _M_Data, target, selector, pUserData);
}

void MyAlbumUsedImageModel::deleteThreadSafe_S_ImageLoader()
{
    if(_S_imageLoader)
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->schedule(schedule_selector(MyAlbumUsedImageModel::check_S_ImageLoaderResponseQueue), this, 0, false);
    }
}

void MyAlbumUsedImageModel::deleteThreadSafe_M_ImageLoader()
{
    if(_M_imageLoader)
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->schedule(schedule_selector(MyAlbumUsedImageModel::check_M_ImageLoaderResponseQueue), this, 0, false);
    }
}

void MyAlbumUsedImageModel::check_S_ImageLoaderResponseQueue(float dt)
{
    if(_S_imageLoader && _S_imageLoader->isLoadingimage() == false)
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->unschedule(schedule_selector(MyAlbumUsedImageModel::check_S_ImageLoaderResponseQueue), this);
        
        destroy_S_Data();
        delete_S_ImageLoader();
    }
}

void MyAlbumUsedImageModel::check_M_ImageLoaderResponseQueue(float dt)
{
    if(_M_imageLoader && _M_imageLoader->isLoadingimage() == false)
    {
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        scheduler->unschedule(schedule_selector(MyAlbumUsedImageModel::check_M_ImageLoaderResponseQueue), this);
        
        destroy_M_Data();
        delete_M_ImageLoader();
    }
}
