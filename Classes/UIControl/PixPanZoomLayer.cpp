//
//  PixPanZoomLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 2..
//
//

#include "PixPanZoomLayer.h"

PixPanZoomLayer::PixPanZoomLayer()
{
    
}


PixPanZoomLayer::~PixPanZoomLayer()
{

}

PixPanZoomLayer* PixPanZoomLayer::create()
{
    PixPanZoomLayer *pRet = new PixPanZoomLayer;
    if( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE( pRet );
    return NULL;
}


bool PixPanZoomLayer::init()
{
    if( LayerColor::initWithColor( Color4B( 0, 0, 255, 0 ) ) == false )
        return false;
    
    this->setAnchorPoint( Vec2::ZERO );
    
    _touches = __Array::create();
    _touches->retain();
    
    _accelerationFactor = 0.0f;
    _productFactor = 55.0f;
    
    _maxScale = 2.5f;
    _minScale = 1.0f;
    
    _isHolding = false;
    
    _finger0 = nullptr;
    _finger1 = nullptr;
    
    return true;
}


void PixPanZoomLayer::onEnter()
{
    LayerColor::onEnter();
    scheduleUpdate();
    
    auto listener = EventListenerTouchAllAtOnce::create();

    listener->onTouchesBegan = CC_CALLBACK_2(PixPanZoomLayer::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(PixPanZoomLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(PixPanZoomLayer::onTouchesEnded, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    

}

void PixPanZoomLayer::onExit()
{
    unscheduleUpdate();

    LayerColor::onExit();
    
    _touches->removeAllObjects();
    CC_SAFE_RELEASE_NULL( _touches );
}

void PixPanZoomLayer::update( float dt )
{
    // Skip smoothe panning when dt is high value
    if( dt > 1.0f / 55 )
        return;
    
    LayerColor::update( dt );
    
    if( _touches->count() == 1 )
    {
        _accelerationFactor *= 40 * dt * 0.95f;
    }
    else if( _touches->count() == 0 )
    {
        _accelerationFactor = fabs( _accelerationFactor - 0 );
        if( _accelerationFactor < FLT_EPSILON )
            return;
        
        if( _accelerationFactor < 0.004f )
        {
            _accelerationFactor = 0;
        }
        else
        {
            double d = dt * 60;
            if( d > 0.99 )
                d = 0.99;
            double i = (0 - _accelerationFactor) * 0.025 * d;
            
            _accelerationFactor = ( _accelerationFactor + i ) * d;
            
            Vec2 adder = _deltaSum;
            adder.x *= this->getContentSize().width;
            adder.y *= this->getContentSize().height;
            
            this->setPosition( this->getPosition() + adder * 2.5 * _accelerationFactor );
        }
    }
}

void PixPanZoomLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{
    if( _isHolding ) return;
    
    Touch *pTouch;
    std::vector<Touch*>::const_iterator setIter;
    
    int cnt = 0;
    for (setIter = touches.begin(); setIter != touches.end(); ++setIter)
    {
        pTouch = (Touch *)(*setIter);
        _touches->addObject(pTouch);
    }
    
    _deltaSum = Vec2::ZERO;
    _accelerationFactor = 0;
//    CCTime::gettimeofdayCocos2d( &_timeStamp, NULL );
    
}


void PixPanZoomLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *event)
{
    if( _isHolding ) return;
    
    if( _touches->count() == 1 )
    {
        Touch *touch = (Touch*)_touches->getObjectAtIndex( 0 );
        Vec2 curTouchPosition = Director::getInstance()->convertToGL( touch->getLocationInView() );
        Vec2 prevTouchPosition = Director::getInstance()->convertToGL( touch->getPreviousLocationInView() );
        Vec2 deltaPosition = curTouchPosition - prevTouchPosition;
        this->setPosition( this->getPosition() + deltaPosition );
        
        float prevAngle = CC_RADIANS_TO_DEGREES( _prevDeltaPoint.getAngle() );
        float angle = CC_RADIANS_TO_DEGREES( deltaPosition.getAngle() );
        if( fabs( prevAngle - angle ) <= 30 )
        {
            _deltaSum = Vec2::ZERO;
        }
        
        _prevDeltaPoint = deltaPosition;
        
        _deltaSum.x = _deltaSum.x + deltaPosition.x / this->getContentSize().width;
        _deltaSum.y = _deltaSum.y + deltaPosition.y / this->getContentSize().height;
        
        _accelerationFactor += _deltaSum.getLength() * 4.0;
    }
    else if( _touches->count() >= 2 )
    {
        // Get the two first touches
        Touch *touch1 = (Touch*)_touches->getObjectAtIndex(0);
        Touch *touch2 = (Touch*)_touches->getObjectAtIndex(1);
        
        // Get current and previous positions of the touches
        Vec2 curPosTouch1 = Director::getInstance()->convertToGL(touch1->getLocationInView());
        Vec2 curPosTouch2 = Director::getInstance()->convertToGL(touch2->getLocationInView());
        Vec2 prevPosTouch1 = Director::getInstance()->convertToGL(touch1->getPreviousLocationInView());
        Vec2 prevPosTouch2 = Director::getInstance()->convertToGL(touch2->getPreviousLocationInView());
        
        // Calculate current and previous positions of the layer relative the anchor point
        Vec2 curPosLayer = curPosTouch1.getMidpoint(curPosTouch2);
        Vec2 prevPosLayer = prevPosTouch1.getMidpoint(prevPosTouch2);
        
        
        // Calculate new scale
        float prevScale = this->getScale();
        float curScale = this->getScale() * curPosTouch1.getDistance(curPosTouch2) / prevPosTouch1.getDistance(prevPosTouch2);
        ;
        
        this->setScale( curScale );
        
        if( this->getScale() != prevScale )
        {
            Vec2 realCurPosLayer = this->convertToNodeSpaceAR(curPosLayer);
            float deltaX = (realCurPosLayer.x) * (this->getScale() - prevScale);
            float deltaY = (realCurPosLayer.y) * (this->getScale() - prevScale);
            
            this->setPosition(Vec2(this->getPosition().x - deltaX, this->getPosition().y - deltaY));
        }
        
        // If current and previous position of the multitouch's center aren't equal -> change position of the layer
        if (!prevPosLayer.equals(curPosLayer))
        {
            this->setPosition(Vec2(this->getPosition().x + curPosLayer.x - prevPosLayer.x,
                                  this->getPosition().y + curPosLayer.y - prevPosLayer.y));
        }
    }
}


void PixPanZoomLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
    if( _isHolding ) return;
    
    Touch *pTouch;
    std::vector<Touch*>::const_iterator setIter;
    
    for (setIter = touches.begin(); setIter != touches.end(); ++setIter)
    {
        pTouch = (Touch *)(*setIter);
        _touches->removeObject(pTouch);
    }
}

void PixPanZoomLayer::setPosition( Vec2 position )
{
    Node::setPosition( position );
    
    if( _panBoundsRect.equals( Rect::ZERO ) == false )
    {
        Rect boundBox;
        boundBox.origin = this->getPosition() / this->getScale();
        boundBox.size = this->getContentSize() / this->getScale();
        //CCLog( "boundBox : origin(%.1f, %.1f), size(%.1f, %.1f)", boundBox.origin.x, boundBox.origin.y, boundBox.size.width, boundBox.size.height );
        
        // OpenGL coordinate system
        float left = boundBox.getMinX();
        float right = boundBox.getMaxX();
        float top = boundBox.getMaxY();
        float bottom = boundBox.getMinY();
        //CCLog( "left,right(%.1f, %.1f), top,bottom(%.1f, %.1f)", left, right, top, bottom );
        
        float min_x = _panBoundsRect.getMinX() + boundBox.size.width;
        float max_x = _panBoundsRect.getMaxX() + boundBox.size.width;
        float min_y = _panBoundsRect.getMinY() + boundBox.size.height;
        float max_y = _panBoundsRect.getMaxY() + boundBox.size.height;
        //CCLog( "min(%.1f, %.1f), max(%.1f, %.1f)", min_x, min_y, max_x, max_y );
        
        float scale = this->getScale();
        float arLeft = min_x * scale;
        float arRight = max_x * scale - this->getContentSize().width;
        float arTop = max_y * scale - this->getContentSize().height;
        float arBottom = min_y * scale;
        
        if( left < min_x )
        {
            Node::setPosition( arLeft, this->getPosition().y );
        }
        
        if( right > max_x )
        {
            Node::setPosition( arRight, this->getPosition().y );
        }
        
        if( top > max_y )
        {
            Node::setPosition( this->getPosition().x, arTop );
        }
        
        if( bottom < min_y )
        {
            Node::setPosition( this->getPosition().x, arBottom );
        }
    }
}


void PixPanZoomLayer::setScale( float scale )
{
    LayerColor::setScale( MIN( MAX( scale, _minScale ), _maxScale ) );
    this->setPosition( this->getPosition() );
}


void PixPanZoomLayer::SetPanBoundsRect( Rect rect )
{
    _panBoundsRect = rect;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float wFactor = _panBoundsRect.size.width / visibleSize.width;
    float hFactor = _panBoundsRect.size.height / visibleSize.height;
    float minScale;
    if( wFactor > hFactor )
        minScale = wFactor;
    else
        minScale = hFactor;
    SetMinScale( minScale );
}


void PixPanZoomLayer::SetMaxScale( float maxScale )
{
    _maxScale = maxScale;
}


float PixPanZoomLayer::GetMaxScale()
{
    return _maxScale;
}


void PixPanZoomLayer::SetMinScale( float minScale )
{
    _minScale = minScale;
}

float PixPanZoomLayer::GetMinScale()
{
    return _minScale;
}


void PixPanZoomLayer::Holding()
{
    _isHolding = true;
}


void PixPanZoomLayer::UnHolding()
{
    _isHolding = false;
}


void PixPanZoomLayer::SetProductFactor( float v )
{
    _productFactor = v;
}
