//
//  PixThemeListView.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 24..
//
//

#ifndef PixThemeListView_h
#define PixThemeListView_h

#include "cocos2d.h"
#include "PixGalleryListView.h"

USING_NS_CC;

class PixThemePageView;

class PixThemeListView : public PixGalleryListView
{
    
public :
    
    static PixThemeListView* create();
    
    PixThemeListView();
    virtual ~PixThemeListView();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void update(float dt) override;
    
private:
    
    inline bool isPaging(const Vec2 &end, const Vec2 &start);
   
public:
    
#pragma mmark - Touch
    
    virtual void interceptTouchEvent(Widget::TouchEventType event,Widget* sender,Touch* touch) override;
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;
    
};

#endif /* PixThemeListView_h */
