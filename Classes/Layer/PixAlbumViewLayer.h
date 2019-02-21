//
//  PixAlbumViewLayer.h
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 12..
//
//

#ifndef PixAlbumViewLayer_h
#define PixAlbumViewLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Info/photoChangeInfo.h"
#include "PixLayerActionDelegate.h"
#include "PixSmartalbumNonSavePopupLayer.h"
#include "PixPageAddPopupLayer.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditSprite.h"
#include "../UIControl/PixSmartSkinSprite.h"
#include "../UIControl/PixAlbumViewLoading.h"
#include "../UIControl/PixGestureRecognizer.h"
#include "../UIControl/PixAlbumViewLayout.h"
#include "../UIControl/PixToast.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Layer/PixPhotoEditMenuLayer.h"
#include "../Layer/PixThemeChangeLayer.h"
#include "../Model/PixSmartMaskBookModel.h"
#include "../Widget/PixPageNavigationCell.h"
#include "../Network/PixHttpClient.h"
#include "../Network/PixDownloader.h"

USING_NS_CC;

#define AUTO_ACTION_POSITIONS               300.0f
#define LOADINGALBUM_TAG                    15678
#define ALBUMVIEW_PAGEPANEL_ACTION_TAG      16612
#define ALBUMVIEW_NAVIGATION_ACTION_TAG     16613
#define ALBUMVIEW_MENU_TAG                  5000
#define ALBUMVIEW_CHANGE_SPRITE_SIZE        235.0f
#define ALBUMSAVE_LAYER_TAG                 6000
#define THEMECHANGE_LAYER_TAG               6700
#define CHANGE_SPRITE_TAG                   7000

typedef enum
{
    BOTTOM_BUTTON_NORMAL,
    BOTTOM_BUTTON_DISABLED,
    BOTTOM_BUTTON_COUNT
}
BOTTOM_BUTTON_TYPE;

static const Vec2 pagePanelToastPos = Vec2(360,115);
static const Vec2 toastPos = Vec2(360,310);

class PixGalleryLevel;
class PixGalleryTouchControlLayer;

class PixAlbumViewLayer : public Layer, public NonSavePopupDelegate, public PixDownloaderDelegate, public PixPhotoEditMenuLayerDelegate, public PixNetworkImageModelDelegate, public PixGestureHandler, public PixLayerActionDelegate, public PixThemeChangeLayerDelegate, public PixPageAddPopupLayerDelegate, public PixPageNavigationCellDelegate
{
    
public :
    
    static PixAlbumViewLayer* create(PixGalleryLevel *galleryLevel);
    
    PixAlbumViewLayer();
    virtual ~PixAlbumViewLayer();
    bool init();
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    virtual void onExit();
    
    void initLayer();
    
    inline bool isAction()
    {
        return mbln_IsAction;
    }
    
    inline bool isShow()
    {
        return mbln_IsShow;
    }
    
    inline Node* getEditAlbumImageLayer()
    {
        return EditAlbumImageLayer;
    }
    
    inline float getPagePanelScale() const
    {
        if(PagePanel)
        {
            return PagePanel->getScale();
        }
        
        return 0.0f;
    }
    
    inline bool getNavigationVisible()
    {
        if(Panel_Navigation)
        {
            return Panel_Navigation->isVisible();
        }
        
        return false;
    }
    
    inline bool getPageNavigationVisible()
    {
        if(Panel_PageNavigation)
        {
            return Panel_PageNavigation->isVisible();
        }
        
        return false;
    }
    
    inline PixSmartMaskBookModel* getSmartMaskBookModel()
    {
        return smartMaskBookModel;
    }
    
private:

    Node *rootNode;
    
    PixHttpClient *pixHttpClient;
    ClippingNode *EditAlbumImageLayer;
    
    Node *AlbumViewTopLayer;
    ui::Button *btn_back;
    ui::Button *Button_complete;
    
    Node *AlbumView_Album;
    ui::Text *period;
    ui::Text *curpagelabel_2;
    
    PixAlbumViewLayout *PagePanel;
    Node *AlbumView_AlbumLayer;
    ui::Widget *skin;
    ui::Widget *item;
    ui::Widget *mask;
    Sprite *album_effect;
    Sprite *albumcover_effect;
    ui::Button *PreviewScaleUpBack;
    ui::Text *curpagelabel;
    
