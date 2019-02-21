//
//  CartInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 8..
//
//

#ifndef CartInfo_h
#define CartInfo_h

#include "cocos2d.h"

class CouponInfo;

class CartItem
{
public:
    
    CartItem();
    virtual ~CartItem();
    
private:
    
    CC_SYNTHESIZE(int, mn_Item_id, Item_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Cover, Cover)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Item_display_f, Item_display_f)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Item_display_s, Item_display_s)
    CC_SYNTHESIZE(int, mn_Item_each_price, Item_each_price)
    CC_SYNTHESIZE(int, mn_Item_price, Item_price)
    CC_SYNTHESIZE(int, mn_Item_pay_price, Item_pay_price)
    CC_SYNTHESIZE(int, mn_Item_basic_page, Item_basic_page)                 // 기본 장수
    CC_SYNTHESIZE(int, mn_Item_add_page, Item_add_page)                     // 추가 장수
    CC_SYNTHESIZE(int, mn_Item_basic_price, Item_basic_price)               // 기본 금액
    CC_SYNTHESIZE(int, mn_Item_basic_event_price, Item_basic_event_price)   // 이벤트 할인 적용금액
    CC_SYNTHESIZE(int, mn_Item_add_price, Item_add_price)                   // 추가 금액
    CC_SYNTHESIZE(bool, mbln_Selected, Selected)
};

class CartInfo
{
public:
    
    CartInfo();
    virtual ~CartInfo();
    
    void deldteCouponInfo();
    void pushCartItem(CartItem *pCartItem);
    int getCartItemCount();
    CartItem* getCartItem(int nItem_id);
    CartItem* getCartItemBySeq(int nSeq);
    CartItem* getSelectedCartItem();
    int getSelectedCartItemSeq();
    void setSelectedCartItem(int nIndex);
    void updatePrice();
    
private:
    
    CC_SYNTHESIZE(int, mn_Cart_id, Cart_id)
    CC_SYNTHESIZE(int, mn_Smart_album_id, Smart_album_id)
    CC_SYNTHESIZE(int, mn_Cnt, Cnt)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title)
    CC_SYNTHESIZE(int, mn_Total_page, Total_page)
    CC_SYNTHESIZE(int, mn_P_theme_id, P_theme_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Cover_image, Cover_image)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Thumb_name, Thumb_name)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Thumb_path, Thumb_path)
    CC_SYNTHESIZE(CouponInfo*, mCouponInfo, CouponInfo)
    
    std::vector<CartItem*> cartItemVec;
};

#endif /* CartInfo_h */
