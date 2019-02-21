//
//  PixGalleryListView.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 26..
//
//

#include "PixGalleryListView.h"
#include "../Util/PixUtils.h"
#include "../Layer/PixSlideLeftMenu.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "PixQuickNavigation.h"

PixGalleryListView* PixGalleryListView::create()
{
    PixGalleryListView* widget = new (std::nothrow) PixGalleryListView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixGalleryListView::PixGalleryListView()
: _topLayer (nullptr)
, _bottomLayer(nullptr)
, _quickNavigation(nullptr)
, _listItemCallback(nullptr)
, _touchMovedCallback(nullptr)
, _touchEndedCallback(nullptr)
, _gestureSwipeCallback(nullptr)
, _isTouchMoved(false)
, _top(false)
, _bottom(false)
, _nextPage(false)
, _touchesStart(Vec2::ZERO)
, _slideLeftMenu(nullptr)
, gestureRecognizer(nullptr)
, mbln_LoadiongCell(false)
{
    initGestureRecognizer();
}

PixGalleryListView::~PixGalleryListView()
{
    CC_SAFE_DELETE(gestureRecognizer);
    CC_SAFE_DELETE(_quickNavigation);
}

bool PixGalleryListView::init()
{
    if (ListView::init())
    {
        return true;
    }
    
    return false;
}

void PixGalleryListView::onEnter()
{
    ListView::onEnter();
   
    removeScrollBar();
//    setScrollBarOpacity(0);
    setBounceEnabled(true);
    
}

void PixGalleryListView::onEnterTransitionDidFinish()
{
    ListView::onEnterTransitionDidFinish();
    
}

void PixGalleryListView::onExit()
{
    ListView::onExit();
}

void PixGalleryListView::update(float dt)
{
    ListView::update(dt);
          
    if(_isTransitionFinished)
    {
        Vec2 offset = _innerContainer->getPosition();
        
        for (int i = 0; i < getChildrenCount(); i++)
        {
            Node *child = getChildren().at(i);
            
            if(child)
            {
                Size size = this->getContentSize();
                float scale = this->getScale();
                
                Rect viewRect;
                
                viewRect = Rect(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale);
                
                if(viewRect.intersectsRect(child->getBoundingBox()))
                {
                    if(isVisible())
                    {
                        child->setVisible(true);
                    }
                    else
                    {
                        child->setVisible(false);
                    }
                }
                else
                {
                    child->setVisible(false);
                }
            }
        }
        
        Node *lastItem = nullptr;
        
        if(!_items.empty())
        {
            lastItem = getChildren().at(_items.size() - 1);
        }
        
        if(lastItem && lastItem->isVisible())
        {
            //bottom
            _bottom = true;
            _top = false;
        }
        else if(offset.y >= _contentSize.height - _innerContainer->getContentSize().height)
        {
            //top
            _bottom = false;
            _top = true;
        }
        else
        {
            _bottom = false;
            _top = false;
        }
        
        if(isVisible())
        {
            if(_quickNavigation)
            {
                _quickNavigation->updateQuickNavigation();
            }
        }
    }
}

void PixGalleryListView::setVisible(bool visible)
{
    ListView::setVisible(visible);
    
    if(visible)
    {
        scheduleUpdate();
        setEnabled(true);
    }
    else
    {
        unscheduleUpdate();
        setEnabled(false);
    }
}

void PixGalleryListView::copyClonedListView(ui::ListView *pListView)
{
    this->copyProperties(pListView);
    this->copyClonedWidgetChildren(pListView);
    
    if(pListView->getParent())
    {
        pListView->getParent()->addChild(this);
        pListView->getParent()->removeChild(pListView);
    }
    
    pListView->removeFromParentAndCleanup(true);
}

void PixGalleryListView::copyProperties(ui::ListView *pListView)
{
    ListView::copyProperties(pListView);
}

void PixGalleryListView::copyClonedWidgetChildren(ui::ListView *pListView)
{
    ListView::copyClonedWidgetChildren(pListView);
}

void PixGalleryListView::setTopLayer(Node* topLayer, bool blnTopMargin)
{
    _topLayer = topLayer;
    
    if(_topLayer != nullptr)
    {
        _topLayer->setLocalZOrder(this->getLocalZOrder() + 1);
        
        if(blnTopMargin == true)
        {
            ui::Widget *margin = Widget::create();
            margin->setContentSize(_topLayer->getContentSize());
            this->insertCustomItem(margin,_items.size());
        }
    }
}

void PixGalleryListView::setBottomLayer(Node* bottomLayer, bool blnBottomMargin)
{
    _bottomLayer = bottomLayer;
    
    if(_bottomLayer != nullptr)
    {
        _bottomLayer->setLocalZOrder(this->getLocalZOrder() + 1);
        
        if(blnBottomMargin == true)
        {
            ui::Widget *margin = Widget::create();
            margin->setContentSize(_bottomLayer->getContentSize());
            this->insertCustomItem(margin,_items.size());
        }
    }
}

Node* PixGalleryListView::getTopLayer()
{
    return _topLayer;
}

Node* PixGalleryListView::getBottomLayer()
{
    return _bottomLayer;
}

void PixGalleryListView::setListViewTouchEnabled(bool enabled)
{
    if(enabled)
    {
        setEnabled(true);
        initGestureRecognizer();
    }
    else
    {
        setEnabled(false);
        CC_SAFE_DELETE(gestureRecognizer);
    }
}

void PixGalleryListView::setLongPressEnabled(bool longPressEnabled)
{
    if(gestureRecognizer)
    {
        gestureRecognizer->setLongPressEnabled(longPressEnabled);
    }
}

bool PixGalleryListView::getLongPressEnabled()
{
    if(gestureRecognizer)
    {
        return gestureRecognizer->getLongPressEnabled();
    }
    
    return false;
}

void PixGalleryListView::setSelectListItemCallBack(onListItemCallBack callback)
{
    _listItemCallback = callback;
}

void PixGalleryListView::setTouchBeganCallBack(onTouchBeganCallback callback)
{
    _touchBeganCallback = callback;
}

void PixGalleryListView::setTouchMovedCallBack(onTouchCallback callback)
{
    _touchMovedCallback = callback;
}

void PixGalleryListView::setTouchEndedCallBack(onTouchCallback callback)
{
    _touchEndedCallback = callback;
}

void PixGalleryListView::setTouchCancelledCallBack(onTouchCallback callback)
{
    _touchCancelledCallback = callback;
}

void PixGalleryListView::setGestureSwipeCallBack(onGestureSwipeCallback callback)
{
    _gestureSwipeCallback = callback;
}

void PixGalleryListView::jumpToDestination(const Vec2 &des)
{
    doLayout();
    
    Vec2 destination = des;
    if(!_bounceEnabled)
    {
        Vec2 delta = destination - getInnerContainerPosition();
        Vec2 outOfBoundary = getHowMuchOutOfBoundary(delta);
        destination += outOfBoundary;
    }
    
    ui::ListView::jumpToDestination(des);
}

bool PixGalleryListView::isAsyncLoadCall()
{
    if(mbln_LoadiongCell)
    {
        return false;
    }
    
    if(_quickNavigation)
    {
        if(_quickNavigation->isMoved())
        {
            return false;
        }
    }
    
    if(_autoScrolling == false)
    {
        return true;
    }
    else
    {
        float percentage = MIN(1, _autoScrollAccumulatedTime / _autoScrollTotalTime);
        
        if(percentage > 0.05f)
        {
            return true;
        }
       
        return false;
    }
   
    return false;
}

void PixGalleryListView::createQuickNavigation()
{
    if(_quickNavigation == nullptr)
    {
        _quickNavigation = new PixQuickNavigation(this);
        _quickNavigation->createQuickNavigation();
    }
}

void PixGalleryListView::removeQuickNavigation()
{
    if(_quickNavigation)
    {
        _quickNavigation->removeQuickNavigation();
        CC_SAFE_DELETE(_quickNavigation);
    }
}

void PixGalleryListView::hideQuickNavigation()
{
    if(_quickNavigation)
    {
        _quickNavigation->hideQuickNavigation();
    }
}

void PixGalleryListView::setQuickNavigationPositionStart(float start)
{
    if(_quickNavigation)
    {
        float pos = start;
        
        if(getTopLayer())
        {
            pos -= getTopLayer()->getContentSize().height;
        }
        
        _quickNavigation->setStartPos(pos);
    }
}

void PixGalleryListView::setQuickNavigationPositionEnd(float end)
{
    if(_quickNavigation)
    {
        float pos = end;
  
//        if(getBottomLayer())
//        {
//            pos -= getBottomLayer()->getContentSize().height;
//        }
        
        _quickNavigation->setEndPos(pos);
    }
}

void PixGalleryListView::resetGestureRecognizer()
{
    CC_SAFE_DELETE(gestureRecognizer);
    initGestureRecognizer();
}

Vec2 PixGalleryListView::getOffset()
{
    return _innerContainer->getPosition();
}

void PixGalleryListView::initGestureRecognizer()
{
    if(gestureRecognizer)
        return;
    
    gestureRecognizer = new PixSimpleGestureRecognizer();
    gestureRecognizer->init();
    gestureRecognizer->setGestureHandler(this);
    
    // Enable all gesture kinds
    gestureRecognizer->setTapEnabled(true);
    gestureRecognizer->setDoubleTapEnabled(false);
    gestureRecognizer->setLongPressEnabled(false);
    gestureRecognizer->setPanEnabled(false);
    gestureRecognizer->setPinchEnabled(false);
    gestureRecognizer->setRotationEnabled(false);
    gestureRecognizer->setSwipeEnabled(false);
    
    // Taps will be fired immediately without waiting for double tap
    gestureRecognizer->setTapRequiresDoubleTapRecognitionToFail(false);

}

bool PixGalleryListView::checkDownScroll(Touch *touch)
{
    if(isNextPage() && isBottom())
    {
        Vec2 directionVec = touch->getLocation() - _touchesStart;
        
        Vec2 origin(0, 1);
        float angle = directionVec.getAngle(origin);
        angle = CC_RADIANS_TO_DEGREES(angle);
        
        if ((90.0f * 5 / 4 <= angle && angle < 90.0f * 7 / 4) ||
            (90.0f * 7 / 4 <= angle && angle <= 90.0f * 8 / 4) ||
            (-90.0f * 8 / 4 <= angle && angle < -90.0f * 7 / 4) ||
            (-90.0f * 7 / 4 <= angle && angle < -90.0f * 5 / 4))
        {

            return false;
        }
    }
    
    return true;
}

#pragma mmark - Touch

bool PixGalleryListView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    if(isVisible() == false)
    {
        return false;
    }
    
    if(_topLayer != nullptr)
    {
        if(NodeContainsPoint(_topLayer, touch->getLocation()))
        {
            return true;
        }
    }
    
    if(_bottomLayer != nullptr)
    {
        if(NodeContainsPoint(_bottomLayer, touch->getLocation()))
        {
            return true;
        }
    }
    
    if(_bottomLayer != nullptr)
    {
        if(NodeContainsPoint(_bottomLayer, touch->getLocation()))
        {
            return true;
        }
    }
    
    _isTouchMoved = true;
    
    if(gestureRecognizer)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            bool isGestureRecognizer = true;
            
            if(getTopLayer())
            {
                if(NodeContainsPoint(getTopLayer(), touch->getLocation()))
                {
                    isGestureRecognizer = false;
                }
            }
            
            if(getBottomLayer())
            {
                if(NodeContainsPoint(getBottomLayer(), touch->getLocation()))
                {
                    isGestureRecognizer = false;
                }
            }
            
            if(isGestureRecognizer)
            {
                gestureRecognizer->onTouchBegan(touch, unusedEvent);
            }
        }
    }
    
    if(_quickNavigation)
    {
        _quickNavigation->onListViewTouchBegan(touch, unusedEvent);
    }
    
    if(_touchBeganCallback)
    {
        _touchBeganCallback(touch,unusedEvent);
    }
            
    _touchesStart = touch->getLocation();
    
    if(_slideLeftMenu)
    {
        _slideLeftMenu->onTouchBegan(touch, unusedEvent);
    }
    
    return ListView::onTouchBegan(touch, unusedEvent);;
}

