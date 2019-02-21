//
//  PixMenuMemberPhoneLayer.hpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#ifndef PixMenuMemberPhoneLayer_h
#define PixMenuMemberPhoneLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixEditBox.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

class PixMenuMemberPhoneLayer : public PixModalLayer, public ui::EditBoxDelegate
{
public:
    
    static PixMenuMemberPhoneLayer* create();
    
    PixMenuMemberPhoneLayer();
    virtual ~PixMenuMemberPhoneLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *MenuMemberPhone_Top;
    ui::Button *Button_back;
    ui::Button *Button_ok;
    
    PixEditBox *phone;
    ui::Button *btn_X;
    
    Sprite *Background_phone;
    Sprite *Background_phone_on;
    ui::Text *infor_phone;

    bool mbln_IsAction;

    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    bool checkchgHp(bool showAlert = false);
    
#pragma mark - Button
    
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonPhonebtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
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
    
    void member_chgHp();
    void member_chgHpCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};


#endif /* PixMenuMemberPhoneLayer_h */
