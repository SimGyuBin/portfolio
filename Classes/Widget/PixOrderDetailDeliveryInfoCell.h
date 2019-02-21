//
//  PixOrderDetailDeliveryInfoCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 17..
//
//

#ifndef PixOrderDetailDeliveryInfoCell_h
#define PixOrderDetailDeliveryInfoCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class OrderCheckInfo;

class PixOrderDetailDeliveryInfoCell : public ui::Widget
{
public:
    
    static PixOrderDetailDeliveryInfoCell* create(OrderCheckInfo *pOrderCheckInfo);
    
    PixOrderDetailDeliveryInfoCell(OrderCheckInfo *pOrderCheckInfo);
    virtual ~PixOrderDetailDeliveryInfoCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    ui::Text *Text_Name;
    ui::Text *Text_Phone;
    ui::Text *Text_ZipCode;
    ui::Text *Text_Address;
    ui::Text *Text_Address2;
    ui::Text *Text_Memo;
    
    CC_SYNTHESIZE(OrderCheckInfo*, mOrderCheckInfo, OrderCheckInfo)
    
    void updateOrderDetailDeliveryInfoCell();
    
};


#endif /* PixOrderDetailDeliveryInfoCell_h */
