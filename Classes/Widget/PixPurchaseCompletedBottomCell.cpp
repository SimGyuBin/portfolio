//
//  PixPurchaseCompletedBottomCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 5..
//
//

#include "PixPurchaseCompletedBottomCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPurchaseCompletedBottomCell* PixPurchaseCompletedBottomCell::create()
{
    PixPurchaseCompletedBottomCell* widget = new (std::nothrow) PixPurchaseCompletedBottomCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;

}

PixPurchaseCompletedBottomCell::PixPurchaseCompletedBottomCell()
: _customItem(nullptr)
, Button_More(nullptr)
, mDelegate(nullptr)
{
    
}

PixPurchaseCompletedBottomCell::~PixPurchaseCompletedBottomCell()
{
    
}

bool PixPurchaseCompletedBottomCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPurchaseCompletedBottomCell::onEnter()
{
    Widget::onEnter();
}

void PixPurchaseCompletedBottomCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuOrderCompleted_04.csb");
    
    if(_customItem)
    {
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_More"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixPurchaseCompletedBottomCell::buttonMorePressed, this));
            
            Button_More = PixScrollMenu::create(menuItem, nullptr);
            Button_More->setPosition(pos);
            
            Button_More->setContentSize(button->getContentSize());
            _customItem->addChild(Button_More);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text_More"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
                
                ui::Text *clone =  dynamic_cast<ui::Text*>(Text->clone());
                
                if(clone)
                {
                    clone->setFontSize(Text->getFontSize());
                    clone->setTextColor(Text->getTextColor());
                    clone->setString(Text->getString());
                    menuItem->addChild(clone);
                }
            }
            
            button->removeFromParentAndCleanup(true);
        }
        
        ui::Text *Notice = dynamic_cast<ui::Text*>(_customItem->getChildByName("Notice"));
        
        if(Notice)
        {
            Notice->setString(CODE_NAME(Notice->getString()));
        }
        
        ui::Text *Text_notice = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_notice"));
        
        if(Text_notice)
        {
            Text_notice->setString(CODE_NAME(Text_notice->getString()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

#pragma mark - Button

void PixPurchaseCompletedBottomCell::buttonMorePressed(cocos2d::Ref *pSender)
{
    if(mDelegate)
    {
        mDelegate->purchaseCompletedBottomCellMoerPressed();
    }
}
