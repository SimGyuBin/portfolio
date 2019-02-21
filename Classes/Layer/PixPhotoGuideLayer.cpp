//
//  PixPhotoGuideLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 11..
//
//

#include "PixPhotoGuideLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUserDefault.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPhotoGuideLayer* PixPhotoGuideLayer::create()
{
    PixPhotoGuideLayer *photoGuideLayer = new (std::nothrow) PixPhotoGuideLayer();
    if (photoGuideLayer && photoGuideLayer->init())
    {
        photoGuideLayer->autorelease();
        return photoGuideLayer;
    }
    else
    {
        CC_SAFE_DELETE(photoGuideLayer);
        return nullptr;
    }
}

PixPhotoGuideLayer::PixPhotoGuideLayer()
: rootNode(nullptr)
{
    hideLayerFunction = std::bind(&PixPhotoGuideLayer::hideLayerAction,this);
}

PixPhotoGuideLayer::~PixPhotoGuideLayer()
{
    
}

void PixPhotoGuideLayer::initLayer()
{
    rootNode = CSLoader::createNode("04_SelectPhoto/PhotoGuidePopLayer.csb");
    
    if(rootNode)
    {
        Node *Node_PhotoGuidePopLayer = dynamic_cast<Node*>(rootNode->getChildByName("Node_PhotoGuidePopLayer"));
        
        if(Node_PhotoGuidePopLayer)
        {
            ui::Text *Text = dynamic_cast<ui::Text*>(Node_PhotoGuidePopLayer->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            ui::Button *Button_close = dynamic_cast<ui::Button*>(Node_PhotoGuidePopLayer->getChildByName("Button_close"));
            
            if(Button_close)
            {
                Button_close->addTouchEventListener(CC_CALLBACK_2(PixPhotoGuideLayer::buttonClosePressed, this));
            }
        }
        
        setContentSize(rootNode->getContentSize());
        addChild(rootNode);
    }
}

void PixPhotoGuideLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixPhotoGuideLayer::hideLayerAction()
{
    this->removeFromParentAndCleanup(true);
}

#pragma mark - Button

void PixPhotoGuideLayer::buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(getParent())
        {
            PixUserDefault::SetPhotoGuide(false);
            hideLayerAction();
        }
    }
}

#pragma mark - Android BackPressed

void PixPhotoGuideLayer::androidBackPressed()
{
//    PixModalLayer::androidBackPressed();
//
//    if(getAndroidBackPressedAction() == false)
//    {
//        setAndroidBackPressedAction(true);
//
//        if(getParent())
//        {
//            this->removeFromParentAndCleanup(true);
//        }
//    }
}
