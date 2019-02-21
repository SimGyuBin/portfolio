//
//  PixPurchaseCompletedTitleCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 5..
//
//

#include "PixPurchaseCompletedTitleCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPurchaseCompletedTitleCell* PixPurchaseCompletedTitleCell::create(const std::string& strOrder_id, const std::string& strReg_date)
{
    PixPurchaseCompletedTitleCell* widget = new (std::nothrow) PixPurchaseCompletedTitleCell(strOrder_id, strReg_date);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPurchaseCompletedTitleCell::PixPurchaseCompletedTitleCell(const std::string& strOrder_id, const std::string& strReg_date)
: _customItem(nullptr)
, mstr_Order_id(strOrder_id)
, mstr_Reg_date(strReg_date)
{
    
}

PixPurchaseCompletedTitleCell::~PixPurchaseCompletedTitleCell()
{
    
}

bool PixPurchaseCompletedTitleCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPurchaseCompletedTitleCell::onEnter()
{
    Widget::onEnter();
}

void PixPurchaseCompletedTitleCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuOrderCompleted_01.csb");
    
    if(_customItem)
    {
        ui::Text *Text_CompletetInfor = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_CompletetInfor"));
        
        if(Text_CompletetInfor)
        {
            Text_CompletetInfor->setString(CODE_NAME(Text_CompletetInfor->getString()));
        }
        
        ui::Text *Text_OrderNumber = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderNumber"));
        
        if(Text_OrderNumber)
        {
            Text_OrderNumber->setString(CODE_NAME(Text_OrderNumber->getString()));
        }
        
        ui::Text *Text_OrderNumber_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderNumber_2"));
        
        if(Text_OrderNumber_2)
        {
            Text_OrderNumber_2->setString(mstr_Order_id);
        }
        
        ui::Text *Text_OrderDate = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderDate"));
        
        if(Text_OrderDate)
        {
            Text_OrderDate->setString(CODE_NAME(Text_OrderDate->getString()));
        }
        
        ui::Text *Text_OrderDate_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderDate_2"));
        
        if(Text_OrderDate_2)
        {
            Text_OrderDate_2->setString(mstr_Reg_date);
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

