//
//  PixThemeChangeLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 9..
//
//

#ifndef PixThemeChangeLayer_h
#define PixThemeChangeLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixThemePageView.h"
#include "../Widget/PixThemeGroupCell.h"
#include "PixThemePopupLayer.h"
#include "PixLayerActionDelegate.h"
#include "PixThemePreviewLayer.h"

USING_NS_CC;

class PixThemeChangeLayerDelegate;

class PixThemeChangeLayer : public PixModalLayer, public PixThemePressedDelegate, public PixThemePreviewLayerDelegate
{
public:
    
    static PixThemeChangeLayer* create();
    
    PixThemeChangeLayer();
    virtual ~PixThemeChangeLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    
    Node *ThemeTopLayer;
    Node *Node_ThemeCategoryLayer;
    ui::Layout *themeCategoryLine;
    Node *ThemeBottomLayer;
    ui::Button *btn_back;
    ui::Button *btn_addphoto;
    PixThemePageView *themePageView;
    ui::Layout *PageDot;
    Sequence *addphotoAction;
    
    std::vector<Node*> pageDotVec;
    std::vector<ui::Button*> categoryButtonVec;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(PixThemeChangeLayerDelegate*, mThemeChangeDelegate, mThemeChangeDelegate)
    CC_SYNTHESIZE_PASS_BY_REF(Vec2, m_AddphotoPos, AddphotoPos)
    
    void createThemePageView();
    void createPageDot();
    void setPageDot(ssize_t index);
    bool checkCellSelected();
    
    void showAddphoto();
    void hideAddphoto();
    void addphotoActionFinish();
    void setCategoryButton(int nIndex);
    void setThemeCategoryLine();
    void themeChange();
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Button
    
    void btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnAddphotoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonCategoryPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Delegate
    
    virtual void themeCellSelectedInitialize();
    virtual void themePressed(bool isCellSelected);
    virtual void themePreviewAddPhotoPressed();
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

class PixThemeChangeLayerDelegate
{
public:
    
    virtual void themeChangePressed() {};
};

#endif /* PixThemeChangeLayer_h */