void PixGalleryListView::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    if(_topLayer != nullptr)
    {
        if(NodeContainsPoint(_topLayer, touch->getLocation()))
        {
            return;
        }
    }
    
    if(_bottomLayer != nullptr)
    {
        if(NodeContainsPoint(_bottomLayer, touch->getLocation()))
        {
            return;
        }
    }
    
    _isTouchMoved = true;

    if(gestureRecognizer)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            bool isGestureRecognizer = true;
            
            if(getTopLayer())
            {
                if(NodeContainsPoint(getTopLayer(), touch->getLocation()))
                {
                    isGestureRecognizer = false;
                }
            }
            
            if(getBottomLayer())
            {
                if(NodeContainsPoint(getBottomLayer(), touch->getLocation()))
                {
                    isGestureRecognizer = false;
                }
            }
            
            if(isGestureRecognizer)
            {
                gestureRecognizer->onTouchMoved(touch, unusedEvent);
            }
        }
    }
    
    if(_quickNavigation)
    {
        _quickNavigation->onListViewTouchMoved(touch, unusedEvent);
    }
    
    if(_touchMovedCallback)
    {
        _touchMovedCallback(touch,unusedEvent);
    }

    if(_slideLeftMenu)
    {
        _slideLeftMenu->onTouchMoved(touch, unusedEvent);
        
        if(_slideLeftMenu->isShow())
            return;
    }
    
    if(checkDownScroll(touch) == false)
    {
        return;
    }
    
    ListView::onTouchMoved(touch, unusedEvent);
}

