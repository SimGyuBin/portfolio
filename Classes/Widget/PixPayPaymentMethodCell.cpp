//
//  PixPayPaymentMethodCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 24..
//
//

#include "PixPayPaymentMethodCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPayPaymentMethodCell* PixPayPaymentMethodCell::create()
{
    PixPayPaymentMethodCell* widget = new (std::nothrow) PixPayPaymentMethodCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPayPaymentMethodCell::PixPayPaymentMethodCell()
: _customItem(nullptr)
, creditCard(nullptr)
, phone(nullptr)
, noBankbook(nullptr)
, realTimeBank(nullptr)
{
    checkBoxVec.clear();
}

PixPayPaymentMethodCell::~PixPayPaymentMethodCell()
{
    checkBoxVec.clear();
}

bool PixPayPaymentMethodCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPayPaymentMethodCell::onEnter()
{
    Widget::onEnter();
}

void PixPayPaymentMethodCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPayItem_08.csb");
    
    if(_customItem)
    {
        ui::Text *Text_Title = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Title"));
        
        if(Text_Title)
        {
            Text_Title->setString(CODE_NAME(Text_Title->getString()));
        }
        
        creditCard = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("CheckBox_1"));
        
        if(creditCard)
        {
            creditCard->addEventListener(CC_CALLBACK_2(PixPayPaymentMethodCell::creditCardPressed, this));
            
            ui::Text *Text = dynamic_cast<ui::Text*>(creditCard->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            creditCard->setSelected(true);
            creditCard->setEnabled(false);
            
            checkBoxVec.push_back(creditCard);
        }
        
        phone = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("CheckBox_2"));
        
        if(phone)
        {
            phone->addEventListener(CC_CALLBACK_2(PixPayPaymentMethodCell::phonePressed, this));
            
            ui::Text *Text = dynamic_cast<ui::Text*>(phone->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            checkBoxVec.push_back(phone);
        }
        
        noBankbook = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("CheckBox_3"));
        
        if(noBankbook)
        {
            noBankbook->addEventListener(CC_CALLBACK_2(PixPayPaymentMethodCell::noBankbookPressed, this));
            
            ui::Text *Text = dynamic_cast<ui::Text*>(noBankbook->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            checkBoxVec.push_back(noBankbook);
        }
        
        realTimeBank = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("CheckBox_4"));
        
        if(realTimeBank)
        {
            realTimeBank->addEventListener(CC_CALLBACK_2(PixPayPaymentMethodCell::realTimeBankPressed, this));
            
            ui::Text *Text = dynamic_cast<ui::Text*>(realTimeBank->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
            }
            
            checkBoxVec.push_back(realTimeBank);
        }

        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

bool PixPayPaymentMethodCell::isCreditCardSelected()
{
    if(creditCard)
    {
        return creditCard->isSelected();
    }
    
    return false;
}

bool PixPayPaymentMethodCell::isPhoneSelected()
{
    if(phone)
    {
        return phone->isSelected();
    }
    
    return false;
}

bool PixPayPaymentMethodCell::isNoBankbookSelected()
{
    if(noBankbook)
    {
        return noBankbook->isSelected();
    }
    
    return false;
}

bool PixPayPaymentMethodCell::isRealTimeBankSelected()
{
    if(realTimeBank)
    {
        return realTimeBank->isSelected();
    }
    
    return false;
}

const char *PixPayPaymentMethodCell::getPay_method()
{
 
    //    pay_method		"결제수단
    //    card (신용카드)
    //    trans (실시간계좌이체)
    //    vbank (가상계좌)
    //    phone (휴대폰 소액결제)
    //    samsung (삼성페이 / 이니시스전용)
    //    kpay (kPay 앱 직접호출 / 이니시스 전용)
    //    cultureland ( 문화상품권 / 이니시스 전용)
    //    smartculture ( 스마트 문화상품권 / 이니시스 전용)
    //    happymoney ( 해피머니 / 이니시스 전용) "
    
    for (auto iter = checkBoxVec.begin(); iter != checkBoxVec.end(); ++iter)
    {
        if((*iter)->isSelected())
        {
            if((*iter) == creditCard)
            {
                return "card";
            }
            else if((*iter) == phone)
            {
                return "phone";
            }
            else if((*iter) == noBankbook)
            {
                return "vbank";
            }
            else if((*iter) == realTimeBank)
            {
                return "trans";
            }
            else
            {
                
            }
        }
    }
    
    return "";
}

void PixPayPaymentMethodCell::updateCheckBox(ui::CheckBox *pCheckBox)
{
    if(pCheckBox == nullptr)
        return;
    
    if(checkBoxVec.empty())
        return;
    
    if(pCheckBox->isSelected())
    {
        pCheckBox->setEnabled(false);
        
        bool blnSelected = !pCheckBox->isSelected();
        
        for (auto iter = checkBoxVec.begin(); iter != checkBoxVec.end(); ++iter)
        {
            if((*iter) != pCheckBox)
            {
                (*iter)->setSelected(blnSelected);
                
                if((*iter)->isSelected())
                {
                    (*iter)->setEnabled(false);
                }
                else
                {
                    (*iter)->setEnabled(true);
                }
            }
        }
    }
    else
    {
        pCheckBox->setEnabled(true);
    }
}

#pragma mark - Button

void PixPayPaymentMethodCell::creditCardPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        updateCheckBox(checkBox);
    }
}

void PixPayPaymentMethodCell::phonePressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        updateCheckBox(checkBox);
    }
}

void PixPayPaymentMethodCell::noBankbookPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        updateCheckBox(checkBox);
    }
}

void PixPayPaymentMethodCell::realTimeBankPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        updateCheckBox(checkBox);
    }
}
