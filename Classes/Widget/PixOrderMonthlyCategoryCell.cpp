//
//  PixOrderMonthlyCategoryCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 12..
//
//

#include "PixOrderMonthlyCategoryCell.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

PixOrderMonthlyCategoryCell* PixOrderMonthlyCategoryCell::create(const std::string& strDate)
{
    PixOrderMonthlyCategoryCell* widget = new (std::nothrow) PixOrderMonthlyCategoryCell(strDate);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixOrderMonthlyCategoryCell::PixOrderMonthlyCategoryCell(const std::string& strDate)
: _customItem(nullptr)
,mstr_Date(strDate)
{
    
}

PixOrderMonthlyCategoryCell::~PixOrderMonthlyCategoryCell()
{
    
}

bool PixOrderMonthlyCategoryCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixOrderMonthlyCategoryCell::onEnter()
{
    Widget::onEnter();
}

void PixOrderMonthlyCategoryCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("12_Oder/MenuOrderItem_02.csb");
    
    if(_customItem)
    {
        ui::ImageView *Sprite_OrderDate = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("Sprite_OrderDate"));
        
        if(Sprite_OrderDate)
        {
            ui::Text *Text_Date = dynamic_cast<ui::Text*>(Sprite_OrderDate->getChildByName("Text_Date"));
            
            if(Text_Date)
            {
                if(!mstr_Date.empty())
                {
                    std::vector<std::string> tokens;
                    PixStringUtils::getTokens(tokens, mstr_Date, "-");
                    
                    if(tokens.size() >= 2)
                    {
                        std::string strText = StringUtils::format(CODE_NAME(Text_Date->getString()), atoi(tokens[0].c_str()), atoi(tokens[1].c_str()));
                        
                        Text_Date->setString(strText);
                    }
                }
            }
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}


