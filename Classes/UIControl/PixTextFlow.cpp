//
//  PixTextFlow.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 21..
//
//

#include "PixTextFlow.h"

PixTextFlow* PixTextFlow::create(Size frame, const char *fontName, int fontSize, FLOW_TYPE type)
{
    PixTextFlow *textFlow = new PixTextFlow(frame, fontName, fontSize, type);
    textFlow->autorelease();
    
    return textFlow;
}

PixTextFlow::PixTextFlow(Size frame, const char *fontName, int fontSize, FLOW_TYPE type)
: flowType(type)
, flowWay((flowType % 2 == 0) ? -FLOW_SPEED : FLOW_SPEED)
, isFlowing(false)
, isFirst(true)
, mDelegate(nullptr)
{
    setContentSize(frame);
    setClippingEnabled(true);
    
    label = ui::Text::create("", fontName, fontSize);
    label->setVisible(false);
    addChild(label);
    
    FadeOut *out = FadeOut::create(0.8f);
    FadeIn *in = FadeIn::create(0.8f);
    Sequence *seq = Sequence::createWithTwoActions(out, in);
    aniAction = RepeatForever::create(seq);
    aniAction->retain();
}

PixTextFlow::~PixTextFlow()
{
    CC_SAFE_RELEASE(aniAction);
}

void PixTextFlow::startFlow(bool isAnimate)
{
    this->isAnimate = isAnimate;
    
    Rect frame = getBoundingBox();
    
    // 텍스트가 짧을 경우 가운데서부터 시작
//    bool isCenterStart = (label->getContentSize().width < frame.size.width);
    // 무조건 왼쪽에서 시작
    bool isCenterStart = false;
    float xPos;
    float delayTime;
    
    
    switch (flowType)
    {
        case FLOW_LEFT:
            delayTime = 2.0f;
            
            if (isFirst)
            {
                xPos = isCenterStart ? frame.size.width * 0.5f : label->getContentSize().width * 0.5f;
            }
            else
            {
                xPos = isCenterStart ? frame.size.width * 0.5f : label->getContentSize().width * 0.5f;
            }
            
            break;
            
        case FLOW_RIGHT:
            delayTime = 2.0f;
            
            if (isFirst)
            {
                xPos = isCenterStart ? frame.size.width * 0.5f : frame.size.width - label->getContentSize().width * 0.5f;
            }
            else
            {
                xPos = isCenterStart ? frame.size.width * 0.5f : frame.size.width - label->getContentSize().width * 0.5f;
            }
            
            break;
            
        case FLOW_LEFT_HIDE:
            delayTime = 0.0f;
            
            if (isFirst)
            {
                // 처음 시작시 라벨을 절반정도 걸처 보이게 한다.
                xPos = frame.size.width - label->getContentSize().width * 0.5f;
            }
            else
            {
                xPos = frame.size.width + label->getContentSize().width * 0.5f;
            }
            
            break;
            
        case FLOW_RIGHT_HIDE:
            delayTime = 0.0f;
            
            if (isFirst)
            {
                xPos = -label->getContentSize().width * 0.5f;
            }
            else
            {
                xPos = -label->getContentSize().width * 0.5f;
            }
            
            break;
    }

    label->setPosition(Vec2(xPos, frame.size.height * 0.5f));
    label->setVisible(true);
    
    // 라벨이 TextFlow 영역보다 작으면 효과를 주지 않는다.
    if(label->getContentSize().width < frame.size.width)
    {
        delayTime = 5.0f;
        schedule(schedule_selector(PixTextFlow::flowAnimationFinish), delayTime);
    }
    else
    {
        schedule(schedule_selector(PixTextFlow::resumeFlow), delayTime);
    }
}

void PixTextFlow::stopFlow()
{
    label->setVisible(false);
    pauseFlow();
}

void PixTextFlow::resumeFlow(float dt)
{
    unschedule(schedule_selector(PixTextFlow::resumeFlow));
    
    if (!isFlowing)
    {
        isFlowing = true;
        schedule(schedule_selector(PixTextFlow::flowAnimation));
        
        if (isAnimate)
        {
            label->runAction(aniAction);
        }
    }
}

void PixTextFlow::pauseFlow()
{
    if (isFlowing)
    {
        isFlowing = false;
        unschedule(schedule_selector(PixTextFlow::flowAnimation));
        
        label->stopAllActions();
    }
}

void PixTextFlow::setString(const char *text)
{
    if(label)
    {
        label->setString(text);
    }
}

const char* PixTextFlow::getString()
{
    if(label)
    {
        return label->getString().c_str();
    }
    
    return "";
}

void PixTextFlow::setTextColor(Color4B textColor)
{
    label->setColor(Color3B(textColor.r, textColor.g, textColor.b));
    label->setOpacity(textColor.a);
}

Color4B PixTextFlow::getTextColor()
{
    Color4B clr;
    
    clr.r = label->getColor().r;
    clr.g = label->getColor().g;
    clr.b = label->getColor().b;
    clr.a = label->getOpacity();
    
    return clr;
}

void PixTextFlow::setflowWay(float fSpeed)
{
    flowWay = (flowType % 2 == 0) ? -fSpeed : fSpeed;
}

void PixTextFlow::flowAnimation(float dt)
{
    label->setPosition( Vec2(label->getPosition() + Vec2(flowWay * (dt * 60) , 0)) );
    
    Vec2 test = label->getPosition();
    
    if ( ((flowType % 2 == 0) && label->getPositionX() + label->getContentSize().width * 0.5f < 0) ||
        ((flowType % 2 == 1) && label->getPositionX() - label->getContentSize().width * 0.5f > getBoundingBox().size.width))
    {
        isFirst = false;
        pauseFlow();
        startFlow(isAnimate);
        
        flowAnimationFinish(0);
    }
}

void PixTextFlow::flowAnimationFinish(float dt)
{
    unschedule(schedule_selector(PixTextFlow::flowAnimationFinish));
    
    if(mDelegate)
    {
        mDelegate->textFlowActionFinish();
    }
}
