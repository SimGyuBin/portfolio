//
//  PixPayDeliveryListCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 23..
//
//

#include "PixPayDeliveryListCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/DeliveryInfo.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPayDeliveryListCell *PixPayDeliveryListCell::create()
{
    PixPayDeliveryListCell* widget = new (std::nothrow) PixPayDeliveryListCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPayDeliveryListCell::PixPayDeliveryListCell()
: _customItem(nullptr)
, Node_MenuPayItem_06(nullptr)
, Node_MenuPayItem_04_2(nullptr)
, Node_MenuPayItem_07(nullptr)
, Button_More(nullptr)
, Text_Name(nullptr)
, Text_Phone(nullptr)
, Text_Address(nullptr)
, Text_Default(nullptr)
, memo(nullptr)
, mDeliveryInfo(nullptr)
, mDelegate(nullptr)
{
    
}

PixPayDeliveryListCell::~PixPayDeliveryListCell()
{
    
}

bool PixPayDeliveryListCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPayDeliveryListCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}

void PixPayDeliveryListCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixPayDeliveryListCell::onExit()
{
    unscheduleUpdate();
    Widget::onExit();
}

void PixPayDeliveryListCell::update(float dt)
{
    Widget::update(dt);
    updateDelivery();
}

void PixPayDeliveryListCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuPayItem_05.csb");
    
    if(_customItem)
    {
        Node_MenuPayItem_06 = dynamic_cast<Node*>(_customItem->getChildByName("Node_MenuPayItem_06"));
        
        if(Node_MenuPayItem_06)
        {
            ui::Button *button = dynamic_cast<ui::Button*>(Node_MenuPayItem_06->getChildByName("Button_More"));
            
            if(button)
            {
                std::string normalImg = button->getNormalFile().file;
                std::string pressedImg = button->getPressedFile().file;
                std::string disabledImg = button->getDisabledFile().file;
                
                Vec2 pos = button->getPosition();
                
                auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                      CC_CALLBACK_1(PixPayDeliveryListCell::ButtonMoerPressed, this));
                
                Button_More = PixScrollMenu::create(menuItem, nullptr);
                Button_More->setPosition(pos);
                
                Button_More->setContentSize(button->getContentSize());
                Node_MenuPayItem_06->addChild(Button_More);
                
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

        }
        
        Node_MenuPayItem_04_2 = dynamic_cast<Node*>(_customItem->getChildByName("Node_MenuPayItem_04_2"));
        
        if(Node_MenuPayItem_04_2)
        {
            Text_Name = dynamic_cast<ui::Text*>(Node_MenuPayItem_04_2->getChildByName("Text_Name"));
            
            if(Text_Name)
            {
                
            }
            
            Text_Phone = dynamic_cast<ui::Text*>(Node_MenuPayItem_04_2->getChildByName("Text_Phone"));
            
            if(Text_Phone)
            {
                
            }
          
            Text_Address = dynamic_cast<ui::Text*>(Node_MenuPayItem_04_2->getChildByName("Text_Address"));
            
            if(Text_Address)
            {
                
            }
            
            Text_Default = dynamic_cast<ui::Text*>(Node_MenuPayItem_04_2->getChildByName("Text_Default"));
            
            if(Text_Default)
            {
                Text_Default->setString(CODE_NAME(Text_Default->getString()));
            }
        }

        Node_MenuPayItem_07 = dynamic_cast<Node*>(_customItem->getChildByName("Node_MenuPayItem_07"));
        
        if(Node_MenuPayItem_07)
        {
            Sprite *sprite = dynamic_cast<Sprite*>(Node_MenuPayItem_07->getChildByName("Memo"));
            
            if(sprite)
            {
                ui::Text *Text = dynamic_cast<ui::Text*>(sprite->getChildByName("Text"));
                
                if(Text)
                {
                    Text->setString(CODE_NAME(Text->getString()));
                }
                
                ui::TextField *textField = dynamic_cast<ui::TextField*>(sprite->getChildByName("TextField"));
                
                if(textField)
                {
                    memo = PixEditBox::convertToEditBox(textField, this);
                    memo->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
                    memo->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
                }
            }
        }

        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

const char* PixPayDeliveryListCell::getMemo()
{
    if(memo)
    {
        return memo->getText();
    }
    
    return "";
}

void PixPayDeliveryListCell::updateDelivery()
{
    if(mDeliveryInfo == nullptr)
        return;
   
    float fPosX = 0.0f;
    
    if(Text_Name)
    {
        Text_Name->setString(mDeliveryInfo->getName());
        fPosX = Text_Name->getPositionX() + Text_Name->getContentSize().width + 15.0f;
    }
    
    if(Text_Phone)
    {
        Text_Phone->setString(mDeliveryInfo->getPhone1());
    }
    
    if(Text_Address)
    {
        std::string strAddress = mDeliveryInfo->getAddr1();
        strAddress.append(" ");
        strAddress.append(mDeliveryInfo->getAddr2());
        strAddress.append(" ");
        strAddress.append("(");
        strAddress.append(mDeliveryInfo->getPost());
        strAddress.append(")");
        
        Text_Address->setString(strAddress);
    }
    
    if(Text_Default)
    {
        Text_Default->setPositionX(fPosX);
        
        if(mDeliveryInfo->getLast())
        {
            Text_Default->setVisible(true);
        }
        else
        {
            Text_Default->setVisible(false);
        }
    }
}

#pragma mark - Button

void PixPayDeliveryListCell::ButtonMoerPressed(cocos2d::Ref *pSender)
{
    if(mDelegate)
    {
        mDelegate->payDeliveryListCellMoerPressed();
    }
}

#pragma mark - EditBox

void PixPayDeliveryListCell::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    
}

void PixPayDeliveryListCell::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    
}

void PixPayDeliveryListCell::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    
}

void PixPayDeliveryListCell::editBoxReturn(ui::EditBox* editBox)
{
    
}
