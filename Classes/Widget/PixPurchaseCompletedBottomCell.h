//
//  PixPurchaseCompletedBottomCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 5..
//
//

#ifndef PixPurchaseCompletedBottomCell_h
#define PixPurchaseCompletedBottomCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

class PixPurchaseCompletedBottomCellDelegate;

class PixPurchaseCompletedBottomCell : public ui::Widget
{
public:
    
    static PixPurchaseCompletedBottomCell* create();
    
    PixPurchaseCompletedBottomCell();
    virtual ~PixPurchaseCompletedBottomCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    PixScrollMenu *Button_More;
    
    CC_SYNTHESIZE(PixPurchaseCompletedBottomCellDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonMorePressed(cocos2d::Ref *pSender);
};

class PixPurchaseCompletedBottomCellDelegate
{
public :
    virtual void purchaseCompletedBottomCellMoerPressed() {};
};

#endif /* PixPurchaseCompletedBottomCell_h */
