//
//  CartInfo.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 8..
//
//

#include "CartInfo.h"
#include "CouponInfo.h"

#pragma mark - CartItem

CartItem::CartItem()
: mn_Item_id(0)
, mstr_Cover("")
, mstr_Item_display_f("")
, mstr_Item_display_s("")
, mn_Item_each_price(0)
, mn_Item_price(0)
, mn_Item_pay_price(0)
, mn_Item_basic_page(0)
, mn_Item_add_page(0)
, mn_Item_basic_price(0)
, mn_Item_basic_event_price(0)
, mn_Item_add_price(0)
, mbln_Selected(false)
{

}

CartItem::~CartItem()
{
    
}

#pragma mark - CartInfo

CartInfo::CartInfo()
: mn_Cart_id(0)
, mn_Smart_album_id(0)
, mn_Cnt(0)
, mstr_Title("")
, mn_Total_page(0)
, mn_P_theme_id(0)
, mstr_Cover_image("")
, mstr_Thumb_name("")
, mstr_Thumb_path("")
, mCouponInfo(nullptr)
{
    cartItemVec.clear();
}

CartInfo::~CartInfo()
{
    deldteCouponInfo();
    
    for(auto iter = cartItemVec.begin(); iter != cartItemVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    cartItemVec.clear();
    
}

void CartInfo::deldteCouponInfo()
{
    CC_SAFE_DELETE(mCouponInfo);
}

void CartInfo::pushCartItem(CartItem *pCartItem)
{
    cartItemVec.push_back(pCartItem);
}

int CartInfo::getCartItemCount()
{
    return (int)cartItemVec.size();
}

CartItem* CartInfo::getCartItem(int nItem_id)
{
    for (auto iter = cartItemVec.begin(); iter != cartItemVec.end(); ++iter)
    {
        if((*iter)->getItem_id() == nItem_id)
        {
            return (*iter);
        }
    }
    
    return nullptr;
}

CartItem* CartInfo::getCartItemBySeq(int nSeq)
{
    if (nSeq < 0 || nSeq >= cartItemVec.size())
    {
        return nullptr;
    }
    
    return cartItemVec.at(nSeq);
}

CartItem* CartInfo::getSelectedCartItem()
{
    for (auto iter = cartItemVec.begin(); iter != cartItemVec.end(); ++iter)
    {
        if((*iter)->getSelected() == true)
        {
            return (*iter);
        }
    }

    return nullptr;
}

int CartInfo::getSelectedCartItemSeq()
{
    int nSeq = 0;
    
    for (auto iter = cartItemVec.begin(); iter != cartItemVec.end(); ++iter)
    {
        if((*iter)->getSelected() == true)
        {
            return nSeq;
        }
        
        ++nSeq;
    }
    
    return nSeq;
}

void CartInfo::setSelectedCartItem(int nIndex)
{
    int nCnt = 0;
    
    for (auto iter = cartItemVec.begin(); iter != cartItemVec.end(); ++iter)
    {
        (*iter)->setSelected((nCnt == nIndex) ? true : false);
        ++nCnt;
    }
}

void CartInfo::updatePrice()
{
    // 사용 안함
    // 수량변경에 의한 값을 계산할떄 사용예정 
    for (auto iter = cartItemVec.begin(); iter != cartItemVec.end(); ++iter)
    {
        (*iter)->setItem_price((*iter)->getItem_each_price() * getCnt());
    }
}
