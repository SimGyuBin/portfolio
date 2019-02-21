//
//  PixAlbumViewLayout.h
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 17..
//
//

#ifndef PixAlbumViewLayout_h
#define PixAlbumViewLayout_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixGestureRecognizer.h"

USING_NS_CC;
using namespace ui;

class CC_GUI_DLL PixAlbumViewLayout : public ui::Layout
{
public:

    static PixAlbumViewLayout* create();

    PixAlbumViewLayout();
    virtual ~PixAlbumViewLayout();
   
    void copyClonedWidget(Widget *model);
    void initGestureRecognizer(PixGestureHandler *gestureHandler);
    
    typedef std::function<bool(Touch*, Event*)> onTouchBeganCallback;
    typedef std::function<void(Touch*, Event*)> onTouchCallback;
    
    void setTouchBeganCallBack(onTouchBeganCallback callback);
    void setTouchMovedCallBack(onTouchCallback callback);
    void setTouchEndedCallBack(onTouchCallback callback);
    void setTouchCancelledCallBack(onTouchCallback callback);
    
#pragma mark - Touch & Touches
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);
    
private:
    
    PixGestureRecognizer *gestureRecognizer;
    
    onTouchBeganCallback _touchBeganCallback;
    onTouchCallback _touchMovedCallback;
    onTouchCallback _touchEndedCallback;
    onTouchCallback _touchCancelledCallback;
};

#endif /* PixAlbumViewLayout_h */
