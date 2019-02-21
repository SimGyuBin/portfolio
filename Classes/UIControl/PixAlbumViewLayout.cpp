//
//  PixAlbumViewLayout.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 17..
//
//

#include "PixAlbumViewLayout.h"

PixAlbumViewLayout* PixAlbumViewLayout::create()
{
    PixAlbumViewLayout* layout = new (std::nothrow) PixAlbumViewLayout();
    if (layout && layout->init())
    {
        layout->autorelease();
        return layout;
    }
    CC_SAFE_DELETE(layout);
    return nullptr;
}

PixAlbumViewLayout::PixAlbumViewLayout()
: gestureRecognizer(nullptr)
, _touchBeganCallback(nullptr)
, _touchMovedCallback(nullptr)
, _touchEndedCallback(nullptr)
, _touchCancelledCallback(nullptr)
{
    
}

PixAlbumViewLayout::~PixAlbumViewLayout()
{
    CC_SAFE_DELETE(gestureRecognizer);
}

void PixAlbumViewLayout::copyClonedWidget(Widget *model)
{
    this->copyProperties(model);
    this->copyClonedWidgetChildren(model);
    
    if(model->getParent())
    {
        model->getParent()->addChild(this);
        model->getParent()->removeChild(model);
    }
    
    model->removeFromParentAndCleanup(true);
}

void PixAlbumViewLayout::initGestureRecognizer(PixGestureHandler *gestureHandler)
{
    gestureRecognizer = new PixSimpleGestureRecognizer();
    gestureRecognizer->init();
    
    gestureRecognizer->setGestureHandler(gestureHandler);
    
    // Enable all gesture kinds
    gestureRecognizer->setTapEnabled(true);
    gestureRecognizer->setDoubleTapEnabled(false);
    gestureRecognizer->setLongPressEnabled(true);
    gestureRecognizer->setPanEnabled(false);
    gestureRecognizer->setPinchEnabled(false);
    gestureRecognizer->setRotationEnabled(false);
    gestureRecognizer->setSwipeEnabled(true);
    
    // Taps will be fired immediately without waiting for double tap
    gestureRecognizer->setTapRequiresDoubleTapRecognitionToFail(false);
    
}

void PixAlbumViewLayout::setTouchBeganCallBack(onTouchBeganCallback callback)
{
    _touchBeganCallback = callback;
}

void PixAlbumViewLayout::setTouchMovedCallBack(onTouchCallback callback)
{
    _touchMovedCallback = callback;
}

void PixAlbumViewLayout::setTouchEndedCallBack(onTouchCallback callback)
{
    _touchEndedCallback = callback;
}

void PixAlbumViewLayout::setTouchCancelledCallBack(onTouchCallback callback)
{
    _touchCancelledCallback = callback;
}

#pragma mark - Touch & Touches

bool PixAlbumViewLayout::onTouchBegan(Touch* touch, Event* event)
{
    bool blnTouchBegan = Layout::onTouchBegan(touch, event);
    
    if(blnTouchBegan)
    {
        if(_touchBeganCallback)
        {
            _touchBeganCallback(touch,event);
        }
        
        if(gestureRecognizer)
        {
            gestureRecognizer->onTouchBegan(touch, event);
        }
    }

    return blnTouchBegan;
}

void PixAlbumViewLayout::onTouchMoved(Touch* touch, Event* event)
{
    Layout::onTouchMoved(touch, event);
    
    if(_touchMovedCallback)
    {
        _touchMovedCallback(touch,event);
    }

    if(gestureRecognizer)
    {
        gestureRecognizer->onTouchMoved(touch, event);
    }
}

void PixAlbumViewLayout::onTouchEnded(Touch* touch, Event* event)
{
    Layout::onTouchEnded(touch, event);
    
    if(_touchEndedCallback)
    {
        _touchEndedCallback(touch,event);
    }
    
    if(gestureRecognizer)
    {
        gestureRecognizer->onTouchEnded(touch, event);
    }
}

void PixAlbumViewLayout::onTouchCancelled(Touch* touch, Event* event)
{
    Layout::onTouchCancelled(touch, event);
    
    if(_touchCancelledCallback)
    {
        _touchCancelledCallback(touch,event);
    }
    
    if(gestureRecognizer)
    {
        gestureRecognizer->onTouchCancelled(touch, event);
    }
}
