//
//  PixCouponPopupEventDiscountCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#include "PixCouponPopupEventDiscountCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixStringUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixCouponPopupEventDiscountCell* PixCouponPopupEventDiscountCell::create(int nEventDiscount, bool blnCheckEnabled)
{
    PixCouponPopupEventDiscountCell* widget = new (std::nothrow) PixCouponPopupEventDiscountCell(nEventDiscount, blnCheckEnabled);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCouponPopupEventDiscountCell::PixCouponPopupEventDiscountCell(int nEventDiscount, bool blnCheckEnabled)
: _customItem(nullptr)
, check(nullptr)
, mn_EventDiscount(nEventDiscount)
, mbln_CheckEnabled(blnCheckEnabled)
, mDelegate(nullptr)
{
    
}

PixCouponPopupEventDiscountCell::~PixCouponPopupEventDiscountCell()
{
    
}

bool PixCouponPopupEventDiscountCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCouponPopupEventDiscountCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}


void PixCouponPopupEventDiscountCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixCouponPopupEventDiscountCell::onExit()
{
    unscheduleUpdate();
    Widget::onExit();
}

void PixCouponPopupEventDiscountCell::update(float dt)
{
    Widget::update(dt);
    
    if(check)
    {
        check->setEnabled(getCheckEnabled());
    }
}

void PixCouponPopupEventDiscountCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPaymen_PopItem_01.csb");
    
    if(_customItem)
    {
        check = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("check"));
        
        if(check)
        {
            check->addEventListener(CC_CALLBACK_2(PixCouponPopupEventDiscountCell::checkBoxPressed, this));
            
            if(mn_EventDiscount > 0)
            {
                setCheckSelected(true);
            }
        }
        
        ui::Text *Event = dynamic_cast<ui::Text*>(_customItem->getChildByName("Event"));
        
        if(Event)
        {
            Event->setString(CODE_NAME(Event->getString()));
        }
        
        ui::Text *pay = dynamic_cast<ui::Text*>(_customItem->getChildByName("pay"));
        
        if(pay)
        {
            std::string strMoney = PixStringUtils::commaMoney(mn_EventDiscount);
            std::string strText = StringUtils::format(CODE_NAME(pay->getString()), strMoney.c_str());
            pay->setString(strText);
        }

        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixCouponPopupEventDiscountCell::setCheckSelected(bool var)
{
    if(check)
    {
        check->setSelected(var);
    }
}

bool PixCouponPopupEventDiscountCell::isCheckSelected()
{
    if(check)
    {
        return check->isSelected();
    }
    
    return false;
}

#pragma mark - Button

void PixCouponPopupEventDiscountCell::checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
       if(mDelegate)
       {
           mDelegate->couponPopupEventDiscountCellCheckBoxPressed(checkBox->isSelected());
       }
    }
}

