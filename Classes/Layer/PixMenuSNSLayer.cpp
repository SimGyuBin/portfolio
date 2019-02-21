//
//  PixMenuSNSLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#include "PixMenuSNSLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixFacebookManager.h"


PixMenuSNSLayer* PixMenuSNSLayer::create()
{
    PixMenuSNSLayer *menuSNSLayer = new (std::nothrow) PixMenuSNSLayer();
    if (menuSNSLayer && menuSNSLayer->init())
    {
        menuSNSLayer->autorelease();
        return menuSNSLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuSNSLayer);
        return nullptr;
    }
}

PixMenuSNSLayer::PixMenuSNSLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MenuSNS_Top(nullptr)
, Button_cancel(nullptr)
, bk_Facebook(nullptr)
, facebook_Check_onoff(nullptr)
, bk_google(nullptr)
, google_Check_onoff(nullptr)
, bk_insta(nullptr)
, insta_Check_onoff(nullptr)
, bk_kakao(nullptr)
, kakao_Check_onoff(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMenuSNSLayer::hideMenuSNSLayerAction,this);
}

PixMenuSNSLayer::~PixMenuSNSLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMenuSNSLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuSNSLayer.csb");
    
    if(rootNode)
    {
        MenuSNS_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuSNS_Top"));
        
        if(MenuSNS_Top)
        {
            Button_cancel = dynamic_cast<ui::Button*>(MenuSNS_Top->getChildByName("Button_cancel"));
            
            if(Button_cancel)
            {
                Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixMenuSNSLayer::buttonCancelPressed, this));
            }
        }
        
        bk_Facebook = dynamic_cast<Sprite*>(rootNode->getChildByName("bk_Facebook"));
        
        if(bk_Facebook)
        {
            facebook_Check_onoff = dynamic_cast<ui::CheckBox*>(bk_Facebook->getChildByName("Check_onoff"));
            
            if(facebook_Check_onoff)
            {
                facebook_Check_onoff->addEventListener(CC_CALLBACK_2(PixMenuSNSLayer::facebookCheckboxPressed, this));
                
//                facebook_Check_onoff->setSelected(PIXAPP->userInfo->getFacebook());
            }
        }
        
        bk_google = dynamic_cast<Sprite*>(rootNode->getChildByName("bk_google"));
        
        if(bk_google)
        {
            google_Check_onoff = dynamic_cast<ui::CheckBox*>(bk_google->getChildByName("Check_onoff"));
            
            if(google_Check_onoff)
            {
                google_Check_onoff->addEventListener(CC_CALLBACK_2(PixMenuSNSLayer::googleCheckboxPressed, this));
                
//                google_Check_onoff->setSelected(PIXAPP->userInfo->getGooglePhoto());
            }
        }
        
        bk_insta = dynamic_cast<Sprite*>(rootNode->getChildByName("bk_insta"));
        
        if(bk_insta)
        {
            insta_Check_onoff = dynamic_cast<ui::CheckBox*>(bk_insta->getChildByName("Check_onoff"));
            
            if(insta_Check_onoff)
            {
                insta_Check_onoff->addEventListener(CC_CALLBACK_2(PixMenuSNSLayer::instaCheckboxPressed, this));
                
//                insta_Check_onoff->setSelected(PIXAPP->userInfo->getInstagram());
            }
        }
        
        bk_kakao = dynamic_cast<Sprite*>(rootNode->getChildByName("bk_kakao"));
        
        if(bk_kakao)
        {
            kakao_Check_onoff = dynamic_cast<ui::CheckBox*>(bk_kakao->getChildByName("Check_onoff"));
            
            if(kakao_Check_onoff)
            {
                kakao_Check_onoff->addEventListener(CC_CALLBACK_2(PixMenuSNSLayer::kakaoCheckboxPressed, this));
            
//                kakao_Check_onoff->setSelected(PIXAPP->userInfo->getKakaoStory());
            }
        }
        
        Size size = rootNode->getContentSize();
        setPosition(Vec2(0,-size.height));
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixMenuSNSLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuSNSLayerAction();
}

void PixMenuSNSLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

#pragma mark - Button

void PixMenuSNSLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideMenuSNSLayerAction();
    }
}

void PixMenuSNSLayer::facebookCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
//        PIXAPP->userInfo->setFacebook(checkBox->isSelected());        
    }
}

void PixMenuSNSLayer::googleCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
//        PIXAPP->userInfo->setGooglePhoto(checkBox->isSelected());
    }
}

void PixMenuSNSLayer::instaCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
//        PIXAPP->userInfo->setInstagram(checkBox->isSelected());
    }
}

void PixMenuSNSLayer::kakaoCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
//        PIXAPP->userInfo->setKakaoStory(checkBox->isSelected());
    }
}

#pragma mark - Action

void PixMenuSNSLayer::showMenuSNSLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuSNSLayer::showMenuSNSLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);

    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuSNSLayer::hideMenuSNSLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuSNSLayer::hideMenuSNSLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
 
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuSNSLayer::showMenuSNSLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuSNSLayer::hideMenuSNSLayerActionFinish(Node* node)
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

#pragma mark - Delegate

void PixMenuSNSLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMenuSNSLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuSNSLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuSNSLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

#pragma mark - Network

void PixMenuSNSLayer::onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (strcmp(response->getHttpRequest()->getTag(), "smart_client_open") == 0)
    {
        PIXAPP->hideLoading();
        
    }
}

#pragma mark - Android BackPressed

void PixMenuSNSLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideMenuSNSLayerAction();
        }
    }
}
