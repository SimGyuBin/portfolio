//
//  PixCouponPopupEventDiscountCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#ifndef PixCouponPopupEventDiscountCell_h
#define PixCouponPopupEventDiscountCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixCouponPopupEventDiscountCellDelegate;

class PixCouponPopupEventDiscountCell : public ui::Widget
{
public:
    
    static PixCouponPopupEventDiscountCell* create(int nEventDiscount, bool blnCheckEnabled);
    
    PixCouponPopupEventDiscountCell(int nEventDiscount, bool blnCheckEnabled);
    virtual ~PixCouponPopupEventDiscountCell();
    
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
    ui::CheckBox *check;
    
    CC_SYNTHESIZE(int, mn_EventDiscount, EventDiscount)
    CC_SYNTHESIZE(bool, mbln_CheckEnabled, CheckEnabled)
    CC_SYNTHESIZE(PixCouponPopupEventDiscountCellDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    
};

class PixCouponPopupEventDiscountCellDelegate
{
public :
    
    virtual void couponPopupEventDiscountCellCheckBoxPressed(bool var) {};
};


#endif /* PixCouponPopupEventDiscountCell_h */
