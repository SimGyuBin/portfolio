//
//  PixOrderMonthlyCategoryCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 12..
//
//

#ifndef PixOrderMonthlyCategoryCell_h
#define PixOrderMonthlyCategoryCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixOrderMonthlyCategoryCell : public ui::Widget
{
public:
    
    static PixOrderMonthlyCategoryCell* create(const std::string& strDate);
    
    PixOrderMonthlyCategoryCell(const std::string& strDate);
    virtual ~PixOrderMonthlyCategoryCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Date, Date)
};

#endif /* PixOrderMonthlyCategoryCell_h */
