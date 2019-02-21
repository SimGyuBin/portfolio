//
//  PixMenuMemberLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#include "PixMenuMemberLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "PixMenuMemberPWLayer.h"
#include "PixMenuMemberNameLayer.h"
#include "PixMenuMemberPhoneLayer.h"
#include "PixMenuLeaveLayer.h"
#include "PixMenuMemberEmailLayer.h"
#include "../Util/PixUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixFacebookManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixKakaoManager.h"
#include "../Manager/PixAccountsGoogleManager.h"
#include "../Manager/PixOneSignalManager.h"
#include "Common.h"

PixMenuMemberLayer* PixMenuMemberLayer::create()
{
    PixMenuMemberLayer *menuMemberLayer = new (std::nothrow) PixMenuMemberLayer();
    if (menuMemberLayer && menuMemberLayer->init())
    {
        menuMemberLayer->autorelease();
        return menuMemberLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuMemberLayer);
        return nullptr;
    }
}

PixMenuMemberLayer::PixMenuMemberLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MenuMemberTopLayer(nullptr)
, Button_cancel(nullptr)
, profliePhoto(nullptr)
, Text_email_2(nullptr)
, Button_Incomingemail(nullptr)
, Text_Incomingemail2(nullptr)
, Button_Password(nullptr)
, Button_Name(nullptr)
, Text_Name(nullptr)
, Button_Phone(nullptr)
, Text_Phone(nullptr)
, Button_Logout(nullptr)
, Button_leave(nullptr)
, Check_onoff(nullptr)
, proflieDefaultPhoto(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMenuMemberLayer::hideMenuMemberLayerAction,this);
}

PixMenuMemberLayer::~PixMenuMemberLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMenuMemberLayer::initLayer()
{    
    if(PIXAPP->userInfo->getUserType() != USER_GENERAL)
    {
        rootNode = CSLoader::createNode("11_Menu/MenuMemberLayer_2.csb");
    }
    else
    {
        rootNode = CSLoader::createNode("11_Menu/MenuMemberLayer.csb");
    }
    
    if(rootNode)
    {
        MenuMemberTopLayer = dynamic_cast<Node*>(rootNode->getChildByName("MenuMemberTopLayer"));
        
        if(MenuMemberTopLayer)
        {
            Button_cancel = dynamic_cast<ui::Button*>(MenuMemberTopLayer->getChildByName("Button_back"));
            
            if(Button_cancel)
            {
                 Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberLayer::buttonCancelPressed, this));
            }
            
            ui::Text *title = dynamic_cast<ui::Text*>(MenuMemberTopLayer->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
        }
        
        ui::Text *Text_memberinfor = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_memberinfor"));
        
        if(Text_memberinfor)
        {
            Text_memberinfor->setString(CODE_NAME(Text_memberinfor->getString()));
        }
     
//        profliePhoto = dynamic_cast<Sprite*>(rootNode->getChildByName("profliePhoto"));
//        
//        if(profliePhoto)
//        {
//            proflieDefaultPhoto = profliePhoto->getTexture();
//        }
        
        ui::Text *Text_email = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_email"));
        
        if(Text_email)
        {
            Text_email->setString(CODE_NAME(Text_email->getString()));
        }
        
        Text_email_2 = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_email_2"));
        
        Button_Incomingemail = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Incomingemail"));
        
        
        if(Button_Incomingemail)
        {
            Button_Incomingemail->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberLayer::buttonIncomingemailPressed, this));
            
            ui::Text *Text_Incomingemail  = dynamic_cast<ui::Text*>(Button_Incomingemail->getChildByName("Text_Incomingemail"));
            
            if(Text_Incomingemail)
            {
                Text_Incomingemail->setString(CODE_NAME(Text_Incomingemail->getString()));
            }
            
            Text_Incomingemail2  = dynamic_cast<ui::Text*>(Button_Incomingemail->getChildByName("Text_Incomingemail2"));
            
            if(Text_Incomingemail2)
            {
                Text_Incomingemail2->setString("");
            }
        }
        
        Button_Password = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Password"));
        
        if(Button_Password)
        {
            Button_Password->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberLayer::buttonPasswordPressed, this));
            
            ui::Text *Text_Password  = dynamic_cast<ui::Text*>(Button_Password->getChildByName("Text_Password"));
            
            if(Text_Password)
            {
                Text_Password->setString(CODE_NAME(Text_Password->getString()));
            }
            
            ui::Text *Text_Password2  = dynamic_cast<ui::Text*>(Button_Password->getChildByName("Text_Password2"));
            
            if(Text_Password2)
            {
                Text_Password2->setString(CODE_NAME(Text_Password2->getString()));
            }
        }
        
        Button_Name = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Name"));
        
        if(Button_Name)
        {
            Button_Name->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberLayer::buttonNamePressed, this));
            
            Text_Name = dynamic_cast<ui::Text*>(Button_Name->getChildByName("Text_Name"));
            
            if(Text_Name)
            {
                Text_Name->setString(CODE_NAME(Text_Name->getString()));
            }
            
            Text_Name = dynamic_cast<ui::Text*>(Button_Name->getChildByName("Text_Name2"));
        }
        
        Button_Phone = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Phone"));
        
        if(Button_Phone)
        {
            Button_Phone->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberLayer::buttonPhonePressed, this));
            
            Text_Phone = dynamic_cast<ui::Text*>(Button_Phone->getChildByName("Text_Phone"));
            
            if(Text_Phone)
            {
                Text_Phone->setString(CODE_NAME(Text_Phone->getString()));
            }
            
            Text_Phone = dynamic_cast<ui::Text*>(Button_Phone->getChildByName("Text_Phone2"));
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
        
        Check_onoff = dynamic_cast<ui::CheckBox*>(rootNode->getChildByName("Check_onoff"));
        
        if(Check_onoff)
        {
            Check_onoff->addEventListener(CC_CALLBACK_2(PixMenuMemberLayer::alarmCheckboxPressed, this));
            
            Check_onoff->setSelected(PIXAPP->userInfo->getAlarm());
        }
        
        Button_Logout = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Logout"));
        
        if(Button_Logout)
        {
            Button_Logout->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberLayer::buttonLogoutPressed, this));
            
            ui::Text *Text_Logout = dynamic_cast<ui::Text*>(Button_Logout->getChildByName("Text_Logout"));
            
            if(Text_Logout)
            {
                Text_Logout->setString(CODE_NAME(Text_Logout->getString()));
            }
        }
        
        Button_leave = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_leave"));
        
        if(Button_leave)
        {
            Button_leave->addTouchEventListener(CC_CALLBACK_2(PixMenuMemberLayer::buttonLeavePressed, this));
            
            ui::Text *Text_leave = dynamic_cast<ui::Text*>(Button_leave->getChildByName("Text_leave"));
            
            if(Text_leave)
            {
                Text_leave->setString(CODE_NAME(Text_leave->getString()));
            }
        }
        
        ui::Text *Text_version = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_version"));
        
        if(Text_version)
        {
            Text_version->setString(CODE_NAME(Text_version->getString()));
        }
        
        ui::Text *Text_versionNumber = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_versionNumber"));
        
        if(Text_versionNumber)
        {
            std::string strVersion = StringUtils::format(CODE_NAME(Text_versionNumber->getString()), PixNativeUtils::getAppVersionString().c_str());
            
            Text_versionNumber->setString(strVersion);
        }
        
        setAnchorPoint(Vec2::ZERO);
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        addChild(rootNode);
    }
}

void PixMenuMemberLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuMemberLayerAction();
}

void PixMenuMemberLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(Text_email_2)
    {
        Text_email_2->setString(PIXAPP->userInfo->getUserID());
    }
    
    if(Text_Incomingemail2)
    {
        Text_Incomingemail2->setString(PIXAPP->userInfo->getEmail());
    }
    
    if(Text_Name)
    {
        Text_Name->setString(PIXAPP->userInfo->getName());
    }
    
    if(Text_Phone)
    {
        Text_Phone->setString(PIXAPP->userInfo->getPhoneNumber());
    }
}

#pragma mark - Button

void PixMenuMemberLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideMenuMemberLayerAction();
    }
}

void PixMenuMemberLayer::buttonIncomingemailPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        PixMenuMemberEmailLayer *menuMemberEmailLayer = PixMenuMemberEmailLayer::create();
        menuMemberEmailLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(menuMemberEmailLayer);
    }
}


void PixMenuMemberLayer::buttonPasswordPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        PixMenuMemberPWLayer *menuMemberPWLayer = PixMenuMemberPWLayer::create();
        menuMemberPWLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(menuMemberPWLayer);
    }
}

void PixMenuMemberLayer::buttonNamePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        PixMenuMemberNameLayer *menuMemberNameLayer = PixMenuMemberNameLayer::create();
        menuMemberNameLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(menuMemberNameLayer);
    }
}

void PixMenuMemberLayer::buttonPhonePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        PixMenuMemberPhoneLayer *menuMemberPhoneLayer = PixMenuMemberPhoneLayer::create();
        menuMemberPhoneLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(menuMemberPhoneLayer);
    }
}

void PixMenuMemberLayer::buttonLogoutPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        std::string msg = CODE_NAME("P24");
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixMenuMemberLayer::logoutCall));
    }
}

void PixMenuMemberLayer::buttonLeavePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        PixMenuLeaveLayer *menuLeaveLayer = PixMenuLeaveLayer::create();
        menuLeaveLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(menuLeaveLayer);
    }
}

void PixMenuMemberLayer::alarmCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        PIXAPP->userInfo->setAlarm(checkBox->isSelected());
        PixUserDefault::SetAlarm(checkBox->isSelected());
        ONESIGNAL_MANAGER->setSubscription(checkBox->isSelected());
    }
}

#pragma mark - Action

void PixMenuMemberLayer::showMenuMemberLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberLayer::showMenuMemberLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }

}

void PixMenuMemberLayer::hideMenuMemberLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuMemberLayer::hideMenuMemberLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuMemberLayer::showMenuMemberLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuMemberLayer::hideMenuMemberLayerActionFinish(Node* node)
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

#pragma mark - Touch & Touches

void PixMenuMemberLayer::onTouchEnded(Touch* touch, Event* event)
{
    PixModalLayer::onTouchEnded(touch, event);
    
//    if(NodeContainsPoint(profliePhoto, touch->getLocation()))
//    {
//        PixMenuProfilePopupLayer *menuProfilePopupLayer = PixMenuProfilePopupLayer::create();
//        menuProfilePopupLayer->setDelegate(this);
//        addChild(menuProfilePopupLayer);
//    }
}

#pragma mark - Delegate

void PixMenuMemberLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMenuMemberLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuMemberLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuMemberLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

void PixMenuMemberLayer::profileChange()
{
    
}

void PixMenuMemberLayer::profileRemove()
{
//    if(profliePhoto)
//    {
//        profliePhoto->setTexture(proflieDefaultPhoto);
//    }
}

#pragma mark - Network

void PixMenuMemberLayer::logoutCall()
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

void PixMenuMemberLayer::onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (strcmp(response->getHttpRequest()->getTag(), "smart_client_open") == 0)
    {
        PIXAPP->hideLoading();
    }
}

#pragma mark - Android BackPressed

void PixMenuMemberLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideMenuMemberLayerAction();
        }
    }
}
