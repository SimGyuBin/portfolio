//
//  CouponInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#ifndef CouponInfo_h
#define CouponInfo_h

#include "cocos2d.h"

class CouponInfo
{
public:
    
    CouponInfo();
    virtual ~CouponInfo();
    
    CouponInfo *clone();
    
private:
    
    CC_SYNTHESIZE(int, mn_Coupon_group_id, Coupon_group_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Coupon_id, Coupon_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Subject, Subject)
    CC_SYNTHESIZE(int, mn_Method, Method)
    CC_SYNTHESIZE(int, mn_Coupon_value, Coupon_value)
    CC_SYNTHESIZE(int, mn_Coupon_price, Coupon_price)
    
};

#endif /* couponInfo_h */
