//
//  PixAlbumPreviewLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 29..
//
//

#ifndef PixAlbumPreviewLayer_h
#define PixAlbumPreviewLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixPageView.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Model/MyAlbumPreviewModel.h"
#include "../Widget/PixPreviewCell.h"

#define PRIVEWLAYER_ACTION_TAG 16617

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class MyAlbumPreviewModel;

class PixAlbumPreviewLayer : public PixModalLayer
{
public:
    
    static PixAlbumPreviewLayer* create(int nAlbum_id);
    
    PixAlbumPreviewLayer(int nAlbum_id);
    virtual ~PixAlbumPreviewLayer();

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
    
    CC_SYNTHESIZE(int, mn_Album_id, Album_id)
    CC_SYNTHESIZE_PASS_BY_REF(Vec2, mPagePanelPos, PagePanelPos)
    
    void createPreview();
    void pagePaneRunAction(bool isScaleUp);
    void hideLayerAction();
    
#pragma mark - Button
    
    void ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonPreviewScaleUpBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonPagePanelPressed(cocos2d::Ref *pSender);
    
#pragma mark - Network
    
    void albumPreview();
    void albumPreviewCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
    
};

#endif /* PixAlbumPreviewLayer_h */
