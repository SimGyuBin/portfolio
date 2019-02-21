//
//  PixCouponTitleCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 7..
//
//

#include "PixCouponTitleCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixCouponTitleCell* PixCouponTitleCell::create(int nCount)
{
    PixCouponTitleCell* widget = new (std::nothrow) PixCouponTitleCell(nCount);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCouponTitleCell::PixCouponTitleCell(int nCount)
: _customItem(nullptr)
, mn_Count(nCount)
{
    
}

PixCouponTitleCell::~PixCouponTitleCell()
{
    
}

bool PixCouponTitleCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCouponTitleCell::onEnter()
{
    Widget::onEnter();
}

void PixCouponTitleCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuCouponItem_01.csb");
    
    if(_customItem)
    {
        ui::Text *CouponBox = dynamic_cast<ui::Text*>(_customItem->getChildByName("CouponBox"));
        
        if(CouponBox)
        {
            CouponBox->setString(CODE_NAME(CouponBox->getString()));
        }
        
        ui::Text *Number = dynamic_cast<ui::Text*>(_customItem->getChildByName("Number"));
        
        if(Number)
        {
            Number->setString(StringUtils::format("%d",getCount()));
            
            if(CouponBox)
            {
                Size size = CouponBox->getContentSize();
                Vec2 pos = CouponBox->getPosition();
                
                pos = Vec2((pos.x + size.width + 15.0f), pos.y);
                
                Number->setPosition(pos);
            }
        }

        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}






