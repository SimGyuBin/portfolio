//
//  PixThemeGroupCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 24..
//
//

#ifndef PixThemeGroupCell_h
#define PixThemeGroupCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Util/PixDictionary.h"
#include "../UIControl/PixThemeListView.h"

USING_NS_CC;

class PixThemePressedDelegate;
class PixThemePreviewLayerDelegate;

class PixThemeGroupCell : public ui::Widget
{
public :
    
    static PixThemeGroupCell* create(PixDictionary *pGroup, Node *pTopLayer, Node *pBottomLayer, bool isThemeChange = false);
    
    PixThemeGroupCell(PixDictionary *pGroup, Node *pTopLayer, Node *pBottomLayer, bool isThemeChange);
    virtual ~PixThemeGroupCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    void setThemeListViewTouchEnabled(bool enabled);
    void themeListViewScrollToTop();
    void setThemeCellSelected(bool isCellSelected);
    bool checkCellSelected();
    
    inline PixThemeListView* getThemeListView()
    {
        return themeListView;
    };
    
private:
    
    Node *_customItem;
    PixDictionary *mGroup;
    Node *mTopLayer;
    Node *mBottomLayer;
    PixThemeListView *themeListView;
    
    void settingthemeListView();
    
    CC_SYNTHESIZE(PixThemePressedDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(PixThemePreviewLayerDelegate*, mThemePreviewLayerDelegate, ThemePreviewLayerDelegate)
    
    CC_SYNTHESIZE(bool, mbln_ThemeChange, ThemeChange)
    
#pragma mark - Touch
    
    void themeListViewTouchEndedCallback(Touch *touch, Event *unusedEvent);
//    void themeListViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
};

class PixThemePressedDelegate
{
public:
    virtual void themeCellSelectedInitialize() {};
    virtual void themePressed(bool isCellSelected) {};
};


#endif /* PixThemeGroupCell_h */
