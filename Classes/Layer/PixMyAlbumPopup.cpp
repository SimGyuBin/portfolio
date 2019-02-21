//
//  PixMyAlbumPopup.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 20..
//
//

#include "PixMyAlbumPopup.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"
#include "../Util/StackBlur_Host.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMyAlbumPopup* PixMyAlbumPopup::create()
{
    PixMyAlbumPopup *myAlbumPopup = new (std::nothrow) PixMyAlbumPopup();
    if (myAlbumPopup && myAlbumPopup->init())
    {
        myAlbumPopup->autorelease();
        return myAlbumPopup;
    }
    else
    {
        CC_SAFE_DELETE(myAlbumPopup);
        return nullptr;
    }
}

PixMyAlbumPopup::PixMyAlbumPopup()
: rootNode(nullptr)
, blockout(nullptr)
, blur(nullptr)
, blurTexture(nullptr)
, MyAlbum_Popup(nullptr)
, Background_my_popup(nullptr)
, Title(nullptr)
, SkinChange(nullptr)
, Remove(nullptr)
, cancel(nullptr)
, mbln_IsAction(false)
, captureImage(nullptr)
, mDelegate(nullptr)
{
    hideLayerFunction = std::bind(&PixMyAlbumPopup::hideLayerAction,this);
}

PixMyAlbumPopup::~PixMyAlbumPopup()
{
    if(blurTexture)
    {
        Director::getInstance()->getTextureCache()->removeTexture(blurTexture);
    }
}

void PixMyAlbumPopup::initLayer()
{
    rootNode = CSLoader::createNode("07_My_AlbumList/MyAlbum_PopupLayer.csb");
    
    if(rootNode)
    {
        blockout = dynamic_cast<Sprite*>(rootNode->getChildByName("blockout"));
        
        if(blockout)
        {
            blockout->setOpacity(0);
        }
        
        MyAlbum_Popup = dynamic_cast<Node*>(rootNode->getChildByName("MyAlbum_Popup"));
        
        if(MyAlbum_Popup)
        {
            Background_my_popup = dynamic_cast<Sprite*>(MyAlbum_Popup->getChildByName("Background_my_popup"));

            if(Background_my_popup)
            {
                Background_my_popup->setOpacity(0);
                
                Title = dynamic_cast<ui::Button*>(Background_my_popup->getChildByName("Title"));
                
                if(Title)
                {
                    Title->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumPopup::editTitlePressed, this));
                    
                    ui::Text *text = dynamic_cast<ui::Text*>(Title->getChildByName("Text_title"));
                    
                    if(text)
                    {
                        text->setString(CODE_NAME(text->getString()));
                    }
                }
                
                SkinChange = dynamic_cast<ui::Button*>(Background_my_popup->getChildByName("SkinChange"));
                
                if(SkinChange)
                {
                    SkinChange->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumPopup::skinChangePressed, this));
                    
                    ui::Text *text = dynamic_cast<ui::Text*>(SkinChange->getChildByName("Text_skin"));
                    
                    if(text)
                    {
                        text->setString(CODE_NAME(text->getString()));
                    }
                }
                
                Remove = dynamic_cast<ui::Button*>(Background_my_popup->getChildByName("Remove"));
                
                if(Remove)
                {
                    Remove->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumPopup::removePressed, this));
                    
                    ui::Text *text = dynamic_cast<ui::Text*>(Remove->getChildByName("Text_remove"));
                    
                    if(text)
                    {
                        text->setString(CODE_NAME(text->getString()));
                    }
                }
                
                cancel = dynamic_cast<ui::Button*>(Background_my_popup->getChildByName("cancel"));
                
                if(cancel)
                {
                    cancel->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumPopup::cancelPressed, this));
                    
                    ui::Text *text = dynamic_cast<ui::Text*>(cancel->getChildByName("Text_cencel"));
                    
                    if(text)
                    {
                        text->setString(CODE_NAME(text->getString()));
                    }
                }
            }
        }
    }
    
    setAnchorPoint(Vec2::ZERO);
    addChild(rootNode);
    
    captureImage = utils::captureNode(Director::getInstance()->getRunningScene());
    
    auto t = std::thread(&PixMyAlbumPopup::threadStack_blur_rgba32, this, captureImage);
    t.detach();
}

void PixMyAlbumPopup::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixMyAlbumPopup::threadStack_blur_rgba32(Image *pImage)
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

void PixMyAlbumPopup::createblur()
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
    
    showLayerAction();

}

#pragma mark - Button

void PixMyAlbumPopup::editTitlePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(mDelegate)
        {
            mDelegate->editMyAlbumTitle();
        }
        
        hideLayerAction();
    }
}

void PixMyAlbumPopup::skinChangePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(mDelegate)
        {
            mDelegate->skinChangeMyAlbum();
        }
        
        hideLayerAction();
    }
}

void PixMyAlbumPopup::removePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        if(mDelegate)
        {
            mDelegate->removeMyAlbum();
        }

        hideLayerAction();
    }
}

void PixMyAlbumPopup::cancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {        
        hideLayerAction();
    }
}

#pragma mark - Touch & Touches

void PixMyAlbumPopup::onTouchEnded(Touch* touch, Event* event)
{
//    if(MyAlbum_Popup && mbln_IsAction == false)
//    {
//        if(NodeContainsPoint(this, touch->getLocation()))
//        {
//            if(!NodeContainsPoint(MyAlbum_Popup, touch->getLocation()))
//            {
//                hideLayerAction();
//            }
//        }
//    }
}

#pragma mark - Action

void PixMyAlbumPopup::showLayerAction()
{
    mbln_IsAction = true;
    
    EaseSineOut *easeSineOut = EaseSineOut::create(FadeIn::create(0.2f));
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMyAlbumPopup::showLayerActionFinish, this));
    
    blockout->runAction(Sequence::create(easeSineOut, callFuncN, NULL));

    blur->runAction(easeSineOut->clone());
    
    ScaleTo *scaleTo_1 = ScaleTo::create(0.1f, 1.1f);
    EaseSineOut *easeSineOut_1 = EaseSineOut::create(scaleTo_1);
    
    ScaleTo *scaleTo_2 = ScaleTo::create(0.1f, 1.0f);
    EaseSineOut *easeSineOut_2 = EaseSineOut::create(scaleTo_2);
    
    Sequence *sequence = Sequence::create(easeSineOut_1, easeSineOut_2, NULL);
    EaseSineOut *easeSineOut_3 = EaseSineOut::create(FadeIn::create(0.2f));
    Spawn *spawn = Spawn::create(sequence, easeSineOut_3, NULL);
        
    Background_my_popup->stopAllActions();
    Background_my_popup->runAction(spawn);
}

void PixMyAlbumPopup::hideLayerAction()
{
    mbln_IsAction = true;
    
    hideLayerActionFinish(nullptr);
}

void PixMyAlbumPopup::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
}

void PixMyAlbumPopup::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixMyAlbumPopup::androidBackPressed()
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