void PixGalleryListView::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    _isTouchMoved = false;
    
    if(gestureRecognizer)
    {
        gestureRecognizer->onTouchEnded(touch, unusedEvent);
    }
    
    if(_quickNavigation)
    {
        _quickNavigation->onListViewTouchEnded(touch, unusedEvent);
    }
    
    if(_touchEndedCallback)
    {
        _touchEndedCallback(touch,unusedEvent);
    }
    
    if(_slideLeftMenu)
    {
        _slideLeftMenu->onTouchEnded(touch, unusedEvent);
    }
    
    if(checkDownScroll(touch) == false)
    {
        return;
    }
    
    _touchesStart = Vec2::ZERO;
    
    ListView::onTouchEnded(touch, unusedEvent);
}

void PixGalleryListView::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    _isTouchMoved = false;
    
    if(gestureRecognizer)
    {
        gestureRecognizer->onTouchCancelled(touch, unusedEvent);
    }
    
    if(_quickNavigation)
    {
        _quickNavigation->onListViewTouchCancelled(touch, unusedEvent);
    }
    
    if(_touchCancelledCallback)
    {
        _touchCancelledCallback(touch,unusedEvent);
    }
    
    if(_slideLeftMenu)
    {
        _slideLeftMenu->onTouchCancelled(touch, unusedEvent);
    }

    ListView::onTouchCancelled(touch, unusedEvent);
}

