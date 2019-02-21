//
//  PixCouponPopupEmptyCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#include "PixCouponPopupEmptyCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixCouponPopupEmptyCell* PixCouponPopupEmptyCell::create()
{
    PixCouponPopupEmptyCell* widget = new (std::nothrow) PixCouponPopupEmptyCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCouponPopupEmptyCell::PixCouponPopupEmptyCell()
: _customItem(nullptr)
{
    
}

PixCouponPopupEmptyCell::~PixCouponPopupEmptyCell()
{
    
}

bool PixCouponPopupEmptyCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCouponPopupEmptyCell::onEnter()
{
    Widget::onEnter();
}

void PixCouponPopupEmptyCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPaymen_PopItem_05.csb");
    
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





