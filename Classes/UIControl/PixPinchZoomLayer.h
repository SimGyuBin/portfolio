//
//  PixPinchZoomLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 2..
//
//

#ifndef PixPinchZoomLayer_h
#define PixPinchZoomLayer_h

#include "cocos2d.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGestureRecognizer.h"

USING_NS_CC;

class PixPinchZoomLayer : public PixModalLayer, public PixGestureHandler
{
    
public :
    
    static PixPinchZoomLayer* create();
    
    PixPinchZoomLayer();
    virtual ~PixPinchZoomLayer();
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
private:
    
    PixGestureRecognizer *gestureRecognizer;
    
    void initGestureRecognizer();
    
#pragma mark - Touch & Touches
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);
    
#pragma mark - GestureHandler
    
    virtual bool onGestureTap(PixGestureTap* gesture);
    virtual bool onGestureLongPress(PixGestureLongPress* gesture);
    virtual bool onGesturePan(PixGesturePan* gesture);
    virtual bool onGesturePinch(PixGesturePinch* gesture);
    virtual bool onGestureRotation(PixGestureRotation* gesture);
    virtual bool onGestureSwipe(PixGestureSwipe* gesture);
};


#endif /* PixPinchZoomLayer_h */
