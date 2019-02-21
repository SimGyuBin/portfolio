//
//  PixCartOrderTotalCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 9..
//
//

#ifndef PixCartOrderTotalCell_h
#define PixCartOrderTotalCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class CartOrderTotalInfo;

class PixCartOrderTotalCell : public ui::Widget
{
public:
    
    static PixCartOrderTotalCell* create(CartOrderTotalInfo *pCartOrderTotalInfo);
    
    PixCartOrderTotalCell(CartOrderTotalInfo *pCartOrderTotalInfo);
    virtual ~PixCartOrderTotalCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    void runDeleteAction(const std::function<void(ui::Widget*)>& callback);
    void deleteActionFinish();
    
private:
    
    Node *_customItem;
    CartOrderTotalInfo *mCartOrderTotalInfo;
    ui::Text *OrderTotal;
    ui::Text *Item_Amount;
    ui::Text *Text_event;
    ui::ImageView *ImageView_Price;
    ui::Text *Discounted;
    ui::Text *Delivery;
    ui::Text *pay1;
    ui::Text *Text_Pay;
    ui::Text *pay3;
    ui::Text *Payment;
    ui::Text *Total_pay;
    
    std::string mstr_Cart07;
    std::string mstr_Cart10;
    
    std::function<void(ui::Widget*)> mCallback;
    
    void updateOrderTotal();
};

#endif /* PixCartOrderTotalCell_h */
