//
//  OrderCheckInfo.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 11..
//
//

#include "OrderCheckInfo.h"

#pragma mark - goodsInfo

GoodsInfo::GoodsInfo()
: mstr_Title("")
, mstr_Album_image("")
, mn_Total_cnt(0)
, mn_Pay_amount(0)
, mstr_Item_display_f("")
, mstr_Item_display_s("")
, mn_Item_basic_price(0)
, mn_Item_add_page(0)
, mn_Item_add_price(0)
, mn_Item_discount_price(0)
, mn_Album_id(0)
{

}

GoodsInfo::~GoodsInfo()
{
    
}

#pragma mark - OrderCheckInfo
    
OrderCheckInfo::OrderCheckInfo()
: mn_Order_id(0)
, mn_Order_addr_id(0)
, mstr_Reg_date("")
, mstr_Pay_method("")
, mn_Pay_status(0)
, mn_Item_status(0)
, mstr_Group_date("")
, mn_Amount(0)
, mn_Discount_amount(0)
, mn_Pay_amount(0)
, mn_Pay_ship_amount(0)
, mstr_Order_no("")
, mstr_Backwoods_list_id("")
, mstr_Invoice_number("")
, mstr_Invoice_url("")
, mstr_StrUserName("")
, mstr_StrPhone01("")
, mstr_StrEmail("")
, mstr_Name("")
, mstr_Phone1("")
, mstr_Post("")
, mstr_Addr1("")
, mstr_Addr2("")
, mstr_Memo("")
, mstr_Vbank_name("")
, mstr_Vbank_num("")
, mstr_Vbank_holder("")
, mstr_Cancel_date("")
{
    goodsVec.clear();
}

OrderCheckInfo::~OrderCheckInfo()
{
    for (auto iter = goodsVec.begin(); iter != goodsVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    goodsVec.clear();
}

int OrderCheckInfo::getGoodsCount()
{
    return (int)goodsVec.size();
}

void OrderCheckInfo::pushGoodsInfo(GoodsInfo* pInfo)
{
    goodsVec.push_back(pInfo);
}

GoodsInfo* OrderCheckInfo::getGoodsInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= goodsVec.size())
    {
        return nullptr;
    }
    
    return goodsVec.at(nIndex);
}

#pragma mark - MonthlyCategoryInfo

MonthlyCategoryInfo::MonthlyCategoryInfo(const std::string &strDate)
: mstr_Date(strDate)
{
    
}

MonthlyCategoryInfo::~MonthlyCategoryInfo()
{
    for (auto iter = orderChecVec.begin(); iter != orderChecVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    orderChecVec.clear();
}

int MonthlyCategoryInfo::getOrderCheckCount()
{
    return (int)orderChecVec.size();
}

void MonthlyCategoryInfo::pushOrderCheckInfo(OrderCheckInfo *pOrderCheckInfo)
{
    orderChecVec.push_back(pOrderCheckInfo);
}

OrderCheckInfo* MonthlyCategoryInfo::getOrderCheckInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= orderChecVec.size())
    {
        return nullptr;
    }
    
    return orderChecVec.at(nIndex);
}
