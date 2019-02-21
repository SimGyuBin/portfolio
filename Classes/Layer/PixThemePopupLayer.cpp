//
//  PixThemePopupLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 22..
//
//

#include "PixThemePopupLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"

PixThemePopupLayer* PixThemePopupLayer::create()
{
    PixThemePopupLayer *themePopupLayer = new (std::nothrow) PixThemePopupLayer();
    if (themePopupLayer && themePopupLayer->init())
    {
        themePopupLayer->autorelease();
        return themePopupLayer;
    }
    else
    {
        CC_SAFE_DELETE(themePopupLayer);
        return nullptr;
    }
}

PixThemePopupLayer::PixThemePopupLayer()
: rootNode(nullptr)
, mDelegate(nullptr)
, blockout(nullptr)
, ThemePopup(nullptr)
, mygallery(nullptr)
, face(nullptr)
, google(nullptr)
, insta(nullptr)
, kakao(nullptr)
, mbln_isAction(false)
{
    hideLayerFunction = std::bind(&PixThemePopupLayer::hideLayerAction,this);
}

PixThemePopupLayer::~PixThemePopupLayer()
{
    
}

void PixThemePopupLayer::initLayer()
{
    rootNode = CSLoader::createNode("03_Theme/PixThemeLevel_Popup.csb");
    
    if(rootNode)
    {
        blockout = dynamic_cast<Sprite*>(rootNode->getChildByName("blockout"));
        
        if(blockout)
        {
            blockout->setOpacity(0);
        }
        
        ThemePopup = dynamic_cast<Node*>(rootNode->getChildByName("ThemePopup"));
        
        if(ThemePopup)
        {
            mygallery = dynamic_cast<ui::Button*>(ThemePopup->getChildByName("mygallery"));
            if(mygallery)
            {
                mygallery->addTouchEventListener(CC_CALLBACK_2(PixThemePopupLayer::mygalleryPressed, this));
            }
            
            face = dynamic_cast<ui::Button*>(ThemePopup->getChildByName("face"));
            if(face)
            {
                face->addTouchEventListener(CC_CALLBACK_2(PixThemePopupLayer::facePressed, this));
            }
            
            google = dynamic_cast<ui::Button*>(ThemePopup->getChildByName("google"));
            if(google)
            {
                google->addTouchEventListener(CC_CALLBACK_2(PixThemePopupLayer::googlePressed, this));
            }
            
            insta = dynamic_cast<ui::Button*>(ThemePopup->getChildByName("insta"));
            if(insta)
            {
                insta->addTouchEventListener(CC_CALLBACK_2(PixThemePopupLayer::instaPressed, this));
            }
            
            kakao = dynamic_cast<ui::Button*>(ThemePopup->getChildByName("Pixintro_popup_back"));
            if(kakao)
            {
                kakao->addTouchEventListener(CC_CALLBACK_2(PixThemePopupLayer::kakaoPressed, this));
            }
            
            Size themePopupSize = ThemePopup->getContentSize();
            
            ThemePopup->setAnchorPoint(Vec2::ZERO);
            ThemePopup->setPosition(Vec2(0,-themePopupSize.height));
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixThemePopupLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    
    if(blockout && ThemePopup)
    {
        showLayerAction();
    }
}

#pragma mark - Button

void PixThemePopupLayer::mygalleryPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        if(mDelegate)
            mDelegate->myGalleryPressed();
    }
}

void PixThemePopupLayer::facePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        if(mDelegate)
            mDelegate->facePressed();
    }
}

void PixThemePopupLayer::googlePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        if(mDelegate)
            mDelegate->googlePressed();
    }
}

void PixThemePopupLayer::instaPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        if(mDelegate)
            mDelegate->instaPressed();
    }
}

void PixThemePopupLayer::kakaoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        if(mDelegate)
            mDelegate->kakaoPressed();
    }
}

#pragma mark - Touch & Touches

void PixThemePopupLayer::onTouchEnded(Touch* touch, Event* event)
{
    if(ThemePopup && mbln_isAction == false)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            if(!NodeContainsPoint(ThemePopup, touch->getLocation()))
            {
                if(blockout && ThemePopup)
                {
                    hideLayerAction();
                }
            }
        }
    }
}

#pragma mark - Action

void PixThemePopupLayer::showLayerAction()
{
    mbln_isAction = true;
    
    FadeIn *fadeIn = FadeIn::create(0.3f);
    EaseSineOut *easeSineOut = EaseSineOut::create(fadeIn);
    blockout->runAction(easeSineOut);
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixThemePopupLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    ThemePopup->runAction(sequence);
}

void PixThemePopupLayer::hideLayerAction()
{
    mbln_isAction = true;
    
    FadeOut *fadeOut = FadeOut::create(0.3f);
    EaseSineOut *easeSineOut = EaseSineOut::create(fadeOut);
    blockout->runAction(easeSineOut);
    
    Size themePopupSize = ThemePopup->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-themePopupSize.height));
    easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixThemePopupLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    ThemePopup->runAction(sequence);
}

void PixThemePopupLayer::showLayerActionFinish(Node* node)
{
    mbln_isAction = false;
}

void PixThemePopupLayer::hideLayerActionFinish(Node* node)
{
    mbln_isAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixThemePopupLayer::androidBackPressed()
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
