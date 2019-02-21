//
//  PixMenuCartLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 6..
//
//

#ifndef PixMenuCartLayer_h
#define PixMenuCartLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"
#include "../Widget/PixCartCell.h"
#include "../Widget/PixCartSelectAllCell.h"
#include "../Widget/PixCartProductGuideCell.h"
#include "../Widget/PixCartOrderTotalCell.h"
#include "../Widget/PixPaymentCell.h"
#include "../Widget/PixPaymentCountCell.h"
#include "../Widget/PixPaymentTotalCell.h"
#include "../Layer/PixPaymentPopup.h"
#include "../Layer/PixPayDeliveryListLayer.h"
#include "../Layer/PixPurchaseLayer.h"
#include "../Layer/PixPurchaseFailedLayer.h"
#include "../Widget/PixPayBuyerInfoCell.h"
#include "../Widget/PixPayDeliveryInfoCell.h"
#include "../Widget/PixPayNewDeliveryCell.h"
#include "../Widget/PixPayDeliveryListCell.h"
#include "../Widget/PixPayPaymentMethodCell.h"
#include "../Widget/PixPayFinalPaymentAmountCell.h"
#include "../Widget/PixPurchaseCompletedTitleCell.h"
#include "../Widget/PixPurchaseCompletedInfoCell.h"
#include "../Widget/PixPurchaseCompletedBottomCell.h"

#define CART_TAG      1160

USING_NS_CC;

typedef enum
{
    CARTLAYER_NONE = 0,
    CARTLAYER_CART,
    CARTLAYER_ORDER,
    CARTLAYER_PAYMENT,
    CARTLAYER_ORDER_COMPLETED,
 
} CARTLAYER_TYPE;

class CartInfo;
class CartOrderTotalInfo;
class DeliveryInfo;

class PixMenuCartLayer : public PixModalLayer, public PixLayerActionDelegate, public PixCartCellDelegate, public PixCartSelectAllCellDelegate, public PixPaymentCellDelegate, public PixPayDeliveryInfoCellDelegate, public PixPayDeliveryListCellDelegate,  public PixPayDeliveryListLayerDelegate, public PixPurchaseLayerDelegate, public PixPurchaseCompletedBottomCellDelegate
{

public:
    
    static PixMenuCartLayer* create();
    
    PixMenuCartLayer();
    virtual ~PixMenuCartLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    PixHttpClient *pixHttpClient;
    Node *Top_01;
    Node *Top_02;
    Node *Top_03;
    Node *Top_04;
    Node *MenuCartEmptyLayer;
    PixGalleryListView *cartListView;
    PixGalleryListView *orderListView;
    PixGalleryListView *paymentListView;
    PixGalleryListView *orderCompletedListView;
    ui::Button *Button_order;
    ui::Button *Button_order_2;
    ui::Button *Button_order_3;
    std::vector<CartInfo*> cartVec;
    std::vector<CartInfo*> orderVec;
    std::vector<DeliveryInfo*> *deliveryVec;
    CartOrderTotalInfo *cartTotalInfo;
    CartOrderTotalInfo *orderTotalInfo;
    CartOrderTotalInfo *finalTotalInfo;
    PixCartSelectAllCell *cartSelectAllCell;
    PixCartOrderTotalCell *cartTotalCell;
    PixPaymentCountCell *paymentCountCell;
    PixPaymentTotalCell *paymentTotalCellCell;
    PixPayDeliveryInfoCell *payDeliveryInfoCell;
    PixPayNewDeliveryCell *payNewDeliveryCell;
    PixPayDeliveryListCell *payDeliveryListCell;
    PixPayPaymentMethodCell *payPaymentMethodCell;
    PixPayFinalPaymentAmountCell *payFinalPaymentAmountCell;
    
    CARTLAYER_TYPE me_Type;
    
    int mn_free_item_price;
    int mn_price;
    
    bool mbln_IsAction;
    bool mbln_showOrderListView;
    bool mbln_showPaymentListView;
    bool mbln_showOrderCompletedListView;

    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    void createCartListView();
    void checkGetCartCallback(float dt);
    void updateCartOrderTotalInfo();
    void setLayerTypr(CARTLAYER_TYPE eType);
    void clearOrderListView();
    void createOrderListView();
    void updateOrderTotalInfo();
    void createPaymentListView();
    void clearPaymentListView();
    void updateFinalTotalInfo();
    void createOrderCompletedListView(int nAmount, const std::string& strPay_method, const std::string& strPay_method_text, const std::string& strVbank_name, const std::string& strVbank_num, const std::string& strVbank_holder, const std::string& strVbank_date, const std::string& strOrder_id, const std::string& strReg_date);
    bool checkPurchase();
    const char* purchaseDataParsing(const std::string& strData, const std::string& strKey);
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonOrderBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonPaymentBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonOrderPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonInputShippingInfoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonPaymentPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showMenuCartLayerAction();
    void hideMenuCartLayerAction();
    void showMenuCartLayerActionFinish(Node* node);
    void hideMenuCartLayerActionFinish(Node* node);
    void runCartListViewDeleteAction();
    void deleteCartListViewActionFinish();
    void showOrderListViewAction();
    void hideOrderListViewAction();
    void showOrderListViewActionFinish(Node* node);
    void hideOrderListViewActionFinish(Node* node);
    void showPaymentListViewAction();
    void hidePaymentListViewAction();
    void showPaymentListViewActionFinish(Node* node);
    void hidePaymentListViewActionFinish(Node* node);
    void showOrderCompletedListViewAction();
    void hideOrderCompletedListViewAction();
    void showOrderCompletedListViewActionFinish(Node* node);
    void hideOrderCompletedListViewActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    virtual void cartCellDeleteActionStart(Ref *pTarget);
    virtual void cartCellDeleteActionFinish(Ref *pTarget);
    virtual void cartCellCheckBoxPressed(bool var);
    virtual void cartSelectAllCellCheckBoxPressed(bool var);
    virtual void paymentCellCouponPressed(cocos2d::Ref *pWidget, bool blnSet);
    virtual void payDeliveryInfoCellListPressed();
    virtual void payDeliveryInfoCellNewPressed();
    virtual void payDeliveryListCellMoerPressed();
    virtual void payDeliveryListLayerCompletePressed(DeliveryInfo *pInfo);
    virtual void payDeliveryListLayerNoAddressAction();
    virtual void purchaseCompile(const std::string& str);
    virtual void purchaseCompletedBottomCellMoerPressed();
    void cellDeleteActionFinish(ui::Widget *pWidget);
    
#pragma mark - Network
    
    void getCartCall();
    void getCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getPayStop01();
    void getPayStop01Callback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getAddre();
    void getAddreCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getPayStop02();
    void getPayStop02Callback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void payDoneCall(const std::string& imp_uid, const std::string& merchant_uid, const std::string& imp_success);
    void payDoneCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void purchaseCall(int nnOrder_id);
    
    std::string getPayStop02JsonString();
    
#pragma mark - Android BackPressed
    
    void androidBackPressed();
};

#endif /* PixMenuCartLayer_h */
