//
//  PixPurchaseCompletedTitleCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 5..
//
//

#ifndef PixPurchaseCompletedTitleCell_h
#define PixPurchaseCompletedTitleCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixPurchaseCompletedTitleCell : public ui::Widget
{
public:
    
    static PixPurchaseCompletedTitleCell* create(const std::string& strOrder_id, const std::string& strReg_date);
    
    PixPurchaseCompletedTitleCell(const std::string& strOrder_id, const std::string& strReg_date);
    virtual ~PixPurchaseCompletedTitleCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Order_id, Order_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Reg_date, Reg_date)
};

#endif /* PixPurchaseCompletedTitleCell_h */
