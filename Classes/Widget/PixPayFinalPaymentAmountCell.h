//
//  PixPayFinalPaymentAmountCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 24..
//
//

#ifndef PixPayFinalPaymentAmountCell_h
#define PixPayFinalPaymentAmountCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

typedef enum
{
    DELIVERY_CHARGE_NONE = 0,
    DELIVERY_CHARGE_ADD,
    DELIVERY_CHARGE_COUNT,
} DELIVERY_CHARGE_TYPE;

class CartOrderTotalInfo;

class PixPayFinalPaymentAmountCell : public ui::Widget
{
public:
    
    static PixPayFinalPaymentAmountCell* create(CartOrderTotalInfo *pFinalTotalInfo);
    
    PixPayFinalPaymentAmountCell(CartOrderTotalInfo *pFinalTotalInfo);
    virtual ~PixPayFinalPaymentAmountCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    bool isAgreeSelected();
    
private:
    
    Node *_customItem;
    CartOrderTotalInfo *mFinalTotalInfo;
    
    ui::Layout *Panel_None;
    ui::Layout *Panel_Add;

    ui::Text *Text_pay_1[DELIVERY_CHARGE_COUNT];
    ui::Text *Text_pay_2[DELIVERY_CHARGE_COUNT];
    ui::Text *Text_pay_3[DELIVERY_CHARGE_COUNT];
    ui::Text *Text_pay_5;
    ui::Text *Text_Delivery_infor;
    
    ui::CheckBox *check;
    PixScrollMenu *Button_agree;
    
    std::string mstr_Cart07;
    std::string mstr_Cart10;
    
    CC_SYNTHESIZE(DELIVERY_CHARGE_TYPE, me_DeliveryChargeType, DeliveryChargeType)
    
    void updatePayFinalPaymentAmount();
    
#pragma mark - Button
    
    void checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void buttonAgreePressed(cocos2d::Ref *pSender);

};

#endif /* PixPayFinalPaymentAmountCell_h */
