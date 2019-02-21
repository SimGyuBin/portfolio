//
//  PixLoginLayer.h
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 24..
//
//

#ifndef PixLoginLayer_h
#define PixLoginLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditBox.h"
#include "../Network/PixHttpClient.h"
#include "../Layer/PixLayerActionDelegate.h"

class PixLoginLayer : public PixModalLayer, public PixLayerActionDelegate, public ui::EditBoxDelegate
{
public:
  
    static PixLoginLayer* create();
    
    PixLoginLayer();
    virtual ~PixLoginLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);

    PixHttpClient *pixHttpClient;
    
    Node  *rootNode;
    
    ui::Button *btn_back;
    ui::EditBox *text_ID;
    Sprite *login_text_back;
    Sprite *login_text_back_on;
    ui::Button *text_ID_btn_x;
    
    ui::EditBox *text_PW;
    Sprite *login_text_back_2;
    Sprite *login_text_back_on2;
    ui::Button *text_PW_btn_x;
    
    ui::Button *Button_login;
    ui::Button *Button_login_face;
    ui::Button *find_ID;
    ui::Button *find_PW;
    ui::Button *join;
    
    ui::Text *Text_infor_Email;
    ui::Text *Text_infor_PW;
    
    bool mbln_IsAction;
    
    bool checkMemberAuth(bool showAlert = false);
    void checkGuidanceMessage();
    
#pragma mark - Button
    
    void btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void loginPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void findIDPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void findPWPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void text_ID_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void text_PW_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(ui::EditBox* editBox);

#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - Action
    
    void showLoginLayerAction();
    void hideLoginLayerAction();
    void showLoginLayerActionFinish(Node* node);
    void hideLoginLayerActionFinish(Node* node);
    
#pragma mark - Network
    
    void memberAuth();
    void memberAuthCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void memberCheckCall();
    void memberCheckCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixLoginLayer_h */
