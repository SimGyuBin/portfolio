//
//  PixCouponPopupTitleCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#include "PixCouponPopupTitleCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixCouponPopupTitleCell* PixCouponPopupTitleCell::create(int nCount)
{
    PixCouponPopupTitleCell* widget = new (std::nothrow) PixCouponPopupTitleCell(nCount);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;

}

PixCouponPopupTitleCell::PixCouponPopupTitleCell(int nCount)
: _customItem(nullptr)
, mn_Count(nCount)
{
    
}

PixCouponPopupTitleCell::~PixCouponPopupTitleCell()
{
    
}

bool PixCouponPopupTitleCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCouponPopupTitleCell::onEnter()
{
    Widget::onEnter();
}

void PixCouponPopupTitleCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPaymen_PopItem_02.csb");
    
    if(_customItem)
    {
        ui::Text *AvailableCoupon = dynamic_cast<ui::Text*>(_customItem->getChildByName("AvailableCoupon"));
        
        if(AvailableCoupon)
        {
            AvailableCoupon->setString(StringUtils::format(CODE_NAME(AvailableCoupon->getString()),mn_Count));
        }

        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}
