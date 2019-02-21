//
//  PixBankItemCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 19..
//
//

#include "PixBankItemCell.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Common.h"

PixBankItemCell* PixBankItemCell::create(const std::string& strBank_name, const std::string& strBank_code)
{
    PixBankItemCell* widget = new (std::nothrow) PixBankItemCell(strBank_name, strBank_code);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixBankItemCell::PixBankItemCell(const std::string& strBank_name, const std::string& strBank_code)
: _customItem(nullptr)
, Image_Check_Bank(nullptr)
, Image_Check_Bank_on(nullptr)
, Text_Bank(nullptr)
, mbln_CheckVisible(false)
, mstr_Bank_name(strBank_name)
, mstr_Bank_code(strBank_code)
{
    
}

PixBankItemCell::~PixBankItemCell()
{
    
}

bool PixBankItemCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixBankItemCell::onEnter()
{
    Widget::onEnter();
}

void PixBankItemCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuOrderCancel_Pop_BankItem.csb");
    
    if(_customItem)
    {
        Image_Check_Bank = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("Image_Check_Bank"));
        
        if(Image_Check_Bank)
        {
            Image_Check_Bank->setVisible(true);
        }
        
        Image_Check_Bank_on = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("Image_Check_Bank_on"));
        
        if(Image_Check_Bank_on)
        {
            Image_Check_Bank_on->setVisible(false);
        }
        
        Text_Bank = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Bank"));
        
        if(Text_Bank)
        {
            Text_Bank->setString(mstr_Bank_name);
        }
        
        setCheckVisible(false);
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixBankItemCell::setCheckVisible(bool bVisible)
{
    mbln_CheckVisible = bVisible;
    
    if(Image_Check_Bank)
    {
        Image_Check_Bank->setVisible(!mbln_CheckVisible);
    }
    
    if(Image_Check_Bank_on)
    {
        Image_Check_Bank_on->setVisible(mbln_CheckVisible);
    }
}

bool PixBankItemCell::isCheckVisible()
{
    return mbln_CheckVisible;
}

