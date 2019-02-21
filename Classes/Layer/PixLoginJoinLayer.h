//
//  PixLoginJoinLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 18..
//
//

#ifndef PixLoginJoinLayer_h
#define PixLoginJoinLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditBox.h"
#include "../Network/PixHttpClient.h"

USING_NS_CC;

class PixLoginJoinLayer : public PixModalLayer, public ui::EditBoxDelegate
{
public:
    
    static PixLoginJoinLayer* create();

    PixLoginJoinLayer();
    virtual ~PixLoginJoinLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
private:
    
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
    
    PixEditBox *Text_ID;
    PixEditBox *Text_PW;
    PixEditBox *name;
    PixEditBox *phone;
    PixEditBox *Text_email;
    ui::Button *Text_ID_btn_X;
    Sprite *Background_ID;
    Sprite *Background_ID_on;
    ui::Button *Text_PW_btn_X;
    Sprite *Background_PW;
    Sprite *Background_PW_on;
    ui::Button *name_btn_X;
    Sprite *Background_name;
    Sprite *Background_name_on;
    ui::Button *phone_btn_X;
    Sprite *Background_phone;
    Sprite *Background_phone_on; 
    ui::Button *email_btn_X;
    Sprite *Background_Email;
    Sprite *Background_Email_on;
    ui::Text *infor_Email_01;
    ui::Text *infor_PW;
    ui::Text *infor_phone;
    ui::Text *infor_Email_02;
    ui::Button *btn_agreement;
    ui::Button *btn_person;
    ui::Button *btn_back;
    ui::Button *btn_join;
    ui::CheckBox *check;
    
    bool mbln_IsAction;
    
    bool checkMemberJoin(bool showAlert = false);
    
#pragma mark - Button
    
    void text_ID_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void text_PW_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void name_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void phone_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void email_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnAgreementPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnPersonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnJoinPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - CheckBox
    
    void check1Pressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void check2Pressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);

#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(ui::EditBox* editBox);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Network
    
    void memberJoin();
    virtual void memberJoinCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};


#endif /* PixLoginJoinLayer_h */
