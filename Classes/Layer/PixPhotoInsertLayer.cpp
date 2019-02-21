//
//  PixPhotoInsertLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 1..
//
//

#include "PixPhotoInsertLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

PixPhotoInsertLayer* PixPhotoInsertLayer::create(PixEditSprite *pSprite)
{
    PixPhotoInsertLayer *photoInsertLayer = new (std::nothrow) PixPhotoInsertLayer(pSprite);
    if (photoInsertLayer && photoInsertLayer->init())
    {
        photoInsertLayer->initLayer();
        photoInsertLayer->autorelease();
        return photoInsertLayer;
    }
    else
    {
        CC_SAFE_DELETE(photoInsertLayer);
        return nullptr;
    }
}

PixPhotoInsertLayer::PixPhotoInsertLayer(PixEditSprite *pSprite)
: rootNode(nullptr)
, Button_putphoto(nullptr)
, editSprite(nullptr)
, mDelegate(nullptr)
{
    editSprite = pSprite;
}

PixPhotoInsertLayer::~PixPhotoInsertLayer()
{
    
}

void PixPhotoInsertLayer::initLayer()
{
    rootNode = CSLoader::createNode("06_EditAlbum/EditAlbumPhototouch_2.csb");
    
    if(rootNode)
    {
        
        Button_putphoto = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_putphoto"));
        if(Button_putphoto)
        {
            Button_putphoto->addTouchEventListener(CC_CALLBACK_2(PixPhotoInsertLayer::btnInsertPressed, this));
        }
        
        setContentSize(rootNode->getContentSize());
        
        addChild(rootNode);
    }
}

void PixPhotoInsertLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

#pragma mark - Button

void PixPhotoInsertLayer::btnInsertPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mDelegate)
        {
            mDelegate->photoInsertLayerPressed(editSprite);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}
