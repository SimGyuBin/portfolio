//
//  PixSmartalbumSharePopupLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 23..
//
//

#include "PixSmartalbumSharePopupLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"
#include "../UIControl/PixToast.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixSmartalbumSharePopupLayer* PixSmartalbumSharePopupLayer::create()
{
    PixSmartalbumSharePopupLayer *smartalbumSharePopupLayer = new (std::nothrow) PixSmartalbumSharePopupLayer();
    if (smartalbumSharePopupLayer && smartalbumSharePopupLayer->init())
    {
        smartalbumSharePopupLayer->autorelease();
        return smartalbumSharePopupLayer;
    }
    else
    {
        CC_SAFE_DELETE(smartalbumSharePopupLayer);
        return nullptr;
    }
}

PixSmartalbumSharePopupLayer::PixSmartalbumSharePopupLayer()
: rootNode(nullptr)
, blockout(nullptr)
, SharePopup(nullptr)
, Button_facebook(nullptr)
, Button_insta(nullptr)
, Button_kas(nullptr)
, Button_kakao(nullptr)
, mbln_isAction(false)
, mShareDelegate(nullptr)
, eShareType(SMARTALBUM_SHARE_NONE)
{
    hideLayerFunction = std::bind(&PixSmartalbumSharePopupLayer::hideLayerAction,this);
}

PixSmartalbumSharePopupLayer::~PixSmartalbumSharePopupLayer()
{
    
}

void PixSmartalbumSharePopupLayer::initLayer()
{
    rootNode = CSLoader::createNode("05_AlbumView/smartalbumSharePopupLayer.csb");
    
    if(rootNode)
    {
        blockout = dynamic_cast<Sprite*>(rootNode->getChildByName("blockout"));
        
        if(blockout)
        {
            blockout->setOpacity(0);
        }
        
        SharePopup = dynamic_cast<Node*>(rootNode->getChildByName("smartalbumShareBottom_Page"));
        
        if(SharePopup)
        {
            Button_facebook = dynamic_cast<ui::Button*>(SharePopup->getChildByName("Button_facebook"));
          
            if(Button_facebook)
            {
                Button_facebook->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumSharePopupLayer::buttonFacebookPressed, this));
            }
            
            Button_insta = dynamic_cast<ui::Button*>(SharePopup->getChildByName("Button_insta"));
            
            if(Button_insta)
            {
                Button_insta->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumSharePopupLayer::buttonInstaPressed, this));
            }
            
            Button_kas = dynamic_cast<ui::Button*>(SharePopup->getChildByName("Button_kas"));
            
            if(Button_kas)
            {
                Button_kas->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumSharePopupLayer::buttonKasPressed, this));
            }
            
            Button_kakao = dynamic_cast<ui::Button*>(SharePopup->getChildByName("Button_kakao"));
            
            if(Button_kakao)
            {
                Button_kakao->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumSharePopupLayer::buttonKakaoPressed, this));
            }
            
            
            Size sharePopupSize = SharePopup->getContentSize();
            
            SharePopup->setAnchorPoint(Vec2::ZERO);
            SharePopup->setPosition(Vec2(0,-sharePopupSize.height));
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixSmartalbumSharePopupLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    
    if(blockout && SharePopup)
    {
        showLayerAction();
    }
}

void PixSmartalbumSharePopupLayer::showServiceComingSoon()
{
    std::string msg = CODE_NAME("T02");
    PixToast::showToast(msg.c_str());
}

#pragma mark - Button

void PixSmartalbumSharePopupLayer::buttonFacebookPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        eShareType = SMARTALBUM_SHARE_FACEBOOK;
        hideLayerAction();
    }
}

void PixSmartalbumSharePopupLayer::buttonInstaPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        showServiceComingSoon();
    }
}

void PixSmartalbumSharePopupLayer::buttonKasPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        showServiceComingSoon();
    }
}

void PixSmartalbumSharePopupLayer::buttonKakaoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        showServiceComingSoon();
    }
}

#pragma mark - Touch & Touches

void PixSmartalbumSharePopupLayer::onTouchEnded(Touch* touch, Event* event)
{
    if(SharePopup && mbln_isAction == false)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            if(!NodeContainsPoint(SharePopup, touch->getLocation()))
            {
                if(blockout && SharePopup)
                {
                    hideLayerAction();
                }
            }
        }
    }
}

#pragma mark - Action

void PixSmartalbumSharePopupLayer::showLayerAction()
{
    mbln_isAction = true;
    
    FadeIn *fadeIn = FadeIn::create(0.3f);
    EaseSineOut *easeSineOut = EaseSineOut::create(fadeIn);
    blockout->runAction(easeSineOut);
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSmartalbumSharePopupLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    SharePopup->stopAllActions();
    SharePopup->runAction(sequence);
}

void PixSmartalbumSharePopupLayer::hideLayerAction()
{
    mbln_isAction = true;
    
    FadeOut *fadeOut = FadeOut::create(0.3f);
    EaseSineOut *easeSineOut = EaseSineOut::create(fadeOut);
    blockout->runAction(easeSineOut);
    
    Size sharePopupSize = SharePopup->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-sharePopupSize.height));
    easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSmartalbumSharePopupLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    SharePopup->stopAllActions();
    SharePopup->runAction(sequence);

    if(eShareType != SMARTALBUM_SHARE_NONE)
    {
        if(mShareDelegate)
        {
            mShareDelegate->smartAlbumShare(eShareType);
        }
    }
}

void PixSmartalbumSharePopupLayer::showLayerActionFinish(Node* node)
{
    mbln_isAction = false;
}

void PixSmartalbumSharePopupLayer::hideLayerActionFinish(Node* node)
{
    mbln_isAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixSmartalbumSharePopupLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_isAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}
