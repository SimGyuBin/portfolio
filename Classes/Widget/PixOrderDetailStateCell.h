//
//  PixOrderDetailStateCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 13..
//
//

#ifndef PixOrderDetailStateCell_h
#define PixOrderDetailStateCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

class OrderCheckInfo;
class PixOrderDetailStateCellDelegate;

class PixOrderDetailStateCell : public ui::Widget
{
public:
    
    static PixOrderDetailStateCell* create(OrderCheckInfo *pOrderCheckInfo);
    
    PixOrderDetailStateCell(OrderCheckInfo *pOrderCheckInfo);
    virtual ~PixOrderDetailStateCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    PixScrollMenu *ChangeAddress;
    PixScrollMenu *CancelOrder;
    PixScrollMenu *TrackingNumber;
    ui::Text *Text_Periodinfor;
    ui::Text *Text_Cancel;
    
    CC_SYNTHESIZE(OrderCheckInfo*, mOrderCheckInfo, OrderCheckInfo)
    CC_SYNTHESIZE(PixOrderDetailStateCellDelegate*, mDelegate, Delegate)
    
    Node* customCall();
    Node* customCancellationCall();
    Node* customCancellationProcessingCeaal();
    
    void updateDetailStateUI();
    void buttonStatePressed(cocos2d::Ref *pSender);
    
};

class PixOrderDetailStateCellDelegate
{
public:
    
    virtual void orderDetailStateCellPressed(OrderCheckInfo *pOrderCheckInfo, int nTag) {};
};


#endif /* PixOrderDetailStateCell_h */
