//
//  PixOrderDetailBillingInfoCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 17..
//
//

#include "PixOrderDetailBillingInfoCell.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/OrderCheckInfo.h"
#include "../Layer/PixMenuOrderLayer.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

PixOrderDetailBillingInfoCell* PixOrderDetailBillingInfoCell::create(OrderCheckInfo *pOrderCheckInfo)
{
    PixOrderDetailBillingInfoCell* widget = new (std::nothrow) PixOrderDetailBillingInfoCell(pOrderCheckInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixOrderDetailBillingInfoCell::PixOrderDetailBillingInfoCell(OrderCheckInfo *pOrderCheckInfo)
: _customItem(nullptr)
, mOrderCheckInfo(pOrderCheckInfo)
{
    
}

PixOrderDetailBillingInfoCell::~PixOrderDetailBillingInfoCell()
{
    
}

bool PixOrderDetailBillingInfoCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixOrderDetailBillingInfoCell::onEnter()
{
    Widget::onEnter();
}

void PixOrderDetailBillingInfoCell::createCustomItem()
{
    if(mOrderCheckInfo == nullptr)
        return;
    
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
    
    std::string strPay_method = mOrderCheckInfo->getPay_method();
    
    if(strPay_method.compare("vbank") == 0)
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
        
        Sprite *TotalItem = dynamic_cast<Sprite*>(_customItem->getChildByName("TotalItem"));
        
        if(TotalItem)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(TotalItem->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(TotalItem->getChildByName("Text_2"));
            
            if(Text_2)
            {
                std::string strMoney = PixStringUtils::commaMoney(mOrderCheckInfo->getAmount());
                std::string strText = StringUtils::format(CODE_NAME(Text_2->getString()), strMoney.c_str());

                Text_2->setString(strText);
            }
        }
        
        Sprite *TotalDiscount = dynamic_cast<Sprite*>(_customItem->getChildByName("TotalDiscount"));
        
        if(TotalDiscount)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(TotalDiscount->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(TotalDiscount->getChildByName("Text_2"));
            
            if(Text_2)
            {
                std::string strMoney = PixStringUtils::commaMoney(mOrderCheckInfo->getDiscount_amount());
                std::string strText = StringUtils::format(CODE_NAME(Text_2->getString()), strMoney.c_str());
                
                Text_2->setString(strText);
            }
        }
        
        Sprite *Shipping = dynamic_cast<Sprite*>(_customItem->getChildByName("Shipping"));
        
        if(Shipping)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Shipping->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Shipping->getChildByName("Text_2"));
            
            if(Text_2)
            {
                std::string strMoney = PixStringUtils::commaMoney(mOrderCheckInfo->getPay_ship_amount());
                std::string strText = StringUtils::format(CODE_NAME(Text_2->getString()), strMoney.c_str());
                
                Text_2->setString(strText);
            }
        }
        
        Sprite *Total = dynamic_cast<Sprite*>(_customItem->getChildByName("Total"));
        
        if(Total)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Total->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Total->getChildByName("Text_2"));
            
            if(Text_2)
            {
                std::string strMoney = PixStringUtils::commaMoney(mOrderCheckInfo->getPay_amount());
                std::string strText = StringUtils::format(CODE_NAME(Text_2->getString()), strMoney.c_str());
                
                Text_2->setString(strText);
            }
        }
        
        Sprite *Method = dynamic_cast<Sprite*>(_customItem->getChildByName("Method"));
        
        if(Method)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Method->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Method->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(PixMenuOrderLayer::getPayMethodToUIString(mOrderCheckInfo->getPay_method()));
            }
        }
        
        
        Sprite *TransferAccount = dynamic_cast<Sprite*>(_customItem->getChildByName("TransferAccount"));
        
        if(TransferAccount)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(TransferAccount->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(TransferAccount->getChildByName("Text_2"));
            
            if(Text_2)
            {
                std::string strText = mOrderCheckInfo->getVbank_name();
                strText.append(" ");
                strText.append(mOrderCheckInfo->getVbank_num());
                
                Text_2->setString(strText);
            }
            
            ui::Text *Text_3 = dynamic_cast<ui::Text*>(TransferAccount->getChildByName("Text_3"));
            
            if(Text_3)
            {
                std::string strText = StringUtils::format(CODE_NAME(Text_3->getString()), mOrderCheckInfo->getVbank_holder().c_str());
                
                Text_3->setString(strText);
            }
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

Node* PixOrderDetailBillingInfoCell::createCell()
{
    return CSLoader::createNode("12_Oder/MenuOrderDetailItem_05.csb");
}

Node* PixOrderDetailBillingInfoCell::createVbankCell()
{
    return CSLoader::createNode("12_Oder/MenuOrderDetailItem_06.csb");
}
