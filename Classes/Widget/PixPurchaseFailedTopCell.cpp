//
//  PixPurchaseFailedTopCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 4..
//
//

#include "PixPurchaseFailedTopCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPurchaseFailedTopCell* PixPurchaseFailedTopCell::create(const std::string& strFail_reason)
{
    PixPurchaseFailedTopCell* widget = new (std::nothrow) PixPurchaseFailedTopCell(strFail_reason);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;

}

PixPurchaseFailedTopCell::PixPurchaseFailedTopCell(const std::string& strFail_reason)
: _customItem(nullptr)
, mstr_Fail_reason(strFail_reason)
{
    
}

PixPurchaseFailedTopCell::~PixPurchaseFailedTopCell()
{

}

bool PixPurchaseFailedTopCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPurchaseFailedTopCell::onEnter()
{
    Widget::onEnter();
}

void PixPurchaseFailedTopCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuOrderFailedItem_01.csb");
    
    if(_customItem)
    {
        ui::Text *Text_failedInfor = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_failedInfor"));
        
        if(Text_failedInfor)
        {
            Text_failedInfor->setString(CODE_NAME(Text_failedInfor->getString()));
        }
        
        ui::Text *Text_Detail = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Detail"));
        
        if(Text_Detail)
        {
            std::string msg = CODE_NAME(Text_Detail->getString());
            msg = StringUtils::format(msg.c_str(),mstr_Fail_reason.c_str());
            Text_Detail->setString(msg);
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}



