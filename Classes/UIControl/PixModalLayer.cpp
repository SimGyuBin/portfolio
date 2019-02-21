//
//  PixModalLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 21..
//
//

#include "PixModalLayer.h"


PixModalLayer::PixModalLayer()
: mListener(nullptr)
, mbln_AndroidBackPressedAction(false)
, hideLayerFunction(nullptr)
{
    
}

PixModalLayer::~PixModalLayer()
{
    if(mListener)
        Director::getInstance()->getEventDispatcher()->removeEventListener(mListener);
}

bool PixModalLayer::init()
{
    if (Layer::init())
    {
        initEventListener();
        initLayer();
        return true;
    }
    
    return false;
}

void PixModalLayer::initLayer()
{
    
}

void PixModalLayer::onEnter()
{
    Layer::onEnter();
    scheduleUpdate();
}

void PixModalLayer::onExit()
{
    unscheduleUpdate();
    Layer::onExit();
}

void PixModalLayer::hideLayer()
{
    if(hideLayerFunction)
    {
        hideLayerFunction();
        hideLayerFunction = nullptr;
    }
}

void PixModalLayer::initEventListener()
{
    mListener = EventListenerTouchOneByOne::create();
    mListener->setSwallowTouches(true);
    
    mListener->onTouchBegan = CC_CALLBACK_2(PixModalLayer::onTouchBegan, this);
    mListener->onTouchMoved = CC_CALLBACK_2(PixModalLayer::onTouchMoved, this);
    mListener->onTouchEnded = CC_CALLBACK_2(PixModalLayer::onTouchEnded, this);
    mListener->onTouchCancelled = CC_CALLBACK_2(PixModalLayer::onTouchCancelled, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mListener, this);
}

#pragma mark - Touch & Touches

bool PixModalLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void PixModalLayer::onTouchMoved(Touch* touch, Event* event)
{
    Layer::onTouchMoved(touch,event);
}
void PixModalLayer::onTouchEnded(Touch* touch, Event* event)
{
    Layer::onTouchEnded(touch,event);
}

void PixModalLayer::onTouchCancelled(Touch* touch, Event* event)
{
    Layer::onTouchCancelled(touch,event);
}

#pragma mark - Android Destroy

void PixModalLayer::androidBackPressed()
{
    
}
