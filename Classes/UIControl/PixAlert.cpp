//
//  PixAlert.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 22..
//
//

#include "PixAlert.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixNativeUtils.h"
#include "../Scene/PixApp.h"
#include "../Util/StackBlur_Host.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

static std::string textureKeyName = "PixAlert_blur";

PixAlert* PixAlert::show(const char* title, const char* message, PixAlertDelegate *delegate, ALERT_TYPE type,
                       int tag, Ref *target, SEL_CallFunc selector, bool isCaptureImage)
{
    PixAlert *alert = new PixAlert();
    alert->init(title, message, delegate, type, target, selector, isCaptureImage);
    alert->setTag(tag);
    alert->setPosition(Vec2::ZERO);
    Director::getInstance()->getRunningScene()->addChild(alert, 1000, tag);
    alert->release();
    
    return alert;
}

PixAlert* PixAlert::show(const char* message, PixAlertDelegate *delegate, ALERT_TYPE type,
                       int tag, Ref *target, SEL_CallFunc selector, bool isCaptureImage)
{
    return PixAlert::show("", message, delegate, type, tag, target, selector, isCaptureImage);
}

PixAlert::PixAlert()
: alert(nullptr)
, blockout(nullptr)
, blur(nullptr)
, blurTexture(nullptr)
, Background_popup(nullptr)
, captureImage(nullptr)
, mbln_IsAction(false)
, mbln_CaptureImage(false)
{
    hideLayerFunction = std::bind(&PixAlert::hideLayerAction,this);
}

PixAlert::~PixAlert()
{
    CCLOG("%s", __FUNCTION__);
    
    if(blurTexture)
    {
        Director::getInstance()->getTextureCache()->removeTexture(blurTexture);
    }
}

#pragma mark - Private

void PixAlert::init(const char* title, const char* message, PixAlertDelegate *delegate, ALERT_TYPE type,
                   Ref *&target, SEL_CallFunc &selector, bool isCaptureImage)
{
    PixModalLayer::init();

    this->delegate = delegate;
    this->target = target;
    this->selector = selector;
    this->mbln_CaptureImage = isCaptureImage;
    
    std::string strFrame;
    std::string strfilename;
    
    if (PIXAPP->frameType == FRAME_VERTICALSIZE)
    {
        strFrame = "Vertical";
    }
    else if(PIXAPP->frameType == FRAME_HORIZONTAL)
    {
        strFrame = "Horizontal";
    }
    
    if (type == ALERT_TYPE_OK)
    {
        strfilename = "00_Common/PopupLayer_" + strFrame + "1.csb";
    }
    else if (type == ALERT_TYPE_ALL)
    {
        strfilename = "00_Common/PopupLayer_" + strFrame + "2.csb";
    }
    
    alert = CSLoader::createNode(strfilename);
    
    if(alert)
    {
        blockout =  dynamic_cast<Sprite*>(alert->getChildByName("blockout"));
        
        if(blockout)
        {
            
            if(isCaptureImage)
            {
                blockout->setOpacity(0);
            }
        }
        
        Background_popup = dynamic_cast<Sprite*>(alert->getChildByName("Background_popup"));
        
        if(Background_popup)
        {
            if(isCaptureImage)
            {
                Background_popup->setOpacity(0);
            }
            
            ui::Text *titleLabel = dynamic_cast<ui::Text*>(Background_popup->getChildByName("titleLabel"));
            
            if(titleLabel)
            {
                titleLabel->setString(title);
            }
            
            ui::Text *messageLabel = dynamic_cast<ui::Text*>(Background_popup->getChildByName("Text"));
            
            if(messageLabel)
            {
                messageLabel->setString(message);
            }
            
            ui::Button *ok = dynamic_cast<ui::Button*>(Background_popup->getChildByName("ok"));
            
            if(ok)
            {
                ok->setTag(ALERT_BUTTON_OK);
                ok->addTouchEventListener(CC_CALLBACK_2(PixAlert::buttonPressed, this));
                
                ui::Text *Text_ok = dynamic_cast<ui::Text*>(ok->getChildByName("Text_ok"));
                
                if(Text_ok)
                {
                    std::string strText = CODE_NAME(Text_ok->getString());
                    
                    if(strText.empty())
                    {
                        strText = "OK";
                    }
                    
                    Text_ok->setString(strText);
                }
            }
            
            ui::Button *cencel = dynamic_cast<ui::Button*>(Background_popup->getChildByName("cencel"));
            
            if(cencel)
            {
                cencel->setTag(ALERT_BUTTON_CANCEL);
                cencel->addTouchEventListener(CC_CALLBACK_2(PixAlert::buttonPressed, this));
                
                ui::Text *Text_cencel = dynamic_cast<ui::Text*>(cencel->getChildByName("Text_cencel"));
                
                if(Text_cencel)
                {
                    std::string strText = CODE_NAME(Text_cencel->getString());
                    
                    if(strText.empty())
                    {
                        strText = "CENCEL";
                    }
                    
                    Text_cencel->setString(strText);
                }
            }
        }
        
        addChild(alert);
    }
    
    if(isCaptureImage)
    {
        captureImage = utils::captureNode(Director::getInstance()->getRunningScene());
        
        auto t = std::thread(&PixAlert::threadStack_blur_rgba32, this, captureImage);
        t.detach();
    }
}

void PixAlert::threadStack_blur_rgba32(Image *pImage)
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

void PixAlert::createblur()
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

void PixAlert::onEnter()
{
    PixModalLayer::onEnter();
    
}

void PixAlert::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixAlert::hideLayerAction()
{
    this->removeFromParentAndCleanup(true);
}

#pragma mark - Button

void PixAlert::buttonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        ui::Button *pButton = dynamic_cast<ui::Button*>(pSender);
        
        if (delegate)
        {
            delegate->clickedButtonInAlert(this, ALERT_BUTTON_TYPE(pButton->getTag()));
        }
        else if (target != NULL && selector != NULL && pButton->getTag() == ALERT_BUTTON_OK)
        {
            (target->*selector)();
        }
        
        hideLayerAction();
    }
}

#pragma mark - Action

void PixAlert::showAlertAction()
{
    mbln_IsAction = true;
    
    EaseSineOut *easeSineOut = EaseSineOut::create(FadeIn::create(0.2f));
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixAlert::showAlertActionFinish, this));
    
    blockout->runAction(Sequence::create(easeSineOut, callFuncN, NULL));
    
    blur->runAction(easeSineOut->clone());
    
    ScaleTo *scaleTo_1 = ScaleTo::create(0.1f, 1.1f);
    EaseSineOut *easeSineOut_1 = EaseSineOut::create(scaleTo_1);
    
    ScaleTo *scaleTo_2 = ScaleTo::create(0.1f, 1.0f);
    EaseSineOut *easeSineOut_2 = EaseSineOut::create(scaleTo_2);
    
    Sequence *sequence = Sequence::create(easeSineOut_1, easeSineOut_2, NULL);
    EaseSineOut *easeSineOut_3 = EaseSineOut::create(FadeIn::create(0.2f));
    Spawn *spawn = Spawn::create(sequence, easeSineOut_3, NULL);
    
    Background_popup->runAction(spawn);
}

void PixAlert::showAlertActionFinish(Node* node)
{
    mbln_IsAction = false;
}
