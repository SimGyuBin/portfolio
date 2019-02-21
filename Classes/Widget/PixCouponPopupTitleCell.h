//
//  PixCouponPopupTitleCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#ifndef PixCouponPopupTitleCell_h
#define PixCouponPopupTitleCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixCouponPopupTitleCell : public ui::Widget
{
public:
    
    static PixCouponPopupTitleCell* create(int nCount);
    
    PixCouponPopupTitleCell(int nCount);
    virtual ~PixCouponPopupTitleCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE(int, mn_Count, Count)
};

#endif /* PixCouponPopupTitleCell_h */
