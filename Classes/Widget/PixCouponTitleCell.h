//
//  PixCouponTitleCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 7..
//
//

#ifndef PixCouponTitleCell_h
#define PixCouponTitleCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixCouponTitleCell : public ui::Widget
{
public:
    
    static PixCouponTitleCell* create(int nCount);
    
    PixCouponTitleCell(int nCount);
    virtual ~PixCouponTitleCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE(int, mn_Count, Count)
};

#endif /* PixCouponTitleCell_h */
