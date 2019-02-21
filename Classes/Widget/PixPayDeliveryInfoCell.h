//
//  PixPayDeliveryInfoCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 20..
//
//

#ifndef PixPayDeliveryInfoCell_h
#define PixPayDeliveryInfoCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

typedef enum
{
    DELIVERY_NONE = 0,
    CARTLAYER_LIST,
    CARTLAYER_NEW,
    CARTLAYER_ONLY_NEW,
    
} DELIVERY_TYPE;

class PixPayDeliveryInfoCellDelegate;

class PixPayDeliveryInfoCell : public ui::Widget
{
public:
    
    static PixPayDeliveryInfoCell* create();
    
    PixPayDeliveryInfoCell();
    virtual ~PixPayDeliveryInfoCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void update(float dt) override;
    virtual void onExit() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    PixScrollMenu *Button_set_List;
    PixScrollMenu *Button_del_List;
    PixScrollMenu *Button_set_New;
    PixScrollMenu *Button_del_New;
    
    CC_SYNTHESIZE(DELIVERY_TYPE, me_DeliveryType, DeliveryType)
    CC_SYNTHESIZE(PixPayDeliveryInfoCellDelegate*, mDelegate, Delegate)
    
    void updateDeliveryInfoUI();
    
#pragma mark - Button
    
    void buttonSetListPressed(cocos2d::Ref *pSender);
    void buttonSetNewPressed(cocos2d::Ref *pSender);
};

class PixPayDeliveryInfoCellDelegate
{
public:
    
    virtual void payDeliveryInfoCellListPressed() {};
    virtual void payDeliveryInfoCellNewPressed() {};
};

#endif /* PixPayDeliveryInfoCell_h */
