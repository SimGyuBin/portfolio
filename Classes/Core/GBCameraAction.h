//
//  GBCameraAction.h
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 8..
//
//

#ifndef GBCameraAction_h
#define GBCameraAction_h

#include "cocos2d.h"

USING_NS_CC;

class GBCameraAction : public cocos2d::ActionCamera
{
public:
    
    static GBCameraAction* create(float duration, Vec2 position, float endZ, float angle);
    bool initWithDuration(float duration, Vec2 position, float endZ, float angle);
    GBCameraAction();
    virtual ~GBCameraAction();
    
    virtual void startWithTarget(Node *pTarget);
    virtual void update(float dt);
    
public:
    
    Vec2 startPosition_;
    Vec2 deltaPosition_;
    float deltaZ_;
    float angleOrig_;
    float deltaAngle_;
};

#endif /* GBCameraAction_h */
