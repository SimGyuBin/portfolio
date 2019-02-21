//
//  PixPaymentTotalCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 14..
//
//

#include "PixPaymentTotalCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"
#include "../Util/PixStringUtils.h"
#include "../Info/CartOrderTotalInfo.h"

PixPaymentTotalCell* PixPaymentTotalCell::create(CartOrderTotalInfo *pCartOrderTotalInfo)
{
    PixPaymentTotalCell* widget = new (std::nothrow) PixPaymentTotalCell(pCartOrderTotalInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPaymentTotalCell::PixPaymentTotalCell(CartOrderTotalInfo *pCartOrderTotalInfo)
: _customItem(nullptr)
, mCartOrderTotalInfo(pCartOrderTotalInfo)
, pay_1(nullptr)
, pay_2(nullptr)
, pay_3(nullptr)
, Total_pay(nullptr)
, mstr_Cart07("")
, mstr_Cart10("")
{
  
}

PixPaymentTotalCell::~PixPaymentTotalCell()
{
    
}

bool PixPaymentTotalCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPaymentTotalCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}

void PixPaymentTotalCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixPaymentTotalCell::onExit()
{
    unscheduleUpdate();
    Widget::onExit();
}

void PixPaymentTotalCell::update(float dt)
{
    Widget::update(dt);
    updatePaymentTotal();
}

void PixPaymentTotalCell::createCustomItem()
{
    if(mCartOrderTotalInfo == nullptr)
        return;
    
    _customItem = CSLoader::createNode("12_Oder/MenuPaymentItem_03.csb");
    
    if(_customItem)
    {
        mstr_Cart07 = CODE_NAME("Cart07");
        mstr_Cart10 = CODE_NAME("Cart10");
        
        ui::Text *OrderTotal = dynamic_cast<ui::Text*>(_customItem->getChildByName("OrderTotal"));
        
        if(OrderTotal)
        {
            OrderTotal->setString(CODE_NAME(OrderTotal->getString()));
        }
        
        ui::Text *Item = dynamic_cast<ui::Text*>(_customItem->getChildByName("Item"));
        
        if(Item)
        {
            Item->setString(CODE_NAME(Item->getString()));
        }
        
        ui::Text *Discounted = dynamic_cast<ui::Text*>(_customItem->getChildByName("Discounted"));
        
        if(Discounted)
        {
            Discounted->setString(CODE_NAME(Discounted->getString()));
        }
        
        ui::Text *Delivery = dynamic_cast<ui::Text*>(_customItem->getChildByName("Delivery"));
        
        if(Delivery)
        {
            Delivery->setString(CODE_NAME(Delivery->getString()));
        }
        
        ui::Text *Payment = dynamic_cast<ui::Text*>(_customItem->getChildByName("Payment"));
        
        if(Payment)
        {
            Payment->setString(CODE_NAME(Payment->getString()));
        }
        
        pay_1 = dynamic_cast<ui::Text*>(_customItem->getChildByName("pay_1"));
        
        if(pay_1)
        {
            pay_1->setString(StringUtils::format(mstr_Cart07.c_str(),"0"));
        }
        
        pay_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("pay_2"));
        
        if(pay_2)
        {
            pay_2->setString(StringUtils::format(mstr_Cart10.c_str(),"0"));
        }
        
        pay_3 = dynamic_cast<ui::Text*>(_customItem->getChildByName("pay_3"));
        
        if(pay_3)
        {
            pay_3->setString(StringUtils::format(mstr_Cart07.c_str(),"0"));
        }
        
        Total_pay = dynamic_cast<ui::Text*>(_customItem->getChildByName("Total_pay"));
        
        if(Total_pay)
        {
            Total_pay->setString(StringUtils::format(mstr_Cart07.c_str(),"0"));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixPaymentTotalCell::updatePaymentTotal()
{
    if(mCartOrderTotalInfo == nullptr)
        return;
    
    if(pay_1)
    {
        std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getTotal_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        pay_1->setString(strText);
    }
    
    if(pay_2)
    {
        std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getTotal_discount_price());
        std::string strText = StringUtils::format(mstr_Cart10.c_str(), strMoney.c_str());
        pay_2->setString(strText);
    }
    
    if(pay_3)
    {
        std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getShip_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        pay_3->setString(strText);
    }
    
    if(Total_pay)
    {
        std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getTotal_pay_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        Total_pay->setString(strText);
    }
}
