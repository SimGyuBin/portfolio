//
//  PixImageSharepopupLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 27..
//
//

#include "PixImageSharepopupLayer.h"
#include "../Model/MyAlbumUsedImageModel.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"
#include "../Util/PixNativeUtils.h"
#include "../UIControl/PixToast.h"
#include "../Util/StackBlur_Host.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixImageSharepopupLayer* PixImageSharepopupLayer::create(MyAlbumUsedImageModel *pModel)
{
    PixImageSharepopupLayer *layer = new (std::nothrow) PixImageSharepopupLayer(pModel);
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    else
    {
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

PixImageSharepopupLayer::PixImageSharepopupLayer(MyAlbumUsedImageModel *pModel)
: rootNode(nullptr)
, background(nullptr)
, blur(nullptr)
, blurTexture(nullptr)
, back_popup_share(nullptr)
, popup_share(nullptr)
, Button_facebook(nullptr)
, Button_insta(nullptr)
, Button_kas(nullptr)
, Button_kakao(nullptr)
, cancel(nullptr)
, mModel(nullptr)
, captureImage(nullptr)
, mbln_IsAction(false)
{
    this->mModel = pModel;
    
    hideLayerFunction = std::bind(&PixImageSharepopupLayer::hideLayerAction,this);
}

PixImageSharepopupLayer::~PixImageSharepopupLayer()
{
    Director::getInstance()->getTextureCache()->removeTexture(blurTexture);
}

void PixImageSharepopupLayer::initLayer()
{
    rootNode = CSLoader::createNode("07_My_AlbumList/MyAlbumSharepopup2Layer.csb");
    
    if(rootNode)
    {
        background = dynamic_cast<Sprite*>(rootNode->getChildByName("background"));
        
        if(background)
        {
            background->setOpacity(0);
        }
        
        back_popup_share = dynamic_cast<Node*>(rootNode->getChildByName("back_popup_share"));
        
        if(back_popup_share)
        {
            popup_share = dynamic_cast<Sprite*>(back_popup_share->getChildByName("back_popup_share"));
            
            if(popup_share)
            {
                popup_share->setOpacity(0);
                
                ui::Text *title = dynamic_cast<ui::Text*>(popup_share->getChildByName("label"));
                
                if(title)
                {
                    title->setString(CODE_NAME(title->getString()));
                }
                
                
                Button_facebook = dynamic_cast<ui::Button*>(popup_share->getChildByName("Button_facebook"));
                
                if(Button_facebook)
                {
                    Button_facebook->addTouchEventListener(CC_CALLBACK_2(PixImageSharepopupLayer::facebookPressed, this));
                }
                
                Button_insta = dynamic_cast<ui::Button*>(popup_share->getChildByName("Button_insta"));
                
                if(Button_insta)
                {
                    Button_insta->addTouchEventListener(CC_CALLBACK_2(PixImageSharepopupLayer::instaPressed, this));
                }
                
                Button_kas = dynamic_cast<ui::Button*>(popup_share->getChildByName("Button_kas"));
                
                if(Button_kas)
                {
                    Button_kas->addTouchEventListener(CC_CALLBACK_2(PixImageSharepopupLayer::kasPressed, this));
                }
                
                Button_kakao = dynamic_cast<ui::Button*>(popup_share->getChildByName("Button_kakao"));
                
                if(Button_kakao)
                {
                    Button_kakao->addTouchEventListener(CC_CALLBACK_2(PixImageSharepopupLayer::kakaoPressed, this));
                }
                
                cancel = dynamic_cast<ui::Button*>(popup_share->getChildByName("cancel"));
                
                if(cancel)
                {
                    cancel->addTouchEventListener(CC_CALLBACK_2(PixImageSharepopupLayer::cancelPressed, this));
                }
            }
        }
    }
    
    setAnchorPoint(Vec2::ZERO);
    addChild(rootNode);

    captureImage = utils::captureNode(Director::getInstance()->getRunningScene());
    
    auto t = std::thread(&PixImageSharepopupLayer::threadStack_blur_rgba32, this, captureImage);
    t.detach();
    
}

void PixImageSharepopupLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixImageSharepopupLayer::threadStack_blur_rgba32(Image *pImage)
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

void PixImageSharepopupLayer::createblur()
{
    if(captureImage)
    {
        blurTexture = Director::getInstance()->getTextureCache()->addImage(captureImage,"MyAlbumSharePopup_blur");
        
        if(blurTexture)
        {
            blur = Sprite::createWithTexture(blurTexture);
            blur->setAnchorPoint(Vec2::ZERO);
            blur->setOpacity(0);
            addChild(blur, -10);
        }
        
        CC_SAFE_RELEASE(captureImage);
    }
    
    if(background && back_popup_share)
    {
        showLayerAction();
    }
}

#pragma mark - Button

void PixImageSharepopupLayer::facebookPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(mModel->get_M_Data())
        {
            PixNativeUtils::nativeCallFacebookSharePhotoData(mModel->get_M_Data());
            hideLayerAction();
        }
    }
}

void PixImageSharepopupLayer::instaPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        std::string msg = CODE_NAME("T02");
        PixToast::showToast(msg.c_str());
    }
}

void PixImageSharepopupLayer::kasPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        std::string msg = CODE_NAME("T02");
        PixToast::showToast(msg.c_str());
    }
}

void PixImageSharepopupLayer::kakaoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        std::string msg = CODE_NAME("T02");
        PixToast::showToast(msg.c_str());
    }
}

void PixImageSharepopupLayer::cancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(back_popup_share)
        {
            hideLayerAction();
        }
    }
}

#pragma mark - Touch & Touches

void PixImageSharepopupLayer::onTouchEnded(Touch* touch, Event* event)
{
//    if(back_popup_share && mbln_IsAction == false)
//    {
//        if(NodeContainsPoint(this, touch->getLocation()))
//        {
//            if(!NodeContainsPoint(back_popup_share, touch->getLocation()))
//            {
//                hideLayerAction();
//            }
//        }
//    }
}

#pragma mark - Action

void PixImageSharepopupLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    EaseSineOut *easeSineOut = EaseSineOut::create(FadeIn::create(0.2f));
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixImageSharepopupLayer::showLayerActionFinish, this));
    
    background->runAction(Sequence::create(easeSineOut, callFuncN, NULL));
    
    blur->runAction(easeSineOut->clone());
    
    ScaleTo *scaleTo_1 = ScaleTo::create(0.1f, 1.1f);
    EaseSineOut *easeSineOut_1 = EaseSineOut::create(scaleTo_1);
    
    ScaleTo *scaleTo_2 = ScaleTo::create(0.1f, 1.0f);
    EaseSineOut *easeSineOut_2 = EaseSineOut::create(scaleTo_2);
    
    Sequence *sequence = Sequence::create(easeSineOut_1, easeSineOut_2, NULL);
    EaseSineOut *easeSineOut_3 = EaseSineOut::create(FadeIn::create(0.2f));
    Spawn *spawn = Spawn::create(sequence, easeSineOut_3, NULL);
    
    popup_share->stopAllActions();
    popup_share->runAction(spawn);
}

void PixImageSharepopupLayer::hideLayerAction()
{
    mbln_IsAction = true;
    hideLayerActionFinish(nullptr);
}

void PixImageSharepopupLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
}

void PixImageSharepopupLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}
#pragma mark - Android BackPressed

void PixImageSharepopupLayer::androidBackPressed()
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
