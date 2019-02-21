//
//  PixCouponPopupItemCell.hpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#ifndef PixCouponPopupItemCell_h
#define PixCouponPopupItemCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class CouponInfo;
class PixCouponPopupItemCellDelegate;

class PixCouponPopupItemCell : public ui::Widget
{
public:
    
    static PixCouponPopupItemCell* create(CouponInfo *pCouponInfo);
    
    PixCouponPopupItemCell(CouponInfo *pCouponInfo);
    virtual ~PixCouponPopupItemCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    void setCheckSelected(bool var);
    bool isCheckSelected();
    
private:
    
    Node *_customItem;
    ui::CheckBox *check;
 
    CC_SYNTHESIZE(CouponInfo*, mCouponInfo, CouponInfo)
    CC_SYNTHESIZE(PixCouponPopupItemCellDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
};

class PixCouponPopupItemCellDelegate
{
public :
    
    virtual void couponPopupItemCellCheckBoxPressed(cocos2d::Ref* pWidget, bool var) {};
};


#endif /* PixCouponPopupItemCell_h */
