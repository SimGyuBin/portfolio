//
//  1.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 25..
//
//

#include "PixNetworkImageModel.h"
#include "../Manager/PixImageCreateManager.h"
#include "PixImageModel.h"
#include "../Scene/PixApp.h"

PixNetworkImageModel* PixNetworkImageModel::create()
{
    PixNetworkImageModel *networkImageModel = new PixNetworkImageModel();
    return networkImageModel;
}

PixNetworkImageModel::PixNetworkImageModel()
: nType(0)
, pos(Vec2::ZERO)
, cocos2dPos(Vec2::ZERO)
, size(Size::ZERO)
, parentSize(Size::ZERO)
, rotate(0.0f)
, cocos2dScale(0.0f)
, zOrder(0)
, path("")
, date("")
, nTag(0)
, nOrientation(0)
, nSmartIndex(0)
, imageLoader(nullptr)
, mCallback(nullptr)
, mlocalImageInfo(nullptr)
, mTexture(nullptr)
, mDelegate(nullptr)
{
    
}

PixNetworkImageModel::~PixNetworkImageModel()
{
    deleteImageLoader();
}

void PixNetworkImageModel::createImageLoader()
{
    imageLoader = new PixImageLoader();
}

void PixNetworkImageModel::deleteImageLoader()
{
    CC_SAFE_DELETE(imageLoader);
}

void PixNetworkImageModel::addImageAsync(onCompleteTextureCallback callback)
{
    this->mCallback = callback;
    
    if(imageLoader == nullptr)
    {
        createImageLoader();
    }
    
    imageLoader->addImageAsync(path, this, imageloader_selector(PixNetworkImageModel::addImageAsyncCallBack));
    
}

void PixNetworkImageModel::addImageAsync(PixNetworkImageModelDelegate *mDelegate)
{
    this->mDelegate = mDelegate;
    
    if(imageLoader == nullptr)
    {
        createImageLoader();
    }
    
    imageLoader->addImageAsync(path, this, imageloader_selector(PixNetworkImageModel::addImageAsyncCallBack));
}

void PixNetworkImageModel::addLocalImageAsync(onCompleteTextureCallback callback)
{
    this->mCallback = callback;
    this->mDelegate = nullptr;
    localImage();
}

void PixNetworkImageModel::addLocalImageAsync(PixNetworkImageModelDelegate *pDelegate)
{
    this->mDelegate = pDelegate;
    this->mCallback = nullptr;
    localImage();
}

void PixNetworkImageModel::localImage()
{
    PixImageModel *imageModel = PIXAPP->userInfo->getImageModelBySmartIndex(nTag);
    
    if(imageModel)
    {
        if (imageModel->getLocalImageInfo())
        {
            mlocalImageInfo = imageModel->getLocalImageInfo();
            
            Data* localImageData = mlocalImageInfo->getFullScreenData();
            
            if (localImageData && !localImageData->isNull())
            {
                localImageData->clear();
            }
            
            std::string strPath = mlocalImageInfo->getPath();
            
            if(!IMAGECREATE_MANAGER->isImageCreate(strPath, IMAGECREATE_FULLSCREEN))
            {
                int nWidth = atoi(mlocalImageInfo->getWidth().c_str());
                int nHeight = atoi(mlocalImageInfo->getHeight().c_str());
                
                IMAGECREATE_MANAGER->imageCreate(strPath, nWidth, nHeight, IMAGECREATE_FULLSCREEN, IMAGECREATE_CALLBACK(PixNetworkImageModel::addLocalImageComplete, this));
            }
        }
        else
        {
            if(mDelegate)
            {
                mDelegate->networkImageModelcompleteTexture(nullptr, this);
            }
            else if (mCallback)
            {
                (mCallback)(nullptr, this);
            }
        }
    }
    else
    {
        if(mDelegate)
        {
            mDelegate->networkImageModelcompleteTexture(nullptr, this);
        }
        else if (mCallback)
        {
            (mCallback)(nullptr, this);
        }
    }
}


void PixNetworkImageModel::addImageAsyncCallBack(Texture2D *pTexture, void* pUserData)
{
    this->mTexture = pTexture;
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        deleteImageLoader();
        
        if(mlocalImageInfo)
        {
            mlocalImageInfo->destroyFullScreenImageData();
        }
        
        if(mDelegate)
        {
            mDelegate->networkImageModelcompleteTexture(mTexture, this);
        }
        else if (mCallback)
        {
            (mCallback)(mTexture, this);
        }
    });
}

void PixNetworkImageModel::addLocalImageComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate)
{
    if (mlocalImageInfo)
    {
        Data* localImageData = mlocalImageInfo->getFullScreenData();
        
        if (localImageData && !localImageData->isNull())
        {
            if(imageLoader == nullptr)
            {
                createImageLoader();
            }
            
            std::string strKey = "FullScreenData" + mlocalImageInfo->getPath();
            imageLoader->addImageAsync(strKey, localImageData, this ,imageloader_selector(PixNetworkImageModel::addImageAsyncCallBack), nullptr);
        }
    }
    else
    {
        if(mDelegate)
        {
            mDelegate->networkImageModelcompleteTexture(nullptr, this);
        }
        else if (mCallback)
        {
            (mCallback)(nullptr, this);
        }
    }
}
