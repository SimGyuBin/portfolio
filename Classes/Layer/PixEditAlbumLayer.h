//
//  PixEditAlbumLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 23..
//
//

#ifndef PixEditAlbumLayer_h
#define PixEditAlbumLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../Model/PixSmartMaskBookModel.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixPhotoEditMenuLayer.h"
#include "PixAddPhotoSelectLayer.h"
#include "../Network/PixHttpClient.h"
#include "../Network/PixDownloader.h"
#include "../UIControl/PixGestureRecognizer.h"
#include "PixPhotoEditingLayer.h"
#include "../Manager/PixEditManager.h"
#include "PixLayerActionDelegate.h"
#include "../Info/photoChangeInfo.h"

#define MENU_TAG 5000
#define INSERT_TAG 6000
#define CHANGESPRITE_SIZE 240.0f

USING_NS_CC;

class PixNetworkImageModel;

class PixEditAlbumLayer : public PixModalLayer, public PixPhotoEditMenuLayerDelegate, public PixLayerActionDelegate, public PixDownloaderDelegate, public PixGestureHandler
{
public:
    
    static PixEditAlbumLayer* create(PixSmartMaskBookModel *pModel, int nPage);
    
    PixEditAlbumLayer(PixSmartMaskBookModel *pModel, int nPage);
    virtual ~PixEditAlbumLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
    void initGestureRecognizer();
    
    inline Node* getEditAlbumImageLayer()
    {
        return EditAlbumImageLayer;
    }
    
    inline PixEditSprite* getPhotoEditSprite()
    {
        return photoEditSprite;
    }
    
    inline int getCurPage()
    {
        return mn_Page;
    }
    
private:
    
    PixSmartMaskBookModel *mModel;
    int mn_Page;
    int mn_RequestImageCount;
    bool mbln_IsRequestImage;
    bool mbln_IsPlace;
  
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
   
    
    Sprite *album_effect;
    Sprite *albumcover_effect;
    
    Node *EditAlbumFunctionLayer;
    ui::Button *btn_complet;
    ui::Button *btn_layout_change;
    ui::Button *btn_layout_remove;
    
    ui::Button *Button_before;
    ui::Button *Button_next;
    ui::Text *curpagelabel;

    ClippingNode *EditAlbumImageLayer;
    ui::Widget *skin;
    ui::Widget *item;
    ui::Widget *mask;
    PixPhotoEditMenuLayer *photoEditMenuLayer;
    PixEditSprite *photoEditSprite;
    Sprite *changeSprite;
    
    std::deque<PixDownloader*> downloaderQueue;
    PixGestureRecognizer *gestureRecognizer;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    void setEditAlbumImageLayer();
    void loadSmartLayout();
    void loadLocalImage();
    void completeTexture(Texture2D *pTexture, void *pUserData);
    
    void sendGetImageUrl_Call(const std::string &url, void *pUserData);
    void sendGetImageCompleted();
    PixEditSprite* addLocalImageSprite(PixNetworkImageModel *networkImageModel, ClippingNode* clippingNode, int nSmartIndex);
    void addEditMenuLayer(PixEditSprite *pSprite);
    void layoutRemove();
    void reomveEditSprite(PhotoEditInfo *info);

    void goPreEditPage(bool blnCheckEmpty = true);
    void goNextEditPage(bool blnCheckEmpty = true);
    bool checkPhotoEmpty();
    void updateCurPagelabel();
    void setAlbumEffect();
    void removeChangeSprite();
    void editSpriteChangeEvent(PixEditSprite *pSprite);

#pragma mark - Button
    
    void btnCompletPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnLayoutChangePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnLayoutRemovePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnbeforePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnNextPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showEditAlbumPhotoLayerAction();
    void hideEditAlbumPhotoLayerAction();
    void showEditAlbumPhotoLayerActionFinish(Node* node);
    void hideEditAlbumPhotoLayerActionFinish(Node* node);
    void showEditAlbumLayerAction();
    void hideEditAlbumLayerAction();
    void showEditAlbumLayerActionFinish(Node* node);
    void hideEditAlbumLayerActionFinish(Node* node);
    
#pragma mark - Touch
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);
    
#pragma mark - GestureHandler
    
    virtual bool onGestureTap(PixGestureTap* gesture);
    virtual bool onGestureLongPress(PixGestureLongPress* gesture);
    virtual bool onGesturePan(PixGesturePan* gesture);
    virtual bool onGesturePinch(PixGesturePinch* gesture);
    virtual bool onGestureRotation(PixGestureRotation* gesture);
    virtual bool onGestureSwipe(PixGestureSwipe* gesture);

#pragma mark - Delegate

    virtual void photoEditMenuLayerChangePressed(PixEditSprite* pSprite);
    virtual void photoEditMenuLayerReplacePressed(PixEditSprite *pSprite);
    virtual void photoEditMenuLayerDeletePressed(PixEditSprite *pSprite);
    virtual void photoInsertLayerPressed(PixEditSprite* pSprite);
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - Network
    
    void layoutChange();
    void layoutChangeCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode);
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent);
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixEditAlbumLayer_h */
