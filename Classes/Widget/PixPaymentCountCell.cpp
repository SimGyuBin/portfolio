//
//  PixPaymentCountCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 14..
//
//

#include "PixPaymentCountCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPaymentCountCell* PixPaymentCountCell::create(int nCount)
{
    PixPaymentCountCell* widget = new (std::nothrow) PixPaymentCountCell(nCount);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPaymentCountCell::PixPaymentCountCell(int nCount)
: _customItem(nullptr)
, mn_Count(nCount)
{
    
}

PixPaymentCountCell::~PixPaymentCountCell()
{
    
}

bool PixPaymentCountCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPaymentCountCell::onEnter()
{
    Widget::onEnter();
}

void PixPaymentCountCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPaymentItem_01.csb");
    
    if(_customItem)
    {
        ui::Text *OrderItem = dynamic_cast<ui::Text*>(_customItem->getChildByName("OrderItem"));
        
        if(OrderItem)
        {
            OrderItem->setString(CODE_NAME(OrderItem->getString()));
        }
        
        ui::Text *Number = dynamic_cast<ui::Text*>(_customItem->getChildByName("Number"));
        
        if(Number)
        {
            Number->setString(StringUtils::format("%d",getCount()));
            
            if(OrderItem)
            {
                Size size = OrderItem->getContentSize();
                Vec2 pos = OrderItem->getPosition();
                
                pos = Vec2((pos.x + size.width + 15.0f), pos.y);
                
                Number->setPosition(pos);
            }
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}
