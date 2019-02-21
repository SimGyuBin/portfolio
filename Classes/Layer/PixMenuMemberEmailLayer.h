//
//  PixMenuMemberEmailLayer.hpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 2..
//
//

#ifndef PixMenuMemberEmailLayer_h
#define PixMenuMemberEmailLayer_h

#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixEditBox.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

class PixMenuMemberEmailLayer : public PixModalLayer, public ui::EditBoxDelegate
{
public:
    
    static PixMenuMemberEmailLayer* create();
    
    PixMenuMemberEmailLayer();
    virtual ~PixMenuMemberEmailLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *MenuMemberEmail_Top;
    ui::Button *Button_back;
    ui::Button *Button_ok;
    
    Sprite *Background_ID;
    Sprite *Background_ID_on;
    
    PixEditBox *Text_ID;
    ui::Button *btn_X;
    
    ui::Text *infor_Email;
    ui::Text *infor_Email_02;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    bool checkChgEmail(bool showAlert = false);
    
#pragma mark - Button
    
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonbtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
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
    
    void member_chgEmail();
    void member_chgEmailCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMenuMemberEmailLayer_h */
