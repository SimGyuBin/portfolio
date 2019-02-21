//
//  PixPurchaseCompletedInfoCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 5..
//
//

#include "PixPurchaseCompletedInfoCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

PixPurchaseCompletedInfoCell* PixPurchaseCompletedInfoCell::create(int nAmount, const std::string& strPay_method, const std::string& strPay_method_text, const std::string& strVbank_name, const std::string& strVbank_num, const std::string& strVbank_holder, const std::string& strVbank_date)
{
    PixPurchaseCompletedInfoCell* widget = new (std::nothrow) PixPurchaseCompletedInfoCell(nAmount, strPay_method, strPay_method_text, strVbank_name, strVbank_num, strVbank_holder, strVbank_date);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPurchaseCompletedInfoCell::PixPurchaseCompletedInfoCell(int nAmount, const std::string& strPay_method, const std::string& strPay_method_text, const std::string& strVbank_name, const std::string& strVbank_num, const std::string& strVbank_holder, const std::string& strVbank_date)
: _customItem(nullptr)
, mn_Amount(nAmount)
, mstr_Pay_method(strPay_method)
, mstr_Pay_method_text(strPay_method_text)
, mstr_Vbank_name(strVbank_name)
, mstr_Vbank_num(strVbank_num)
, mstr_Vbank_holder(strVbank_holder)
, mstr_Vbank_date(strVbank_date)
{
    
}

PixPurchaseCompletedInfoCell::~PixPurchaseCompletedInfoCell()
{
    
}

bool PixPurchaseCompletedInfoCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPurchaseCompletedInfoCell::onEnter()
{
    Widget::onEnter();
}

void PixPurchaseCompletedInfoCell::createCustomItem()
{
    //    "결제수단
    //    card (신용카드)
    //    trans (실시간계좌이체)
    //    vbank (가상계좌)
    //    phone (휴대폰 소액결제)
    //    samsung (삼성페이 / 이니시스전용)
    //    kpay (kPay 앱 직접호출 / 이니시스 전용)
    //    cultureland ( 문화상품권 / 이니시스 전용)
    //    smartculture ( 스마트 문화상품권 / 이니시스 전용)
    //    happymoney ( 해피머니 / 이니시스 전용) "
    
    if(mstr_Pay_method.compare("vbank") == 0)
    {
        _customItem = createVbankCell();
    }
    else
    {
        _customItem = createCell();
    }
    
    if(_customItem)
    {
        ui::Text *Text_Title = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Title"));
        
        if(Text_Title)
        {
            Text_Title->setString(CODE_NAME(Text_Title->getString()));
        }
        
        Sprite *Payments = dynamic_cast<Sprite*>(_customItem->getChildByName("Payments"));
        
        if(Payments)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Payments->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Payments->getChildByName("Text_2"));
            
            if(Text_2)
            {
                std::string strMoney = PixStringUtils::commaMoney(mn_Amount);
                std::string strText = StringUtils::format(CODE_NAME(Text_2->getString()), strMoney.c_str());
                Text_2->setString(strText);
            }
        }
        
        Sprite *PaymentMethod = dynamic_cast<Sprite*>(_customItem->getChildByName("PaymentMethod"));
        
        if(PaymentMethod)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(PaymentMethod->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(PaymentMethod->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(mstr_Pay_method_text);
            }
        }

        
        Sprite *Bank = dynamic_cast<Sprite*>(_customItem->getChildByName("Bank"));
        
        if(Bank)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Bank->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Bank->getChildByName("Text_2"));
            
            if(Text_2)
            {
                std::string strBank = mstr_Vbank_name;
                strBank.append(" ");
                strBank.append(mstr_Vbank_num);
                Text_2->setString(strBank);
            }
            
            ui::Text *Text_3 = dynamic_cast<ui::Text*>(Bank->getChildByName("Text_3"));
            
            if(Text_3)
            {
                std::string strText = StringUtils::format(CODE_NAME(Text_3->getString()), mstr_Vbank_holder.c_str());
                
                Text_3->setString(strText);
            }
        }

        Sprite *Date = dynamic_cast<Sprite*>(_customItem->getChildByName("Date"));
        
        if(Date)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Date->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Date->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(mstr_Vbank_date);
            }
        }

        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

Node* PixPurchaseCompletedInfoCell::createCell()
{
    return CSLoader::createNode("12_Oder/MenuOrderCompleted_02.csb");
}

Node* PixPurchaseCompletedInfoCell::createVbankCell()
{
    return CSLoader::createNode("12_Oder/MenuOrderCompleted_03.csb");
}

