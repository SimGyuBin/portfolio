//
//  PixMenuLeaveLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#include "PixMenuLeaveLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixAccountsGoogleManager.h"
#include "../Manager/PixGoogleAnalyticsManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixFacebookManager.h"
#include "../Manager/PixKakaoManager.h"
#include "../Common.h"

PixMenuLeaveLayer* PixMenuLeaveLayer::create()
{
    PixMenuLeaveLayer *menuLeaveLayer = new (std::nothrow) PixMenuLeaveLayer();
    if (menuLeaveLayer && menuLeaveLayer->init())
    {
        menuLeaveLayer->autorelease();
        return menuLeaveLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuLeaveLayer);
        return nullptr;
    }
}

PixMenuLeaveLayer::PixMenuLeaveLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MenuLeave_Top(nullptr)
, Button_back(nullptr)
, check(nullptr)
, Button_leave(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMenuLeaveLayer::hideLayerAction,this);
}

PixMenuLeaveLayer::~PixMenuLeaveLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMenuLeaveLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuLeaveLayer.csb");
    
    if(rootNode)
    {
        MenuLeave_Top = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuLeave_Top"));
        
        if(MenuLeave_Top)
        {
            Button_back = dynamic_cast<ui::Button*>(MenuLeave_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuLeaveLayer::buttonBackPressed, this));
            }
            
            ui::Text *Teat = dynamic_cast<ui::Text*>(MenuLeave_Top->getChildByName("Teat"));
            
            if(Teat)
            {
                Teat->setString(CODE_NAME(Teat->getString()));
            }
        }
        
        ui::Layout *Panel = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Panel"));
        
        if(Panel)
        {
            ui::Text *Text_info_1 = dynamic_cast<ui::Text*>(Panel->getChildByName("Text_info_1"));
            
            if(Text_info_1)
            {
                Text_info_1->setString(CODE_NAME(Text_info_1->getString()));
            }
            
            ui::Text *Text_info_2 = dynamic_cast<ui::Text*>(Panel->getChildByName("Text_info_2"));
            
            if(Text_info_2)
            {
                Text_info_2->setString(CODE_NAME(Text_info_2->getString()));
            }
        }
        
        check = dynamic_cast<ui::CheckBox*>(rootNode->getChildByName("CheckBox"));
        
        if(check)
        {
            check->setSelected(false);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(check->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
        }
        
        Button_leave = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_ok"));
        
        if(Button_leave)
        {
            Button_leave->addTouchEventListener(CC_CALLBACK_2(PixMenuLeaveLayer::buttonLeavePressed, this));
            
            ui::Text *Text_ok = dynamic_cast<ui::Text*>(Button_leave->getChildByName("Text_ok"));
            
            if(Text_ok)
            {
                Text_ok->setString(CODE_NAME(Text_ok->getString()));
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
    }
}

void PixMenuLeaveLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixMenuLeaveLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(check)
    {
        if(Button_leave)
        {
            Button_leave->setEnabled(check->isSelected());
        }
    }
}

#pragma mark - Button

void PixMenuLeaveLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixMenuLeaveLayer::buttonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        
    }
}

void PixMenuLeaveLayer::buttonLeavePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(check)
        {
            if(check->isSelected())
            {
                GOOGLEANALYTICS_MANAGER->logEvent("logout", "logout", PIXAPP->userInfo->getUserID(), 1);
                
                PixUserDefault::SetMemberID("");
                PixUserDefault::SetUserID("0");
                PixUserDefault::SetName("");
                PixUserDefault::SetGrade("0");
                PixUserDefault::SetPhone("0");
                PixUserDefault::SetEmail("");
                PixUserDefault::setNoticeID(0);
                PixUserDefault::setEventID(0);
                PixUserDefault::SetUserType(0);
                
                if(PIXAPP->userInfo->getUserType() == USER_FACEBOOK)
                {
                    if(FACEBOOK_MANAGER->onLoginClick())
                    {
                        FACEBOOK_MANAGER->onLogout();
                    }
                }
                else if(PIXAPP->userInfo->getUserType() == USER_GOOGLE)
                {
                    if(ACCOUNTS_GOOGLE_MANAGER->isAuthorize())
                    {
                        ACCOUNTS_GOOGLE_MANAGER->clearAuthState();
                    }
                }
                else if(PIXAPP->userInfo->getUserType() == USER_KAKAO)
                {
                    KAKAO_MANAGER->kakaoLogout();
                }
                
                Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
            
                PIXAPP->logOut();
                PIXAPP->goLevel(0, PT_LOGOUT, TR_FADE);
                
            }
            else
            {
                std::string msg = CODE_NAME("P25");
                PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
            }
        }
    }
}

#pragma mark - Action Finish

void PixMenuLeaveLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuLeaveLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuLeaveLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuLeaveLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuLeaveLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuLeaveLayer::hideLayerActionFinish(Node* node)
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

void PixMenuLeaveLayer::onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (strcmp(response->getHttpRequest()->getTag(), "smart_client_open") == 0)
    {
        PIXAPP->hideLoading();
        
    }
}

#pragma mark - Android BackPressed

void PixMenuLeaveLayer::androidBackPressed()
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
