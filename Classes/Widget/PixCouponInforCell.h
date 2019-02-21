//
//  PixCouponInforCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 7..
//
//

#ifndef PixCouponInforCell_h
#define PixCouponInforCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixCouponInforCell : public ui::Widget
{
public:
    
    static PixCouponInforCell* create();
    
    PixCouponInforCell();
    virtual ~PixCouponInforCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
};

#endif /* PixCouponInforCell_h */
