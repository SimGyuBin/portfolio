//
//  PixPurchaseFailedBottomCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 4..
//
//

#include "PixPurchaseFailedBottomCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPurchaseFailedBottomCell* PixPurchaseFailedBottomCell::create()
{
    PixPurchaseFailedBottomCell* widget = new (std::nothrow) PixPurchaseFailedBottomCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;

}

PixPurchaseFailedBottomCell::PixPurchaseFailedBottomCell()
: _customItem(nullptr)
{
    
}

PixPurchaseFailedBottomCell::~PixPurchaseFailedBottomCell()
{
    
}

bool PixPurchaseFailedBottomCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPurchaseFailedBottomCell::onEnter()
{
    Widget::onEnter();
}

void PixPurchaseFailedBottomCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuOrderFailedItem_02.csb");
    
    if(_customItem)
    {
        ui::Text *Notice = dynamic_cast<ui::Text*>(_customItem->getChildByName("Notice"));
        
        if (Notice)
        {
            Notice->setString(CODE_NAME(Notice->getString()));
        }
        
        ui::Text *Text_1 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_1"));
        
        if (Text_1)
        {
            Text_1->setString(CODE_NAME(Text_1->getString()));
        }
        
        ui::Text *Text_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_2"));
        
        if (Text_2)
        {
            Text_2->setString(CODE_NAME(Text_2->getString()));
        }
        
        ui::Text *Text_3 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_3"));
        
        if (Text_3)
        {
            Text_3->setString(CODE_NAME(Text_3->getString()));
        }
        
        ui::Text *Text_4 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_4"));
        
        if (Text_4)
        {
            Text_4->setString(CODE_NAME(Text_4->getString()));
        }
        
        ui::Text *Text_5 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_5"));
        
        if (Text_5)
        {
            Text_5->setString(CODE_NAME(Text_5->getString()));
        }
        
        ui::Text *Text_6 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_6"));
        
        if (Text_6)
        {
            Text_6->setString(CODE_NAME(Text_6->getString()));
        }
        
        ui::Text *Text_7 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_7"));
        
        if (Text_7)
        {
            Text_7->setString(CODE_NAME(Text_7->getString()));
        }
        
        ui::Text *Text_8 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_8"));
        
        if (Text_8)
        {
            Text_8->setString(CODE_NAME(Text_8->getString()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}
