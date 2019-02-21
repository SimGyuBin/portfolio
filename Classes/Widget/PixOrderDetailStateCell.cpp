//
//  PixOrderDetailStateCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 13..
//
//

#include "PixOrderDetailStateCell.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/OrderCheckInfo.h"
#include "../Layer/PixMenuOrderLayer.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

PixOrderDetailStateCell* PixOrderDetailStateCell::create(OrderCheckInfo *pOrderCheckInfo)
{
    PixOrderDetailStateCell* widget = new (std::nothrow) PixOrderDetailStateCell(pOrderCheckInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixOrderDetailStateCell::PixOrderDetailStateCell(OrderCheckInfo *pOrderCheckInfo)
: _customItem(nullptr)
, ChangeAddress(nullptr)
, CancelOrder(nullptr)
, TrackingNumber(nullptr)
, Text_Periodinfor(nullptr)
, mOrderCheckInfo(pOrderCheckInfo)
, mDelegate(nullptr)
{
    
}

PixOrderDetailStateCell::~PixOrderDetailStateCell()
{
    
}

bool PixOrderDetailStateCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixOrderDetailStateCell::onEnter()
{
    Widget::onEnter();
}

void PixOrderDetailStateCell::createCustomItem()
{
    if(mOrderCheckInfo == nullptr)
        return;
    
    ORDER_TYPE eType = PixMenuOrderLayer::getOrderStatus(mOrderCheckInfo->getPay_status(),mOrderCheckInfo->getItem_status());
    
    if(eType == ORDER_CANCELLATION)
    {
        _customItem = customCancellationCall();
    }
    else if(eType == ORDER_CANCELLATION_PROCESSING)
    {
        _customItem = customCancellationProcessingCeaal();
    }
    else
    {
        _customItem = customCall();
    }
    
    if(_customItem)
    {
        ui::Text *Text_OrderNumber = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderNumber"));
        
        if(Text_OrderNumber)
        {
            Text_OrderNumber->setString(CODE_NAME(Text_OrderNumber->getString()));
        }
        
        ui::Text *Text_OrderNumber_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderNumber_2"));
        
        if(Text_OrderNumber_2)
        {
            Text_OrderNumber_2->setString(mOrderCheckInfo->getOrder_no());
        }
        
        ui::Text *Text_OrderDate = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_OrderDate"));
        
        if(Text_OrderDate)
        {
            Text_OrderDate->setString(CODE_NAME(Text_OrderDate->getString()));
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

        ui::Text *Text_Condition = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Condition"));
        
        if(Text_Condition)
        {
            Text_Condition->setString(CODE_NAME(Text_Condition->getString()));
        }
        
        ui::Text *Text_Condition_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Condition_2"));
        
        if(Text_Condition_2)
        {
            Text_Condition_2->setString(PixMenuOrderLayer::getOrderStatusToUIString(mOrderCheckInfo->getPay_status(),mOrderCheckInfo->getItem_status()));
        }
        
//        DETAIL_ACTION_CHANGE_ADDRESS = 0,   // 배송지 변경
//        DETAIL_ACTION_CANCEL_ORDER,         // 주문취소
//        DETAIL_ACTION_TRACKING_NUMBER,      // 배송조회

        // 배송지 변경
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_01"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixOrderDetailStateCell::buttonStatePressed, this));
           
            menuItem->setTag(DETAIL_ACTION_CHANGE_ADDRESS);
            
            ChangeAddress = PixScrollMenu::create(menuItem, nullptr);
            ChangeAddress->setPosition(pos);
            ChangeAddress->setContentSize(button->getContentSize());
            _customItem->addChild(ChangeAddress);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text_01"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
                
                ui::Text *clone =  dynamic_cast<ui::Text*>(Text->clone());
                
                if(clone)
                {
                    clone->setFontSize(Text->getFontSize());
                    clone->setTextColor(Text->getTextColor());
                    clone->setString(Text->getString());
                    menuItem->addChild(clone);
                }
            }
            
            button->removeFromParentAndCleanup(true);
            
        }
        
        // 주문취소
        button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_02"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixOrderDetailStateCell::buttonStatePressed, this));
            menuItem->setTag(DETAIL_ACTION_CANCEL_ORDER);
            
            CancelOrder = PixScrollMenu::create(menuItem, nullptr);
            CancelOrder->setPosition(pos);
            CancelOrder->setContentSize(button->getContentSize());
            _customItem->addChild(CancelOrder);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text_02"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
                
                ui::Text *clone =  dynamic_cast<ui::Text*>(Text->clone());
                
                if(clone)
                {
                    clone->setFontSize(Text->getFontSize());
                    clone->setTextColor(Text->getTextColor());
                    clone->setString(Text->getString());
                    menuItem->addChild(clone);
                }
            }
            
            button->removeFromParentAndCleanup(true);
            
        }
        
        // 배송조회
        button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_03"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixOrderDetailStateCell::buttonStatePressed, this));
            menuItem->setTag(DETAIL_ACTION_TRACKING_NUMBER);
            
            TrackingNumber = PixScrollMenu::create(menuItem, nullptr);
            TrackingNumber->setPosition(pos);
            TrackingNumber->setContentSize(button->getContentSize());
            _customItem->addChild(TrackingNumber);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text_03"));
            
            if(Text)
            {
                Text->setString(CODE_NAME(Text->getString()));
                
                ui::Text *clone =  dynamic_cast<ui::Text*>(Text->clone());
                
                if(clone)
                {
                    clone->setFontSize(Text->getFontSize());
                    clone->setTextColor(Text->getTextColor());
                    clone->setString(Text->getString());
                    menuItem->addChild(clone);
                }
            }
            
            button->removeFromParentAndCleanup(true);
            
        }
        
        Text_Periodinfor = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Periodinfor"));
        
        if(Text_Periodinfor)
        {
            Text_Periodinfor->setString(CODE_NAME(Text_Periodinfor->getString()));
        }
        
        Text_Cancel = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Cancel"));
        
        if(Text_Cancel)
        {
            if(eType == ORDER_CANCELLATION_PROCESSING)
            {
                Text_Cancel->setString(CODE_NAME(Text_Cancel->getString()));
            }
            else
            {
                //            "2017-04-17 00:10:01"
                std::string strDate = mOrderCheckInfo->getCancel_date().substr(0,10);
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
                strDate.append(" ");
                strDate.append(CODE_NAME("Order40"));
                
                Text_Cancel->setString(strDate);
            }
        }

        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
        
        updateDetailStateUI();
    }
}

