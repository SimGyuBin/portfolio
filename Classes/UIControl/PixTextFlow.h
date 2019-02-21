//
//  PixTextFlow.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 21..
//
//

#ifndef PixTextFlow_h
#define PixTextFlow_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

#define FLOW_SPEED      1.0f        // 텍스트 이동 스피드

enum FLOW_TYPE
{
    FLOW_LEFT,          // 좌로 흐른다 (처음부터 보임)
    FLOW_RIGHT,         // 우로 흐른다 (처음부터 보임)
    FLOW_LEFT_HIDE,     // 좌로 흐른다 (바깥에서부터 등장)
    FLOW_RIGHT_HIDE,    // 우로 흐른다 (바깥에서부터 등장)
};

class PixTextFlowActionDelegate;

class PixTextFlow : public ui::Layout
{
    
public:
    
    static PixTextFlow* create(Size frame, const char *fontName, int fontSize, FLOW_TYPE type);
    
    PixTextFlow(Size frame, const char *fontName, int fontSize, FLOW_TYPE type);
    virtual ~PixTextFlow();
    
    void startFlow(bool isAnimate = false);
    void stopFlow();
    void resumeFlow(float dt);
    void pauseFlow();
    
    void setString(const char *text);
    const char* getString();
    
    void setTextColor(Color4B textColor);
    Color4B getTextColor();
    void setflowWay(float fSpeed);
    
private:
    
    ui::Text *label;
    
    FLOW_TYPE flowType;
    float flowWay;
    bool isFlowing;
    bool isFirst;
    
    RepeatForever *aniAction;
    bool isAnimate;
    
    void flowAnimation(float dt);
    void flowAnimationFinish(float dt);
    
    CC_SYNTHESIZE(PixTextFlowActionDelegate*, mDelegate, Delegate)
};

class PixTextFlowActionDelegate
{
public :
    virtual void textFlowActionFinish() {};
};

#endif /* PixTextFlow_h */
