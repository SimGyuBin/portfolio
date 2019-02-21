//
//  PixDeliveryChangeLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 17..
//
//

#ifndef PixDeliveryChangeLayer_h
#define PixDeliveryChangeLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditBox.h"
#include "PixLayerActionDelegate.h"
#include "PixSearchAddressLayer.h"

USING_NS_CC;

class OrderCheckInfo;

class PixDeliveryChangeLayer : public PixModalLayer, public PixLayerActionDelegate, public ui::EditBoxDelegate, public PixSearchAddressLayerDelegate
{
public:
    
    static PixDeliveryChangeLayer* create(OrderCheckInfo *pOrderCheckInfo);
    
    PixDeliveryChangeLayer(OrderCheckInfo *pOrderCheckInfo);
    virtual ~PixDeliveryChangeLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    PixHttpClient *pixHttpClient;
    ui::Button *Button_ok;
    PixEditBox *name;
    PixEditBox *phoneNumber;
    PixEditBox *detailedAddress;
    PixEditBox *memo;
    
    ui::Text *text_ZipCode;
    ui::Text *text_Address;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Post, Post)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Old_post, Old_post)
    CC_SYNTHESIZE(OrderCheckInfo*, mOrderCheckInfo, OrderCheckInfo)
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    const char* getName();
    const char* getPhoneNumber();
    const char* getZipCode();
    const char* getAddress();
    const char* getDetailedAddress();
    const char* getMemo();
    
    bool checkCompile();
    bool checkDelivery();
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonSearchPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(ui::EditBox* editBox);
    
#pragma mark - Action
    
    void showDeliveryChangeLayerAction();
    void hideDeliveryChangeLayerAction();
    void showDeliveryChangeLayerActionFinish(Node* node);
    void hideDeliveryChangeLayerActionFinish(Node* node);

#pragma mark - Delegate
    
    virtual void searchAddressCompile(const std::string& str);
    
#pragma mark - Network
    
    void getAddShipPriceCall();
    void getAddShipPriceCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void updOrderAddrCall();
    void updOrderAddrCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    void androidBackPressed();
};

#endif /* PixDeliveryChangeLayer_h */
