//
//  PixOrderCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 12..
//
//

#include "PixOrderCell.h"
#include "PixOrderGoodsCell.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Util/PixStringUtils.h"
#include "../Info/OrderCheckInfo.h"
#include "../Layer/PixMenuOrderLayer.h"
#include "../Common.h"

PixOrderCell* PixOrderCell::create(OrderCheckInfo *pOrderCheckInfo)
{
    PixOrderCell* widget = new (std::nothrow) PixOrderCell(pOrderCheckInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixOrderCell::PixOrderCell(OrderCheckInfo *pOrderCheckInfo)
: _customItem(nullptr)
, mOrderCheckInfo(pOrderCheckInfo)
{
    
}

PixOrderCell::~PixOrderCell()
{
    
}

bool PixOrderCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixOrderCell::onEnter()
{
    Widget::onEnter();
}

void PixOrderCell::createCustomItem()
{
    if(mOrderCheckInfo == nullptr)
        return;
        
    _customItem = CSLOADER_MANAGER->getCustomItem("12_Oder/MenuOrderItem_03.csb");
    
    if(_customItem)
    {
        ui::Text *Text_OrderNumber = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderNumber"));
        
        if(Text_OrderNumber)
        {
            Text_OrderNumber->setString(CODE_NAME(Text_OrderNumber->getString()));
        }
        
        ui::Text *Text_OrderDate = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderDate"));
        
        if(Text_OrderDate)
        {
            Text_OrderDate->setString(CODE_NAME(Text_OrderDate->getString()));
        }
        
        ui::Text *Text_OrderPrice = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderPrice"));
        
        if(Text_OrderPrice)
        {
            Text_OrderPrice->setString(CODE_NAME(Text_OrderPrice->getString()));
        }
        
        ui::Text *Text_Condition = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Condition"));
        
        if(Text_Condition)
        {
            Text_Condition->setString(CODE_NAME(Text_Condition->getString()));
        }
        
        ui::Text *Text_OrderNumber_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderNumber_2"));
        
        if(Text_OrderNumber_2)
        {
            Text_OrderNumber_2->setString(mOrderCheckInfo->getOrder_no());
        }
        
        ui::Text *Text_OrderDate_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderDate_2"));
        
        if(Text_OrderDate_2)
        {
            //            "2017-04-11 17:04:58"
            std::string strDate = mOrderCheckInfo->getReg_date().substr(0,10);
            std::vector<std::string> tokens;
            PixStringUtils::getTokens(tokens, strDate, "-");
            
            strDate = StringUtils::format(CODE_NAME("Date01"),atoi(tokens[0].c_str()),atoi(tokens[1].c_str()),atoi(tokens[2].c_str()));
            
            tokens.clear();
            std::string strTime = mOrderCheckInfo->getReg_date().substr(11,5);
            PixStringUtils::getTokens(tokens, strTime, ":");
            int nHour = atoi(tokens[0].c_str());
            int nMinute = atoi(tokens[1].c_str());
            strTime.clear();
            
            strTime.append(" ");
            if(nHour > 12)
            {
                nHour -= 12;
                strTime.append(CODE_NAME("Date04"));
            }
            else
            {
                strTime.append(CODE_NAME("Date03"));
            }
            strTime.append(" ");
            
            strTime.append(StringUtils::format("%d:%02d",nHour,nMinute));
            strDate.append(strTime);
            
            Text_OrderDate_2->setString(strDate);
        }
        
        ui::Text *Text_OrderPrice_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderPrice_2"));
        
        if(Text_OrderPrice_2)
        {
            std::string strMoney = PixStringUtils::commaMoney(mOrderCheckInfo->getPay_amount());
            std::string strText = StringUtils::format(CODE_NAME(Text_OrderPrice_2->getString()), strMoney.c_str());
            
            Text_OrderPrice_2->setString(strText);
        }
        
        ui::Text *Text_Condition_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Condition_2"));
        
        if(Text_Condition_2)
        {
            Text_Condition_2->setString(PixMenuOrderLayer::getOrderStatusToUIString(mOrderCheckInfo->getPay_status(),mOrderCheckInfo->getItem_status()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}



