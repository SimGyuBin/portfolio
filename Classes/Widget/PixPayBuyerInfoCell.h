//
//  PixPayBuyerInfoCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 20..
//
//

#ifndef PixPayBuyerInfoCell_h
#define PixPayBuyerInfoCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixPayBuyerInfoCell : public ui::Widget
{
public:
    
    static PixPayBuyerInfoCell* create();
    
    PixPayBuyerInfoCell();
    virtual ~PixPayBuyerInfoCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
};


#endif /* PixPayBuyerInfoCell_h */
