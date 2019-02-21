//
//  PixMenuCouponLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 6..
//
//

#ifndef PixMenuCouponLayer_h
#define PixMenuCouponLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixEditBox.h"
#include "../Widget/PixCouponTitleCell.h"
#include "../Widget/PixCouponCell.h"
#include "../Widget/PixCouponInforCell.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

typedef enum
{
    COUPONLAYER_NONE = 0,
    COUPONLAYER_LIST,
    COUPONLAYER_REGISTER,
} COUPONLAYER_TYPE;

class PixMenuCouponLayer : public PixModalLayer, public PixLayerActionDelegate, public ui::EditBoxDelegate
{
public:
    
    static PixMenuCouponLayer* create();
    
    PixMenuCouponLayer();
    virtual ~PixMenuCouponLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    void showRegisterLayer();
    
private:
    
    struct MenuCouponStruct;
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    Node *Top_01;
    Node *Top_02;
    Node *Node_MenuCoupon_Empty;
    ui::Button *Button_Register;
    ui::Button *Button_Register_2;
    Node *Node_MenuCouponRegisterLayer;
    PixEditBox *TextField_1;
    PixEditBox *TextField_2;
    PixEditBox *TextField_3;
    PixEditBox *TextField_4;
    PixGalleryListView *couponListView;
    COUPONLAYER_TYPE me_Type;
    bool mbln_IsAction;
    bool mbln_showCouponRegisterLayer;
    std::vector<MenuCouponStruct*> couponList;

    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    void createCouponListView();
    void setLayerTypr(COUPONLAYER_TYPE eType);
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonRegisterCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonRegisterPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonCouponRegisterPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showMenuCouponLayerAction();
    void hideMenuCouponLayerAction();
    void showMenuCouponLayerActionFinish(Node* node);
    void hideMenuCouponLayerActionFinish(Node* node);
    void showCouponRegisterLayerAction();
    void hideCouponRegisterLayerAction();
    void showCouponRegisterLayerActionFinish(Node* node);
    void hideCouponRegisterLayerActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(ui::EditBox* editBox);
    
#pragma mark - Network
    
    void getAllCouponCall();
    void getAllCouponCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void setCouponCall();
    void setCouponCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();

};

#endif /* PixMenuCouponLayer_h */
