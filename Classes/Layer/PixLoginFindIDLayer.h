//
//  PixLoginFindIDLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 18..
//
//

#ifndef PixLoginFindIDLayer_h
#define PixLoginFindIDLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditBox.h"

USING_NS_CC;

class PixLayerActionDelegate;

class PixLoginFindIDLayer : public PixModalLayer, public ui::EditBoxDelegate
{
public:
    
    static PixLoginFindIDLayer* create();
    
    PixLoginFindIDLayer();
    virtual ~PixLoginFindIDLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
private:
    
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
    PixEditBox *name;
    PixEditBox *phone;
    ui::Button *name_btn_X;
    ui::Button *phone_btn_X;
    ui::Text *text_result;
    ui::Button *btn_findID;
    ui::Button *btn_back;
    ui::Text *infor_findID;
    ui::Text *Text_email;
    Sprite *Background_name;
    Sprite *Background_name_on;
    Sprite *Background_phone;
    Sprite *Background_phone_on;
    
    ui::Button *btn_login;
    ui::ListView *ListView;
    std::vector<Node*> listItemVec;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(bool, mbln_MemberFindID, MemberFindID)
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_IDFindSuccess, IDFindSuccess)
    
#pragma mark - Button
    
    void btnFindIDPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnLoginPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void name_btn_X_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void phone_btn_X_Pressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
        
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

    void memberFindID();
    void memberFindIDCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};


#endif /* PixLoginFindIDLayer_h */
