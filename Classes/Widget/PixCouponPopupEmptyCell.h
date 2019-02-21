//
//  PixCouponPopupEmptyCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#ifndef PixCouponPopupEmptyCell_h
#define PixCouponPopupEmptyCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixCouponPopupEmptyCell : public ui::Widget
{
public:
    
    static PixCouponPopupEmptyCell* create();
    
    PixCouponPopupEmptyCell();
    virtual ~PixCouponPopupEmptyCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
};

#endif /* PixCouponPopupEmptyCell_h */
