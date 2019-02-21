//
//  PixPushPopupLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 7. 7..
//
//

#include "PixPushPopupLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/StackBlur_Host.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixOneSignalManager.h"
#include "../Common.h"

static std::string textureKeyName = "PixPushPopupLayer_blur";

PixPushPopupLayer* PixPushPopupLayer::create(const std::string &strMessage, const std::string &strTitle, const std::string &strPush_event)
{
    PixPushPopupLayer *pushPopupLayer = new (std::nothrow) PixPushPopupLayer(strMessage, strTitle, strPush_event);
    if (pushPopupLayer && pushPopupLayer->init())
    {
        pushPopupLayer->autorelease();
        return pushPopupLayer;
    }
    else
    {
        CC_SAFE_DELETE(pushPopupLayer);
        return nullptr;
    }
}

PixPushPopupLayer::PixPushPopupLayer(const std::string &strMessage, const std::string &strTitle, const std::string &strPush_event)
: rootNode(nullptr)
, blockout(nullptr)
, Background_popup(nullptr)
, blur(nullptr)
, blurTexture(nullptr)
, captureImage(nullptr)
, startPos(Vec2::ZERO)
, endPos(Vec2::ZERO)
, mbln_IsAction(false)
, mstr_Message(strMessage)
, mstr_Title(strTitle)
, mstr_Push_event(strPush_event)
{
    hideLayerFunction = std::bind(&PixPushPopupLayer::hideLayerAction,this);
}

PixPushPopupLayer::~PixPushPopupLayer()
{
    CCLOG("%s", __FUNCTION__);
    
    if(blurTexture)
    {
        Director::getInstance()->getTextureCache()->removeTexture(blurTexture);
    }
}

void PixPushPopupLayer::initLayer()
{
    rootNode = CSLoader::createNode("00_Common/Push_PopupLayer.csb");
    
    if(rootNode)
    {
        blockout = dynamic_cast<Sprite*>(rootNode->getChildByName("blockout"));
        
        if(blockout)
        {
            blockout->setOpacity(0);
        }
        
        Background_popup = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_popup"));
        
        if(Background_popup)
        {
            endPos = Background_popup->getPosition();
            
            startPos = Vec2(endPos.x,
                            (rootNode->getContentSize().height + (Background_popup->getContentSize().height * 0.5f)));
            
            Background_popup->setPosition(startPos);
            
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Background_popup->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(getTitle());
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Background_popup->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(getMessage());
            }
            
            ui::Button *Button_ok = dynamic_cast<ui::Button*>(Background_popup->getChildByName("Button_ok"));
            
            if(Button_ok)
            {
                Button_ok->addTouchEventListener(CC_CALLBACK_2(PixPushPopupLayer::buttonOkPressed, this));
                
                ui::Text *Text_ok = dynamic_cast<ui::Text*>(Button_ok->getChildByName("Text_ok"));
                
                if(Text_ok)
                {
                    Text_ok->setString(CODE_NAME(Text_ok->getString()));
                }
            }
            
            ui::Button *Button_close = dynamic_cast<ui::Button*>(Background_popup->getChildByName("Button_close"));
            
            if(Button_close)
            {
                Button_close->addTouchEventListener(CC_CALLBACK_2(PixPushPopupLayer::buttonClosePressed, this));
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
        
        captureImage = utils::captureNode(Director::getInstance()->getRunningScene());
        
        auto t = std::thread(&PixPushPopupLayer::threadStack_blur_rgba32, this, captureImage);
        t.detach();
    }
}

void PixPushPopupLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixPushPopupLayer::threadStack_blur_rgba32(Image *pImage)
{
    int nW = pImage->getWidth();
    int nH = pImage->getHeight();
    
    stack_blur_rgba32((unsigned int*)pImage->getData(), nW, nH, pImage->getHeight() / 1280.0f * 15);
    
    if (nullptr != _scheduler)
    {
        _scheduler->performFunctionInCocosThread([this] {
            
            createblur();
        });
    }
}

void PixPushPopupLayer::createblur()
{
    if(captureImage)
    {
        blurTexture = Director::getInstance()->getTextureCache()->addImage(captureImage, textureKeyName);
        
        if(blurTexture)
        {
            blur = Sprite::createWithTexture(blurTexture);
            blur->setAnchorPoint(Vec2::ZERO);
            blur->setOpacity(0);
            addChild(blur, -10);
        }
        
        CC_SAFE_RELEASE(captureImage);
    }
    
    showAlertAction();
}

void PixPushPopupLayer::hideLayerAction()
{
    this->removeFromParentAndCleanup(true);
}

#pragma mark - Action

void PixPushPopupLayer::showAlertAction()
{
    if(blockout == nullptr)
        return;
    
    if(blur == nullptr)
        return;
    
    if(Background_popup == nullptr)
        return;
    
    mbln_IsAction = true;
    
    EaseSineOut *easeSineOut = EaseSineOut::create(FadeIn::create(0.2f));
    blockout->runAction(easeSineOut);
    blur->runAction(easeSineOut->clone());
    
    EaseBackOut *easeElasticOut = EaseBackOut::create(MoveTo::create(0.3f,endPos));
    
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixPushPopupLayer::showAlertActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeElasticOut, callFuncN, NULL);
    
    Background_popup->runAction(sequence);
}

void PixPushPopupLayer::showAlertActionFinish(Node* node)
{
    mbln_IsAction = false;
}

#pragma mark - Button

void PixPushPopupLayer::buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        hideLayerFunction = nullptr;
        ONESIGNAL_MANAGER->moveEventPage(getPush_event());
        hideLayerAction();
    }
}

void PixPushPopupLayer::buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        hideLayerFunction = nullptr;
        hideLayerAction();
    }
}

#pragma mark - Android BackPressed

void PixPushPopupLayer::androidBackPressed()
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
