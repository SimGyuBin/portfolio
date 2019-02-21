//
//  PixPayDeliveryInfoCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 20..
//
//

#include "PixPayDeliveryInfoCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPayDeliveryInfoCell* PixPayDeliveryInfoCell::create()
{
    PixPayDeliveryInfoCell* widget = new (std::nothrow) PixPayDeliveryInfoCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPayDeliveryInfoCell::PixPayDeliveryInfoCell()
: _customItem(nullptr)
, Button_set_List(nullptr)
, Button_del_List(nullptr)
, Button_set_New(nullptr)
, Button_del_New(nullptr)
, me_DeliveryType(CARTLAYER_NEW)
, mDelegate(nullptr)
{

}

PixPayDeliveryInfoCell::~PixPayDeliveryInfoCell()
{
    
}

bool PixPayDeliveryInfoCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPayDeliveryInfoCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}

void PixPayDeliveryInfoCell::update(float dt)
{
    Widget::update(dt);
    updateDeliveryInfoUI();
}

void PixPayDeliveryInfoCell::onExit()
{
    unscheduleUpdate();
    Widget::onExit();
}

void PixPayDeliveryInfoCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPayItem_02.csb");
    
    if(_customItem)
    {
        ui::Text *Text_Title = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Title"));
        
        if(Text_Title)
        {
            Text_Title->setString(CODE_NAME(Text_Title->getString()));
        }
        
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_set_List"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixPayDeliveryInfoCell::buttonSetListPressed, this));
            
            Button_set_List = PixScrollMenu::create(menuItem, nullptr);
            Button_set_List->setPosition(pos);
            
            Button_set_List->setContentSize(button->getContentSize());
            _customItem->addChild(Button_set_List);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
            
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
        
        button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_del_List"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixPayDeliveryInfoCell::buttonSetListPressed, this));
            
            Button_del_List = PixScrollMenu::create(menuItem, nullptr);
            Button_del_List->setPosition(pos);
            
            Button_del_List->setContentSize(button->getContentSize());
            _customItem->addChild(Button_del_List);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
            
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
        
        button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_set_New"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixPayDeliveryInfoCell::buttonSetNewPressed, this));
            
            Button_set_New = PixScrollMenu::create(menuItem, nullptr);
            Button_set_New->setPosition(pos);
            
            Button_set_New->setContentSize(button->getContentSize());
            _customItem->addChild(Button_set_New);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
            
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
        
        button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_del_New"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixPayDeliveryInfoCell::buttonSetNewPressed, this));
            
            Button_del_New = PixScrollMenu::create(menuItem, nullptr);
            Button_del_New->setPosition(pos);
            
            Button_del_New->setContentSize(button->getContentSize());
            _customItem->addChild(Button_del_New);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
            
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

void PixPayDeliveryInfoCell::updateDeliveryInfoUI()
{
    switch (me_DeliveryType)
    {
        case CARTLAYER_LIST:
        {
            if(Button_set_List)
            {
                Button_set_List->setVisible(false);
                Button_set_List->setEnabled(false);
            }
            
            if(Button_del_List)
            {
                Button_del_List->setVisible(true);
                Button_del_List->setEnabled(false);
            }
            
            if(Button_set_New)
            {
                Button_set_New->setVisible(true);
                Button_set_New->setEnabled(true);
            }
            
            if(Button_del_New)
            {
                Button_del_New->setVisible(false);
                Button_del_New->setEnabled(false);
            }
        }
        break;
            
        case CARTLAYER_NEW:
        {
            if(Button_set_List)
            {
                Button_set_List->setVisible(true);
                Button_set_List->setEnabled(true);
            }
            
            if(Button_del_List)
            {
                Button_del_List->setVisible(false);
                Button_del_List->setEnabled(false);
            }
            
            if(Button_set_New)
            {
                Button_set_New->setVisible(false);
                Button_set_New->setEnabled(false);

            }
            
            if(Button_del_New)
            {
                Button_del_New->setVisible(true);
                Button_del_New->setEnabled(false);
            }
        }
        break;
            
        case CARTLAYER_ONLY_NEW:
        {
            if(Button_set_List)
            {
                Button_set_List->setVisible(true);
                Button_set_List->setEnabled(false);
            }
            
            if(Button_del_List)
            {
                Button_del_List->setVisible(false);
                Button_del_List->setEnabled(false);
            }
            
            if(Button_set_New)
            {
                Button_set_New->setVisible(false);
                Button_set_New->setEnabled(false);
                
            }
            
            if(Button_del_New)
            {
                Button_del_New->setVisible(true);
                Button_del_New->setEnabled(false);
            }
        }
        break;
   
        default:
            break;
    }
}

#pragma mark - Button

void PixPayDeliveryInfoCell::buttonSetListPressed(cocos2d::Ref *pSender)
{
    setDeliveryType(CARTLAYER_LIST);
    
    if(mDelegate)
    {
        mDelegate->payDeliveryInfoCellListPressed();
    }
}

void PixPayDeliveryInfoCell::buttonSetNewPressed(cocos2d::Ref *pSender)
{
    setDeliveryType(CARTLAYER_NEW);
    
    if(mDelegate)
    {
        mDelegate->payDeliveryInfoCellNewPressed();
    }
}
