//
//  PixOrderMoreCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 13..
//
//

#include "PixOrderMoreCell.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Common.h"

PixOrderMoreCell* PixOrderMoreCell::create(const ccMenuCallback& callback)
{
    PixOrderMoreCell* widget = new (std::nothrow) PixOrderMoreCell();
    if (widget && widget->init())
    {
        widget->createCustomItem(callback);
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixOrderMoreCell::PixOrderMoreCell()
: _customItem(nullptr)
{
    
}

PixOrderMoreCell::~PixOrderMoreCell()
{
    
}

bool PixOrderMoreCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixOrderMoreCell::onEnter()
{
    Widget::onEnter();
}

void PixOrderMoreCell::createCustomItem(const ccMenuCallback& callback)
{
    _customItem = CSLOADER_MANAGER->getCustomItem("12_Oder/MenuOrderItem_05.csb");
    
    if(_customItem)
    {
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_More"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  callback);
            
            PixScrollMenu *Button_More = PixScrollMenu::create(menuItem, nullptr);
            Button_More->setPosition(pos);
            
            Button_More->setContentSize(button->getContentSize());
            _customItem->addChild(Button_More);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text_More"));
            
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
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}