Node* PixOrderDetailStateCell::customCall()
{
    return CSLoader::createNode("12_Oder/MenuOrderDetailItem_01.csb");
}

Node* PixOrderDetailStateCell::customCancellationCall()
{
    return CSLoader::createNode("12_Oder/MenuOrderDetailItem_00.csb");
}

Node* PixOrderDetailStateCell::customCancellationProcessingCeaal()
{
    return CSLoader::createNode("12_Oder/MenuOrderDetailItem_07.csb");
}

void PixOrderDetailStateCell::updateDetailStateUI()
{
    if(mOrderCheckInfo == nullptr)
        return;
    
//    ORDER_DEPOSIT_CHECK = 1,         // 입금 확인중
//    ORDER_PRODUCTION_PREPARING = 2,  // 제작 준비중
//    ORDER_IN_PRODUCTION = 4,         // 제작중
//    ORDER_DELIVERY = 8,              // 배송중
//    ORDER_COMPLETE_DELIVERY = 16,    // 배송완료
//    ORDER_CANCELLATION = 32,         // 주문취소
    
    ORDER_TYPE eType = PixMenuOrderLayer::getOrderStatus(mOrderCheckInfo->getPay_status(),mOrderCheckInfo->getItem_status());
    
    switch (eType) {
        
        // 배송지 변경, 주문취소만
        case ORDER_DEPOSIT_CHECK:
        case ORDER_PRODUCTION_PREPARING:
        {
            if(ChangeAddress)
            {
                ChangeAddress->setVisible(true);
            }
            
            if(CancelOrder)
            {
                CancelOrder->setVisible(true);
            }
            
            if(TrackingNumber)
            {
                TrackingNumber->setVisible(false);
            }
            
            if(Text_Periodinfor)
            {
                Text_Periodinfor->setVisible(false);
            }
            
            if(Text_Cancel)
            {
                Text_Cancel->setVisible(false);
            }

        }
        break;
            
        // 배송조회만
        case ORDER_DELIVERY:
        case ORDER_COMPLETE_DELIVERY:
        {
            if(ChangeAddress)
            {
                ChangeAddress->setVisible(false);
            }
            
            if(CancelOrder)
            {
                CancelOrder->setVisible(false);
            }
            
            if(TrackingNumber)
            {
                TrackingNumber->setVisible(true);
            }
            
            if(Text_Periodinfor)
            {
                Text_Periodinfor->setVisible(false);
            }
        }
        break;
           
        case ORDER_CANCELLATION_PROCESSING:
        {
            if(ChangeAddress)
            {
                ChangeAddress->setVisible(false);
            }
            
            if(CancelOrder)
            {
                CancelOrder->setVisible(false);
            }
            
            if(TrackingNumber)
            {
                TrackingNumber->setVisible(false);
            }
            
            if(Text_Periodinfor)
            {
                Text_Periodinfor->setVisible(false);
            }
            
            if(Text_Cancel)
            {
                Text_Cancel->setVisible(true);
            }
        }
        break;
        
        case ORDER_CANCELLATION:
        {
            if(ChangeAddress)
            {
                ChangeAddress->setVisible(false);
            }
            
            if(CancelOrder)
            {
                CancelOrder->setVisible(false);
            }
            
            if(TrackingNumber)
            {
                TrackingNumber->setVisible(false);
            }
            
            if(Text_Periodinfor)
            {
                Text_Periodinfor->setVisible(false);
            }
            
            if(Text_Cancel)
            {
                Text_Cancel->setVisible(true);
            }
        }
        break;
        
        // 정보만
        case ORDER_IN_PRODUCTION:
        {
            if(ChangeAddress)
            {
                ChangeAddress->setVisible(false);
            }
            
            if(CancelOrder)
            {
                CancelOrder->setVisible(false);
            }
            
            if(TrackingNumber)
            {
                TrackingNumber->setVisible(false);
            }
            
            if(Text_Periodinfor)
            {
                Text_Periodinfor->setVisible(true);
            }
            
            if(Text_Cancel)
            {
                Text_Cancel->setVisible(false);
            }
        }
        break;
            
        default:
            break;
    }
    
}

void PixOrderDetailStateCell::buttonStatePressed(cocos2d::Ref *pSender)
{
    Node *node = dynamic_cast<Node*>(pSender);
    
    if(node)
    {
        if(mDelegate)
        {
            mDelegate->orderDetailStateCellPressed(mOrderCheckInfo, node->getTag());
        }
    }
}
