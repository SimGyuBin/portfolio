//
//  PixPaymentCountCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 14..
//
//

#ifndef PixPaymentCountCell_h
#define PixPaymentCountCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixPaymentCountCell : public ui::Widget
{
public:
    
    static PixPaymentCountCell* create(int nCount);
    
    PixPaymentCountCell(int nCount);
    virtual ~PixPaymentCountCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();

private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE(int, mn_Count, Count)
};


#endif /* PixPaymentCountCell_h */
