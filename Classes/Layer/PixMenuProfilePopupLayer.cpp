//
//  PixMenuProfilePopupLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 18..
//
//

#include "PixMenuProfilePopupLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"

PixMenuProfilePopupLayer* PixMenuProfilePopupLayer::create()
{
    PixMenuProfilePopupLayer *menuProfilePopupLayer = new (std::nothrow) PixMenuProfilePopupLayer();
    if (menuProfilePopupLayer && menuProfilePopupLayer->init())
    {
        menuProfilePopupLayer->autorelease();
        return menuProfilePopupLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuProfilePopupLayer);
        return nullptr;
    }
}

PixMenuProfilePopupLayer::PixMenuProfilePopupLayer()
: rootNode(nullptr)
, blockout(nullptr)
, buttonNode(nullptr)
, Button_remove(nullptr)
, Button_photochoice(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
{
    hideLayerFunction = std::bind(&PixMenuProfilePopupLayer::hideProfilePopupAction,this);
}

PixMenuProfilePopupLayer::~PixMenuProfilePopupLayer()
{
    
}

void PixMenuProfilePopupLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuProfilePopup.csb");
    
    if(rootNode)
    {
        rootNode->setOpacity(0);
        
        blockout = dynamic_cast<Sprite*>(rootNode->getChildByName("blockout"));
        
        buttonNode = dynamic_cast<Node*>(rootNode->getChildByName("buttonNode"));
        
        if(buttonNode)
        {
            Button_remove = dynamic_cast<ui::Button*>(buttonNode->getChildByName("Button_remove"));
            
            if(Button_remove)
            {
                Button_remove->addTouchEventListener(CC_CALLBACK_2(PixMenuProfilePopupLayer::buttonRemovePressed, this));
            }
            
            Button_photochoice = dynamic_cast<ui::Button*>(buttonNode->getChildByName("Button_photochoice"));
            
            if(Button_photochoice)
            {
                Button_photochoice->addTouchEventListener(CC_CALLBACK_2(PixMenuProfilePopupLayer::buttonPhotoChoicePressed, this));
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixMenuProfilePopupLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showProfilePopupAction();
}

#pragma mark - Button

void PixMenuProfilePopupLayer::buttonRemovePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(mDelegate)
            mDelegate->profileRemove();
        
         hideProfilePopupAction();
    }
}

void PixMenuProfilePopupLayer::buttonPhotoChoicePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(mDelegate)
            mDelegate->profileChange();
        
        hideProfilePopupAction();
    }
}

#pragma mark - Touch & Touches

void PixMenuProfilePopupLayer::onTouchEnded(Touch* touch, Event* event)
{
    if(buttonNode)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            if(!NodeContainsPoint(buttonNode, touch->getLocation()) )
            {
                if(mbln_IsAction == false)
                {
                    hideProfilePopupAction();
                }
            }
        }
    }
}

#pragma mark - Action

void PixMenuProfilePopupLayer::showProfilePopupAction()
{
    mbln_IsAction = true;
    
    FadeIn *fadeIn = FadeIn::create(0.1f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuProfilePopupLayer::showProfilePopupActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);
    
    rootNode->runAction(sequence);
}

void PixMenuProfilePopupLayer::hideProfilePopupAction()
{
    this->removeFromParentAndCleanup(true);
}

void PixMenuProfilePopupLayer::showProfilePopupActionFinish(Node* node)
{
    mbln_IsAction = false;
}

#pragma mark - Android BackPressed

void PixMenuProfilePopupLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(getAndroidBackPressedAction() == false)
    {
        setAndroidBackPressedAction(true);
        hideProfilePopupAction();
    }
}
