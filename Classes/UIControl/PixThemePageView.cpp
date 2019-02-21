//
//  PixThemePageView.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 24..
//
//

#include "PixThemePageView.h"
#include "../Widget/PixThemeGroupCell.h"
#include "../Util/PixUtils.h"

PixThemePageView* PixThemePageView::create()
{
    PixThemePageView* widget = new (std::nothrow) PixThemePageView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixThemePageView::PixThemePageView()
: _touchesStart(Vec2::ZERO)
{
    
}

PixThemePageView::~PixThemePageView()
{
    
}

bool PixThemePageView::init()
{
    if (PixPageView::init())
    {
        setAutoScrollStopEpsilon(0.005f);
        return true;
    }
    
    return false;
}

void PixThemePageView::onEnter()
{
    PixPageView::onEnter();
    
    addEventListener(CC_CALLBACK_2(PixThemePageView::eventCallBack, this));
}
void PixThemePageView::onEnterTransitionDidFinish()
{
    PixPageView::onEnterTransitionDidFinish();
}

void PixThemePageView::update(float dt)
{
    PixPageView::update(dt);
}

bool PixThemePageView::isVerticalScroll(const Vec2 &end, const Vec2 &start)
{
    Vec2 difference = end - start;
    Vec2 origin(0, 1);
    float angle = difference.getAngle(origin);
    angle = CC_RADIANS_TO_DEGREES(angle);
    
    bool isVerticalScroll = false;
    
    do
    {
        if (-90.0f * 1 / 4 <= angle && angle < 90.0f * 1 / 4)
        {
            // 북
            isVerticalScroll = true;
            break;
        }
        else if (90.0f * 1 / 4 <= angle && angle < 90.0f * 3 / 4)
        {
            // 북동
            isVerticalScroll = true;
            break;
        }
        else if (90.0f * 3 / 4 <= angle && angle < 90.0f * 5 / 4)
        {
            // 동
        }
        else if (90.0f * 5 / 4 <= angle && angle < 90.0f * 7 / 4)
        {
            // 남동
            isVerticalScroll = true;
            break;
        }
        else if ((90.0f * 7 / 4 <= angle && angle <= 90.0f * 8 / 4) || (-90.0f * 8 / 4 <= angle && angle < -90.0f * 7 / 4))
        {
            // 남
            isVerticalScroll = true;
            break;
        }
        else if (-90.0f * 7 / 4 <= angle && angle < -90.0f * 5 / 4)
        {
            // 남서
            isVerticalScroll = true;
            break;
        }
        else if (-90.0f * 5 / 4 <= angle && angle < -90.0f * 3 / 4)
        {
            // 서
        }
        else if (-90.0f * 3 / 4 <= angle && angle < -90.0f * 1 / 4)
        {
            // 북서
            isVerticalScroll = true;
            break;
        }
        else
        {
            CCLOG("PixThemeListView isPaging - where is the error?");
        }
        
    } while (0);
    
    return isVerticalScroll;
}

void PixThemePageView::setThemeListViewTouchEnabled(bool enabled)
{
    Vector<Widget*>& items = getItems();
    ssize_t nCount = items.size();
    
    for (ssize_t i = 0; i < nCount; ++i)
    {
        PixThemeGroupCell *cell = dynamic_cast<PixThemeGroupCell*>(items.at(i));
        
        if(cell)
        {
            cell->setThemeListViewTouchEnabled(enabled);
        }
    }
}

void PixThemePageView::pageTurningEvent()
{
    PixPageView::pageTurningEvent();
}

#pragma mmark - Touch

void PixThemePageView::interceptTouchEvent(Widget::TouchEventType event,Widget* sender,Touch* touch)
{
    PixPageView::interceptTouchEvent(event, sender, touch);
}

bool PixThemePageView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    _touchesStart = touch->getLocation();
    
    ssize_t nPageIndex = getCurrentPageIndex();
    PixThemeGroupCell *cell = dynamic_cast<PixThemeGroupCell*>(getItem(nPageIndex));
    
    if(cell)
    {
        PixThemeListView *themeListView = cell->getThemeListView();
        
        if(themeListView)
        {
            if(NodeContainsPoint(themeListView, touch->getLocation()))
            {
                themeListView->onTouchBegan(touch, unusedEvent);
            }
            else
            {
                themeListView->onTouchCancelled(touch, unusedEvent);
            }
        }
    }

    return PixPageView::onTouchBegan(touch, unusedEvent);
}

