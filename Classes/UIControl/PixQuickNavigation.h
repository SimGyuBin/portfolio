//
//  PixQuickNavigation.h
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 29..
//
//

#ifndef PixQuickNavigation_h
#define PixQuickNavigation_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

#define QUICK_NAVIGATION_TAG            1130
#define QUICK_NAVIGATION_ACTION_TAG     1129
#define QUICK_NAVIGATION_HIDE           3.0f

typedef enum __MULTILINGUAL_MONTH
{
    MULTILINGUAL_KO,
    MULTILINGUAL_ETC,
    MULTILINGUAL_COUNT,
    
} MULTILINGUAL_MONTH;

static const char *QUICKNAVIGATION_MONTH[MULTILINGUAL_COUNT][12] =
{"1","2","3","4","5","6","7","8","9","10","11","12",
 "JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

class PixGalleryListView;

class PixQuickNavigation : public Ref
{
public:
    
    static std::string getQuickNavigationYearMonth(int nYear, int nMonth);
    
    PixQuickNavigation(PixGalleryListView *pListView);
    virtual ~PixQuickNavigation();
    
    void createQuickNavigation();
    void removeQuickNavigation();
    void hideQuickNavigation();
    void updateQuickNavigation();
    float getStartPos();
    void setStartPos(float fStart);
    float getEndPos();
    void setEndPos(float fEnd);
    
    inline bool isMoved()
    {
        return _isMoved;
    }
    
private:
    
    PixGalleryListView *_listView;
    Node *_quickNavigation;
    ui::Button *_quickNavigationBar;
    Sprite *_quickNavigationLabelBackground;
    ui::Text *_quickNavigationLabel;
    Vec2 _previousLocation;
    
    float _startPos;
    float _endPos;
    
    bool _isMoved;
    bool _isQuickNavigationBarShow;
    
#pragma mmark - QuickNavigationBar Event
    
    void onQuickNavigationBarTouch(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);

#pragma mmark - ListView Touch
public:
    
    void onListViewTouchBegan(Touch *touch, Event *event);
    void onListViewTouchMoved(Touch *touch, Event *event);
    void onListViewTouchEnded(Touch *touch, Event *event);
    void onListViewTouchCancelled(Touch *touch, Event *event);
    
#pragma mark - Action
private:
    
    void showLayerAction();
    void hideLayerAction(float dt = 0);
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
};

class PixQuickNavigationUpdateLabel
{
public:
    virtual void setQuickNavigationUpdateLabel(ui::Text *pLabel) {};
};

#endif /* PixQuickNavigation_h */
