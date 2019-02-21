//
//  PixMenuNoticeCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#include "PixMenuNoticeCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Layer/PixMenuNoticeiDetailLayer.h"
#include "../Util/PixStringUtils.h"
#include "../Manager/PixServerUrlManager.h"

PixMenuNoticeCell* PixMenuNoticeCell::create(NoticeInfo *pInfo)
{
    PixMenuNoticeCell* widget = new (std::nothrow) PixMenuNoticeCell(pInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixMenuNoticeCell::PixMenuNoticeCell(NoticeInfo *pInfo)
: _customItem(nullptr)
, Text_title(nullptr)
, Text_day(nullptr)
, mInfo(nullptr)
, mDelegate(nullptr)
{
    this->mInfo = pInfo;
}

PixMenuNoticeCell::~PixMenuNoticeCell()
{
   
}

bool PixMenuNoticeCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixMenuNoticeCell::onEnter()
{
    Widget::onEnter();
}

void PixMenuNoticeCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixMenuNoticeCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
}

void PixMenuNoticeCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("11_Menu/MenuNoticeItem.csb");
    
    if(_customItem)
    {
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button"));
        
        if(button)
        {
            Text_title = dynamic_cast<ui::Text*>(button->getChildByName("Text_title"));
            
            if(Text_title)
            {
                Text_title->setString(mInfo->getTitle());
            }
            
            Text_day = dynamic_cast<ui::Text*>(button->getChildByName("Text_day"));
            
            if(Text_day)
            {
                std::string strDate = mInfo->getDate().substr(0,10);
                std::vector<std::string> tokens;
                PixStringUtils::getTokens(tokens, strDate, "-");
                
                strDate = StringUtils::format("%s.%s.%s",tokens[0].c_str(),tokens[1].c_str(),tokens[2].c_str());
                Text_day->setString(strDate);
            }
            
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixMenuNoticeCell::ButtonNoticeCellPressed, this));
            
            PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
            menu->setPosition(pos);
            menu->setContentSize(button->getContentSize());
            
            Text_title = dynamic_cast<ui::Text*>(Text_title->clone());
            Text_day = dynamic_cast<ui::Text*>(Text_day->clone());
            
            menuItem->addChild(Text_title);
            menuItem->addChild(Text_day);
            _customItem->addChild(menu);
            
            button->removeFromParentAndCleanup(true);
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}


#pragma mark - Button

void PixMenuNoticeCell::ButtonNoticeCellPressed(cocos2d::Ref *pSender)
{
    if(mInfo == nullptr)
        return;
    
    std::string strUrl = SREVERURL_MANAGER->getBoardWebViewUrl(BOARD_NOTICE,mInfo->getID());
       
    PixMenuNoticeiDetailLayer *menuNoticeiDetailLayer = PixMenuNoticeiDetailLayer::create(strUrl);
    menuNoticeiDetailLayer->setDelegate(mDelegate);
    Director::getInstance()->getRunningScene()->addChild(menuNoticeiDetailLayer);
}