    PixGalleryListView *Bottom_ListView;
    
    PixScrollMenu *Theme[BOTTOM_BUTTON_COUNT];
    PixScrollMenu *MovePage[BOTTOM_BUTTON_COUNT];
    PixScrollMenu *PageDelet[BOTTOM_BUTTON_COUNT];
    PixScrollMenu *PageAdd[BOTTOM_BUTTON_COUNT];
    PixScrollMenu *Layout[BOTTOM_BUTTON_COUNT];
    PixScrollMenu *AddPhoto[BOTTOM_BUTTON_COUNT];
    PixScrollMenu *UsedPhoto[BOTTOM_BUTTON_COUNT];
    
    Sprite *completealbum;
    
    ui::Button *btn_Expansion;
    ui::Button *btn_Next;
    ui::Button *btn_Next_1;
    
    PixPhotoEditMenuLayer *photoEditMenuLayer;
    PixEditSprite *photoEditSprite;
    
    PixToast *pagePanelToast;
    ui::Layout *Panel_Navigation;
    PixGalleryListView *NavigationListView;
    ui::Button *Navigation_Close;
    ui::Button *btn_Pagemove;
    ui::Button *btn_Layout;
    ui::Button *btn_PanelNext;
    ui::Button *btn_PanelNext_1;
    ui::Layout *Panel_PageNavigation;
    PixGalleryListView *Panel_PageNavigationListView;
    ui::Button *PageNavigation_Close;
    ui::ImageView *Image_toast;
    Vec2 panelNavigationStartPos;
    
    bool mbln_IsAction;
    bool mbln_IsShow;
    bool mbln_IsFirst;
    
    PixSmartMaskBookModel *smartMaskBookModel;
    int mn_Page;
    bool mbln_IsRequestImage;
    bool mbln_IsPageModelTexture;
    bool mbln_IsPlace;
    
    std::deque<PixDownloader*> downloaderQueue;
    std::vector<PixNetworkImageModel*> networkImageModelVec;
    
    CC_SYNTHESIZE(int, mn_User_page_max_pic, User_page_max_pic)
    CC_SYNTHESIZE(int, mn_Min_page, Min_page)
    CC_SYNTHESIZE(int, mn_Max_page, Max_page)
    CC_SYNTHESIZE(PixGalleryLevel*, mGalleryLevel, GalleryLevel)
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(PixGalleryTouchControlLayer*, mGalleryTouchControlLayer, GalleryTouchControlLayer)
    CC_SYNTHESIZE_PASS_BY_REF(Vec2, mHidePosition, HidePosition)
    CC_SYNTHESIZE_PASS_BY_REF(Vec2, mShowPosition, ShowPosition)
    CC_SYNTHESIZE_PASS_BY_REF(Vec2, mPagePanelPos, PagePanelPos)
    CC_SYNTHESIZE(bool, mbln_PreScaleUp, PreScaleUp)
    CC_SYNTHESIZE(bool, mbln_FirstPagePaneAction, FirstPagePaneAction)
    
    void createSmartAlbum();
    void setEditAlbumImageLayer();
    void infiniteLoadingCheck(float dt);
    void loadSmartLayout();
    void loadLocalImage();
    void completeTexture(Texture2D *pTexture, void *pUserData);
    void sendGetImageUrl_Call(const std::string &url, void *pUserData);
    void sendGetImageCompleted();
    void renderTexture();
    void renderTextureCallBack(PixSmartMaskPageModel *pModel);
    void goMyAlbum();
    void goThemeLevel();
    void createLoadingAlbumView();
    void createLoadingAlbumDesignChanges();
    void createSmartalbumIntro();
    void removeLoadingView();
    bool isLoadingView();
    void updateCurPagelabe();
    void updateAlbumEffect();
    void updateBottomLayer();
    void updatePageNavigation();
    void goPreEditPage(bool blnCheckEmpty = true);
    void goNextEditPage(bool blnCheckEmpty = true);
    bool checkPhotoEmpty();
    void addEditMenuLayer(PixEditSprite *pSprite);
    void removeEditMenuLayer();
    void editSpriteChangeEvent(PixEditSprite *pSprite);
    void layoutChangeEvent(float dt);
    void removeChangeSprite();
    void layoutRemove();
    void repalaceComplete(PixImageModel *pModel);
    void addPhoto(PixImageModel *pModel);
    void addPhotoComplete();
    void photoLayerCancelEvent();
    void reomveNetworkImageModel(PixNetworkImageModel *pModel);
    void reomveAllNetworkImageModel();
    void layoutChangeSchedule(float dt);
    void themeChenge();
    void setBottomItemVisible(PixScrollMenu **item, bool isVisible);
    void createPageNavigation();
    PixSmartMaskPageModel* getCurSmartMaskPageModel();
    void addPage(int nTag, std::vector<PixImageModel*> *addPhotoVec);
    bool isImageRequestCompleted();
    
#pragma mark - Button
    
