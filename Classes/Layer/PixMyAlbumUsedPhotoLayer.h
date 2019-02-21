//
//  PixMyAlbumUsedPhotoLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 21..
//
//

#ifndef PixMyAlbumUsedPhotoLayer_h
#define PixMyAlbumUsedPhotoLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Widget/PixAlbumSelectUsedPhotoCell.h"
#include "PixLayerActionDelegate.h"
#include "../Network/PixHttpClient.h"
#include "../Layer/PixMyAlbumUsedPhotoSelectLayer.h"

USING_NS_CC;

class MyAlbumModel;
class MyAlbumUsedImageModel;

class PixMyAlbumUsedPhotoLayer : public PixModalLayer, public PixLayerActionDelegate
{
    
public:
    
    static PixMyAlbumUsedPhotoLayer* create(MyAlbumModel *pModel);
    
    PixMyAlbumUsedPhotoLayer(MyAlbumModel *pModel);
    virtual ~PixMyAlbumUsedPhotoLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node *rootNode;
    PixHttpClient *pixHttpClient;
    
    PixGalleryListView *usedPhotoListView;
    Node *MyAlbumUsedPhotoTop;
    ui::Button *close;
    ui::Text *label;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(MyAlbumModel*, mAlbumModel, AlbumModel)
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Count, Count)
   
    void createPhotoListView(float dt);
    void scrollToItemFinish(float dt);
      
#pragma mark - Action
    
    void showAlbumUsedPhotoLayerAction();
    void hideAlbumUsedPhotoLayerAction();
    void showAlbumUsedPhotoLayerActionFinish(Node* node);
    void hideAlbumUsedPhotoLayerActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - Button
    
    void buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Touch
    
    bool listViewTouchBeganCallBack(Touch *touch, Event *unusedEvent);
    void listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
    
#pragma mark - Network

    void albumPhotos();
    void albumPhotosCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMyAlbumUsedPhotoLayer_h */
