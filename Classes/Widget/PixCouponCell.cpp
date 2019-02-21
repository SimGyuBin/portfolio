//
//  PixCouponCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 7..
//
//

#include "PixCouponCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

PixCouponCell* PixCouponCell::create(const std::string &strSubject, const std::string &strCoupon_price, const std::string &strEdate)
{
    PixCouponCell* widget = new (std::nothrow) PixCouponCell(strSubject, strCoupon_price, strEdate);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCouponCell::PixCouponCell(const std::string &strSubject, const std::string &strCoupon_price, const std::string &strEdate)
: _customItem(nullptr)
, mstr_Subject(strSubject)
, mstr_Coupon_price(strCoupon_price)
, mstr_Edate(strEdate)
{
    
}

PixCouponCell::~PixCouponCell()
{
    
}

bool PixCouponCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCouponCell::onEnter()
{
    Widget::onEnter();
}

void PixCouponCell::createCustomItem()
{
    _customItem = CSLoader::createNode("12_Oder/MenuCouponItem_02.csb");
    
    if(_customItem)
    {
        ui::Text *CouponTitle = dynamic_cast<ui::Text*>(_customItem->getChildByName("CouponTitle"));
        
        if(CouponTitle)
        {
            CouponTitle->setString(getSubject());
        }
        
        ui::Text *CouponDetail = dynamic_cast<ui::Text*>(_customItem->getChildByName("CouponDetail"));
        
        if(CouponDetail)
        {
            std::string strMoney = PixStringUtils::commaMoney(getCoupon_price());
            std::string strText = StringUtils::format(CODE_NAME(CouponDetail->getString()),strMoney.c_str());
            CouponDetail->setString(strText);
        }

        ui::Text *CouponDate = dynamic_cast<ui::Text*>(_customItem->getChildByName("CouponDate"));
        
        if(CouponDate)
        {
            std::vector<std::string> tokens;
            PixStringUtils::getTokens(tokens, getEdate(), "-");
            
            if(!tokens.empty() && tokens.size() >= 3)
            {
                std::string strText = StringUtils::format(CODE_NAME(CouponDate->getString()),atoi(tokens[0].c_str()), atoi(tokens[1].c_str()), atoi(tokens[2].c_str()));
                
                CouponDate->setString(strText);
            }
        }
       
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}
