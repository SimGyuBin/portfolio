//
//  PixPurchaseFailedTopCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 4..
//
//

#ifndef PixPurchaseFailedTopCell_h
#define PixPurchaseFailedTopCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixPurchaseFailedTopCell : public ui::Widget
{
public:
    
    static PixPurchaseFailedTopCell* create(const std::string& strFail_reason);
    
    PixPurchaseFailedTopCell(const std::string& strFail_reason);
    virtual ~PixPurchaseFailedTopCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Fail_reason, Fail_reason)
};

#endif /* PixPurchaseFailedTopCell_h */
