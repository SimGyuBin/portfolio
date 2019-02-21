//
//  PixSelectedPhotoAmountLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 18..
//
//

#include "PixSelectedPhotoAmountLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/StackBlur_Host.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixSelectedPhotoAmountLayer* PixSelectedPhotoAmountLayer::create()
{
    PixSelectedPhotoAmountLayer *selectedPhotoAmountLayer = new (std::nothrow) PixSelectedPhotoAmountLayer();
    if (selectedPhotoAmountLayer && selectedPhotoAmountLayer->init())
    {
        selectedPhotoAmountLayer->autorelease();
        return selectedPhotoAmountLayer;
    }
    else
    {
        CC_SAFE_DELETE(selectedPhotoAmountLayer);
        return nullptr;
    }
}

PixSelectedPhotoAmountLayer::PixSelectedPhotoAmountLayer()
: rootNode(nullptr)
, mDelegate(nullptr)
, background(nullptr)
, blur(nullptr)
, blurTexture(nullptr)
, SelectedPhotoAmount(nullptr)
, Background_Photo_popup_1(nullptr)
, Button_1(nullptr)
, Button_2(nullptr)
, Button_3(nullptr)
, Button_4(nullptr)
, cancel(nullptr)
, mbln_isAction(false)
{
    hideLayerFunction = std::bind(&PixSelectedPhotoAmountLayer::hideLayerAction,this);
}

PixSelectedPhotoAmountLayer::~PixSelectedPhotoAmountLayer()
{
    Director::getInstance()->getTextureCache()->removeTexture(blurTexture);
}

void PixSelectedPhotoAmountLayer::initLayer()
{
    rootNode = CSLoader::createNode("04_SelectPhoto/SelectedPhotoAmount_Popup.csb");
    
    if(rootNode)
    { 
        Image *image = utils::captureNode(PIXAPP->curLevel);
        
        int nW = image->getWidth();
        int nH = image->getHeight();
        
        stack_blur_rgba32((unsigned int*)image->getData(), nW, nH, image->getHeight() / 1280.0f * 15);
        blurTexture = Director::getInstance()->getTextureCache()->addImage(image,"MyAlbumSharePopup_blur");
        CC_SAFE_RELEASE(image);
        
        blur = Sprite::createWithTexture(blurTexture);
        blur->setAnchorPoint(Vec2::ZERO);
        blur->setOpacity(0);
        addChild(blur);
        
        background = dynamic_cast<Sprite*>(rootNode->getChildByName("Background"));
        
        if(background)
        {
            background->setOpacity(0);
        }
        
        SelectedPhotoAmount = dynamic_cast<Node*>(rootNode->getChildByName("SelectedPhotoAmount"));
        
        if(SelectedPhotoAmount)
        {
            Background_Photo_popup_1 = dynamic_cast<Sprite*>(SelectedPhotoAmount->getChildByName("Background_Photo_popup_1"));

            if(Background_Photo_popup_1)
            {
                Background_Photo_popup_1->setOpacity(0);
                
                ui::Text *title =  dynamic_cast<ui::Text*>(Background_Photo_popup_1->getChildByName("Text"));
                
                if(title)
                {
                    title->setString(CODE_NAME(title->getString()));
                }
                
                Button_1 = dynamic_cast<ui::Button*>(Background_Photo_popup_1->getChildByName("Button_1"));
                
                if(Button_1)
                {
                    Button_1->addTouchEventListener(CC_CALLBACK_2(PixSelectedPhotoAmountLayer::buttonPressed, this));
                }
                
                Button_2 = dynamic_cast<ui::Button*>(Background_Photo_popup_1->getChildByName("Button_2"));
                
                if(Button_2)
                {
                    Button_2->addTouchEventListener(CC_CALLBACK_2(PixSelectedPhotoAmountLayer::buttonPressed, this));
                }
                
                Button_3 = dynamic_cast<ui::Button*>(Background_Photo_popup_1->getChildByName("Button_3"));
                
                if(Button_3)
                {
                    Button_3->addTouchEventListener(CC_CALLBACK_2(PixSelectedPhotoAmountLayer::buttonPressed, this));
                }
                
                Button_4 = dynamic_cast<ui::Button*>(Background_Photo_popup_1->getChildByName("Button_4"));
                
                if(Button_4)
                {
                    Button_4->addTouchEventListener(CC_CALLBACK_2(PixSelectedPhotoAmountLayer::buttonPressed, this));
                }
                
                cancel = dynamic_cast<ui::Button*>(Background_Photo_popup_1->getChildByName("cancel"));
                
                if(cancel)
                {
                    cancel->addTouchEventListener(CC_CALLBACK_2(PixSelectedPhotoAmountLayer::cancelPressed, this));
                }
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixSelectedPhotoAmountLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    
    if(background && SelectedPhotoAmount)
    {
        showLayerAction();
    }
}

void PixSelectedPhotoAmountLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(_isTransitionFinished)
    {
        if(blur)
        {
//            Director::getInstance()->getTextureCache()->removeTexture(blurTexture);
//            
//            Image *image = utils::captureNode(PIXAPP->curLevel);
//            
//            int nW = image->getWidth();
//            int nH = image->getHeight();
//            
//            stack_blur_rgba32((unsigned int*)image->getData(), nW, nH, image->getHeight() / 1280.0f * 15);
//            blurTexture = Director::getInstance()->getTextureCache()->addImage(image,"MyAlbumSharePopup_blur");
//            CC_SAFE_RELEASE(image);
//            blur->setTexture(blurTexture);
        }
    }
}

#pragma mark - Button

void PixSelectedPhotoAmountLayer::buttonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        ui::Button *button =  dynamic_cast<ui::Button*>(pSender);

        if(button)
        {
            if(mDelegate)
                mDelegate->buttonAmountPressed(button->getTag());
            
            if(background && SelectedPhotoAmount)
            {
                hideLayerAction();
            }
        }
    }
}

