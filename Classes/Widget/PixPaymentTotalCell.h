//
//  PixPaymentTotalCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 14..
//
//

#ifndef PixPaymentTotalCell_h
#define PixPaymentTotalCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class CartOrderTotalInfo;

class PixPaymentTotalCell : public ui::Widget
{
public:
    
    static PixPaymentTotalCell* create(CartOrderTotalInfo *pCartOrderTotalInfo);
    
    PixPaymentTotalCell(CartOrderTotalInfo *pCartOrderTotalInfo);
    virtual ~PixPaymentTotalCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    CartOrderTotalInfo *mCartOrderTotalInfo;
    ui::Text *pay_1;
    ui::Text *pay_2;
    ui::Text *pay_3;
    ui::Text *Total_pay;
    std::string mstr_Cart07;
    std::string mstr_Cart10;
    
    void updatePaymentTotal();
};

#endif /* PixPaymentTotalCell_h */
