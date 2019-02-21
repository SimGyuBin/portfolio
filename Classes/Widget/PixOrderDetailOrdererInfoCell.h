//
//  PixOrderDetailOrdererInfoCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 17..
//
//

#ifndef PixOrderDetailOrdererInfoCell_h
#define PixOrderDetailOrdererInfoCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class OrderCheckInfo;

class PixOrderDetailOrdererInfoCell : public ui::Widget
{
public:
    
    static PixOrderDetailOrdererInfoCell* create(OrderCheckInfo *pOrderCheckInfo);
    
    PixOrderDetailOrdererInfoCell(OrderCheckInfo *pOrderCheckInfo);
    virtual ~PixOrderDetailOrdererInfoCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE(OrderCheckInfo*, mOrderCheckInfo, OrderCheckInfo)
    
};

#endif /* PixOrderDetailOrdererInfoCell_h */