void PixSelectedPhotoAmountLayer::cancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        if(background && SelectedPhotoAmount)
        {
            hideLayerAction();
        }
    }
}

#pragma mark - Touch & Touches

void PixSelectedPhotoAmountLayer::onTouchEnded(Touch* touch, Event* event)
{
//    if(SelectedPhotoAmount && mbln_isAction == false)
//    {
//        if(NodeContainsPoint(this, touch->getLocation()))
//        {
//            if(!NodeContainsPoint(SelectedPhotoAmount, touch->getLocation()))
//            {
//                if(background && SelectedPhotoAmount)
//                {
//                    hideLayerAction();
//                }
//            }
//        }
//    }
}

#pragma mark - Action

void PixSelectedPhotoAmountLayer::showLayerAction()
{
    mbln_isAction = true;
    
    EaseSineOut *easeSineOut = EaseSineOut::create(FadeIn::create(0.2f));
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSelectedPhotoAmountLayer::showLayerActionFinish, this));
    
    background->runAction(Sequence::create(easeSineOut, callFuncN, NULL));
    
    blur->runAction(easeSineOut->clone());
    
    ScaleTo *scaleTo_1 = ScaleTo::create(0.1f, 1.1f);
    EaseSineOut *easeSineOut_1 = EaseSineOut::create(scaleTo_1);
    
    ScaleTo *scaleTo_2 = ScaleTo::create(0.1f, 1.0f);
    EaseSineOut *easeSineOut_2 = EaseSineOut::create(scaleTo_2);
    
    Sequence *sequence = Sequence::create(easeSineOut_1, easeSineOut_2, NULL);
    EaseSineOut *easeSineOut_3 = EaseSineOut::create(FadeIn::create(0.2f));
    Spawn *spawn = Spawn::create(sequence, easeSineOut_3, NULL);
    
    Background_Photo_popup_1->stopAllActions();
    Background_Photo_popup_1->runAction(spawn);

}

void PixSelectedPhotoAmountLayer::hideLayerAction()
{
    mbln_isAction = true;
    hideLayerActionFinish(nullptr);
}

void PixSelectedPhotoAmountLayer::showLayerActionFinish(Node* node)
{
    mbln_isAction = false;
}

void PixSelectedPhotoAmountLayer::hideLayerActionFinish(Node* node)
{
    mbln_isAction = false;

    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixSelectedPhotoAmountLayer::androidBackPressed()
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
