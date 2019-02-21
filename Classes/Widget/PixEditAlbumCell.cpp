//
//  PixEditAlbumCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 24..
//
//

#include "PixEditAlbumCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixEditManager.h"
#include "../Model/PixSmartImageModel.h"

PixEditAlbumCell* PixEditAlbumCell::create(PhotoEditInfo *info)
{
    PixEditAlbumCell* widget = new (std::nothrow) PixEditAlbumCell(info);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixEditAlbumCell::PixEditAlbumCell(PhotoEditInfo *info)
: customItem(nullptr)
, thum_photoselect_effect(nullptr)
, mSprite(nullptr)
, mEditInfo(nullptr)
, mTexture(nullptr)
{
    this->mEditInfo = info;
}

PixEditAlbumCell::~PixEditAlbumCell()
{
    
}

bool PixEditAlbumCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixEditAlbumCell::onEnter()
{
    Widget::onEnter();
}

void PixEditAlbumCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
}

void PixEditAlbumCell::createCustomItem()
{
    customItem = CSLoader::createNode("06_EditAlbum/ThumPhotoLayer.csb");
    
    if(customItem)
    {
        setContentSize(customItem->getContentSize());
        
        thum_photoselect_effect = dynamic_cast<Sprite*>(customItem->getChildByName("thum_photoselect_effect"));
        
        Size effectSize = Size::ZERO;
        Vec2 effectPos = Vec2::ZERO;
        
        if(thum_photoselect_effect)
        {
            thum_photoselect_effect->setVisible(false);
            effectSize = thum_photoselect_effect->getContentSize();
            effectPos = thum_photoselect_effect->getPosition();
        }
        
        PixSmartImageModel *smartImageModel = mEditInfo->getSmartImageModel();
        
        if(smartImageModel)
        {
            mTexture = Director::getInstance()->getTextureCache()->getTextureForKey(smartImageModel->getPath());
            
            if(mTexture)
            {
                mSprite = PixGallerySprite::createWithTexture(mTexture, "Common/photo_thum_120X120.png");
                
                mSprite->setPosition(Vec2(customItem->getContentSize().width / 2,
                                          customItem->getContentSize().height / 2));
                
                customItem->addChild(mSprite,-10);
            }
        }
        
        addChild(customItem);
    }
}

void PixEditAlbumCell::setVisiblEeffect(bool visible)
{
    if(thum_photoselect_effect)
    {
        thum_photoselect_effect->setVisible(visible);
    }
}

bool PixEditAlbumCell::isVisiblEeffect()
{
    if(thum_photoselect_effect)
    {
        return thum_photoselect_effect->isVisible();
    }
    
    return false;
}

void PixEditAlbumCell::spriteFadeOutAction()
{
    if(mSprite)
    {
        mSprite->fadeOutSpriteAction();
    }
}

void PixEditAlbumCell::changeTexture(const std::string& path)
{
    mTexture = Director::getInstance()->getTextureCache()->getTextureForKey(path);
    
    if(mTexture)
    {
        customItem->removeChild(mSprite);
        
        mSprite = PixGallerySprite::createWithTexture(mTexture, "Common/photo_thum_120X120.png");
        
        mSprite->setPosition(Vec2(customItem->getContentSize().width / 2,
                                  customItem->getContentSize().height / 2));
        
        customItem->addChild(mSprite,-10);
    }
}
