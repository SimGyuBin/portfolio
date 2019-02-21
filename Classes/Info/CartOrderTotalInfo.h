//
//  CartOrderTotalInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 8..
//
//

#ifndef CartOrderTotalInfo_h
#define CartOrderTotalInfo_h

#include "cocos2d.h"

class CartOrderTotalInfo
{
public:
    
    CartOrderTotalInfo();
    virtual ~CartOrderTotalInfo();
    
    CartOrderTotalInfo *clone();
    
private:
    
    CC_SYNTHESIZE(int, mn_Total_price, Total_price)
    CC_SYNTHESIZE(int, mn_Total_pay_price, Total_pay_price)
    CC_SYNTHESIZE(int, mn_Total_discount_price, Total_discount_price)
    CC_SYNTHESIZE(int, mn_Ship_price, Ship_price)
    CC_SYNTHESIZE(int, mn_Pay_add_ship_amount, Pay_add_ship_amount)
};

#endif /* CartOrderTotalInfo_h */
