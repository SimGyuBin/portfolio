//
//  PixScrollMenu.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 2..
//
//

#include "PixScrollMenu.h"

bool PixScrollMenu::init()
{
    return initWithArray(Vector<MenuItem*>());
}

bool PixScrollMenu::initWithArray( const Vector<MenuItem*>& arrayOfItems )
{
    if (Layer::init()) {
        _enabled = true;
        
        Size sizeVisible = Director::getInstance()->getWinSize();
        
        this->setIgnoreAnchorPointForPosition(true);
//        setAnchorPoint(Vec2(0.5f, 0.5f));
//        this->setContentSize(sizeVisible);
//        
//        setPosition(Vec2(sizeVisible.width/2, sizeVisible.height/2));
        
        int count = 0;
        
        for (auto& item : arrayOfItems) {
            this->addChild(item, count);
            count++;
        }
        
        _selectedItem = (*arrayOfItems.end());
        _state = Menu::State::WAITING;
        
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
        
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(false);
        touchListener->onTouchBegan = CC_CALLBACK_2(PixScrollMenu::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(PixScrollMenu::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(PixScrollMenu::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(PixScrollMenu::onTouchCancelled, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        _scrollMenuTouchListener = touchListener;
        
        return true;
    }
    return false;
}

PixScrollMenu* PixScrollMenu::createWithArray( const Vector<MenuItem*>& arrayOfItems )
{
    auto ret = new PixScrollMenu();
    if (ret && ret->initWithArray(arrayOfItems)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

PixScrollMenu* PixScrollMenu::createWithItem( MenuItem* item )
{
    return PixScrollMenu::create(item, nullptr);
}

PixScrollMenu* PixScrollMenu::createWithItems( MenuItem *item, va_list args )
{
    Vector<MenuItem*> items;
    if( item ) {
        items.pushBack(item);
        MenuItem *i = va_arg(args, MenuItem*);
        while(i) {
            items.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }
    return PixScrollMenu::createWithArray(items);
}

PixScrollMenu* PixScrollMenu::create( MenuItem* item, ... )
{
    va_list args;
    va_start(args,item);
    
    PixScrollMenu *ret = PixScrollMenu::createWithItems(item, args);
    
    va_end(args);
    
    return ret;
}

bool PixScrollMenu::onTouchBegan(Touch* touch, Event* event)
{
    m_touchesStart = touch->getStartLocation();
    
    return Menu::onTouchBegan(touch, event);
    
}

void PixScrollMenu::onTouchMoved(Touch* touch, Event* event)
{
    Menu::onTouchMoved(touch, event);
}

void PixScrollMenu::onTouchEnded(Touch* touch, Event *event)
{
    // move은 가로 or 세로로 얼마만큼 Move되었나를 체크하여 그 이상이면 버튼 동작을 해지 합니다.
    // 값이 낮을수록 민감하게 반응하지만 너무 낮으면 버튼 터치가 힘들 수 있습니다.
    float move = 30.f;
    
    m_touchesEnd = touch->getLocation();
    Vec2 difference = m_touchesEnd - m_touchesStart;
    
    if (difference.x > move || difference.y > move )
    {
        Menu::onTouchCancelled(touch, event);
    }
    else if (difference.x < -move || difference.y < -move)
    {
        Menu::onTouchCancelled(touch, event);
    }
    else
    {
        Menu::onTouchEnded(touch, event);
    }
}
