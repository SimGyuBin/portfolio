//
//  PixOrderMoreCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 13..
//
//

#ifndef PixOrderMoreCell_h
#define PixOrderMoreCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

class PixOrderMoreCell : public ui::Widget
{
public:
    
    static PixOrderMoreCell* create(const ccMenuCallback& callback);
    
    PixOrderMoreCell();
    virtual ~PixOrderMoreCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem(const ccMenuCallback& callback);
    
private:
    
    Node *_customItem;
    
};

#endif /* PixOrderMoreCell_h */
