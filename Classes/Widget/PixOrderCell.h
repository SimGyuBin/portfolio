//
//  PixOrderCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 12..
//
//

#ifndef PixOrderCell_h
#define PixOrderCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class OrderCheckInfo;
class PixOrderGoodsCell;

class PixOrderCell : public ui::Widget
{
public:
    
    static PixOrderCell* create(OrderCheckInfo *pOrderCheckInfo);
    
    PixOrderCell(OrderCheckInfo *pOrderCheckInfo);
    virtual ~PixOrderCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE(OrderCheckInfo*, mOrderCheckInfo, OrderCheckInfo)
    
};

#endif /* PixOrderCell_h */
