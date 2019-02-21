//
//  PixAlbumSelectLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 2..
//
//

#ifndef PixAlbumSelectLayer_h
#define PixAlbumSelectLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixPageView.h"
#include "PixLayerActionDelegate.h"
#include "../Widget/PixAlbumSelectUsedPhotoCell.h"
#include "../Widget/PixPreviewCell.h"
#include "../Layer/PixMyAlbumSharePopup.h"
#include "../Layer/PixMyAlbumPopup.h"
#include "../Layer/PixMyAlbumCorrectLayer.h"
#include "../Layer/PixMyAlbumUsedPhotoLayer.h"

#define IMAGESAVE_TOAST_ACTION_TAG  16626
#define SELECTLAYER_TITLE_MARGIN    35.0f

USING_NS_CC;

class MyAlbumModel;
class MyAlbumPreviewModel;
class MyAlbumUsedImageModel;

class PixAlbumSelectLayer : public PixModalLayer, public PixMyAlbumPopupDelegate, public PixLayerActionDelegate
{
public:

    static PixAlbumSelectLayer* create(MyAlbumModel *pModel);
    
    PixAlbumSelectLayer(MyAlbumModel *pModel);
    virtual ~PixAlbumSelectLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *AlbumSelectTopLayer;
    ui::Button *btn_back;
    ui::Button *btn_share;
    ui::Button *btn_more;
    ui::Button *btn_set_cart;
    ui::Button *btn_del_cart;
    ui::Button *btn_Expansion;

    Node *AlbumSelect_Album;
    ui::Text *covertitle;
    ui::Text *period;
    ui::Text *guidedlabel;
    Sprite *Title_line;

    Node *AlbumSelect_UsedPhoto;
    ui::Text *photoamount;
    PixGalleryListView *UsedPhotoListView;
    PixAlbumSelectUsedPhotoCell *albumSelectUsedPhotoCell;
    
    Layout *PagePanel;
    PixPageView *PreView;
    ui::Button *PreviewScaleUpBack;
    ui::Button *btn_PreviewShare;
    ui::Button *btn_Imgsave;
    Sprite *toast_slide;
    ui::Text *curpagelabel;
    ui::Text *curpagelabel_2;
    PixScrollMenu *scrollMenu;
    ui::ImageView *Image_toast;
    
    ssize_t curPreViewPage;
    bool removeAlbum;
    bool mbln_IsAction;
    bool mbln_PagePaneAction;
    
    float mf_CovertitlePositionY;
    
    CC_SYNTHESIZE_PASS_BY_REF(Vec2, mPagePanelPos, PagePanelPos)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Rename, Rename)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Photoamount, Photoamount)
    CC_SYNTHESIZE(MyAlbumModel*, mMyAlbumModel, MyAlbumModel)
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    void createPreview();
    void createUsedImage();
    
    void toastSlideAction(Node* node);
    void toastSlideActionFinish(Node* node);
    void addMyAlbumUsedPhotoLayer();
    void pagePaneRunAction(bool isScaleUp);
    void pagePaneRunActionFinish(Node* node);
    void updateTitle();
    void moveCart();
    
#pragma mark - Button
    
    void ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonSharePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonMorePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonSetCartPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonDelCartPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonPreviewScaleUpBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonPreviewSharePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonPreviewSavePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonPagePanelPressed(cocos2d::Ref *pSender);
    
#pragma mark - ListView Event
    
    void albumSelectListViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);

#pragma mark - Action
    
    void showAlbumSelectLayerAction();
    void hideAlbumSelectLayerAction();
    void showAlbumSelectLayerActionFinish(Node* node);
    void hideAlbumSelectLayerActionFinish(Node* node);
    void showSaveImageToastAction();
    void showSaveImageToastActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void editMyAlbumTitle();
    virtual void skinChangeMyAlbum();
    virtual void removeMyAlbum();
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);

#pragma mark - Network

    void albumPreview();
    void albumPreviewCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumPhotos();
    void albumPhotosCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumDelete();
    void albumDeleteCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void setCart();
    void setCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void delCart();
    void delCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();

};

#endif /* PixAlbumSelectLayer_h */
