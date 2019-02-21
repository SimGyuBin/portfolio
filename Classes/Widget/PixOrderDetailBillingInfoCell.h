//
//  PixOrderDetailBillingInfoCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 17..
//
//

#ifndef PixOrderDetailBillingInfoCell_h
#define PixOrderDetailBillingInfoCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class OrderCheckInfo;

class PixOrderDetailBillingInfoCell : public ui::Widget
{
public:
    
    static PixOrderDetailBillingInfoCell* create(OrderCheckInfo *pOrderCheckInfo);
    
    PixOrderDetailBillingInfoCell(OrderCheckInfo *pOrderCheckInfo);
    virtual ~PixOrderDetailBillingInfoCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE(OrderCheckInfo*, mOrderCheckInfo, OrderCheckInfo)
    
    Node* createCell();
    Node* createVbankCell();
    
};

#endif /* PixOrderDetailBillingInfoCell_h */
