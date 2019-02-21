//
//  PixGalleryListView.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 26..
//
//

// 갤러리 씬에서 사용되고 있는 커스텀 리스트 뷰
// 그리드 뷰의 기능을 가지고 있다 

#ifndef PixGalleryListView_h
#define PixGalleryListView_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "PixQuickNavigation.h"
#include "PixGestureRecognizer.h"

USING_NS_CC;

typedef enum {
 
    LISTVIEW_TAP,
    LISTVIEW_DOUBLE_TAP,
    LISTVIEW_LONG_TAP,
}
LISTVIEW_TOUCH_TYPE;


class PixSlideLeftMenu;

class PixGalleryListView : public ui::ListView, public PixGestureHandler
{
    
public :
    
    static PixGalleryListView* create();
    
    PixGalleryListView();
    virtual ~PixGalleryListView();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    void setVisible(bool visible) override;
    
    void copyClonedListView(ui::ListView *pListView);
    void copyProperties(ui::ListView *pListView);
    void copyClonedWidgetChildren(ui::ListView *pListView);

    void setTopLayer(Node* topLayer, bool blnTopMargin = false);
    void setBottomLayer(Node* bottomLayer, bool blnBottomMargin = false);
    Node* getTopLayer();
    Node* getBottomLayer();
    void setListViewTouchEnabled(bool enabled);
    
    typedef std::function<void(Ref*, const std::vector<Vec2>&, LISTVIEW_TOUCH_TYPE)> onListItemCallBack;
    typedef std::function<bool(Touch*, Event*)> onTouchBeganCallback;
    typedef std::function<void(Touch*, Event*)> onTouchCallback;
    typedef std::function<void(PixGestureSwipe*)> onGestureSwipeCallback;
    
    void setLongPressEnabled(bool longPressEnabled);
    bool getLongPressEnabled();
    void setSelectListItemCallBack(onListItemCallBack callback);
    void setTouchBeganCallBack(onTouchBeganCallback callback);
    void setTouchMovedCallBack(onTouchCallback callback);
    void setTouchEndedCallBack(onTouchCallback callback);
    void setTouchCancelledCallBack(onTouchCallback callback);
    void setGestureSwipeCallBack(onGestureSwipeCallback callback);
    
    void jumpToDestination(const Vec2 &des);
    bool isAsyncLoadCall();
    
    void createQuickNavigation();
    void removeQuickNavigation();
    void hideQuickNavigation();
    void setQuickNavigationPositionStart(float start);
    void setQuickNavigationPositionEnd(float end);
    void resetGestureRecognizer();
    Vec2 getOffset();
    
    onListItemCallBack _listItemCallback;
    onTouchBeganCallback _touchBeganCallback;
    onTouchCallback _touchMovedCallback;
    onTouchCallback _touchEndedCallback;
    onTouchCallback _touchCancelledCallback;
    onGestureSwipeCallback _gestureSwipeCallback;
    
    inline bool isTouchMoved()
    {
        return _isTouchMoved;
    }
    
    inline bool isAutoScrolling()
    {
        return _autoScrolling;
    }

    inline bool isTop()
    {
        return _top;
    }
    
    inline bool isBottom()
    {
        return _bottom;
    }
    
    inline void setNextPage(bool var)
    {
        _nextPage = var;
    }
    
    inline bool isNextPage()
    {
        return _nextPage;
    }

protected:
    
    Node *_topLayer;
    Node *_bottomLayer;
     
    Vec2 _touchesStart;
    
    PixQuickNavigation *_quickNavigation;
    PixGestureRecognizer *gestureRecognizer;
    
    bool _isTouchMoved;
    bool _top;
    bool _bottom;
    bool _nextPage;
    
    CC_SYNTHESIZE(PixSlideLeftMenu*, _slideLeftMenu, SlideLeftMenu)
    CC_SYNTHESIZE(bool, mbln_LoadiongCell, LoadiongCell)
    
    void initGestureRecognizer();
    bool checkDownScroll(Touch *touch);
    
#pragma mmark - Touch
    
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;
    
#pragma mark - GestureHandler
    
    virtual bool onGestureTap(PixGestureTap* gesture) override;
    virtual bool onGestureLongPress(PixGestureLongPress* gesture) override;
    virtual bool onGesturePan(PixGesturePan* gesture) override;
    virtual bool onGesturePinch(PixGesturePinch* gesture) override;
    virtual bool onGestureRotation(PixGestureRotation* gesture) override;
    virtual bool onGestureSwipe(PixGestureSwipe* gesture) override;
};

#endif /* PixGalleryListView_h */
