//
//  PixPayBuyerInfoCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 20..
//
//

#include "PixPayBuyerInfoCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

PixPayBuyerInfoCell* PixPayBuyerInfoCell::create()
{
    PixPayBuyerInfoCell* widget = new (std::nothrow) PixPayBuyerInfoCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPayBuyerInfoCell::PixPayBuyerInfoCell()
: _customItem(nullptr)
{
    
}

PixPayBuyerInfoCell::~PixPayBuyerInfoCell()
{
    
}

bool PixPayBuyerInfoCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPayBuyerInfoCell::onEnter()
{
    Widget::onEnter();
}

void PixPayBuyerInfoCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPayItem_01.csb");
    
    if(_customItem)
    {
        ui::Text *Text_Title = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Title"));
        
        if(Text_Title)
        {
            Text_Title->setString(CODE_NAME(Text_Title->getString()));
        }
        
        Sprite *Name = dynamic_cast<Sprite*>(_customItem->getChildByName("Name"));
        
        if(Name)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Name->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Name->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(PIXAPP->userInfo->getName());
            }
        }
        
        Sprite *Phone = dynamic_cast<Sprite*>(_customItem->getChildByName("Phone"));
        
        if(Phone)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Phone->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Phone->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(PIXAPP->userInfo->getPhoneNumber());
            }
        }

        Sprite *Email = dynamic_cast<Sprite*>(_customItem->getChildByName("Email"));
        
        if(Email)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Email->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Email->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(PIXAPP->userInfo->getEmail());
            }
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}
