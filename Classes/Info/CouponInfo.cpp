//
//  CouponInfo.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#include "CouponInfo.h"

CouponInfo::CouponInfo()
: mn_Coupon_group_id(0)
, mstr_Coupon_id("")
, mstr_Subject("")
, mn_Method(0)
, mn_Coupon_value(0)
, mn_Coupon_price(0)
{
    
}

CouponInfo::~CouponInfo()
{
    
}

CouponInfo *CouponInfo::clone()
{
    CouponInfo *clone = new CouponInfo();

    clone->setCoupon_group_id(getCoupon_group_id());
    clone->setCoupon_id(getCoupon_id());
    clone->setSubject(getSubject());
    clone->setMethod(getMethod());
    clone->setCoupon_value(getCoupon_value());
    clone->setCoupon_price(getCoupon_price());
    
    return clone;
}



