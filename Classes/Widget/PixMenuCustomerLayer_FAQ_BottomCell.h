//
//  PixMenuCustomerLayer_FAQ_BottomCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 10..
//
//

#ifndef PixMenuCustomerLayer_FAQ_BottomCell_h
#define PixMenuCustomerLayer_FAQ_BottomCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixMenuCustomerLayer_FAQ_BottomCell : public ui::Widget
{
    
public :
    
    static PixMenuCustomerLayer_FAQ_BottomCell* create();
    
    PixMenuCustomerLayer_FAQ_BottomCell();
    virtual ~PixMenuCustomerLayer_FAQ_BottomCell();
    
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
        
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
};

#endif /* PixMenuCustomerLayer_FAQ_BottomCell_h */
