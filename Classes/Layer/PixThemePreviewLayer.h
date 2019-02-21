//
//  PixThemePreviewLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 8. 2..
//
//

#ifndef PixThemePreviewLayer_h
#define PixThemePreviewLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixPageView.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Widget/PixPreviewCell.h"

#define THEME_PRIVEWLAYER_ACTION_TAG 16618

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class MyAlbumPreviewModel;
class PixThemePreviewLayerDelegate;

class PixThemePreviewLayer : public PixModalLayer
{
public:
    
    static PixThemePreviewLayer* create(PixDictionary *pSelectInfo, bool blnThemeChange);
    
    PixThemePreviewLayer(PixDictionary *pSelectInfo, bool blnThemeChange);
    virtual ~PixThemePreviewLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    PixHttpClient *pixHttpClient;
    PixPageView *PreView;
    ui::Text *curpagelabel;
    ui::Text *curpagelabel_2;
    ui::Layout *PagePanel;
    PixScrollMenu *scrollMenu;
    ui::Button *PreviewScaleUpBack;
    Node *loading;
    
    ssize_t curPreViewPage;
    
    std::vector<MyAlbumPreviewModel*> previewModelVec;
    
    CC_SYNTHESIZE_PASS_BY_REF(Vec2, mPagePanelPos, PagePanelPos)
    CC_SYNTHESIZE(bool, mbln_ThemeChange, ThemeChange)
    CC_SYNTHESIZE(PixDictionary*, mSelectInfo, SelectInfo)
    CC_SYNTHESIZE(PixThemePreviewLayerDelegate*, mDelegate, Delegate)
    
    void createPreview();
    void pagePaneRunAction(bool isScaleUp);
    void hideLayerAction();
    void themeChange();
    
#pragma mark - Button
    
    void ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonPreviewScaleUpBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonAddPhotoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonPagePanelPressed(cocos2d::Ref *pSender);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

class PixThemePreviewLayerDelegate
{
public:
    
    virtual void themePreviewAddPhotoPressed() {};
};

#endif /* PixThemePreviewLayer_h */
