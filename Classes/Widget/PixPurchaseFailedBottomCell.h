//
//  PixPurchaseFailedBottomCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 4..
//
//

#ifndef PixPurchaseFailedBottomCell_h
#define PixPurchaseFailedBottomCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixPurchaseFailedBottomCell : public ui::Widget
{
public:
    
    static PixPurchaseFailedBottomCell* create();
    
    PixPurchaseFailedBottomCell();
    virtual ~PixPurchaseFailedBottomCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
};

#endif /* PixPurchaseFailedBottomCell_h */
