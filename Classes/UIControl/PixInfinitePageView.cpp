//
//  PixInfinitePageView.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 7..
//
//

#include "PixInfinitePageView.h"

PixInfinitePageView* PixInfinitePageView::create()
{
    PixInfinitePageView* widget = new (std::nothrow) PixInfinitePageView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixInfinitePageView::PixInfinitePageView()
: mn_realItemSize(0)
{
    
}

PixInfinitePageView::~PixInfinitePageView()
{
    
}

bool PixInfinitePageView::init()
{
    if (PixPageView::init())
    {
        return true;
    }
    
    return false;
}

void PixInfinitePageView::addPage(Widget* page)
{
    _frontItems.pushBack(page);
    _backItems.pushBack(page);
    
    pushBackCustomItem(page);
}

void PixInfinitePageView::setInfinitePageView()
{
    mn_realItemSize = _items.size();
     
    for (auto& item : _frontItems)
    {
        _items.pushBack(item);
    }
    
    for (auto& item : _backItems)
    {
        _items.pushBack(item);
    }

    setCurrentPageIndex(mn_realItemSize);
}

void PixInfinitePageView::handleReleaseLogic(Touch *touch)
{
    // Use `ScrollView` method in order to avoid `startMagneticScroll()` by `ListView`.
    ScrollView::handleReleaseLogic(touch);
    
    if(_items.empty())
    {
        return;
    }
    Vec2 touchMoveVelocity = flattenVectorByDirection(calculateTouchMoveVelocity());
    
    static const float INERTIA_THRESHOLD = 500;
    if(touchMoveVelocity.length() < INERTIA_THRESHOLD)
    {
        startMagneticScroll();
    }
    else
    {
        // Handle paging by inertia force.
        Widget* currentPage = getItem(_currentPageIndex);
        Vec2 destination = calculateItemDestination(Vec2::ANCHOR_MIDDLE, currentPage, Vec2::ANCHOR_MIDDLE);
        Vec2 deltaToCurrentpage = destination - getInnerContainerPosition();
        deltaToCurrentpage = flattenVectorByDirection(deltaToCurrentpage);
        
        // If the direction of displacement to current page and the direction of touch are same, just start magnetic scroll to the current page.
        // Otherwise, move to the next page of touch direction.
        if(touchMoveVelocity.x * deltaToCurrentpage.x > 0 || touchMoveVelocity.y * deltaToCurrentpage.y > 0)
        {
            startMagneticScroll();
        }
        else
        {
            if(touchMoveVelocity.x < 0 || touchMoveVelocity.y > 0)
            {
                ++_currentPageIndex;
            }
            else
            {
                --_currentPageIndex;
            }
           
            if(_currentPageIndex >= _items.size() - 1)
            {
                for(int i = 0; i < mn_realItemSize; ++i)
                {
                    _items.erase(i);
                }
                
                for (auto& item : _frontItems)
                {
                    _items.pushBack(item);
                }
                
                _currentPageIndex = (_items.size() - 1) * 2;
            }
            
            if(_currentPageIndex <= 0)
            {
                for(int i = 0; i < mn_realItemSize; ++i)
                {
                    _items.popBack();
                }
                
                for (auto& item : _backItems)
                {
                    _items.insert(0, item);
                }
                
                _currentPageIndex = _items.size() - 1;
            }

//            _currentPageIndex = MIN(_currentPageIndex, _items.size() - 1);
//            _currentPageIndex = MAX(_currentPageIndex, 0);
            scrollToItem(_currentPageIndex);
        }
    }
}
