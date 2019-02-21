//
//  PixPaymentPopup.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#ifndef PixPaymentPopup_h
#define PixPaymentPopup_h

#include "cocos2d.h"
#include "../Network/PixHttpClient.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Widget/PixCouponPopupEventDiscountCell.h"
#include "../Widget/PixCouponPopupTitleCell.h"
#include "../Widget/PixCouponPopupBottomCell.h"
#include "../Widget/PixCouponPopupItemCell.h"
#include "../Widget/PixCouponPopupEmptyCell.h"

USING_NS_CC;

class CartInfo;
class CouponInfo;
class PixPaymentPopupDelegate;

class PixPaymentPopup : public PixModalLayer, public PixCouponPopupItemCellDelegate, public PixCouponPopupEventDiscountCellDelegate
{
    
public:
    
    static PixPaymentPopup* create(CartInfo *pCartInfo, const std::string &strParamCoupon_group_id, const std::string &strParamCoupon_id);
    
    PixPaymentPopup(CartInfo *pCartInfo, const std::string &strParamCoupon_group_id, const std::string &strParamCoupon_id);
    virtual ~PixPaymentPopup();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
private:
    
    Node *rootNode;
    PixHttpClient *pixHttpClient;
    Node *MenuPaymen_Pop;
    ui::Button *Button_complete;
    PixGalleryListView *paymentPopupListView;
    PixCouponPopupEventDiscountCell *couponPopupEventDiscountCell;
    
    std::vector<CouponInfo*> couponVec;
    
    CC_SYNTHESIZE(CartInfo*, mCartInfo, CartInfo)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_ParamCoupon_group_id, ParamCoupon_group_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_ParamCoupon_id, ParamCoupon_id)
    
    void createPaymentPopupListView();
    void hideLayerAction();
    
#pragma mark - Button
    
    void ButtonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void ButtonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Delegate
    
    void couponPopupItemCellCheckBoxPressed(cocos2d::Ref* pWidget, bool var);
    void couponPopupEventDiscountCellCheckBoxPressed(bool var);
    
#pragma mark - Network
    
    void getCouponCall();
    void getCouponCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    void androidBackPressed();
};

#endif /* PixPaymentPopup_h */
