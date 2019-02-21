//
//  PixPayFinalPaymentAmountCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 24..
//
//

#include "PixPayFinalPaymentAmountCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/CartOrderTotalInfo.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"
#include "../Util/PixStringUtils.h"
#include "../Layer/PixPurchaseAgreementLayer.h"

PixPayFinalPaymentAmountCell* PixPayFinalPaymentAmountCell::create(CartOrderTotalInfo *pFinalTotalInfo)
{
    PixPayFinalPaymentAmountCell* widget = new (std::nothrow) PixPayFinalPaymentAmountCell(pFinalTotalInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPayFinalPaymentAmountCell::PixPayFinalPaymentAmountCell(CartOrderTotalInfo *pFinalTotalInfo)
: _customItem(nullptr)
, mFinalTotalInfo(pFinalTotalInfo)
, Panel_None(nullptr)
, Panel_Add(nullptr)
, Text_pay_5(nullptr)
, Text_Delivery_infor(nullptr)
, check(nullptr)
, Button_agree(nullptr)
, me_DeliveryChargeType(DELIVERY_CHARGE_NONE)
, mstr_Cart07("")
, mstr_Cart10("")
{
    for (int i = 0; i < DELIVERY_CHARGE_COUNT; ++i)
    {
        Text_pay_1[i] = nullptr;
        Text_pay_2[i] = nullptr;
        Text_pay_3[i] = nullptr;
    }
}

PixPayFinalPaymentAmountCell::~PixPayFinalPaymentAmountCell()
{
    
}

bool PixPayFinalPaymentAmountCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPayFinalPaymentAmountCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}

void PixPayFinalPaymentAmountCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixPayFinalPaymentAmountCell::onExit()
{
    unscheduleUpdate();
    Widget::onExit();
}

void PixPayFinalPaymentAmountCell::update(float dt)
{
    Widget::update(dt);
    updatePayFinalPaymentAmount();
}

void PixPayFinalPaymentAmountCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPayItem_09.csb");
    
    if(_customItem)
    {
        mstr_Cart07 = CODE_NAME("Cart07");
        mstr_Cart10 = CODE_NAME("Cart10");
        
        ui::Text *Text_Title = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Title"));

        if(Text_Title)
        {
            Text_Title->setString(CODE_NAME(Text_Title->getString()));
        }
        
        Panel_None = dynamic_cast<ui::Layout*>(_customItem->getChildByName("Panel_1"));
        
        if(Panel_None)
        {
            ui::Text *Text_Item = dynamic_cast<ui::Text*>(Panel_None->getChildByName("Text_Item"));
            
            if(Text_Item)
            {
                Text_Item->setString(CODE_NAME(Text_Item->getString()));
            }
            
            ui::Text *Text_Discounted = dynamic_cast<ui::Text*>(Panel_None->getChildByName("Text_Discounted"));
            
            if(Text_Discounted)
            {
                Text_Discounted->setString(CODE_NAME(Text_Discounted->getString()));
            }
            
            ui::Text *Text_Delivery = dynamic_cast<ui::Text*>(Panel_None->getChildByName("Text_Delivery"));
            
            if(Text_Delivery)
            {
                Text_Delivery->setString(CODE_NAME(Text_Delivery->getString()));
            }
            
            ui::Text *Text = dynamic_cast<ui::Text*>(Panel_None->getChildByName("Text_pay_1"));
            
            if(Text)
            {
                Text_pay_1[DELIVERY_CHARGE_NONE] = Text;
            }
            
            Text = dynamic_cast<ui::Text*>(Panel_None->getChildByName("Text_pay_2"));
            
            if(Text)
            {
                Text_pay_2[DELIVERY_CHARGE_NONE] = Text;
            }
            
            Text = dynamic_cast<ui::Text*>(Panel_None->getChildByName("Text_pay_3"));
            
            if(Text)
            {
                Text_pay_3[DELIVERY_CHARGE_NONE] = Text;
            }
        }
        
        Panel_Add = dynamic_cast<ui::Layout*>(_customItem->getChildByName("Panel_2"));
        
        if(Panel_Add)
        {
            ui::Text *Text_Item = dynamic_cast<ui::Text*>(Panel_Add->getChildByName("Text_Item"));
            
            if(Text_Item)
            {
                Text_Item->setString(CODE_NAME(Text_Item->getString()));
            }
            
            ui::Text *Text_Discounted = dynamic_cast<ui::Text*>(Panel_Add->getChildByName("Text_Discounted"));
            
            if(Text_Discounted)
            {
                Text_Discounted->setString(CODE_NAME(Text_Discounted->getString()));
            }
            
            ui::Text *Text_Delivery = dynamic_cast<ui::Text*>(Panel_Add->getChildByName("Text_Delivery"));
            
            if(Text_Delivery)
            {
                Text_Delivery->setString(CODE_NAME(Text_Delivery->getString()));
            }
            
            ui::Text *Text_Delivery_2 = dynamic_cast<ui::Text*>(Panel_Add->getChildByName("Text_Delivery_2"));
            
            if(Text_Delivery_2)
            {
                Text_Delivery_2->setString(CODE_NAME(Text_Delivery_2->getString()));
            }
            
            Text_Delivery_infor = dynamic_cast<ui::Text*>(Panel_Add->getChildByName("Text_Delivery_infor"));
            
            if(Text_Delivery_infor)
            {
                Text_Delivery_infor->setString(CODE_NAME(Text_Delivery_infor->getString()));
            }
            
            ui::Text *Text = dynamic_cast<ui::Text*>(Panel_Add->getChildByName("Text_pay_1"));
            
            if(Text)
            {
                Text_pay_1[DELIVERY_CHARGE_ADD] = Text;
            }
            
            Text = dynamic_cast<ui::Text*>(Panel_Add->getChildByName("Text_pay_2"));
            
            if(Text)
            {
                Text_pay_2[DELIVERY_CHARGE_ADD] = Text;
            }
            
            Text = dynamic_cast<ui::Text*>(Panel_Add->getChildByName("Text_pay_3"));
            
            if(Text)
            {
                Text_pay_3[DELIVERY_CHARGE_ADD] = Text;
            }
        }

        ui::Text *Text_Payment = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Payment"));
        
        if(Text_Payment)
        {
            Text_Payment->setString(CODE_NAME(Text_Payment->getString()));
        }
        
        Text_pay_5 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_pay_5"));
        
        if(Text_pay_5)
        {
            
        }
        
        check = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("check"));
        
        if(check)
        {
            check->addEventListener(CC_CALLBACK_2(PixPayFinalPaymentAmountCell::checkBoxPressed, this));
        }
        
        ui::Text *Text = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text"));
        
        if(Text)
        {
            Text->setString(CODE_NAME(Text->getString()));
        }
        
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_agree"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixPayFinalPaymentAmountCell::buttonAgreePressed, this));
            
            Button_agree = PixScrollMenu::create(menuItem, nullptr);
            Button_agree->setPosition(pos);
            
            Button_agree->setContentSize(button->getContentSize());
            _customItem->addChild(Button_agree);
            
            button->removeFromParentAndCleanup(true);
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

bool PixPayFinalPaymentAmountCell::isAgreeSelected()
{
    if(check)
    {
        return check->isSelected();
    }
    
    return false;
}

void PixPayFinalPaymentAmountCell::updatePayFinalPaymentAmount()
{
    if(mFinalTotalInfo == nullptr)
        return;
    
    DELIVERY_CHARGE_TYPE eType = getDeliveryChargeType();
    
    if(eType == DELIVERY_CHARGE_ADD)
    {
        if(Panel_None)
        {
            Panel_None->setVisible(false);
        }
        
        if(Panel_Add)
        {
            Panel_Add->setVisible(true);
        }
    }
    else
    {
        if(Panel_None)
        {
            Panel_None->setVisible(true);
        }
        
        if(Panel_Add)
        {
            Panel_Add->setVisible(false);
        }
    }
    
    if(Text_pay_1[eType])
    {
        std::string strMoney = PixStringUtils::commaMoney(mFinalTotalInfo->getTotal_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        Text_pay_1[eType]->setString(strText);
    }
    
    if(Text_pay_2[eType])
    {
        std::string strMoney = PixStringUtils::commaMoney(mFinalTotalInfo->getTotal_discount_price());
        std::string strText = StringUtils::format(mstr_Cart10.c_str(), strMoney.c_str());
        Text_pay_2[eType]->setString(strText);
    }
    
    if(Text_pay_3[eType])
    {
        int nMoney = mFinalTotalInfo->getShip_price() + mFinalTotalInfo->getPay_add_ship_amount();
        std::string strMoney = PixStringUtils::commaMoney(nMoney);
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        Text_pay_3[eType]->setString(strText);
    }
  
    if(Text_pay_5)
    {
        std::string strMoney = PixStringUtils::commaMoney(mFinalTotalInfo->getTotal_pay_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(),strMoney.c_str());
        Text_pay_5->setString(strText);
    }
}

#pragma mark - Button

void PixPayFinalPaymentAmountCell::checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        
    }
}

void PixPayFinalPaymentAmountCell::buttonAgreePressed(cocos2d::Ref *pSender)
{
    Director::getInstance()->getRunningScene()->addChild(PixPurchaseAgreementLayer::create());
}
