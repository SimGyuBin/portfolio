//
//  PixMenuSNSLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#ifndef PixMenuSNSLayer_h
#define PixMenuSNSLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

class PixMenuSNSLayer : public PixModalLayer, public PixLayerActionDelegate
{
public:
    
    static PixMenuSNSLayer* create();
    
    PixMenuSNSLayer();
    virtual ~PixMenuSNSLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *MenuSNS_Top;
    ui::Button *Button_cancel;
    
    Sprite *bk_Facebook;
    ui::CheckBox *facebook_Check_onoff;
    
    Sprite *bk_google;
    ui::CheckBox *google_Check_onoff;
    
    Sprite *bk_insta;
    ui::CheckBox *insta_Check_onoff;
    
    Sprite *bk_kakao;
    ui::CheckBox *kakao_Check_onoff;
    
    bool mbln_IsAction;

    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void facebookCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void googleCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void instaCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void kakaoCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    
#pragma mark - Action
    
    void showMenuSNSLayerAction();
    void hideMenuSNSLayerAction();
    void showMenuSNSLayerActionFinish(Node* node);
    void hideMenuSNSLayerActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - Network
    
    void onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMenuSNSLayer_h */
