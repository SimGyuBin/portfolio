//
//  PixModalLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 21..
//
//

#ifndef PixModalLayer_h
#define PixModalLayer_h

#include "cocos2d.h"

USING_NS_CC;

class PixModalLayer : public Layer
{

public :
    
    PixModalLayer();
    virtual ~PixModalLayer();
    bool init();
    
    virtual void initLayer();
    virtual void onEnter();
    virtual void onExit();
    
    void hideLayer();

#pragma mark - Touch & Touches
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);

#pragma mark - Android Destroy
    
    virtual void androidBackPressed();

private :
    
    EventListenerTouchOneByOne *mListener;
    CC_SYNTHESIZE(bool, mbln_AndroidBackPressedAction, AndroidBackPressedAction)
    
protected:
    
    void initEventListener();
    
    std::function<void()> hideLayerFunction;
    
};

#endif /* PixModalLayer_h */







