//
//  PixMenuCustomerLayer_FAQ_Cell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#include "PixMenuCustomerLayer_FAQ_Cell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Layer/PixMenuCustomerLayer.h"

PixMenuCustomerLayer_FAQ_Cell* PixMenuCustomerLayer_FAQ_Cell::create(FaqInfo *pFaqInfo)
{
    PixMenuCustomerLayer_FAQ_Cell* widget = new (std::nothrow) PixMenuCustomerLayer_FAQ_Cell(pFaqInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixMenuCustomerLayer_FAQ_Cell::PixMenuCustomerLayer_FAQ_Cell(FaqInfo *pFaqInfo)
: _customItem(nullptr)
, menu(nullptr)
, Text_question(nullptr)
, mMenuCustomerLayer(nullptr)
{
    this->mFaqInfo = pFaqInfo;
}

PixMenuCustomerLayer_FAQ_Cell::~PixMenuCustomerLayer_FAQ_Cell()
{
    
}

bool PixMenuCustomerLayer_FAQ_Cell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixMenuCustomerLayer_FAQ_Cell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixMenuCustomerLayer_FAQ_Cell::createCustomItem()
{
    if(mFaqInfo == nullptr)
        return;
    
    _customItem = CSLOADER_MANAGER->getCustomItem("11_Menu/MenuCustomerLayer_FAQ_item.csb");
    
    if(_customItem)
    {
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_question"));
        
        if(button)
        {
            Text_question = dynamic_cast<ui::Text*>(button->getChildByName("Text_question"));
            
            if(Text_question)
            {
                Text_question->setString(mFaqInfo->getTitle());
            }
            
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixMenuCustomerLayer_FAQ_Cell::ButtonFAQCellPressed, this));
            
            menu = PixScrollMenu::create(menuItem, nullptr);
            menu->setPosition(pos);
            menu->setContentSize(button->getContentSize());
            
            Text_question = dynamic_cast<ui::Text*>(Text_question->clone());
            
            menuItem->addChild(Text_question);
            _customItem->addChild(menu);
            
            button->removeFromParentAndCleanup(true);
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

#pragma mark - Button

void PixMenuCustomerLayer_FAQ_Cell::ButtonFAQCellPressed(cocos2d::Ref *pSender)
{
    
}
