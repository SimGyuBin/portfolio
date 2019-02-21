//
//  PixMyAlbumLevel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 7..
//
//

#ifndef PixMyAlbumLevel_h
#define PixMyAlbumLevel_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Core/GbLevel.h"
#include "../Network/PixHttpClient.h"
#include "../Manager/PixMyAlbumManager.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Layer/PixSlideLeftMenu.h"
#include "../Layer/PixAlbumSelectLayer.h"
#include "../Layer/PixThemeLayer.h"
#include "../Layer/PixMembershipEventLayer.h"
#include "../Layer/PixMenuEventDetailLayer.h"
#include "../Layer/PixMenuNoticeiDetailLayer.h"
#include "../Widget/PixAlbumCoverCell.h"
#include "../Widget/PixAlbumLoadingCoverCell.h"
#include "../Widget/PixMyAlbumNewsCell.h"
#include "../Widget/PixEventCell.h"

USING_NS_CC;

#define EMPTYLAYER_ZORDER       100
#define ALBUM_SELECTLAYER_TAG   1142
#define THEMELAYER_TAG          1146

class EventInfo;
class BoardBastInfo;

class PixMyAlbumLevel : public GbLevel, public PixLayerActionDelegate, public PixAlbumCoverCellDeleteActionDelegate
{
public:
    
    static Scene* scene();
    static Scene* themeLayer();
  
    PixMyAlbumLevel();
    virtual ~PixMyAlbumLevel();
    
    virtual void initLevel();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    virtual void onExit();

    void addThemeLayer(bool showAction = true);
    void removeAlbumListViewItem(MyAlbumModel *pModel ,bool isAction);
    void skinChange(MyAlbumModel *pModel);
    
    inline bool isSlideLeftMenuShow()
    {
        if(slideLeftMenu)
        {
            return slideLeftMenu->isShow();
        }
        
        return false;
    }
    
    inline void hideSlideLeftMenu()
    {
        if(slideLeftMenu)
        {
            return slideLeftMenu->hideSlideLeftMenu();
        }
    }
    
private:
    
    Node *MyAlbumListLayer;
    
    PixHttpClient *pixHttpClient;
    
    PixGalleryListView *albumListView;
    PixGalleryListView *newsListView;
    PixGalleryListView *eventListView;
    Node *Node_MyAlbumEventLayer;
    Node *MyAlbumTopLayer;
    
    Node *AlbumEmptyLayer;
    PixSlideLeftMenu *slideLeftMenu;
    
    ui::Button *Button_menu;
    ui::Button *mylogo_1;
    PixAlbumLoadingCoverCell *albumLoadingCoverCell;
    
    ui::Text *AlbumCount;
    std::string AlbumCountCode;
    ui::Text *Hello;
    ui::Button *Button_createalbum;
    ui::Layout *background;
    Sequence *albumUpAction;
    Sequence *albumDownAction;
    Sprite *buttonEffect;
    ui::CheckBox *EventCheckBox;
    
    bool mbln_IsAllbumListCall;
    bool mbln_IsLogoPressed;
    
    EventListenerTouchOneByOne *mTouchDelegate;
    
    std::vector<BoardBastInfo*> boardBastVec;
    std::vector<EventInfo*> eventVec;
    
    void createAlbumListView();
    void createNewsListView();
    void createEventListView();
    void deleteEventEventListData();
    void initEventListener();
    void setButtonEffect();
    void buttonEffectFinish();
    void updateCoverCellTitle(int nAlbum_id);
    void addEventDetailLayer(const std::string &strUrl);
    void addNoticeiDetailLayer(const std::string &strUrl);
    
    CC_SYNTHESIZE(bool, mbln_ShowThemeLayer, ShowThemeLayer)
    CC_SYNTHESIZE_PASS_BY_REF(Vec2, m_CreatealbumPos, CreatealbumPos)
    CC_SYNTHESIZE_PASS_BY_REF(Vec2, m_ListViewtouchesStart, ListViewtouchesStart)
    
#pragma mark - Button
    
    void ButtonMenuPressed(Ref *pSender, Widget::TouchEventType type);
    void ButtonLogoPressed(Ref *pSender, Widget::TouchEventType type);
    void ButtonStartPressed(Ref *pSender, Widget::TouchEventType type);
    void ButtonReTryPressed(Ref *pSender, Widget::TouchEventType type);
    void ButtonEventClosePressed(Ref *pSender, Widget::TouchEventType type);
    void EventCheckBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    
#pragma mark - Select List View
    
    void albumListViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
    bool albumListViewTouchBeganCallBack(Touch *touch, Event *unusedEvent);
    void albumListViewTouchMovedCallBack(Touch *touch, Event *unusedEvent);
    void onScrollViewCallback(Ref *pTarget, cocos2d::ui::ScrollView::EventType eventType);
    
#pragma mark - Action
    
    void runAlbumUpAction();
    void albumUpActionFinish();
    void runAlbumDownAction();
    void albumDownActionFinish();
    
#pragma mark - Touch & Touches
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);

#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    virtual void albumCoverCellDeleteActionFinish(Ref *pTarget);

    
#pragma mark - Network
    
    void albumList(bool isShowLoading);
    void albumListNextPage(float dt);
    void albumListCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumListError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getBoardBastCall();
    void getBoardBastCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getEventCall();
    void getEventCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
};

#endif /* PixMyAlbumLevel_h */
