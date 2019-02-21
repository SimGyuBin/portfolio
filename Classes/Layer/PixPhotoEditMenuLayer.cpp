//
//  PixPhotoEditMenuLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 24..
//
//

#include "PixPhotoEditMenuLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

PixPhotoEditMenuLayer* PixPhotoEditMenuLayer::create(PixEditSprite *pSprite, EDIT_MENU_TYPE type)
{
    PixPhotoEditMenuLayer *photoEditMenuLayer = new (std::nothrow) PixPhotoEditMenuLayer(pSprite);
    if (photoEditMenuLayer && photoEditMenuLayer->init())
    {
        photoEditMenuLayer->setType(type);
        photoEditMenuLayer->initLayer();
        photoEditMenuLayer->autorelease();
        return photoEditMenuLayer;
    }
    else
    {
        CC_SAFE_DELETE(photoEditMenuLayer);
        return nullptr;
    }
}

PixPhotoEditMenuLayer::PixPhotoEditMenuLayer(PixEditSprite *pSprite)
: rootNode(nullptr)
, bnt_phototouch_back1(nullptr)
, bnt_phototouch_back2(nullptr)
, bnt_phototouch_back3(nullptr)
, Button_change(nullptr)
, Button_replace(nullptr)
, Button_delete(nullptr)
, editSprite(nullptr)
, mDelegate(nullptr)
, mType(MENU_NONE)
{
    editSprite = pSprite;
}

PixPhotoEditMenuLayer::~PixPhotoEditMenuLayer()
{
    
}

void PixPhotoEditMenuLayer::initLayer()
{
    if(getType() == MENU_ONE_BUTTON)
    {
        initCoverMenu();
    }
    else if(getType() == MENU_TWO_BUTTON)
    {
        initPageMenu();
    }
}

void PixPhotoEditMenuLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void PixPhotoEditMenuLayer::initCoverMenu()
{
    rootNode = CSLoader::createNode("06_EditAlbum/EditAlbumPhototouch_2.csb");
    
    if(rootNode)
    {
        Button_replace = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_replace"));
       
        if(Button_replace)
        {
            Button_replace->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditMenuLayer::btnReplacePressed, this));
        }
        
        setContentSize(rootNode->getContentSize());
        
        addChild(rootNode);
    }
}

void PixPhotoEditMenuLayer::initPageMenu()
{
    rootNode = CSLoader::createNode("06_EditAlbum/EditAlbumPhototouch_1.csb");
    
    if(rootNode)
    {
        bnt_phototouch_back1 = dynamic_cast<Sprite*>(rootNode->getChildByName("bnt_phototouch_back1"));
        bnt_phototouch_back2 = dynamic_cast<Sprite*>(rootNode->getChildByName("bnt_phototouch_back2"));
        bnt_phototouch_back3 = dynamic_cast<Sprite*>(rootNode->getChildByName("bnt_phototouch_back3"));
        
        Button_change = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_change"));
        if(Button_change)
        {
            Button_change->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditMenuLayer::btnChangePressed, this));
        }
        
        Button_replace = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_replace"));
        if(Button_replace)
        {
            Button_replace->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditMenuLayer::btnReplacePressed, this));
        }
        
        Button_delete = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_delete"));
        if(Button_delete)
        {
            Button_delete->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditMenuLayer::btnDeletePressed, this));
        }
        
        setContentSize(rootNode->getContentSize());
        
        addChild(rootNode);
    }
}

void PixPhotoEditMenuLayer::setLeft()
{
    if(bnt_phototouch_back1)
    {
        bnt_phototouch_back1->setVisible(true);
    }
    
    if(bnt_phototouch_back2)
    {
        bnt_phototouch_back2->setVisible(false);
    }
    
    if(bnt_phototouch_back3)
    {
        bnt_phototouch_back3->setVisible(false);
    }
}

void PixPhotoEditMenuLayer::setCenter()
{
    if(bnt_phototouch_back1)
    {
        bnt_phototouch_back1->setVisible(false);
    }
    
    if(bnt_phototouch_back2)
    {
        bnt_phototouch_back2->setVisible(true);
    }
    
    if(bnt_phototouch_back3)
    {
        bnt_phototouch_back3->setVisible(false);
    }
}

void PixPhotoEditMenuLayer::setRight()
{
    if(bnt_phototouch_back1)
    {
        bnt_phototouch_back1->setVisible(false);
    }
    
    if(bnt_phototouch_back2)
    {
        bnt_phototouch_back2->setVisible(false);
    }
    
    if(bnt_phototouch_back3)
    {
        bnt_phototouch_back3->setVisible(true);
    }
}

#pragma mark - Button

void PixPhotoEditMenuLayer::btnChangePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mDelegate)
        {
            mDelegate->photoEditMenuLayerChangePressed(editSprite);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

void PixPhotoEditMenuLayer::btnReplacePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mDelegate)
        {
            mDelegate->photoEditMenuLayerReplacePressed(editSprite);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

void PixPhotoEditMenuLayer::btnDeletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mDelegate)
        {
            mDelegate->photoEditMenuLayerDeletePressed(editSprite);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Action

void PixPhotoEditMenuLayer::showLayerAction()
{
    
}

void PixPhotoEditMenuLayer::hideLayerAction()
{
    
}

void PixPhotoEditMenuLayer::showLayerActionFinish(Node* node)
{
    
}

void PixPhotoEditMenuLayer::hideLayerActionFinish(Node* node)
{
    
}

