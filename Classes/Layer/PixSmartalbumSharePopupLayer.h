//
//  PixSmartalbumSharePopupLayer.
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 23..
//
//

#ifndef PixSmartalbumSharePopupLayer_h
#define PixSmartalbumSharePopupLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;


typedef enum
{
    SMARTALBUM_SHARE_NONE = 0,
    SMARTALBUM_SHARE_FACEBOOK,
    SMARTALBUM_SHARE_INSTA,
    SMARTALBUM_SHARE_KAS,
    SMARTALBUM_SHARE_KAKAO,
   
} PIX_SMARTALBUM_SHARE_TYPE;

class PixSmartalbumShareDelegate;

class PixSmartalbumSharePopupLayer : public PixModalLayer
{
public:
    
    static PixSmartalbumSharePopupLayer* create();
    
    PixSmartalbumSharePopupLayer();
    virtual ~PixSmartalbumSharePopupLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node  *rootNode;
    
    Sprite *blockout;
    Node *SharePopup;
    
    ui::Button *Button_facebook;
    ui::Button *Button_insta;
    ui::Button *Button_kas;
    ui::Button *Button_kakao;
    
    bool mbln_isAction;
    PIX_SMARTALBUM_SHARE_TYPE eShareType;
    
    void showServiceComingSoon();
    
    CC_SYNTHESIZE(PixSmartalbumShareDelegate*, mShareDelegate, ShareDelegate)
    
#pragma mark - Button
    
    void buttonFacebookPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonInstaPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonKasPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonKakaoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

class PixSmartalbumShareDelegate
{
public:
    
    virtual void smartAlbumShare(PIX_SMARTALBUM_SHARE_TYPE type) {};
};



#endif /* PixSmartalbumSharePopupLayer_h */
