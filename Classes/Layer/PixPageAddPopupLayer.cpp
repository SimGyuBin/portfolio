//
//  PixPageAddPopupLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 8. 3..
//
//

#include "PixPageAddPopupLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPageAddPopupLayer* PixPageAddPopupLayer::create()
{
    PixPageAddPopupLayer *pageAddPopupLayer = new (std::nothrow) PixPageAddPopupLayer();
    if (pageAddPopupLayer && pageAddPopupLayer->init())
    {
        pageAddPopupLayer->autorelease();
        return pageAddPopupLayer;
    }
    else
    {
        CC_SAFE_DELETE(pageAddPopupLayer);
        return nullptr;
    }
}

PixPageAddPopupLayer::PixPageAddPopupLayer()
: rootNode(nullptr)
, Button_cancel(nullptr)
, pre(nullptr)
, next(nullptr)
, end(nullptr)
, Button_Complete(nullptr)
, mDelegate(nullptr)
{
    hideLayerFunction = std::bind(&PixPageAddPopupLayer::hideLayerAction,this);
}

PixPageAddPopupLayer::~PixPageAddPopupLayer()
{
    
}

void PixPageAddPopupLayer::initLayer()
{
    rootNode = CSLoader::createNode("05_AlbumView/AddPage_PopupLayer.csb");
    
    if(rootNode)
    {
        Node *Node_AddPage_Popup = dynamic_cast<Node*>(rootNode->getChildByName("Node_AddPage_Popup"));
        
        if(Node_AddPage_Popup)
        {
            Node *Node_AddPage_PopTopLayer = dynamic_cast<Node*>(Node_AddPage_Popup->getChildByName("Node_AddPage_PopTopLayer"));
            
            if(Node_AddPage_PopTopLayer)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Node_AddPage_PopTopLayer->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                Button_cancel = dynamic_cast<ui::Button*>(Node_AddPage_PopTopLayer->getChildByName("Button_cancel"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixPageAddPopupLayer::ButtonCancelPressed, this));
                }
            }
            
            pre = dynamic_cast<ui::CheckBox*>(Node_AddPage_Popup->getChildByName("CheckBox_2"));
            
            if(pre)
            {
                pre->setSelected(false);
                pre->addEventListener(CC_CALLBACK_2(PixPageAddPopupLayer::checkBoxPressed, this));
                
                ui::Text *Text = dynamic_cast<ui::Text*>(pre->getChildByName("Text"));
                
                if(Text)
                {
                    Text->setString(CODE_NAME(Text->getString()));
                }
            }
            
            next = dynamic_cast<ui::CheckBox*>(Node_AddPage_Popup->getChildByName("CheckBox"));
            
            if(next)
            {
                next->setSelected(true);
                next->addEventListener(CC_CALLBACK_2(PixPageAddPopupLayer::checkBoxPressed, this));
                
                ui::Text *Text = dynamic_cast<ui::Text*>(next->getChildByName("Text"));
                
                if(Text)
                {
                    Text->setString(CODE_NAME(Text->getString()));
                }
            }
            
            end = dynamic_cast<ui::CheckBox*>(Node_AddPage_Popup->getChildByName("CheckBox_1"));
            
            if(end)
            {
                end->setSelected(false);
                end->addEventListener(CC_CALLBACK_2(PixPageAddPopupLayer::checkBoxPressed, this));
                
                ui::Text *Text = dynamic_cast<ui::Text*>(end->getChildByName("Text"));
                
                if(Text)
                {
                    Text->setString(CODE_NAME(Text->getString()));
                }
            }
            
            Button_Complete = dynamic_cast<ui::Button*>(Node_AddPage_Popup->getChildByName("Button_Complete"));
            
            if(Button_Complete)
            {
                Button_Complete->addTouchEventListener(CC_CALLBACK_2(PixPageAddPopupLayer::ButtonCompletePressed, this));
                
                ui::Text *Text = dynamic_cast<ui::Text*>(Button_Complete->getChildByName("Text"));
                
                if(Text)
                {
                    Text->setString(CODE_NAME(Text->getString()));
                }
            }
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
}

void PixPageAddPopupLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixPageAddPopupLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixPageAddPopupLayer::ButtonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        hideLayerAction();
    }
}

void PixPageAddPopupLayer::ButtonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(pre)
        {
            if(pre->isSelected())
            {
                if(mDelegate)
                {
                    mDelegate->addPrePage();
                }
            }
        }
        
        if(next)
        {
            if(next->isSelected())
            {
                if(mDelegate)
                {
                    mDelegate->addNextPage();
                }
            }
        }
        
        if(end)
        {
            if(end->isSelected())
            {
                if(mDelegate)
                {
                    mDelegate->addEndPage();
                }
            }
        }
        
        hideLayerAction();
    }
}

void PixPageAddPopupLayer::checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        if(checkBox == pre)
        {
            if(checkBox->isSelected())
            {
                if(end)
                {
                    end->setSelected(false);
                }
                
                if(next)
                {
                    next->setSelected(false);
                }
            }
        }
        
        if(checkBox == next)
        {
            if(checkBox->isSelected())
            {
                if(end)
                {
                    end->setSelected(false);
                }
                
                if(pre)
                {
                    pre->setSelected(false);
                }
            }
        }
        
        if(checkBox == end)
        {
            if(checkBox->isSelected())
            {
                if(next)
                {
                    next->setSelected(false);
                }
                
                if(pre)
                {
                    pre->setSelected(false);
                }
            }
        }
    }
}

#pragma mark - Android BackPressed

void PixPageAddPopupLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    setAndroidBackPressedAction(true);

    hideLayerAction();
}
