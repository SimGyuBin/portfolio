//
//  PixPageNavigationCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 8. 4..
//
//

#include "PixPageNavigationCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Util/PixUtils.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Common.h"

PixPageNavigationCell* PixPageNavigationCell::create(int nIndex, PixGalleryListView *pListView)
{
    PixPageNavigationCell* widget = new (std::nothrow) PixPageNavigationCell(nIndex, pListView);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPageNavigationCell::PixPageNavigationCell(int nIndex, PixGalleryListView *pListView)
: _customItem(nullptr)
, normal(nullptr)
, pressed(nullptr)
, disabled(nullptr)
, ListView(pListView)
, mn_Index(nIndex)
, mDelegate(nullptr)
{
    
}

PixPageNavigationCell::~PixPageNavigationCell()
{
    
}

bool PixPageNavigationCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPageNavigationCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixPageNavigationCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("05_AlbumView/Pagenumber_cell.csb");
    
    if(_customItem)
    {
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_Pagenumber"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItemNormal = MenuItemImage::create(normalImg, pressedImg,
                                                        CC_CALLBACK_1(PixPageNavigationCell::ButtonNormalPressed, this));
            
            normal = PixScrollMenu::create(menuItemNormal, nullptr);
            normal->setPosition(pos);
            normal->setContentSize(button->getContentSize());
            normal->setVisible(true);
            _customItem->addChild(normal);
            
            auto menuItemPressed = MenuItemImage::create(pressedImg, pressedImg,
                                                          CC_CALLBACK_1(PixPageNavigationCell::ButtonPressedPressed, this));
            
            pressed = PixScrollMenu::create(menuItemPressed, nullptr);
            pressed->setPosition(pos);
            pressed->setContentSize(button->getContentSize());
            pressed->setVisible(false);
            _customItem->addChild(pressed);
            
            auto menuItemDisabled = MenuItemImage::create(disabledImg, disabledImg,
                                                          CC_CALLBACK_1(PixPageNavigationCell::ButtonDisabledPressed, this));
            
            disabled = PixScrollMenu::create(menuItemDisabled, nullptr);
            disabled->setPosition(pos);
            disabled->setContentSize(button->getContentSize());
            disabled->setVisible(false);
            _customItem->addChild(disabled);
            
            std::string strPage = SMARTALBUM_MANAGER->getPageString(getIndex());
            
            ui::Text *Text = dynamic_cast<ui::Text*>(button->getChildByName("Text"));
            
            if(Text)
            {
                Text = dynamic_cast<ui::Text*>(Text->clone());
                Text->setString(strPage);
                menuItemNormal->addChild(Text);
                
                Text = dynamic_cast<ui::Text*>(Text->clone());
                Text->setString(strPage);
                menuItemPressed->addChild(Text);
            }
            
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(button->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1 = dynamic_cast<ui::Text*>(Text_1->clone());
                Text_1->setString(strPage);
                menuItemDisabled->addChild(Text_1);
            }
            
            button->removeFromParentAndCleanup(true);
        }

        setDisabled(false);
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixPageNavigationCell::setDisabled(bool var)
{
    if(var)
    {
        if(normal)
        {
            normal->setVisible(false);
        }
        
        if(pressed)
        {
            pressed->setVisible(false);
        }
        
        if(disabled)
        {
            disabled->setVisible(true);
        }
    }
    else
    {
        if(normal)
        {
            normal->setVisible(true);
        }
        
        if(pressed)
        {
            pressed->setVisible(true);
        }
        
        if(disabled)
        {
            disabled->setVisible(false);
        }
    }
}

void PixPageNavigationCell::setPressed(bool var)
{
    if(var)
    {
        if(normal)
        {
            normal->setVisible(false);
        }
        
        if(pressed)
        {
            pressed->setVisible(true);
        }
        
        if(disabled)
        {
            disabled->setVisible(false);
        }
    }
    else
    {
        if(normal)
        {
            normal->setVisible(true);
        }
        
        if(pressed)
        {
            pressed->setVisible(false);
        }
        
        if(disabled)
        {
            disabled->setVisible(false);
        }
    }
}

#pragma mark - Button

void PixPageNavigationCell::ButtonNormalPressed(cocos2d::Ref *pSender)
{
    if(ListView)
    {
        Vec2 offset = ListView->getOffset();
        Size size = ListView->getContentSize();
        float scale = ListView->getScale();
        
        Rect viewRect;
        
        viewRect = Rect(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale);
        
        Rect boundingBox = this->getBoundingBox();
        boundingBox.origin.x += this->getContentSize().width;
        
        if(viewRect.intersectsRect(boundingBox))
        {
            if(mDelegate)
            {
                mDelegate->pageNavigationCellPressed(getIndex());
            }
        }
    }
    else
    {
        if(mDelegate)
        {
            mDelegate->pageNavigationCellPressed(getIndex());
        }
    }
    
}

void PixPageNavigationCell::ButtonPressedPressed(cocos2d::Ref *pSender)
{
    
}

void PixPageNavigationCell::ButtonDisabledPressed(cocos2d::Ref *pSender)
{
    
}
