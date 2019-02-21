//
//  PixSocialLoginLayer.hpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 1..
//
//

#ifndef PixSocialLoginLayer_hpp
#define PixSocialLoginLayer_hpp

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditBox.h"
#include "../Network/PixHttpClient.h"
#include "../Layer/PixLayerActionDelegate.h"

class PixSocialLoginLayer : public PixModalLayer, public ui::EditBoxDelegate
{
public:
    
    static PixSocialLoginLayer* facebookLogin(std::map<std::string, std::string> &facebookUserInfo);
    
    PixSocialLoginLayer();
    virtual ~PixSocialLoginLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);

    void setFacebookUserInfo(std::map<std::string, std::string> &facebookUserInfo);
    bool checkMemberJoin(bool showAlert = false);
    
private:
    
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
    ui::Button *btn_back;
    
    ui::Text *text_01;
    ui::Text *text_02;
    
    Sprite *Background_ID;
    Sprite *Background_ID_on;
    PixEditBox *Text_ID;
    ui::Button *btn_X;
    ui::Text *infor_Email_02;
    ui::Text *infor_Email;
    
    Sprite *Background_name;
    Sprite *Background_name_on;
    PixEditBox *name;
    ui::Button *btn_X_2;
    ui::Text *infor_Name;

    ui::Button *btn_join;
    ui::Text *Text_btn_join;
    
    ui::CheckBox *check2;
    ui::Text *sms;
    
    bool mbln_IsAction;

    std::map<std::string, std::string> facebookUserInfo;

#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);

#pragma mark - Button
    
    void btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void email_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void name_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnJoinPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(ui::EditBox* editBox);
    
#pragma mark - Network
    
    void facebookMemberAuth();
    void memberAuthCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void memberCheckCall();
    void memberCheckCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};


#endif /* PixSocialLoginLayer_hpp */
