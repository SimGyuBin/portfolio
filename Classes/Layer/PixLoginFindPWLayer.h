//
//  PixLoginFindPWLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 18..
//
//

#ifndef PixLoginFindPWLayer_h
#define PixLoginFindPWLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditBox.h"

USING_NS_CC;

class PixLoginFindPWLayer : public PixModalLayer, public ui::EditBoxDelegate
{
public:
    
    static PixLoginFindPWLayer* create();
    
    PixLoginFindPWLayer();
    virtual ~PixLoginFindPWLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
private:
   
    PixHttpClient *pixHttpClient;
    
    Node  *rootNode;
    PixEditBox *name;
    PixEditBox *email;
    ui::Text *text_result;
    ui::Button *name_btn_X;
    ui::Button *email_btn_X;
    ui::Button *btn_ok;
    ui::Button *emailsend;
    ui::Button *btn_back;
    Sprite *Background_name;
    Sprite *Background_name_on;
    Sprite *Background_email;
    Sprite *Background_email_on;
    
    bool mbln_IsAction;
    
#pragma mark - Button
    
    void btnOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void emailsendPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void name_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void email_btn_x_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
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
    
    void memberFindPW();
    void memberFindPWCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};


#endif /* PixLoginFindPWLayer_h */
