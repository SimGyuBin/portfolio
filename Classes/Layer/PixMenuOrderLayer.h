//
//  PixMenuOrderLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 11..
//
//

#ifndef PixMenuOrderLayer_h
#define PixMenuOrderLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"
#include "PixOrderSearchLayer.h"
#include "../Widget/PixOrderMonthlyCategoryCell.h"
#include "../Widget/PixOrderCell.h"
#include "../Widget/PixOrderGoodsCell.h"
#include "../Widget/PixOrderMoreCell.h"
#include "../Widget/PixOrderDetailStateCell.h"
#include "../Widget/PixOrderDetailGoodsCell.h"
#include "../Widget/PixOrderDetailOrdererInfoCell.h"
#include "../Widget/PixOrderDetailDeliveryInfoCell.h"
#include "../Widget/PixOrderDetailBillingInfoCell.h"
#include "../Info/OrderCheckInfo.h"
#include "PixOrderCancelLayer.h"

#define FIRST_ORDER_SEARCH_LIMIT    5
#define MORE_ORDER_SEARCH_LIMIT     5
#define FIRST_ORDER_SEARCH_TAG      "GET_ORDER_LIST"
#define MORE_ORDER_SEARCH_TAG       "GET_ORDER_LIST_MORE"
#define ORDER_CANCEL_LAYER_TAG      620418

USING_NS_CC;

//상태 검색
//(' :전체, 1: 입금확인중, 2: 제작준비중, 4: 제작중, 8: 배송중, 16: 배송완료, 32: 주문취소)

typedef enum
{
    ORDERLAYER_NONE = 0,
    ORDERLAYER_LIST,
    ORDERLAYER_DETAIL,
    
} ORDERLAYER_TYPE;

typedef enum
{
    ORDER_NONE = 0,
    ORDER_DEPOSIT_CHECK = 1,                // 입금 확인중
    ORDER_PRODUCTION_PREPARING = 2,         // 제작 준비중
    ORDER_IN_PRODUCTION = 4,                // 제작중
    ORDER_DELIVERY = 8,                     // 배송중
    ORDER_COMPLETE_DELIVERY = 16,           // 배송완료
    ORDER_CANCELLATION_PROCESSING = 100,    // 주문취소 처리중(UI에서만 사용 검색에서는 사용안함)
    ORDER_CANCELLATION = 32,                // 주문취소
    
} ORDER_TYPE;

typedef enum
{
    DETAIL_ACTION_CHANGE_ADDRESS = 0,   // 배송지 변경
    DETAIL_ACTION_CANCEL_ORDER,         // 주문취소
    DETAIL_ACTION_TRACKING_NUMBER,      // 배송조회
    
} DETAIL_ACTION_TYPE;

class PixMenuOrderLayer : public PixModalLayer, public PixLayerActionDelegate, public PixOrderSearchLayerDelegate, public PixOrderDetailStateCellDelegate, public PixOrderCancelLayerDelegate
{
public:
    
    static PixMenuOrderLayer* create();
    
    PixMenuOrderLayer();
    virtual ~PixMenuOrderLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    PixHttpClient *pixHttpClient;
    Node *Top_01;
    Node *Top_02;
    Node *Node_MenuOrder_Empty;
    ui::Layout *Panel_1;
    ui::Layout *Panel_2;
    Node *Node_MenuOrderItem_01;
    ui::Text *Text_Category;
    PixGalleryListView *orderListView;
    PixGalleryListView *detailListView;
    PixOrderMoreCell *orderMoreCell;
    ORDERLAYER_TYPE me_OrderLayerType;
    ORDER_TYPE me_OrderType;
    PixOrderCancelLayer *mOrderCancelLayer;
    
    int mn_Page;
    bool mbln_Is_End;
    bool mbln_IsAction;
    bool mbln_showDetailListView;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(OrderCheckInfo*, mCancelOrderCheckInfo, CancelOrderCheckInfo)
    
    std::unordered_map<std::string, MonthlyCategoryInfo*> monthlyCategorys;
    
    void removeAllMonthlyCategorys();
    void addMonthlyCategoryInfo(MonthlyCategoryInfo *pInfo);
    MonthlyCategoryInfo* getMonthlyCategoryInfo(const std::string &strDate);
    std::string getSearchkey();
    
    void setOrderLayerType(ORDERLAYER_TYPE eType);
    void setOrderSearchType(ORDER_TYPE eType);
    ui::Widget* addOrderMonthlyCategoryCell(const std::string &strDate);
    ui::Widget* addOrderCell(OrderCheckInfo *pOrderCheckInfo);
    ui::Widget* addOrderGoodsCell(OrderCheckInfo *pOrderCheckInfo, GoodsInfo* pGoodsInfo);
    ui::Widget* addOrderMoreCell();
    ui::Widget* addOrderListItem(ui::Widget* item);
    void createDetailListView(OrderCheckInfo *pOrderCheckInfo);
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonMenuOrderDetailCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonCategoryPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonMoerPressed(cocos2d::Ref *pSender);

#pragma mark - Select List View
    
    void orderListViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
     
#pragma mark - Action
    
    void showMenuOrderLayerAction();
    void hideMenuOrderLayerAction();
    void showMenuOrderLayerActionFinish(Node* node);
    void hideMenuOrderLayerActionFinish(Node* node);
    void showDetailListViewAction();
    void hideDetailListViewAction();
    void showDetailListViewActionFinish(Node* node);
    void hideDetailListViewActionFinish(Node* node);

#pragma mark - Delegate
    
    virtual void orderSearchLayerSearchPressed(int nTag, const std::string& strText);
    virtual void orderDetailStateCellPressed(OrderCheckInfo *pOrderCheckInfo, int nTag);
    virtual void orderCancellation(const std::string &strBank_num, const std::string &strBank_code, const std::string &strBank_holder);
    
#pragma mark - Network
    
    void getOrderListCall(int nlimit, const std::string& strTag);
    void getOrderListCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getOrderCancelCall();
    void getOrderCancelCall(const std::string &strBank_num, const std::string &strBank_code, const std::string &strBank_holder);
    void getOrderCancelCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

#pragma mark - Android BackPressed
    
    void androidBackPressed();

#pragma mark - static method
    
    static std::string getPayMethodToUIString(const std::string &strPay_method);
    static std::string getOrderStatusToUIString(int nPay_status, int nItem_status);
    static ORDER_TYPE getOrderStatus(int nPay_status, int nItem_status);
};

#endif /* PixMenuOrderLayer_h */
