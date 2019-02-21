//
//  PixMenuMemberPWLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#ifndef PixMenuMemberPWLayer_h
#define PixMenuMemberPWLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixEditBox.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

class PixMenuMemberPWLayer : public PixModalLayer, public ui::EditBoxDelegate
{
public:
    
    static PixMenuMemberPWLayer* create();
    
    PixMenuMemberPWLayer();
    virtual ~PixMenuMemberPWLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *MenuMemberPW_Top;
    ui::Button *Button_back;
    ui::Button *Button_ok;
    
    PixEditBox *NowPW;
    ui::Button *NowPW_btn_X;
    
    PixEditBox *NewPW;
    ui::Button *NewPW_btn_X;
    
    PixEditBox *NewPW2;
    ui::Button *NewPW2_btn_X;

    Sprite *Background_PW;
    Sprite *Background_PW_on;
    Sprite *Background_PW_2;
    Sprite *Background_PW_on_2;
    Sprite *Background_PW_3;
    Sprite *Background_PW_on_3;
    
    ui::Text *infor_PW;
    ui::Text *infor_PW_2;
    ui::Text *infor_PW_3;
    
    bool mbln_IsAction;

    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    bool checkChgPw(bool showAlert = false);
  
#pragma mark - Button
    
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonNowPWbtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonNewPWbtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonNewPWbtnX2Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
        
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
    
    void member_chgPw();
    void member_chgPwCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMenuMemberPWLayer_h */
