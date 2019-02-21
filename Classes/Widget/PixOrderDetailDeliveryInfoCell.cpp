//
//  PixOrderDetailDeliveryInfoCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 17..
//
//

#include "PixOrderDetailDeliveryInfoCell.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/OrderCheckInfo.h"
#include "../Common.h"

PixOrderDetailDeliveryInfoCell* PixOrderDetailDeliveryInfoCell::create(OrderCheckInfo *pOrderCheckInfo)
{
    PixOrderDetailDeliveryInfoCell* widget = new (std::nothrow) PixOrderDetailDeliveryInfoCell(pOrderCheckInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixOrderDetailDeliveryInfoCell::PixOrderDetailDeliveryInfoCell(OrderCheckInfo *pOrderCheckInfo)
: _customItem(nullptr)
, Text_Name(nullptr)
, Text_Phone(nullptr)
, Text_ZipCode(nullptr)
, Text_Address(nullptr)
, Text_Address2(nullptr)
, Text_Memo(nullptr)
, mOrderCheckInfo(pOrderCheckInfo)
{
    
}

PixOrderDetailDeliveryInfoCell::~PixOrderDetailDeliveryInfoCell()
{
    
}

bool PixOrderDetailDeliveryInfoCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixOrderDetailDeliveryInfoCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}

void PixOrderDetailDeliveryInfoCell::onExit()
{
    mOrderCheckInfo = nullptr;
    unscheduleUpdate();
    Widget::onExit();
}

void PixOrderDetailDeliveryInfoCell::update(float dt)
{
    Widget::update(dt);
    updateOrderDetailDeliveryInfoCell();
}

void PixOrderDetailDeliveryInfoCell::createCustomItem()
{
    if(mOrderCheckInfo == nullptr)
        return;
    
    _customItem = CSLoader::createNode("12_Oder/MenuOrderDetailItem_04.csb");
    
    if(_customItem)
    {
        ui::Text *Text_Title = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Title"));
        
        if(Text_Title)
        {
            Text_Title->setString(CODE_NAME(Text_Title->getString()));
        }
        
        Sprite *Name = dynamic_cast<Sprite*>(_customItem->getChildByName("Name"));
        
        if(Name)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Name->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            Text_Name = dynamic_cast<ui::Text*>(Name->getChildByName("Text_2"));
            
            if(Text_Name)
            {
                Text_Name->setString(mOrderCheckInfo->getName());
            }
        }
        
        Sprite *Phone = dynamic_cast<Sprite*>(_customItem->getChildByName("Phone"));
        
        if(Phone)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Phone->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            Text_Phone = dynamic_cast<ui::Text*>(Phone->getChildByName("Text_2"));
            
            if(Text_Phone)
            {
                Text_Phone->setString(mOrderCheckInfo->getPhone1());
            }
        }
        
        Sprite *ZipCode = dynamic_cast<Sprite*>(_customItem->getChildByName("ZipCode"));
        
        if(ZipCode)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(ZipCode->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            Text_ZipCode = dynamic_cast<ui::Text*>(ZipCode->getChildByName("Text_2"));
            
            if(Text_ZipCode)
            {
                Text_ZipCode->setString(mOrderCheckInfo->getPost());
            }
        }
        
        Sprite *Address = dynamic_cast<Sprite*>(_customItem->getChildByName("Address"));
        
        if(Address)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Address->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            Text_Address = dynamic_cast<ui::Text*>(Address->getChildByName("Text_2"));
            
            if(Text_Address)
            {
                Text_Address->setString(mOrderCheckInfo->getAddr1());
            }
        }
        
        Sprite *Address2 = dynamic_cast<Sprite*>(_customItem->getChildByName("Address2"));
        
        if(Address2)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Address2->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            Text_Address2 = dynamic_cast<ui::Text*>(Address2->getChildByName("Text_2"));
            
            if(Text_Address2)
            {
                Text_Address2->setString(mOrderCheckInfo->getAddr2());
            }
        }
        
        Sprite *Memo = dynamic_cast<Sprite*>(_customItem->getChildByName("Memo"));
        
        if(Memo)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Memo->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            Text_Memo = dynamic_cast<ui::Text*>(Memo->getChildByName("Text_2"));
            
            if(Text_Memo)
            {
                Text_Memo->setString(mOrderCheckInfo->getMemo());
            }
        }
                
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixOrderDetailDeliveryInfoCell::updateOrderDetailDeliveryInfoCell()
{
    if(mOrderCheckInfo == nullptr)
        return;
    
    if(Text_Name)
    {
        Text_Name->setString(mOrderCheckInfo->getName());
    }
    
    if(Text_Phone)
    {
        Text_Phone->setString(mOrderCheckInfo->getPhone1());
    }
    
    if(Text_ZipCode)
    {
        Text_ZipCode->setString(mOrderCheckInfo->getPost());
    }
    
    if(Text_Address)
    {
        Text_Address->setString(mOrderCheckInfo->getAddr1());
    }
    
    if(Text_Address2)
    {
        Text_Address2->setString(mOrderCheckInfo->getAddr2());
    }
    
    if(Text_Memo)
    {
        Text_Memo->setString(mOrderCheckInfo->getMemo());
    }
}
