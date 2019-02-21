//
//  PixSmartalbumLevel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 17..
//
//

// 스마트 앨범 씬 

#ifndef PixSmartalbumLevel_h
#define PixSmartalbumLevel_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Core/GbLevel.h"
#include "../Network/PixHttpClient.h"
#include "../Network/PixDownloader.h"
#include "../Model/PixSmartMaskBookModel.h"
#include "../Layer/PixEditAlbumLayer.h"
#include "../UIControl/PixGestureRecognizer.h"
#include "../Layer/PixSmartalbumNonSavePopupLayer.h"
#include "../Layer/PixSmartalbumSharePopupLayer.h"

USING_NS_CC;

typedef enum
{
    EFFECT_START,
    EFFECT_END,
    EFFECT_COUNT,
} BUTTON_EFFECT;

class PixSmartAlbumLevel : public GbLevel, public PixGestureHandler, public NonSavePopupDelegate, public PixSmartalbumShareDelegate, public PixLayerActionDelegate, public PixDownloaderDelegate
{
public:
    
    static Scene* scene();
    PixSmartAlbumLevel();
    virtual ~PixSmartAlbumLevel();
    
private:
    
    PixHttpClient *pixHttpClient;
    ClippingNode *smartalbumLayer;
    ui::Widget *skin;
    ui::Widget *item;
    ui::Widget *mask;
    Node *smartalbumMenuLayer;
    Sprite *bottomMenu;
    ui::Button *edit;
    ui::Button *share;
    ui::Button *save;
    ui::Button *back;
    ui::Text *curpagelabel;
    Sprite *completealbum;
    PixGestureRecognizer *gestureRecognizer;
    EventListenerTouchOneByOne *touchDelegate;
    
    Sprite *album_effect;
    Sprite *albumcover_effect;
    
    Vec2 prepagePos[EFFECT_COUNT];
    Vec2 nextpagePos[EFFECT_COUNT];
    
    int mn_CurpageNum;
    
    int mn_RequestImageCount;
    bool mbln_IsRequestImage;
    bool mbln_smartalbumMenuLayerVisible;
    bool mbln_IsFirst;
    PixSmartMaskBookModel *smartMaskBookModel;
    
    std::deque<PixDownloader*> downloaderQueue;
    
    virtual void initLevel();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    void createSmartAlbum();
    void setCurPagelabel();
    void setCurPage();
    void loadSmartLayout();
    void loadLocalImage();
    void completeTexture(Texture2D *pTexture, void *pUserData);
    void sendGetImageCompleted();
    void imageDataInfoVecClear();
    void showCompleteAlbum(bool blnShow);
    void initGestureRecognizer();
    void goPrePage();
    void goNextPage();
    void goMyAlbum();
    void goThemeLevel();
    void setAlbumEffect();
    
#pragma mark - Action
    
    void smartalbumMenuLayerVisible(bool isFirst = false);
    void showSmartalbumLayerAction();
    void hideSmartalbumLayerAction();
    void showSmartalbumLayerActionFinish(Node* node);
    void hideSmartalbumLayerActionFinish(Node* node);
    
#pragma mark - Touch
public:
    
    virtual bool TouchBegan(Touch* touch, Event* event);
    virtual void TouchMoved(Touch* touch, Event* event);
    virtual void TouchEnded(Touch* touch, Event* event);
    virtual void TouchCancelled(Touch* touch, Event* event);
    
#pragma mark - GestureHandler
    
    virtual bool onGestureTap(PixGestureTap* gesture);
    virtual bool onGestureLongPress(PixGestureLongPress* gesture);
    virtual bool onGesturePan(PixGesturePan* gesture);
    virtual bool onGesturePinch(PixGesturePinch* gesture);
    virtual bool onGestureRotation(PixGestureRotation* gesture);
    virtual bool onGestureSwipe(PixGestureSwipe* gesture);
  
private:
#pragma mark - Button

    void editPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void sharePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void savePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void backPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Network
    
    void sendGetImageUrl_Call(const std::string &url, void *pUserData);
    void layoutChange();
    void layoutChangeCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
     
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode);
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent);
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data);
    

#pragma mark - Delegate

    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    virtual void clickedButtonInNonSavePopup(PixSmartalbumNonSavePopupLayer *popup, NONSAVE_POPUP_TYPE buttonType);
    virtual void smartAlbumShare();
    
    void smartAlbumShareSave(RenderTexture* pRenderTexture, const std::string& filename);
};


#endif /* PixSmartalbumLevel_h */
