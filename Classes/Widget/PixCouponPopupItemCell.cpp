//
//  PixCouponPopupItemCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#include "PixCouponPopupItemCell.h"
#include "../Info/CouponInfo.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixStringUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixCouponPopupItemCell* PixCouponPopupItemCell::create(CouponInfo *pCouponInfo)
{
    PixCouponPopupItemCell* widget = new (std::nothrow) PixCouponPopupItemCell(pCouponInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCouponPopupItemCell::PixCouponPopupItemCell(CouponInfo *pCouponInfo)
: _customItem(nullptr)
, check(nullptr)
, mCouponInfo(pCouponInfo)
, mDelegate(nullptr)
{
    
}

PixCouponPopupItemCell::~PixCouponPopupItemCell()
{
    
}

bool PixCouponPopupItemCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCouponPopupItemCell::onEnter()
{
    Widget::onEnter();
}

void PixCouponPopupItemCell::createCustomItem()
{
    if(mCouponInfo == nullptr)
        return;
    
    _customItem = CSLoader::createNode("12_Oder/MenuPaymen_PopItem_04.csb");
    
    if(_customItem)
    {
        check = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("check"));
        
        if(check)
        {
            check->addEventListener(CC_CALLBACK_2(PixCouponPopupItemCell::checkBoxPressed, this));
        }
        
        ui::Text *CouponTitle = dynamic_cast<ui::Text*>(_customItem->getChildByName("CouponTitle"));
        
        if(CouponTitle)
        {
            CouponTitle->setString(mCouponInfo->getSubject());
        }
        ui::Text *CouponDetail = dynamic_cast<ui::Text*>(_customItem->getChildByName("CouponDetail"));
        
        if(CouponDetail)
        {
            std::string strMoney = PixStringUtils::commaMoney(mCouponInfo->getCoupon_price());
            CouponDetail->setString(StringUtils::format(CODE_NAME("Cart07"),strMoney.c_str()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixCouponPopupItemCell::setCheckSelected(bool var)
{
    if(check)
    {
        check->setSelected(var);
    }
}

bool PixCouponPopupItemCell::isCheckSelected()
{
    if(check)
    {
        return check->isSelected();
    }
    
    return false;
}

#pragma mark - Button

void PixCouponPopupItemCell::checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        if(mDelegate)
        {
            mDelegate->couponPopupItemCellCheckBoxPressed(this, checkBox->isSelected());
        }
    }
}

