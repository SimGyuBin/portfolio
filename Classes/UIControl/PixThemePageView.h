//
//  PixThemePageView.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 24..
//
//

#ifndef PixThemePageView_h
#define PixThemePageView_h

#include "cocos2d.h"
#include "PixPageView.h"

USING_NS_CC;

class PixThemePageView : public PixPageView
{
    
public :
    
    static PixThemePageView* create();
    
    PixThemePageView();
    virtual ~PixThemePageView();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void update(float dt) override;

private :
    
    Vec2 _touchesStart;
    
    inline bool isAutoScrolling()
    {
        return _autoScrolling;
    }
    
    inline bool isVerticalScroll(const Vec2 &end, const Vec2 &start);
    void setThemeListViewTouchEnabled(bool enabled);
    virtual void pageTurningEvent();
    
#pragma mmark - Touch
   
public :
    
    virtual void interceptTouchEvent(Widget::TouchEventType event,Widget* sender,Touch* touch) override;
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;

#pragma mmark - Event
    
    void eventCallBack(cocos2d::Ref* pSender,  ScrollView::EventType type);
};

#endif /* PixThemePageView_h */
