//
//  GBCameraAction.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 8..
//
//

#include "GBCameraAction.h"

GBCameraAction* GBCameraAction::create(float duration, Vec2 position, float endZ, float angle)
{
    GBCameraAction * pRet = new GBCameraAction();
    if(pRet->initWithDuration(duration, position, endZ, angle))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool GBCameraAction::initWithDuration(float duration, Vec2 position, float endZ, float angle)
{
    if( ActionCamera::initWithDuration(duration) )
    {
        deltaPosition_ = position;
        deltaZ_ = endZ;
        deltaAngle_ = angle;
        return true;
    }
    
    return false;
}

GBCameraAction::GBCameraAction()
: deltaZ_(0.0f)
, angleOrig_(0.0f)
, deltaAngle_(0.0f)
, startPosition_(Vec2::ZERO)
, deltaPosition_(Vec2::ZERO)
{

}

GBCameraAction::~GBCameraAction()
{
    
}

void GBCameraAction::startWithTarget(Node *pTarget)
{
    ActionCamera::startWithTarget(pTarget);
    
    startPosition_ = Vec2(_center.x, _center.y);
    angleOrig_ = CC_RADIANS_TO_DEGREES( Vec2(_up.x, _up.y).getAngle());
}

void GBCameraAction::update(float t)
{
    Vec3 eye(startPosition_.x + deltaPosition_.x * t, startPosition_.y + deltaPosition_.y * t, _eye.z + deltaZ_ * t);
    Vec3 center(startPosition_.x + deltaPosition_.x * t, startPosition_.y + deltaPosition_.y * t, _center.z);
    Vec3 up(cosf( CC_DEGREES_TO_RADIANS(angleOrig_) + CC_DEGREES_TO_RADIANS(  deltaAngle_) * t), sinf( CC_DEGREES_TO_RADIANS(angleOrig_) + CC_DEGREES_TO_RADIANS( deltaAngle_) *t), 0);
 
    setEye(eye);
    setCenter(center);
    setUp(up);
}


