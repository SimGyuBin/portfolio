//
//  PixPanZoomLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 2..
//
//

#ifndef PixPanZoomLayer_h
#define PixPanZoomLayer_h

#include "cocos2d.h"

USING_NS_CC;

class PixPanZoomLayer : public LayerColor
{
private:
    __Array *_touches;
    Vec2 _beganTouchPoint;
    Vec2 _endedTouchPoint;
    Vec2 _deltaSum;
    Vec2 _prevDeltaPoint;
    double _accelerationFactor;
    float _timeStamp;
    Rect _panBoundsRect;
    float _maxScale;
    float _minScale;
    float _productFactor;
    
    bool _isHolding;
 
    Touch* _finger0;
    Touch* _finger1;
    
public:
    PixPanZoomLayer();
    virtual ~PixPanZoomLayer();
    
    static PixPanZoomLayer* create();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void update( float dt );
        
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *event);
     
    virtual void setPosition( Vec2 position );
    virtual void setScale( float scale );
    
    void SetPanBoundsRect( Rect rect );
    void SetMaxScale( float maxScale );
    float GetMaxScale();
    void SetMinScale( float minScale );
    float GetMinScale();
    
    void Holding();
    void UnHolding();
    
    void SetProductFactor( float v );
    
};


#endif /* PixPanZoomLayer_h */
