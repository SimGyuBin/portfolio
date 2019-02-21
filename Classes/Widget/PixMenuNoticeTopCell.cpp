//
//  PixMenuNoticeTopCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 19..
//
//

#include "PixMenuNoticeTopCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuNoticeTopCell* PixMenuNoticeTopCell::create()
{
    PixMenuNoticeTopCell* widget = new (std::nothrow) PixMenuNoticeTopCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixMenuNoticeTopCell::PixMenuNoticeTopCell()
: _customItem(nullptr)
{
    
}

PixMenuNoticeTopCell::~PixMenuNoticeTopCell()
{
    
}

bool PixMenuNoticeTopCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixMenuNoticeTopCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixMenuNoticeTopCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("11_Menu/MenuNoticeItem_2.csb");
    
    if(_customItem)
    {
        ui::Text *Text = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text"));
        
        if(Text)
        {
            Text->setString(CODE_NAME(Text->getString()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

