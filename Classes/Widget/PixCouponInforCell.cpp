//
//  PixCouponInforCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 7..
//
//

#include "PixCouponInforCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixCouponInforCell* PixCouponInforCell::create()
{
    PixCouponInforCell* widget = new (std::nothrow) PixCouponInforCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCouponInforCell::PixCouponInforCell()
: _customItem(nullptr)
{
    
}

PixCouponInforCell::~PixCouponInforCell()
{
    
}

bool PixCouponInforCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCouponInforCell::onEnter()
{
    Widget::onEnter();
}

void PixCouponInforCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuCoupon_Infor.csb");
    
    if(_customItem)
    {
        ui::Text *Text_Notice = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Notice"));
        
        if(Text_Notice)
        {
            Text_Notice->setString(CODE_NAME(Text_Notice->getString()));
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



