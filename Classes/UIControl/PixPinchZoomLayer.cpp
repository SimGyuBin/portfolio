//
//  PixPinchZoomSprite.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 2..
//
//

#include "PixPinchZoomLayer.h"

PixPinchZoomLayer* PixPinchZoomLayer::create()
{
    PixPinchZoomLayer *sprite = new (std::nothrow) PixPinchZoomLayer();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}


PixPinchZoomLayer::PixPinchZoomLayer()
: gestureRecognizer(nullptr)
{
    
}

PixPinchZoomLayer::~PixPinchZoomLayer()
{
    CC_SAFE_DELETE(gestureRecognizer);
}

void PixPinchZoomLayer::onEnter()
{
    PixModalLayer::onEnter();
}

void PixPinchZoomLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onExitTransitionDidStart();
    
    initGestureRecognizer();
}

void PixPinchZoomLayer::onExit()
{
    PixModalLayer::onExit();
}

#pragma mark - private

void PixPinchZoomLayer::initGestureRecognizer()
{
    // Init gesture recognizer
    gestureRecognizer = new PixSimpleGestureRecognizer();
    gestureRecognizer->init();
    gestureRecognizer->setGestureHandler(this);
    
    // Enable all gesture kinds
    gestureRecognizer->setTapEnabled(false);
    gestureRecognizer->setDoubleTapEnabled(false);
    gestureRecognizer->setLongPressEnabled(false);
    gestureRecognizer->setPanEnabled(true);
    gestureRecognizer->setPinchEnabled(false);
    gestureRecognizer->setRotationEnabled(false);
    gestureRecognizer->setSwipeEnabled(false);
    
    // Taps will be fired immediately without waiting for double tap
    gestureRecognizer->setTapRequiresDoubleTapRecognitionToFail(false);

}

#pragma mark - Touch & Touches

bool PixPinchZoomLayer::onTouchBegan(Touch* touch, Event* event)
{
    gestureRecognizer->onTouchBegan(touch, event);
    
    return PixModalLayer::onTouchBegan(touch, event);

}

void PixPinchZoomLayer::onTouchMoved(Touch* touch, Event* event)
{
    PixModalLayer::onTouchMoved(touch, event);

    // Let the gesture recognizer to do its work
    gestureRecognizer->onTouchMoved(touch, event);
}

void PixPinchZoomLayer::onTouchEnded(Touch* touch, Event* event)
{
    PixModalLayer::onTouchEnded(touch, event);
    
    gestureRecognizer->onTouchEnded(touch, event);
}

void PixPinchZoomLayer::onTouchCancelled(Touch* touch, Event* event)
{
    PixModalLayer::onTouchCancelled(touch, event);
    
    gestureRecognizer->onTouchCancelled(touch, event);
}

#pragma mark - GestureHandler

bool PixPinchZoomLayer::onGestureTap(PixGestureTap* gesture)
{
    return false;
}

bool PixPinchZoomLayer::onGestureLongPress(PixGestureLongPress* gesture)
{
    return false;
}
bool PixPinchZoomLayer::onGesturePan(PixGesturePan* gesture)
{
    return false;
}

bool PixPinchZoomLayer::onGesturePinch(PixGesturePinch* gesture)
{
    return false;
}

bool PixPinchZoomLayer::onGestureRotation(PixGestureRotation* gesture)
{
    return false;
}

bool PixPinchZoomLayer::onGestureSwipe(PixGestureSwipe* gesture)
{
    return false;
}
