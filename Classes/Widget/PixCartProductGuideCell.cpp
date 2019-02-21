//
//  PixCartProductGuideCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 10..
//
//

#include "PixCartProductGuideCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Layer/PixCartProductLayer.h"
#include "../Common.h"

PixCartProductGuideCell* PixCartProductGuideCell::create()
{
    PixCartProductGuideCell* widget = new (std::nothrow) PixCartProductGuideCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCartProductGuideCell::PixCartProductGuideCell()
: _customItem(nullptr)
{
    
}

PixCartProductGuideCell::~PixCartProductGuideCell()
{
    
}

bool PixCartProductGuideCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCartProductGuideCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixCartProductGuideCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("12_Oder/MenuCartItem_04.csb");
    
    if(_customItem)
    {
        Size Text_TitleSize = Size::ZERO;
        Vec2 Text_TitlePos = Vec2::ZERO;
        
        ui::Text *Text_title = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_title"));
        
        if(Text_title)
        {
            Text_title->setString(CODE_NAME(Text_title->getString()));
            Text_TitleSize = Text_title->getContentSize();
            Text_TitlePos = Text_title->getPosition();
        }
        
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button_Infor"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            pos.x = (Text_TitlePos.x + Text_TitleSize.width) + (button->getContentSize().width * 0.5f);
            pos.x += 15.0f;
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixCartProductGuideCell::buttonProductGuidPressed, this));
            
            PixScrollMenu *Button_Infor = PixScrollMenu::create(menuItem, nullptr);
            Button_Infor->setPosition(pos);
            
            Button_Infor->setContentSize(button->getContentSize());
            _customItem->addChild(Button_Infor);
            
            button->removeFromParentAndCleanup(true);
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

#pragma mark - Button

void PixCartProductGuideCell::buttonProductGuidPressed(cocos2d::Ref *pSender)
{
    PixCartProductLayer *cartProductLayer = PixCartProductLayer::create();
    Director::getInstance()->getRunningScene()->addChild(cartProductLayer);
}
