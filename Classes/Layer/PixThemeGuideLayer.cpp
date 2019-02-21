//
//  PixThemeGuideLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 11..
//
//

#include "PixThemeGuideLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUserDefault.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixThemeGuideLayer* PixThemeGuideLayer::create()
{
    PixThemeGuideLayer *themeGuideLayer = new (std::nothrow) PixThemeGuideLayer();
    if (themeGuideLayer && themeGuideLayer->init())
    {
        themeGuideLayer->autorelease();
        return themeGuideLayer;
    }
    else
    {
        CC_SAFE_DELETE(themeGuideLayer);
        return nullptr;
    }
}

PixThemeGuideLayer::PixThemeGuideLayer()
: rootNode(nullptr)
{
    hideLayerFunction = std::bind(&PixThemeGuideLayer::hideLayerAction,this);
}

PixThemeGuideLayer::~PixThemeGuideLayer()
{
    
}

void PixThemeGuideLayer::initLayer()
{
    rootNode = CSLoader::createNode("03_Theme/ThemeGuidePopLayer.csb");
    
    if(rootNode)
    {
        Node *Node_ThemeGuidePop = dynamic_cast<Node*>(rootNode->getChildByName("Node_ThemeGuidePop"));
        
        if(Node_ThemeGuidePop)
        {
            ui::Layout *bottom = dynamic_cast<ui::Layout*>(Node_ThemeGuidePop->getChildByName("bottom"));
            
            if(bottom)
            {
                ui::Text *Text = dynamic_cast<ui::Text*>(bottom->getChildByName("Text"));
                
                if(Text)
                {
                    Text->setString(CODE_NAME(Text->getString()));
                }
                
                ui::Button *Button_close = dynamic_cast<ui::Button*>(bottom->getChildByName("Button_close"));
                
                if(Button_close)
                {
                    Button_close->addTouchEventListener(CC_CALLBACK_2(PixThemeGuideLayer::buttonClosePressed, this));
                }
            }
        }
        
        setContentSize(rootNode->getContentSize());
        addChild(rootNode);
    }
}

void PixThemeGuideLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixThemeGuideLayer::hideLayerAction()
{
    this->removeFromParentAndCleanup(true);
}

#pragma mark - Button

void PixThemeGuideLayer::buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(getParent())
        {
            PixUserDefault::SetThemeGuide(false);
            hideLayerAction();
        }
    }
}

#pragma mark - Android BackPressed

void PixThemeGuideLayer::androidBackPressed()
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
