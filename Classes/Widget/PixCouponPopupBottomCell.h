//
//  PixCouponPopupBottomCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#ifndef PixCouponPopupBottomCell_h
#define PixCouponPopupBottomCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixCouponPopupBottomCell : public ui::Widget
{
public:
    
    static PixCouponPopupBottomCell* create();
    
    PixCouponPopupBottomCell();
    virtual ~PixCouponPopupBottomCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
};

#endif /* PixCouponPopupBottomCell_h */
