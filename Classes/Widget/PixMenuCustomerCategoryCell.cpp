//
//  PixMenuCustomerCategoryCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 28..
//
//

#include "PixMenuCustomerCategoryCell.h"
#include "../Manager/PixMessageManager.h"
#include "../Info/FaqInfo.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Common.h"

PixMenuCustomerCategoryCell* PixMenuCustomerCategoryCell::create(FaqCategoryInfo *pFaqCategoryInfo)
{
    PixMenuCustomerCategoryCell* widget = new (std::nothrow) PixMenuCustomerCategoryCell(pFaqCategoryInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixMenuCustomerCategoryCell::PixMenuCustomerCategoryCell(FaqCategoryInfo *pFaqCategoryInfo)
: _customItem(nullptr)
, mFaqCategoryInfo(pFaqCategoryInfo)
, mDelegate(nullptr)
{
    
}

PixMenuCustomerCategoryCell::~PixMenuCustomerCategoryCell()
{
    
}

bool PixMenuCustomerCategoryCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixMenuCustomerCategoryCell::onEnter()
{
    Widget::onEnter();
}

void PixMenuCustomerCategoryCell::createCustomItem()
{
    if(mFaqCategoryInfo == nullptr)
        return;
    
    _customItem = CSLoader::createNode("11_Menu/MenuCustomerPop_item.csb");
    
    if(_customItem)
    {
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_Category"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixMenuCustomerCategoryCell::buttonCategoryPressed, this));
            
            PixScrollMenu *Button_Category = PixScrollMenu::create(menuItem, nullptr);
            Button_Category->setPosition(pos);
            
            Button_Category->setContentSize(button->getContentSize());
            _customItem->addChild(Button_Category);
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
            
            if(Text)
            {
                Text->setString(mFaqCategoryInfo->getCategory_name());
                
                ui::Text *clone = dynamic_cast<ui::Text*>(Text->clone());
                
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

#pragma mark - Button

void PixMenuCustomerCategoryCell::buttonCategoryPressed(cocos2d::Ref *pSender)
{
    if(mDelegate)
    {
        mDelegate->categoryPressed(mFaqCategoryInfo);
    }
}
