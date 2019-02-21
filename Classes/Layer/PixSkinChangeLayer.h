//
//  PixSkinChangeLayer.hpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 22..
//
//

#ifndef PixSkinChangeLayer_h
#define PixSkinChangeLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "PixLayerActionDelegate.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Widget/PixAlbumCoverSkinItemCell.h"
#include "../Network/PixHttpClient.h"
#include "../Manager/PixSmartAlbumManager.h"

USING_NS_CC;

class MyAlbumModel;

class PixSkinChangeLayer : public PixModalLayer
{
    
public:
    
    static PixSkinChangeLayer* create(MyAlbumModel *pModel);
    
    PixSkinChangeLayer(MyAlbumModel *pModel);
    virtual ~PixSkinChangeLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
private:
    
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
    ui::Button *Button_cencel;
    ui::Button *Button_ok;
    PixGalleryListView *skinListView;
    MyAlbumModel *mMyAlbumModel;
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    void createSkinListView();
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Button
   
    void buttonCencelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - Touch
    
    void listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
    
#pragma mark - Network
    
    void skinChange();
    void skinChangeCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();

};


#endif /* PixSkinChangeLayer_h */
