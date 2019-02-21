//
//  PixMenuMemberLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#ifndef PixMenuMemberLayer_h
#define PixMenuMemberLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"
#include "PixMenuProfilePopupLayer.h"

USING_NS_CC;

class PixMenuMemberLayer : public PixModalLayer, public PixLayerActionDelegate, public PixMenuProfilePopupDelegate
{
public:
    
    static PixMenuMemberLayer* create();
    
    PixMenuMemberLayer();
    virtual ~PixMenuMemberLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *MenuMemberTopLayer;
    ui::Button *Button_cancel;
    
    Sprite *profliePhoto;
    ui::Text *Text_email_2;
    
    ui::Button *Button_Incomingemail;
    ui::Text *Text_Incomingemail2;
    ui::Button *Button_Password;
    ui::Button *Button_Name;
    ui::Text *Text_Name;
    ui::Button *Button_Phone;
    ui::Text *Text_Phone;
    ui::Button *Button_Logout;
    ui::Button *Button_leave;
    ui::CheckBox *Check_onoff;
    
    Texture2D *proflieDefaultPhoto;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonIncomingemailPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonPasswordPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonNamePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonPhonePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonLogoutPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonLeavePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void alarmCheckboxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    
#pragma mark - Action
    
    void showMenuMemberLayerAction();
    void hideMenuMemberLayerAction();
    void showMenuMemberLayerActionFinish(Node* node);
    void hideMenuMemberLayerActionFinish(Node* node);

#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    virtual void profileChange();
    virtual void profileRemove();
    
#pragma mark - Network
    
    void logoutCall();
    void onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMenuMemberLayer_h */