#pragma mark - GestureHandler

bool PixGalleryListView::onGestureTap(PixGestureTap* gesture)
{
    Vec2 difference = gesture->getLocation() - _touchesStart;
    
    std::vector<Vec2> touchVec;
    touchVec.push_back(_touchesStart);
    touchVec.push_back(gesture->getLocation());
    
    LISTVIEW_TOUCH_TYPE type;
    
    if (gesture->getNumClicks() == 1)
    {
        type = LISTVIEW_TAP;
    }
    else
    {
        type = LISTVIEW_DOUBLE_TAP;
    }
    
    float move = 30.f;
    
    if (difference.x > move || difference.y > move)
    {
        
    }
    else if (difference.x < -move || difference.y < -move)
    {
        
    }
    else
    {
        for (int i = 0; i < getChildrenCount(); i++)
        {
            Node *child = getChildren().at(i);
            
            if(child)
            {
                if(NodeContainsPoint(child, gesture->getLocation()))
                {
                    if(getTopLayer())
                    {
                        if(_listItemCallback)
                        {
                            if(!NodeContainsPoint(getTopLayer(), gesture->getLocation()))
                            {
                                if(_slideLeftMenu && _slideLeftMenu->isShow())
                                    break;
                                
                                _listItemCallback(child,touchVec,type);
                                break;
                            }
                        }
                    }
                    else if(getBottomLayer())
                    {
                        if(_listItemCallback)
                        {
                            if(!NodeContainsPoint(getBottomLayer(), gesture->getLocation()))
                            {
                                if(_slideLeftMenu && _slideLeftMenu->isShow())
                                    break;
                                
                                _listItemCallback(child,touchVec,type);
                                break;
                            }
                        }
                    }
                    else
                    {
                        if(_listItemCallback)
                        {
                            if(_slideLeftMenu && _slideLeftMenu->isShow())
                                break;
                            
                            _listItemCallback(child,touchVec,type);
                            break;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool PixGalleryListView::onGestureLongPress(PixGestureLongPress* gesture)
{
    Vec2 difference = gesture->getLocation() - _touchesStart;
    
    std::vector<Vec2> touchVec;
    touchVec.push_back(_touchesStart);
    touchVec.push_back(gesture->getLocation());
    
    float move = 30.f;
    
    if (difference.x > move || difference.y > move)
    {
        
    }
    else if (difference.x < -move || difference.y < -move)
    {
        
    }
    else
    {
        for (int i = 0; i < getChildrenCount(); i++)
        {
            Node *child = getChildren().at(i);
            
            if(child)
            {
                if(NodeContainsPoint(child, gesture->getLocation()))
                {
                    if(getTopLayer())
                    {
                        if(_listItemCallback)
                        {
                            if(!NodeContainsPoint(getTopLayer(), gesture->getLocation()))
                            {
                                if(_slideLeftMenu && _slideLeftMenu->isShow())
                                    break;
                                
                                _listItemCallback(child,touchVec,LISTVIEW_LONG_TAP);
                                break;
                            }
                        }
                    }
                    else if(getBottomLayer())
                    {
                        if(_listItemCallback)
                        {
                            if(!NodeContainsPoint(getBottomLayer(), gesture->getLocation()))
                            {
                                if(_slideLeftMenu && _slideLeftMenu->isShow())
                                    break;
                                
                                _listItemCallback(child,touchVec,LISTVIEW_LONG_TAP);
                                break;
                            }
                        }
                    }
                    else
                    {
                        if(_listItemCallback)
                        {
                            if(_slideLeftMenu && _slideLeftMenu->isShow())
                                break;
                            
                            _listItemCallback(child,touchVec,LISTVIEW_LONG_TAP);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    _touchesStart = Vec2::ZERO;
    
    return false;
}

bool PixGalleryListView::onGesturePan(PixGesturePan* gesture)
{
    return false;
}

bool PixGalleryListView::onGesturePinch(PixGesturePinch* gesture)
{
    return false;
}

bool PixGalleryListView::onGestureRotation(PixGestureRotation* gesture)
{
    return false;
}

bool PixGalleryListView::onGestureSwipe(PixGestureSwipe* gesture)
{
    if(_gestureSwipeCallback)
        (_gestureSwipeCallback)(gesture);
    
    return false;
}