void PixThemePageView::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    if(isVerticalScroll(touch->getLocation(), touch->getPreviousLocation()))
    {
        ssize_t nPageIndex = getCurrentPageIndex();
        PixThemeGroupCell *cell = dynamic_cast<PixThemeGroupCell*>(getItem(nPageIndex));
        
        if(cell)
        {
            PixThemeListView *themeListView = cell->getThemeListView();
            
            if(themeListView)
            {
                Size size = this->getContentSize();
                float scale = this->getScale();
                
                Rect viewRect;
                Vec2 offset = _innerContainer->getPosition();

                viewRect = Rect(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale);
                
                Rect r = viewRect.unionWithRect(cell->getBoundingBox());
                
                float move = 30.f;
                Size difference = r.size - cell->getContentSize();
                
                if (difference.width > move || difference.height > move)
                {
                    
                }
                else if (difference.width < -move || difference.height < -move)
                {
                    
                }
                else
                {
                    themeListView->onTouchMoved(touch, unusedEvent);
                }
                
//                if(NodeContainsPoint(themeListView, touch->getLocation()))
//                {
//                    themeListView->onTouchMoved(touch, unusedEvent);
//                }
            }
        }
    }
    else
    {
        PixPageView::onTouchMoved(touch, unusedEvent);
    }
}

void PixThemePageView::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    if(isVerticalScroll(touch->getLocation(), touch->getPreviousLocation()))
    {
        ssize_t nPageIndex = getCurrentPageIndex();
        PixThemeGroupCell *cell = dynamic_cast<PixThemeGroupCell*>(getItem(nPageIndex));
        
        if(cell)
        {
            PixThemeListView *themeListView = cell->getThemeListView();
         
            if(themeListView)
            {
                if(NodeContainsPoint(themeListView, touch->getLocation()))
                {
                    themeListView->onTouchEnded(touch, unusedEvent);
                    
                    float move = 30.f;
                    Vec2 difference = touch->getLocation() - _touchesStart;
                    
                    if (difference.x > move || difference.y > move)
                    {
                        
                    }
                    else if (difference.x < -move || difference.y < -move)
                    {
                        
                    }
                    else
                    {
                        cell->themeListViewTouchEndedCallback(touch, unusedEvent);
                    }
                }
                else
                {
                    themeListView->onTouchCancelled(touch, unusedEvent);
                }
            }
        }
    }
    
    _touchesStart = Vec2::ZERO;
    
    PixPageView::onTouchEnded(touch, unusedEvent);
}

void PixThemePageView::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    ssize_t nPageIndex = getCurrentPageIndex();
    PixThemeGroupCell *cell = dynamic_cast<PixThemeGroupCell*>(getItem(nPageIndex));
    
    if(cell)
    {
        PixThemeListView *themeListView = cell->getThemeListView();
        
        if(themeListView)
        {
            themeListView->onTouchCancelled(touch, unusedEvent);
        }
    }
    
    PixPageView::onTouchCancelled(touch, unusedEvent);
}

#pragma mmark - Event

void PixThemePageView::eventCallBack(cocos2d::Ref* pSender,  ScrollView::EventType type)
{
    if(type == ScrollView::EventType::AUTOSCROLL_ENDED)
    {
        ssize_t nPageIndex = getCurrentPageIndex();
        ssize_t nCount = getItems().size();
        
        for (ssize_t i = 0; i < nCount; ++i)
        {
            if(i != nPageIndex)
            {
                PixThemeGroupCell *cell = dynamic_cast<PixThemeGroupCell*>(getItem(i));
                
                if(cell)
                {
                    cell->themeListViewScrollToTop();
                }
            }
        }
    }
}