    void btnbackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void scaleUpBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonThemeNormalPressed(cocos2d::Ref *pSender);
    void ButtonThemeDisabledPressed(cocos2d::Ref *pSender);
    void ButtonMovePageNormalPressed(cocos2d::Ref *pSender);
    void ButtonMovePageDisabledPressed(cocos2d::Ref *pSender);
    void ButtonPageDeletNormalPressed(cocos2d::Ref *pSender);
    void ButtonPageDeletDisabledPressed(cocos2d::Ref *pSender);
    void ButtonPageAddNormalPressed(cocos2d::Ref *pSender);
    void ButtonPageAddDisabledPressed(cocos2d::Ref *pSender);
    void ButtonLayoutNormalPressed(cocos2d::Ref *pSender);
    void ButtonLayoutDisabledPressed(cocos2d::Ref *pSender);
    void ButtonAddPhotoNormalPressed(cocos2d::Ref *pSender);
    void ButtonAddPhotoDisabledPressed(cocos2d::Ref *pSender);
    void ButtonUsedPhotoNormalPressed(cocos2d::Ref *pSender);
    void ButtonUsedPhotoDisabledPressed(cocos2d::Ref *pSender);
    void btnExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnNextPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnPrePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnNavigationClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnPanelPagemovePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnPanelLayoutPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnPageNavigationClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - Touch & Touches
    
    bool onLayoutTouchBeganCallBack(Touch *touch, Event *event);
    void onLayoutTouchMovedCallBack(Touch *touch, Event *event);
    void onLayoutTouchEndedCallBack(Touch *touch, Event *event);
    void onLayoutTouchCancelledCallBack(Touch *touch, Event *event);
    
#pragma mark - GestureHandler
    
    virtual bool onGestureTap(PixGestureTap* gesture);
    virtual bool onGestureLongPress(PixGestureLongPress* gesture);
    virtual bool onGesturePan(PixGesturePan* gesture);
    virtual bool onGesturePinch(PixGesturePinch* gesture);
    virtual bool onGestureRotation(PixGestureRotation* gesture);
    virtual bool onGestureSwipe(PixGestureSwipe* gesture);
    
#pragma mark - Action
    
    void showAlbumViewLayerAction();
    void hideAlbumViewLayerAction();
    void showAlbumViewLayerActionFinish(Node* node);
    void hideAlbumViewLayerActionFinish(Node* node);
    void moveAlbumViewLayerAction(Touch* touch, const Vec2& touchesStart);
    void moveAlbumViewLayerActionEnd();
    void pagePaneRunAction(bool isScaleUp);
    void showCompletealbum();
    void showCompletealbumFinish(Node* node);
    void showFirstPagePaneAction();
    void showPanelPageNavigation();
    void hidePanelPageNavigation();
    void showNavigation();
    void showNavigationFinish(Node* node);
    void hideNavigation();
    void hideNavigationFinish(Node* node);
    void scrollToItemFinish(float dt);

#pragma mark - Network
    
    void layoutChange(int nIndex = -1);
    void layoutChangeCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void layoutChangeError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumNew();
    void albumNewCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void division();
    void divisionCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
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
    virtual void photoEditMenuLayerChangePressed(PixEditSprite* pSprite);
    virtual void photoEditMenuLayerReplacePressed(PixEditSprite* pSprite);
    virtual void photoEditMenuLayerDeletePressed(PixEditSprite* pSprite);
    virtual void networkImageModelcompleteTexture(Texture2D *pTexture, void *pUserData);
    virtual void themeChangePressed();
    virtual void addPrePage();
    virtual void addNextPage();
    virtual void addEndPage();
    virtual void pageNavigationCellPressed(int nIndex);
};

#endif /* PixAlbumViewLayer_h */
