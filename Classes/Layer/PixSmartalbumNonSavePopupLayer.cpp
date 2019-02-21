//
//  PixSmartalbumNonSavePopupLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 7..
//
//

#include "PixSmartalbumNonSavePopupLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixSmartalbumNonSavePopupLayer* PixSmartalbumNonSavePopupLayer::show(NonSavePopupDelegate *delegate)
{
    PixSmartalbumNonSavePopupLayer *popup = new PixSmartalbumNonSavePopupLayer();
    popup->init(delegate);
    popup->setPosition(Vec2::ZERO);
    Director::getInstance()->getRunningScene()->addChild(popup, 2, NONSAVE_POPUP_TAG);
    popup->release();
    
    return popup;
}

PixSmartalbumNonSavePopupLayer::PixSmartalbumNonSavePopupLayer()
: rootNode(nullptr)
, popup(nullptr)
, delegate(nullptr)
, mbln_IsAction(false)
{
    hideLayerFunction = std::bind(&PixSmartalbumNonSavePopupLayer::hideLayerAction,this);
}

PixSmartalbumNonSavePopupLayer::~PixSmartalbumNonSavePopupLayer()
{
    
}

void PixSmartalbumNonSavePopupLayer::init(NonSavePopupDelegate *delegate)
{
    PixModalLayer::init();
    
    this->delegate = delegate;
    
    rootNode = CSLoader::createNode("05_AlbumView/smartalbumNonSavePopupLayer.csb");
    
    if(rootNode)
    {
        ui::Button *Button_exit = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_exit"));
        
        if(Button_exit)
        {
            Button_exit->setTag(NONSAVE_POPUP_EXIT);
            Button_exit->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumNonSavePopupLayer::buttonPressed, this));
            Button_exit->setTitleText(CODE_NAME(Button_exit->getTitleText()));
        }
        
        ui::Button *Button_new = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_new"));
        
        if(Button_new)
        {
            Button_new->setTag(NONSAVE_POPUP_NEW);
            Button_new->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumNonSavePopupLayer::buttonPressed, this));
            Button_new->setTitleText(CODE_NAME(Button_new->getTitleText()));
        }
        
        ui::Button *Button_save = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_save"));
        
        if(Button_save)
        {
            Button_save->setTag(NONSAVE_POPUP_SAVE);
            Button_save->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumNonSavePopupLayer::buttonPressed, this));
            Button_save->setTitleText(CODE_NAME(Button_save->getTitleText()));
        }
        
        ui::Button *back = dynamic_cast<ui::Button*>(rootNode->getChildByName("back"));
        
        if(back)
        {
            back->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumNonSavePopupLayer::backPressed, this));
        }
        
        ui::Text *text = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text"));
        
        if(text)
        {
            text->setString(CODE_NAME(text->getString()));
        }
        
        text = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_2"));
        
        if(text)
        {
           text->setString(CODE_NAME(text->getString()));
        }
        
        addChild(rootNode);
    }
}

void PixSmartalbumNonSavePopupLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    
    showAlertAction();
}

void PixSmartalbumNonSavePopupLayer::hideLayerAction()
{
    this->removeFromParentAndCleanup(true);
}

#pragma mark - Button

void PixSmartalbumNonSavePopupLayer::buttonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        ui::Button *pButton = dynamic_cast<ui::Button*>(pSender);
        
        if (delegate)
        {
            delegate->clickedButtonInNonSavePopup(this, NONSAVE_POPUP_TYPE(pButton->getTag()));
        }
        
        hideLayerAction();
    }
}

void PixSmartalbumNonSavePopupLayer::backPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        hideLayerAction();
    }
}

#pragma mark - Touch & Touches

void PixSmartalbumNonSavePopupLayer::onTouchEnded(Touch* touch, Event* event)
{
    if(popup && mbln_IsAction == false)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            if(!NodeContainsPoint(popup, touch->getLocation()))
            {
                hideLayerAction();
            }
        }
    }
}

#pragma mark - Action

void PixSmartalbumNonSavePopupLayer::showAlertAction()
{
    mbln_IsAction = true;
    
    FadeIn *fadeIn = FadeIn::create(0.1f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSmartalbumNonSavePopupLayer::showAlertActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);
    
    rootNode->runAction(sequence);
}
void PixSmartalbumNonSavePopupLayer::showAlertActionFinish(Node* node)
{
    mbln_IsAction = false;
}

#pragma mark - Android BackPressed

void PixSmartalbumNonSavePopupLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}
