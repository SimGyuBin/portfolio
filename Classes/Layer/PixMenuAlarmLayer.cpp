//
//  PixMenuAlarmLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#include "PixMenuAlarmLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuAlarmLayer* PixMenuAlarmLayer::create()
{
    PixMenuAlarmLayer *menuAlarmLayer = new (std::nothrow) PixMenuAlarmLayer();
    if (menuAlarmLayer && menuAlarmLayer->init())
    {
        menuAlarmLayer->autorelease();
        return menuAlarmLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuAlarmLayer);
        return nullptr;
    }
}

PixMenuAlarmLayer::PixMenuAlarmLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MenuAlarm_Top(nullptr)
, Button_cancel(nullptr)
, Check_onoff(nullptr)
, mbln_IsAction(false)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMenuAlarmLayer::hideLayerAction,this);
}

PixMenuAlarmLayer::~PixMenuAlarmLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMenuAlarmLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuAlarmLayer.csb");
    
    if(rootNode)
    {
        MenuAlarm_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuAlarm_Top"));
        
        if(MenuAlarm_Top)
        {
            Button_cancel = dynamic_cast<ui::Button*>(MenuAlarm_Top->getChildByName("Button_cancel"));
            
            if(Button_cancel)
            {
                Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixMenuAlarmLayer::buttonCancelPressed, this));
            }
            
            ui::Text *title = dynamic_cast<ui::Text*>(MenuAlarm_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
        }
        
        Check_onoff = dynamic_cast<ui::CheckBox*>(rootNode->getChildByName("Check_onoff"));
        
        if(Check_onoff)
        {
            Check_onoff->addEventListener(CC_CALLBACK_2(PixMenuAlarmLayer::alarmCheckboxPressed, this));
            
            Check_onoff->setSelected(PIXAPP->userInfo->getAlarm());
        }
        
        ui::Text *alarm_text = dynamic_cast<ui::Text*>(rootNode->getChildByName("alarm_text"));
        
        if(alarm_text)
        {
            alarm_text->setString(CODE_NAME(alarm_text->getString()));
        }
        
        ui::Text *alarn_info = dynamic_cast<ui::Text*>(rootNode->getChildByName("alarn_info"));
        
        if(alarn_info)
        {
            alarn_info->setString(CODE_NAME(alarn_info->getString()));
        }
        
        Size size = rootNode->getContentSize();
        setPosition(Vec2(0,-size.height));
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixMenuAlarmLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixMenuAlarmLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

#pragma mark - Button

void PixMenuAlarmLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixMenuAlarmLayer::alarmCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        PIXAPP->userInfo->setAlarm(checkBox->isSelected());
    }
}

#pragma mark - Action

void PixMenuAlarmLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuAlarmLayer::showLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuAlarmLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuAlarmLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuAlarmLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuAlarmLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionFinish(this);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Network

void PixMenuAlarmLayer::onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (strcmp(response->getHttpRequest()->getTag(), "smart_client_open") == 0)
    {
        PIXAPP->hideLoading();
        
    }
}

#pragma mark - Android BackPressed

void PixMenuAlarmLayer::androidBackPressed()
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
