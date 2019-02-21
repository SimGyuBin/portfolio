//
//  DeliveryInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 21..
//
//

#ifndef DeliveryInfo_h
#define DeliveryInfo_h

#include "cocos2d.h"

class DeliveryInfo
{
public:
    
    DeliveryInfo();
    virtual ~DeliveryInfo();
    
private:

    CC_SYNTHESIZE(int, mn_Member_addr_id, Member_addr_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Name, Name)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Post, Post)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Addr1, Addr1)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Addr2, Addr2)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Phone1, Phone1)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Phone2, Phone2)
    CC_SYNTHESIZE(bool, mbln_Last, Last)
    CC_SYNTHESIZE(int, mn_Backwoods_list_id, Backwoods_list_id)
    CC_SYNTHESIZE(int, mn_Pay_add_ship_amount, Pay_add_ship_amount)
    
    // ui용 데이터
    CC_SYNTHESIZE(bool, mbln_Selected, Selected)
    
};

#endif /* DeliveryInfo_h */
