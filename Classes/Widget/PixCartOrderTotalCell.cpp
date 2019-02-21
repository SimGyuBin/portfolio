//
//  PixCartOrderTotalCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 9..
//
//

#include "PixCartOrderTotalCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"
#include "../Util/PixStringUtils.h"
#include "../Info/CartOrderTotalInfo.h"

PixCartOrderTotalCell* PixCartOrderTotalCell::create(CartOrderTotalInfo *pCartOrderTotalInfo)
{
    PixCartOrderTotalCell* widget = new (std::nothrow) PixCartOrderTotalCell(pCartOrderTotalInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCartOrderTotalCell::PixCartOrderTotalCell(CartOrderTotalInfo *pCartOrderTotalInfo)
: _customItem(nullptr)
, mCartOrderTotalInfo(pCartOrderTotalInfo)
, OrderTotal(nullptr)
, Item_Amount(nullptr)
, Text_event(nullptr)
, ImageView_Price(nullptr)
, Discounted(nullptr)
, Delivery(nullptr)
, pay1(nullptr)
, Text_Pay(nullptr)
, pay3(nullptr)
, Payment(nullptr)
, Total_pay(nullptr)
, mstr_Cart07("")
, mstr_Cart10("")
{
    
}

PixCartOrderTotalCell::~PixCartOrderTotalCell()
{

}

bool PixCartOrderTotalCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCartOrderTotalCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}

void PixCartOrderTotalCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixCartOrderTotalCell::onExit()
{
    unscheduleUpdate();
    Widget::onExit();
}

void PixCartOrderTotalCell::update(float dt)
{
    Widget::update(dt);
    updateOrderTotal();
}

void PixCartOrderTotalCell::createCustomItem()
{
    if(mCartOrderTotalInfo == nullptr)
        return;
    
    _customItem = CSLOADER_MANAGER->getCustomItem("12_Oder/MenuCartItem_03.csb");
    
    if(_customItem)
    {
        mstr_Cart07 = CODE_NAME("Cart07");
        mstr_Cart10 = CODE_NAME("Cart10");
        
        OrderTotal = dynamic_cast<ui::Text*>(_customItem->getChildByName("OrderTotal"));
        
        if(OrderTotal)
        {
            OrderTotal->setString(CODE_NAME(OrderTotal->getString()));
        }
        
        Item_Amount = dynamic_cast<ui::Text*>(_customItem->getChildByName("01_Item Amount"));
        
        if(Item_Amount)
        {
            Item_Amount->setString(CODE_NAME(Item_Amount->getString()));
        }
        
        Text_event = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_event"));
        
        if(Text_event)
        {
            Text_event->setString(CODE_NAME(Text_event->getString()));
        }
        
        ImageView_Price = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("ImageView_Price"));
        
        if(ImageView_Price)
        {
            
        }
        
        Discounted = dynamic_cast<ui::Text*>(_customItem->getChildByName("02_Discounted"));
        
        if(Discounted)
        {
            Discounted->setString(CODE_NAME(Discounted->getString()));
        }
        
        Delivery = dynamic_cast<ui::Text*>(_customItem->getChildByName("03_Delivery"));
        
        if(Delivery)
        {
            Delivery->setString(CODE_NAME(Delivery->getString()));
        }
        
        pay1 = dynamic_cast<ui::Text*>(_customItem->getChildByName("01_pay"));
        
        if(pay1)
        {
            int nMoney = mCartOrderTotalInfo->getTotal_price() - mCartOrderTotalInfo->getTotal_discount_price();
            std::string strMoney = PixStringUtils::commaMoney(nMoney);
            std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
            pay1->setString(strText);
        }
        
        Text_Pay = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Pay"));
        
        if(Text_Pay)
        {
            std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getTotal_price());
            std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
            Text_Pay->setString(strText);
        }
        
        pay3 = dynamic_cast<ui::Text*>(_customItem->getChildByName("03_pay"));
        
        if(pay3)
        {
            std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getShip_price());
            std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
            pay3->setString(strText);
        }
        
        Payment = dynamic_cast<ui::Text*>(_customItem->getChildByName("Payment"));
        
        if(Payment)
        {
            Payment->setString(CODE_NAME(Payment->getString()));
        }
        
        Total_pay = dynamic_cast<ui::Text*>(_customItem->getChildByName("Total_pay"));
        
        if(Total_pay)
        {
            std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getTotal_pay_price());
            std::string strText = StringUtils::format(mstr_Cart07.c_str(),strMoney.c_str());
            Total_pay->setString(strText);
        }
        
        ui::Text *Text_Coupon = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Coupon"));
        
        if(Text_Coupon)
        {
            Text_Coupon->setString(CODE_NAME(Text_Coupon->getString()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixCartOrderTotalCell::updateOrderTotal()
{
    if(mCartOrderTotalInfo == nullptr)
        return;
    
    Size size = Size::ZERO;
    Vec2 pos = Vec2::ZERO;
    
    if(pay1)
    {
        int nMoney = mCartOrderTotalInfo->getTotal_price() - mCartOrderTotalInfo->getTotal_discount_price();
        std::string strMoney = PixStringUtils::commaMoney(nMoney);
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        pay1->setString(strText);
        
        
        size = pay1->getContentSize();
        pos = pay1->getPosition();
    }
    
    if(Text_Pay)
    {
        std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getTotal_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        Text_Pay->setString(strText);
        
        float fPosX = pos.x - size.width - 15.0f;
        Text_Pay->setPositionX(fPosX);
        
        if(ImageView_Price)
        {
            float fMargin = 10.0f;
            
            Size size = Text_Pay->getContentSize();
            Vec2 pos = Text_Pay->getPosition();
            
            ImageView_Price->setContentSize(Size(size.width + fMargin,
                                                 ImageView_Price->getContentSize().height));
            
            ImageView_Price->setPosition(Vec2((pos.x + (fMargin * 0.5f)),
                                              pos.y - (size.height * 0.5f)));
        }
        
        size = Text_Pay->getContentSize();
        pos = Text_Pay->getPosition();

    }
    
    if(Text_event)
    {
        float fPosX = pos.x - size.width - 15.0f;
        Text_event->setPositionX(fPosX);
    }
    
    if(pay3)
    {
        std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getShip_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        pay3->setString(strText);
    }
    
    if(Total_pay)
    {
        std::string strMoney = PixStringUtils::commaMoney(mCartOrderTotalInfo->getTotal_pay_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(),strMoney.c_str());
        Total_pay->setString(strText);
    }
}

void PixCartOrderTotalCell::runDeleteAction(const std::function<void(ui::Widget*)>& callback)
{
    mCallback = callback;
    
    Vec2 movePos = Vec2(getPositionX() + getContentSize().width,getPositionY());
    MoveTo *moveTo = MoveTo::create(0.25f,movePos);
    EaseSineIn *easeSineIn = EaseSineIn::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixCartOrderTotalCell::deleteActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineIn, callFuncN, NULL);
    runAction(sequence);
}

void PixCartOrderTotalCell::deleteActionFinish()
{
    if(mCallback)
    {
        (mCallback)(this);
        mCallback = nullptr;
    }
}
