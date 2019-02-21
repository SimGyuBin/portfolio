//
//  PixPayDeliveryCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 23..
//
//

#ifndef PixPayDeliveryCell_h
#define PixPayDeliveryCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Network/PixHttpClient.h"

USING_NS_CC;

class DeliveryInfo;
class PixPayDeliveryCellDelegate;

class PixPayDeliveryCell : public ui::Widget
{
public:
    
    static PixPayDeliveryCell *create(DeliveryInfo *pInfo);
    
    PixPayDeliveryCell(DeliveryInfo *pInfo);
    virtual ~PixPayDeliveryCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    void setCheckSelected(bool var);
    bool isCheckSelected();
    
private:
    
    Node *_customItem;
    PixHttpClient *pixHttpClient;
    ui::CheckBox *check;
    ui::Text *Text_Name;
    ui::Text *Text_Phone;
    ui::Text *Text_Address;
    ui::Text *Text_Default;
    PixScrollMenu *Button_remove;
    
    CC_SYNTHESIZE(DeliveryInfo*, mDeliveryInfo, DeliveryInfo)
    CC_SYNTHESIZE(PixPayDeliveryCellDelegate*, mDelegate, Delegate)
    
    void updateDelivery();

#pragma mark - Action
    
    void runDeleteAction();
    void deleteActionFinish();
    
#pragma mark - Button
    
    void ButtonRemovePressed(cocos2d::Ref *pSender);
    void checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    
#pragma mark - Network
    
    void delAddrCall();
    void delAddrCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};

class PixPayDeliveryCellDelegate
{
public:
    
    virtual void payDeliveryCellcheckBoxPressed(DeliveryInfo *pInfo) {};
    virtual void payDeliveryCellDeleteActionStart(Ref *pTarget) {};
    virtual void payDeliveryCellDeleteActionFinish(Ref *pTarget) {};
    virtual void payDeliveryCellNoAddressAction(Ref *pTarget) {};
    
};


#endif /* PixPayDeliveryCell_h */
