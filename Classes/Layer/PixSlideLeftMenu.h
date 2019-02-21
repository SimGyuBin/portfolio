//
//  PixSlideLeftMenu.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 11..
//
//

#ifndef PixSlideLeftMenu_h
#define PixSlideLeftMenu_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

#define SLIDE_LEFT_ACTION_TAG 512

class PixMyAlbumLevel;

class PixSlideLeftMenu : public Layer
{
public:
    
    static PixSlideLeftMenu *create();
    
    PixSlideLeftMenu();
    virtual ~PixSlideLeftMenu();
    
    virtual void initLayer();
    virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
    void showSlideLeftMenu();
    void hideSlideLeftMenu();
    
    inline bool isShow()
    {
        return mbln_isShow;
    }
    
private:
    
    Node *rootNode;
    EventListenerTouchOneByOne *mListener;
    
    ui::Layout *MenuListLayer;
    Node *Layout;
    Sprite *blockout;
    
    Sprite *profliePhoto;
    ui::Text *Text_name;
    ui::Text *Text_email;
    ui::Text *CartCount;
    ui::ImageView *eventNew;
    ui::ImageView *noticeNew;
    
    bool mbln_isShow;
    
    Vec2 _touchesStart;
    Vec2 _touchesEnd;
    Node* mAddMenuLayer;
    
    void initEventListener();
    void setAddMenuLayer(Node *pLayer);
    void addMenuLayer();
    
    CC_SYNTHESIZE(bool, mbln_showLayerActionFinish, ShowLayerActionFinish);
    CC_SYNTHESIZE(bool, mbln_hideLayerActionFinish, HideLayerActionFinish);
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Button
    
    void ButtonSettingPressed(cocos2d::Ref *pSender);
    void ButtonCartPressed(cocos2d::Ref *pSender);
    void ButtonCouponPressed(cocos2d::Ref *pSender);
    void ButtonOrderHistoryPressed(cocos2d::Ref *pSender);
    void ButtonServiceInforPressed(cocos2d::Ref *pSender);
    void ButtonProductInforPressed(cocos2d::Ref *pSender);
    void ButtonEventPressed(cocos2d::Ref *pSender);
    void ButtonNoticePressed(cocos2d::Ref *pSender);
    void ButtonHelpPressed(cocos2d::Ref *pSender);
    
#pragma mark - Touch & Touches
public:
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);
    
};




#endif /* PixSlideLeftMenu_h */
