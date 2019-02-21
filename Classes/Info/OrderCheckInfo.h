//
//  OrderCheckInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 11..
//
//

#ifndef OrderCheckInfo_h
#define OrderCheckInfo_h

#include "cocos2d.h"

class GoodsInfo
{
public:
    
    GoodsInfo();
    virtual ~GoodsInfo();
    
private:
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Album_image, Album_image)
    CC_SYNTHESIZE(int, mn_Total_cnt, Total_cnt)
    CC_SYNTHESIZE(int, mn_Pay_amount, Pay_amount)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Item_display_f, Item_display_f)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Item_display_s, Item_display_s)
    CC_SYNTHESIZE(int, mn_Item_basic_price, Item_basic_price)
    CC_SYNTHESIZE(int, mn_Item_add_page, Item_add_page)
    CC_SYNTHESIZE(int, mn_Item_add_price, Item_add_price)
    CC_SYNTHESIZE(int, mn_Item_discount_price, Item_discount_price)
    CC_SYNTHESIZE(int, mn_Album_id, Album_id)
};

class OrderCheckInfo
{
public:
    
    OrderCheckInfo();
    virtual ~OrderCheckInfo();
    
    int getGoodsCount();
    void pushGoodsInfo(GoodsInfo* pInfo);
    GoodsInfo* getGoodsInfo(int nIndex);
    
private:
    
    CC_SYNTHESIZE(int, mn_Order_id, Order_id)
    CC_SYNTHESIZE(int, mn_Order_addr_id, Order_addr_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Reg_date, Reg_date)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Pay_method, Pay_method)
    CC_SYNTHESIZE(int, mn_Pay_status, Pay_status)
    CC_SYNTHESIZE(int, mn_Item_status, Item_status)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Group_date, Group_date)
    CC_SYNTHESIZE(int, mn_Amount, Amount)
    CC_SYNTHESIZE(int, mn_Discount_amount, Discount_amount)
    CC_SYNTHESIZE(int, mn_Pay_amount, Pay_amount)
    CC_SYNTHESIZE(int, mn_Pay_ship_amount, Pay_ship_amount)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Order_no, Order_no)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Backwoods_list_id, Backwoods_list_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Invoice_number, Invoice_number)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Invoice_url, Invoice_url)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_StrUserName, StrUserName)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_StrPhone01, StrPhone01)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_StrEmail, StrEmail)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Name, Name)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Phone1, Phone1)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Post, Post)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Addr1, Addr1)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Addr2, Addr2)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Memo, Memo)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Vbank_name, Vbank_name)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Vbank_num, Vbank_num)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Vbank_holder, Vbank_holder)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Cancel_date, Cancel_date)
    
    std::vector<GoodsInfo*> goodsVec;
};

class MonthlyCategoryInfo
{
public:
    
    MonthlyCategoryInfo(const std::string &strDate);
    virtual ~MonthlyCategoryInfo();
    
    int getOrderCheckCount();
    void pushOrderCheckInfo(OrderCheckInfo *pOrderCheckInfo);
    OrderCheckInfo* getOrderCheckInfo(int nIndex);
    
private:
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Date, Date)
    std::vector<OrderCheckInfo*> orderChecVec;
};


#endif /* OrderCheckInfo_h */
