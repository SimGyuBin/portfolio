//
//  PixSelectImageModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 26..
//
//

#include "PixImageModel.h"
#include "ui/CocosGUI.h"
#include "../Info/UserInfo.h"
#include "../Util/PixNativeUtils.h"
#include "../UIControl/PixGallerySprite.h"
#include "../Manager/PixImageCreateManager.h"
#include "../Manager/PixImageCacheManager.h"
#include "../Widget/PixSelectImageCell.h"

PixImageModel::PixImageModel()
: _localImageInfo(nullptr)
, mn_index(-1)
, mn_smartIndex(-1)
, _cellItem(nullptr)
, _cell(nullptr)
, _texture(nullptr)
, mstr_Option("")
, mstr_MaskFile("")
, mbln_loading(false)
, _dateCell(nullptr)
, imageLoader(nullptr)
, target(nullptr)
, selector(nullptr)
{
   
}

PixImageModel::~PixImageModel()
{
    if (this->getTexture() == nullptr)
    {
        unbindThumbnailImageAsync();
    }
    else
    {
        Director::getInstance()->getTextureCache()->removeTexture(_texture);
        _texture = nullptr;
    }
    
    deleteImageLoader();
    
    target = nullptr;
    selector = nullptr;
}

PixImageModel* PixImageModel::clone()
{
    PixImageModel *clone = new PixImageModel();
    
    clone->_localImageInfo = _localImageInfo;
    clone->mn_index = mn_index;
    clone->mn_smartIndex = mn_smartIndex;
    clone->_cellItem = _cellItem;
    clone->_cell = _cell;
    clone->_texture = _texture;
    clone->mstr_Option = mstr_Option;
    clone->mstr_MaskFile = mstr_MaskFile;
    clone->mbln_loading = mbln_loading;
    clone->_dateCell = _dateCell;
    
    return clone;
}

void PixImageModel::addThumbnailImageAsync(Ref *target, SEL_ImageLoader selector, void *pUserData)
{
    if(getTexture())
    {
        if (target != nullptr && selector != nullptr)
        {
            (target->*selector)(getTexture(), pUserData);
        }
    }
    else
    {
        if(getLoading())
            return;
        
        if(_localImageInfo)
        {
            if(IMAGECREATE_MANAGER->isTheradPool())
            {
                std::string strPath = _localImageInfo->getPath();
                
                if(IMAGECREATE_MANAGER->isImageCreate(strPath, IMAGECREATE_THUMBNAIL))
                {
                    return;
                }
                
                this->target = target;
                this->selector = selector;
                
                if(getCellItem())
                {
                    getCellItem()->removeChildByTag(getIndex());
                    
                    Node *loading = getCellItem()->getChildByName("loading");
                    
                    if(loading)
                    {
                        loading->setVisible(true);
                    }
                }
                
                Data *pThumbnaildata = _localImageInfo->getThumbnailData();
                
                if(pThumbnaildata && !pThumbnaildata->isNull())
                {
                    pThumbnaildata->clear();
                }
                
                int nWidth = atoi(_localImageInfo->getWidth().c_str());
                int nHeight = atoi(_localImageInfo->getHeight().c_str());
                
                IMAGECREATE_MANAGER->imageCreate(strPath, nWidth, nHeight, IMAGECREATE_THUMBNAIL ,IMAGECREATE_CALLBACK(PixImageModel::addThumbnailImageComplete, this));
                
                setLoading(true);

            }
        }
    }
}

void PixImageModel::addThumbnailImageComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate)
{
    Data *pThumbnaildata = _localImageInfo->getThumbnailData();
    
    if(pThumbnaildata && !pThumbnaildata->isNull())
    {
        std::string strPath = _localImageInfo->getPath();
        
        addThumbnailImageAsyncCall(pThumbnaildata, strPath);
    }
}

void PixImageModel::addThumbnailImageAsyncCall(Data *pThumbnaildata, std::string strPath)
{
    if (target != nullptr && selector != nullptr)
    {
        if(imageLoader == nullptr)
        {
            createImageLoader();
        }
        
        imageLoader->addImageAsync(strPath, pThumbnaildata, target, selector, this);
    }
}

void PixImageModel::unbindThumbnailImageAsync()
{
    if(_localImageInfo)
    {
        if(IMAGECREATE_MANAGER->unbindImage(_localImageInfo->getPath(), IMAGECREATE_THUMBNAIL))
        {
            setLoading(false);
        }
        
        if(imageLoader)
        {
            std::string strPath = _localImageInfo->getPath();
            
            if(imageLoader->unbindImageAsync(strPath))
            {
                setLoading(false);
            }
            
            deleteImageLoader();
        }
    }
}

