//
//  PixOrderDetailOrdererInfoCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 17..
//
//

#include "PixOrderDetailOrdererInfoCell.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/OrderCheckInfo.h"
#include "../Common.h"

PixOrderDetailOrdererInfoCell* PixOrderDetailOrdererInfoCell::create(OrderCheckInfo *pOrderCheckInfo)
{
    PixOrderDetailOrdererInfoCell* widget = new (std::nothrow) PixOrderDetailOrdererInfoCell(pOrderCheckInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixOrderDetailOrdererInfoCell::PixOrderDetailOrdererInfoCell(OrderCheckInfo *pOrderCheckInfo)
: _customItem(nullptr)
, mOrderCheckInfo(pOrderCheckInfo)
{
    
}

PixOrderDetailOrdererInfoCell::~PixOrderDetailOrdererInfoCell()
{
    
}

bool PixOrderDetailOrdererInfoCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixOrderDetailOrdererInfoCell::onEnter()
{
    Widget::onEnter();
}

void PixOrderDetailOrdererInfoCell::createCustomItem()
{
    if(mOrderCheckInfo == nullptr)
        return;
    
    _customItem = CSLoader::createNode("12_Oder/MenuOrderDetailItem_03.csb");
    
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
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Name->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(mOrderCheckInfo->getStrUserName());
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
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Phone->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(mOrderCheckInfo->getStrPhone01());
            }
        }
        
        Sprite *Email = dynamic_cast<Sprite*>(_customItem->getChildByName("Email"));
        
        if(Email)
        {
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Email->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Email->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(mOrderCheckInfo->getStrEmail());
            }
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}




