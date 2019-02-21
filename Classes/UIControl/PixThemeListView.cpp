//
//  PixThemeListView.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 24..
//
//

#include "PixThemeListView.h"
#include "PixThemePageView.h"

PixThemeListView* PixThemeListView::create()
{
    PixThemeListView* widget = new (std::nothrow) PixThemeListView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixThemeListView::PixThemeListView()
{
    
}

PixThemeListView::~PixThemeListView()
{
    
}

bool PixThemeListView::init()
{
    if (PixGalleryListView::init())
    {
        return true;
    }
    
    return false;
}

void PixThemeListView::onEnter()
{
    PixGalleryListView::onEnter();
    setTouchEnabled(false);
    setBounceEnabled(true);
}

void PixThemeListView::onEnterTransitionDidFinish()
{
    PixGalleryListView::onEnterTransitionDidFinish();
}

void PixThemeListView::update(float dt)
{
    PixGalleryListView::update(dt);
}

bool PixThemeListView::isPaging(const Vec2 &end, const Vec2 &start)
{
    Vec2 difference = end - start;
    Vec2 origin(0, 1);
    float angle = difference.getAngle(origin);
    angle = CC_RADIANS_TO_DEGREES(angle);
    
    bool isPaging = false;
    
    do
    {
        if (-90.0f * 1 / 4 <= angle && angle < 90.0f * 1 / 4)
        {
            // 북
        }
        else if (90.0f * 1 / 4 <= angle && angle < 90.0f * 3 / 4)
        {
            // 북동
        }
        else if (90.0f * 3 / 4 <= angle && angle < 90.0f * 5 / 4)
        {
            // 동
            isPaging = true;
            break;
        }
        else if (90.0f * 5 / 4 <= angle && angle < 90.0f * 7 / 4)
        {
            // 남동
        }
        else if ((90.0f * 7 / 4 <= angle && angle <= 90.0f * 8 / 4) || (-90.0f * 8 / 4 <= angle && angle < -90.0f * 7 / 4))
        {
            // 남
        }
        else if (-90.0f * 7 / 4 <= angle && angle < -90.0f * 5 / 4)
        {
            // 남서
        }
        else if (-90.0f * 5 / 4 <= angle && angle < -90.0f * 3 / 4)
        {
            // 서
            isPaging = true;
            break;
        }
        else if (-90.0f * 3 / 4 <= angle && angle < -90.0f * 1 / 4)
        {
            // 북서
        }
        else
        {
            CCLOG("PixThemeListView isPaging - where is the error?");
        }
        
    } while (0);
    
    return isPaging;

}

#pragma mmark - Touch

void PixThemeListView::interceptTouchEvent(Widget::TouchEventType event,Widget* sender,Touch* touch)
{
    PixGalleryListView::interceptTouchEvent(event, sender, touch);
}

bool PixThemeListView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    return PixGalleryListView::onTouchBegan(touch, unusedEvent);
}

void PixThemeListView::onTouchMoved(Touch *touch, Event *unusedEvent)
{
//    if(isPaging(touch->getLocation(), touch->getPreviousLocation()))
//    {
//        setTouchEnabled(false);
//        
//        PixGalleryListView::onTouchCancelled(touch, unusedEvent);
//    }
//    else
    {
        PixGalleryListView::onTouchMoved(touch, unusedEvent);
    }
}

void PixThemeListView::onTouchEnded(Touch *touch, Event *unusedEvent)
{
//    if(isPaging(touch->getLocation(), touch->getPreviousLocation()))
//    {
//        setTouchEnabled(false);
//        
//        PixGalleryListView::onTouchCancelled(touch, unusedEvent);
//    }
//    else
    {
        PixGalleryListView::onTouchEnded(touch, unusedEvent);
    }
}

void PixThemeListView::onTouchCancelled(Touch *touch, Event *unusedEvent)
{    
    PixGalleryListView::onTouchCancelled(touch, unusedEvent);
}