void PixImageModel::addImageCache()
{
    IMAGECACHE_MANAGER->addImageModel(this);
}

void PixImageModel::removeThumbnailImage()
{
    if (getTexture() == nullptr)
    {
        // 이미지 로드 쓰레드 호출 해제
        unbindThumbnailImageAsync();
    }
    else
    {
        removeTexture();
        
        if(_localImageInfo)
        {
            _localImageInfo->destroyThumbnailImageData();
        }
        
        PixGallerySprite *sprite = dynamic_cast<PixGallerySprite*>(getCellItem()->getChildByTag(getIndex()));
        
        if(sprite)
        {
            sprite->removeFromParentAndCleanup(true);
        }
    }
    
    deleteImageLoader();

    setLoading(false);
}

void PixImageModel::removeTexture()
{   
    if(_texture)
    {
        Director::getInstance()->getTextureCache()->removeTexture(_texture);
        _texture = nullptr;
    }

    if(getCellItem())
    {
        getCellItem()->removeChildByTag(getIndex());
        
        Node *loading = getCellItem()->getChildByName("loading");
        
        if(loading)
        {
            loading->setVisible(true);
        }
    }
}

void PixImageModel::setSelected(bool isSelected)
{
    if(_localImageInfo)
    {
        _localImageInfo->setSelected(isSelected);
        updateSelected(_localImageInfo->getSelected());
    }
}

void PixImageModel::setSelectImage(bool isVisible)
{
    Sprite *selectImage = dynamic_cast<Sprite*>(getCellItem()->getChildByName("frame_2"));
    
    if (selectImage)
    {
        selectImage->setVisible(false);
    }
}

void PixImageModel::selectedEffect(bool effect)
{
    if(getCellItem())
    {
        ui::ImageView *selected = dynamic_cast<ui::ImageView*>(getCellItem()->getChildByName("selected"));
        
        if(selected)
        {
            selected->setVisible(effect);
            
            if(effect)
            {
                updateSelected(false);
            }
        }
    }
}

void PixImageModel::updateSelected()
{
    if(_localImageInfo)
    {
        updateSelected(_localImageInfo->getSelected());
    }
}

void PixImageModel::updateSelected(bool isSelected)
{
    if(getCellItem())
    {
        ui::ImageView *frame = dynamic_cast<ui::ImageView*>(getCellItem()->getChildByName("frame"));
        
        if (frame)
        {
            frame->setVisible(isSelected);
        }
        
        ui::ImageView *check = dynamic_cast<ui::ImageView*>(getCellItem()->getChildByName("check"));
        
        if(check)
        {
            check->setVisible(isSelected);
        }
        
        PixGallerySprite *sprite = dynamic_cast<PixGallerySprite*>(getCellItem()->getChildByTag(getIndex()));
        
        if(sprite)
        {
            sprite->stopActionByTag(getIndex());
            
            if(isSelected)
            {
                ScaleTo *scaleTo = ScaleTo::create(0.1f, 0.8f);
                EaseSineOut *easeSineOut = EaseSineOut::create(scaleTo);
                easeSineOut->setTag(getIndex());
                sprite->runAction(easeSineOut);
            }
            else
            {
                ScaleTo *scaleTo = ScaleTo::create(0.1f, 1.0f);
                EaseSineOut *easeSineOut = EaseSineOut::create(scaleTo);
                easeSineOut->setTag(getIndex());
                sprite->runAction(easeSineOut);
            }
        }
    }
}

bool PixImageModel::isSelected()
{
    if(_localImageInfo)
    {
        return _localImageInfo->getSelected();
    }
    
    return false;
}

bool PixImageModel::isSelectedEffect()
{
    if(getCellItem())
    {
        ui::ImageView *selected = dynamic_cast<ui::ImageView*>(getCellItem()->getChildByName("selected"));
        
        if (selected)
        {
            return selected->isVisible();
        }
    }
    
    return false;
}

void PixImageModel::createImageLoader()
{
    imageLoader = new PixImageLoader();
}

void PixImageModel::deleteImageLoader()
{
    CC_SAFE_DELETE(imageLoader);
}

int PixImageModel::getCategoryCount()
{
    if(_localImageInfo)
    {
        return _localImageInfo->getCategorCount();
    }
    
    return 0;
}

CategoryInfo *PixImageModel::getCategoryInfo(int nIndex)
{
    if(_localImageInfo)
    {
        return _localImageInfo->getCategoryInfo(nIndex);
    }
    
    return nullptr;
}
