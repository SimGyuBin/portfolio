//
//  CartOrderTotalInfo.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 8..
//
//

#include "CartOrderTotalInfo.h"

CartOrderTotalInfo::CartOrderTotalInfo()
: mn_Total_price(0)
, mn_Total_pay_price(0)
, mn_Total_discount_price(0)
, mn_Ship_price(0)
, mn_Pay_add_ship_amount(0)
{
    
}

CartOrderTotalInfo::~CartOrderTotalInfo()
{
    
}

CartOrderTotalInfo *CartOrderTotalInfo::clone()
{
    CartOrderTotalInfo *clone = new CartOrderTotalInfo();
    
    clone->setTotal_price(getTotal_price());
    clone->setTotal_pay_price(getTotal_pay_price());
    clone->setTotal_discount_price(getTotal_discount_price());
    clone->setShip_price(getShip_price());
    clone->setPay_add_ship_amount(getPay_add_ship_amount());
    
    return clone;
}
