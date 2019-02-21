//
//  PixMenuCustomerLayer_FAQ_BottomCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 10..
//
//

#include "PixMenuCustomerLayer_FAQ_BottomCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuCustomerLayer_FAQ_BottomCell* PixMenuCustomerLayer_FAQ_BottomCell::create()
{
    PixMenuCustomerLayer_FAQ_BottomCell* widget = new (std::nothrow) PixMenuCustomerLayer_FAQ_BottomCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixMenuCustomerLayer_FAQ_BottomCell::PixMenuCustomerLayer_FAQ_BottomCell()
: _customItem(nullptr)
{
    
}

PixMenuCustomerLayer_FAQ_BottomCell::~PixMenuCustomerLayer_FAQ_BottomCell()
{
    
}

bool PixMenuCustomerLayer_FAQ_BottomCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixMenuCustomerLayer_FAQ_BottomCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixMenuCustomerLayer_FAQ_BottomCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("11_Menu/MenuCustomerLayer_FAQ_item_2.csb");
    
    if(_customItem)
    {
        ui::Text *Text_Email = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Email"));
        
        if(Text_Email)
        {
            Text_Email->setString(CODE_NAME(Text_Email->getString()));
        }
        
        ui::Text *Text_PhoneNumber = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_PhoneNumber"));
        
        if(Text_PhoneNumber)
        {
            Text_PhoneNumber->setString(CODE_NAME(Text_PhoneNumber->getString()));
        }
        
        ui::Text *Text_Infor = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Infor"));
        
        if(Text_Infor)
        {
            Text_Infor->setString(CODE_NAME(Text_Infor->getString()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

