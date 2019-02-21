//
//  PixCouponCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 7..
//
//

#ifndef PixCouponCell_h
#define PixCouponCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixCouponCell : public ui::Widget
{
public:
    
    static PixCouponCell* create(const std::string &strSubject, const std::string &strCoupon_price, const std::string &strEdate);
    
    PixCouponCell(const std::string &strSubject, const std::string &strCoupon_price, const std::string &strEdate);
    virtual ~PixCouponCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Subject, Subject)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Coupon_price, Coupon_price)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Edate, Edate)
};

#endif /* PixCouponCell_h */
