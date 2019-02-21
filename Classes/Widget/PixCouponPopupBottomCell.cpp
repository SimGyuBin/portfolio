//
//  PixCouponPopupBottomCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#include "PixCouponPopupBottomCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixCouponPopupBottomCell* PixCouponPopupBottomCell::create()
{
    PixCouponPopupBottomCell* widget = new (std::nothrow) PixCouponPopupBottomCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCouponPopupBottomCell::PixCouponPopupBottomCell()
: _customItem(nullptr)
{
    
}

PixCouponPopupBottomCell::~PixCouponPopupBottomCell()
{
    
}

bool PixCouponPopupBottomCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCouponPopupBottomCell::onEnter()
{
    Widget::onEnter();
}

void PixCouponPopupBottomCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPaymen_PopItem_03.csb");
    
    if(_customItem)
    {
        ui::Text *CouponInfor = dynamic_cast<ui::Text*>(_customItem->getChildByName("CouponInfor"));
        
        if(CouponInfor)
        {
            CouponInfor->setString(CODE_NAME(CouponInfor->getString()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}



