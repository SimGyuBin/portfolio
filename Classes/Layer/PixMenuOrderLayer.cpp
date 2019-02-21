//
//  PixMenuOrderLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 11..
//
//

#include "PixMenuOrderLayer.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../UIControl/PixToast.h"
#include "PixDeliveryChangeLayer.h"
#include "PixDeliveryTrackingLayer.h"

PixMenuOrderLayer* PixMenuOrderLayer::create()
{
    PixMenuOrderLayer *menuOrderLayer = new (std::nothrow) PixMenuOrderLayer();
    if (menuOrderLayer && menuOrderLayer->init())
    {
        menuOrderLayer->autorelease();
        return menuOrderLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuOrderLayer);
        return nullptr;
    }
}

PixMenuOrderLayer::PixMenuOrderLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, Top_01(nullptr)
, Top_02(nullptr)
, Node_MenuOrder_Empty(nullptr)
, Panel_1(nullptr)
, Panel_2(nullptr)
, Node_MenuOrderItem_01(nullptr)
, Text_Category(nullptr)
, orderListView(nullptr)
, detailListView(nullptr)
, orderMoreCell(nullptr)
, me_OrderLayerType(ORDERLAYER_NONE)
, me_OrderType(ORDER_NONE)
, mOrderCancelLayer(nullptr)
, mn_Page(1)
, mbln_Is_End(false)
, mbln_IsAction(false)
, mbln_showDetailListView(false)
, mDelegate(nullptr)
, mCancelOrderCheckInfo(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    monthlyCategorys.clear();
    
    hideLayerFunction = std::bind(&PixMenuOrderLayer::hideMenuOrderLayerAction,this);
}

PixMenuOrderLayer::~PixMenuOrderLayer()
{
    pixHttpClient->cancelRequest(this);
    removeAllMonthlyCategorys();
}

void PixMenuOrderLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuOrderLayer.csb");
    
    if(rootNode)
    {
        Node *Node_MenuOrder_Top = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuOrder_Top"));
        
        if(Node_MenuOrder_Top)
        {
            Top_01 = dynamic_cast<Node*>(Node_MenuOrder_Top->getChildByName("Top_01"));
            
            if(Top_01)
            {
                ui::Text *Text_label = dynamic_cast<ui::Text*>(Top_01->getChildByName("Text_label"));
                
                if(Text_label)
                {
                    Text_label->setString(CODE_NAME(Text_label->getString()));
                }
                
                ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Top_01->getChildByName("Button_back"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixMenuOrderLayer::buttonCancelPressed, this));
                }
            }
            
            Top_02 = dynamic_cast<Node*>(Node_MenuOrder_Top->getChildByName("Top_02"));
            
            if(Top_02)
            {
                ui::Text *Text_label = dynamic_cast<ui::Text*>(Top_02->getChildByName("Text_label"));
                
                if(Text_label)
                {
                    Text_label->setString(CODE_NAME(Text_label->getString()));
                }
                
                ui::Button *Button_back = dynamic_cast<ui::Button*>(Top_02->getChildByName("Button_back"));
                
                if(Button_back)
                {
                    Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuOrderLayer::buttonMenuOrderDetailCancelPressed, this));
                }
            }
        }
        
        Node_MenuOrder_Empty = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuOrder_Empty"));
        
        if(Node_MenuOrder_Empty)
        {
            Panel_1 = dynamic_cast<ui::Layout*>(Node_MenuOrder_Empty->getChildByName("Panel_1"));
            
            if(Panel_1)
            {
                ui::Text *Text_empty = dynamic_cast<ui::Text*>(Panel_1->getChildByName("Text_empty"));
                
                if(Text_empty)
                {
                    Text_empty->setString(CODE_NAME(Text_empty->getString()));
                }
                
                Panel_1->setVisible(true);
            }
            
            
            
            Panel_2 = dynamic_cast<ui::Layout*>(Node_MenuOrder_Empty->getChildByName("Panel_2"));
            
            if(Panel_2)
            {
                ui::Text *Text_empty = dynamic_cast<ui::Text*>(Panel_2->getChildByName("Text_empty"));
                
                if(Text_empty)
                {
                    Text_empty->setString(CODE_NAME(Text_empty->getString()));
                }
                
                Panel_2->setVisible(false);

            }
            
            ui::Text *Text_empty = dynamic_cast<ui::Text*>(Node_MenuOrder_Empty->getChildByName("Text_empty"));
            
            if(Text_empty)
            {
                Text_empty->setString(CODE_NAME(Text_empty->getString()));
            }
        }
        
        Node_MenuOrderItem_01 = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuOrderItem_01"));
        
        if(Node_MenuOrderItem_01)
        {
            ui::Button *Button_All = dynamic_cast<ui::Button*>(Node_MenuOrderItem_01->getChildByName("Button_All"));
            
            if(Button_All)
            {
                Button_All->addTouchEventListener(CC_CALLBACK_2(PixMenuOrderLayer::buttonCategoryPressed, this));
             
                Text_Category = dynamic_cast<ui::Text*>(Button_All->getChildByName("Text_All"));
                
                if(Text_Category)
                {
                    Text_Category->setString(CODE_NAME(Text_Category->getString()));
                }
            }
        }
        
        ui::ListView *listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView"));
        
        if(listView)
        {
            orderListView = PixGalleryListView::create();
            orderListView->copyClonedListView(listView);
            orderListView->setSelectListItemCallBack(CC_CALLBACK_3(PixMenuOrderLayer::orderListViewTouchCallBack, this));
        }
        
        listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView_2"));
        
        if(listView)
        {
            detailListView = PixGalleryListView::create();
            detailListView->copyClonedListView(listView);
        }
        
        setAnchorPoint(Vec2::ZERO);
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        addChild(rootNode);
        
        getOrderListCall(FIRST_ORDER_SEARCH_LIMIT, FIRST_ORDER_SEARCH_TAG);
        setOrderLayerType(ORDERLAYER_LIST);
    }
}

void PixMenuOrderLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuOrderLayerAction();
}

void PixMenuOrderLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(me_OrderLayerType == ORDERLAYER_LIST)
    {
        if(Node_MenuOrder_Empty)
        {
            if(monthlyCategorys.empty())
            {
                Node_MenuOrder_Empty->setVisible(true);
            }
            else
            {
                Node_MenuOrder_Empty->setVisible(false);
            }
        }
    }
    else
    {
        if(Node_MenuOrder_Empty)
        {
            Node_MenuOrder_Empty->setVisible(false);
        }
        
        if(Node_MenuOrderItem_01)
        {
            Node_MenuOrderItem_01->setVisible(false);
        }
    }
    
    
    if(rootNode)
    {
        Size size = rootNode->getContentSize();
        
        if(detailListView)
        {
            if(detailListView->getPosition().x >= size.width)
            {
                mbln_showDetailListView = false;
            }
            else
            {
                mbln_showDetailListView = true;
            }
        }
    }
}

void PixMenuOrderLayer::removeAllMonthlyCategorys()
{
    for (auto iter = monthlyCategorys.begin(); iter != monthlyCategorys.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter).second);
    }
    
    monthlyCategorys.clear();
    
    mn_Page = 1;
    mbln_Is_End = false;
}

void PixMenuOrderLayer::addMonthlyCategoryInfo(MonthlyCategoryInfo *pInfo)
{
    if(pInfo)
    {
        auto ret = monthlyCategorys.insert(std::make_pair(pInfo->getDate(), pInfo));
        
        if (ret.second == false)
        {
            CC_SAFE_DELETE((ret.first->second));
            monthlyCategorys[pInfo->getDate()] = pInfo;
        }
    }
}

MonthlyCategoryInfo* PixMenuOrderLayer::getMonthlyCategoryInfo(const std::string &strDate)
{
    std::string strKey = strDate;
    auto it = monthlyCategorys.find(strKey);
    
    if (it != monthlyCategorys.end())
    {
        return (*it).second;
    }
    
    return nullptr;
}

std::string PixMenuOrderLayer::getSearchkey()
{
    if(me_OrderType == ORDER_NONE)
    {
        return "";
    }
    else
    {
        return StringUtils::format("%d",(int)me_OrderType);
    }
}

void PixMenuOrderLayer::setOrderLayerType(ORDERLAYER_TYPE eType)
{
    if(me_OrderLayerType == eType)
        return;
    
    me_OrderLayerType = eType;
    
    switch (me_OrderLayerType)
    {
        case ORDERLAYER_LIST:
        {
            if(Top_01)
            {
                Top_01->setVisible(true);
            }
            
            if(orderListView)
            {
                orderListView->setVisible(true);
            }
            
            if(Node_MenuOrderItem_01)
            {
                Node_MenuOrderItem_01->setVisible(true);
            }
            
            if(Top_02)
            {
                Top_02->setVisible(false);
            }
            
            if(detailListView)
            {
                detailListView->setVisible(false);
            }
        }
        break;
            
        case ORDERLAYER_DETAIL:
        {
            if(Top_01)
            {
                Top_01->setVisible(false);
            }
            
            if(orderListView)
            {
                orderListView->setVisible(false);
            }
            
            if(Node_MenuOrderItem_01)
            {
                Node_MenuOrderItem_01->setVisible(false);
            }
            
            if(Top_02)
            {
                Top_02->setVisible(true);
            }
            
            if(detailListView)
            {
                detailListView->setVisible(true);
            }
        }
        break;
            
        default:
            break;
    }

}

void PixMenuOrderLayer::setOrderSearchType(ORDER_TYPE eType)
{
    if(me_OrderType == eType)
        return;
    
    me_OrderType = eType;
}

ui::Widget* PixMenuOrderLayer::addOrderMonthlyCategoryCell(const std::string &strDate)
{
    PixOrderMonthlyCategoryCell *orderMonthlyCategoryCell = PixOrderMonthlyCategoryCell::create(strDate);
    return addOrderListItem(orderMonthlyCategoryCell);
}

ui::Widget* PixMenuOrderLayer::addOrderCell(OrderCheckInfo *pOrderCheckInfo)
{
    PixOrderCell *orderCell = PixOrderCell::create(pOrderCheckInfo);
    return addOrderListItem(orderCell);
}

ui::Widget* PixMenuOrderLayer::addOrderGoodsCell(OrderCheckInfo *pOrderCheckInfo, GoodsInfo* pGoodsInfo)
{
    PixOrderGoodsCell *orderGoodsCell = PixOrderGoodsCell::create(pOrderCheckInfo, pGoodsInfo);
    return addOrderListItem(orderGoodsCell);
}

ui::Widget* PixMenuOrderLayer::addOrderMoreCell()
{
    if(orderListView == nullptr)
        return nullptr;
    
    if(mbln_Is_End)
    {
        if(orderMoreCell)
        {
            ssize_t nIndex = orderListView->getIndex(orderMoreCell);
            
            if(nIndex > 0)
            {
                orderListView->removeItem(nIndex);
                orderMoreCell = nullptr;
            }
        }
    }
    else
    {
        if(orderMoreCell == nullptr)
        {
            orderMoreCell = PixOrderMoreCell::create(CC_CALLBACK_1(PixMenuOrderLayer::buttonMoerPressed, this));
            
            orderListView->pushBackCustomItem(orderMoreCell);
        }
    }
    
    return orderMoreCell;
}

ui::Widget* PixMenuOrderLayer::addOrderListItem(ui::Widget* item)
{
    if(orderListView == nullptr)
        return nullptr;
    
    if(orderMoreCell)
    {
        ssize_t nIndex = orderListView->getIndex(orderMoreCell);
        
        if(nIndex > 0)
        {
            orderListView->insertCustomItem(item, nIndex);
        }
    }
    else
    {
        orderListView->pushBackCustomItem(item);
    }
    
    return item;
}

void PixMenuOrderLayer::createDetailListView(OrderCheckInfo *pOrderCheckInfo)
{
    if(detailListView == nullptr)
        return;
    
    if(pOrderCheckInfo == nullptr)
        return;
    
    detailListView->removeAllItems();
    
    PixOrderDetailStateCell *orderDetailStateCell = PixOrderDetailStateCell::create(pOrderCheckInfo);
    orderDetailStateCell->setDelegate(this);
    detailListView->pushBackCustomItem(orderDetailStateCell);
    
    int nCount = pOrderCheckInfo->getGoodsCount();
    
    for (int i = 0; i < nCount; ++i)
    {
        GoodsInfo *goodsInfo = pOrderCheckInfo->getGoodsInfo(i);
        
        if(goodsInfo)
        {
            PixOrderDetailGoodsCell *orderDetailGoodsCell = PixOrderDetailGoodsCell::create(goodsInfo);
            detailListView->pushBackCustomItem(orderDetailGoodsCell);
        }
    }
    
    PixOrderDetailOrdererInfoCell *orderDetailOrdererInfoCell = PixOrderDetailOrdererInfoCell::create(pOrderCheckInfo);
    detailListView->pushBackCustomItem(orderDetailOrdererInfoCell);
    
    PixOrderDetailDeliveryInfoCell *orderDetailDeliveryInfoCell = PixOrderDetailDeliveryInfoCell::create(pOrderCheckInfo);
    detailListView->pushBackCustomItem(orderDetailDeliveryInfoCell);
    
    PixOrderDetailBillingInfoCell *orderDetailBillingInfoCell = PixOrderDetailBillingInfoCell::create(pOrderCheckInfo);
    detailListView->pushBackCustomItem(orderDetailBillingInfoCell);
}

#pragma mark - Button

void PixMenuOrderLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hideMenuOrderLayerAction();
    }
}

void PixMenuOrderLayer::buttonMenuOrderDetailCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hideDetailListViewAction();
    }
}

void PixMenuOrderLayer::buttonCategoryPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        PixOrderSearchLayer *orderSearchLayer = PixOrderSearchLayer::create();
        orderSearchLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(orderSearchLayer);
    }
}

void PixMenuOrderLayer::buttonMoerPressed(cocos2d::Ref *pSender)
{
    getOrderListCall(MORE_ORDER_SEARCH_LIMIT, MORE_ORDER_SEARCH_TAG);
}

#pragma mark - Select List View

void PixMenuOrderLayer::orderListViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
    if(touchType == LISTVIEW_LONG_TAP)
    {
        
    }
    else
    {
        OrderCheckInfo *orderCheckInfo = nullptr;
        
        do
        {
            PixOrderCell *orderCell = dynamic_cast<PixOrderCell*>(pSender);
            
            if(orderCell)
            {
                if (NodeContainsPoint(orderCell, touchVec.at(1)))
                {
                    orderCheckInfo = orderCell->getOrderCheckInfo();
                    break;
                }
            }
            
            PixOrderGoodsCell *orderGoodsCell = dynamic_cast<PixOrderGoodsCell*>(pSender);
            
            if(orderGoodsCell)
            {
                if (NodeContainsPoint(orderGoodsCell, touchVec.at(1)))
                {
                    orderCheckInfo = orderGoodsCell->getOrderCheckInfo();
                    break;
                }
            }
            
        } while (0);
        
        if(orderCheckInfo)
        {
            createDetailListView(orderCheckInfo);
            showDetailListViewAction();
        }
    }
}

#pragma mark - Action

void PixMenuOrderLayer::showMenuOrderLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuOrderLayer::showMenuOrderLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuOrderLayer::hideMenuOrderLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuOrderLayer::hideMenuOrderLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuOrderLayer::showMenuOrderLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuOrderLayer::hideMenuOrderLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionFinish(this);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

void PixMenuOrderLayer::showDetailListViewAction()
{
    if(detailListView == nullptr)
        return;
    
    if(mbln_showDetailListView == true)
        return;
    
    setOrderLayerType(ORDERLAYER_DETAIL);
    
    detailListView->setEnabled(false);
    
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0, detailListView->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuOrderLayer::showDetailListViewActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    detailListView->runAction(sequence);
}

void PixMenuOrderLayer::hideDetailListViewAction()
{
    if(detailListView == nullptr)
        return;
    
    if(mbln_showDetailListView == false)
        return;
    
    detailListView->stopAutoScroll();
    detailListView->setEnabled(false);
    
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth, detailListView->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuOrderLayer::hideDetailListViewActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    detailListView->stopAllActions();
    detailListView->runAction(sequence);
    
}

void PixMenuOrderLayer::showDetailListViewActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(detailListView)
    {
        detailListView->setEnabled(true);
    }
}

void PixMenuOrderLayer::hideDetailListViewActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    setOrderLayerType(ORDERLAYER_LIST);
    
    if(detailListView)
    {
        detailListView->setEnabled(true);
        detailListView->removeAllItems();
    }
}

#pragma mark - Delegate

void PixMenuOrderLayer::orderSearchLayerSearchPressed(int nTag, const std::string& strText)
{
    if(me_OrderType == ORDER_TYPE(nTag))
        return;
    
    setOrderSearchType(ORDER_TYPE(nTag));
    
    if(Text_Category)
    {
        Text_Category->setString(strText);
    }
    
    if(orderListView)
    {
        orderListView->removeAllItems();
        orderListView->stopAutoScroll();
        // 더보기 셀 초기화
        orderMoreCell = nullptr;
    }
    
    // 빈화면 연출
    if(Panel_1)
    {
        Panel_1->setVisible(false);
    }
    
    if(Panel_2)
    {
        Panel_2->setVisible(true);
    }
    
    removeAllMonthlyCategorys();
    getOrderListCall(FIRST_ORDER_SEARCH_LIMIT, FIRST_ORDER_SEARCH_TAG);
}

void PixMenuOrderLayer::orderDetailStateCellPressed(OrderCheckInfo *pOrderCheckInfo, int nTag)
{
    //        DETAIL_ACTION_CHANGE_ADDRESS = 0,   // 배송지 변경
    //        DETAIL_ACTION_CANCEL_ORDER,         // 주문취소
    //        DETAIL_ACTION_TRACKING_NUMBER,      // 배송조회
    
    DETAIL_ACTION_TYPE eType = DETAIL_ACTION_TYPE(nTag);
    
    switch (eType)
    {
        case DETAIL_ACTION_CHANGE_ADDRESS:
        {
            PixDeliveryChangeLayer *deliveryChangeLayer = PixDeliveryChangeLayer::create(pOrderCheckInfo);
            deliveryChangeLayer->setDelegate(this);
            Director::getInstance()->getRunningScene()->addChild(deliveryChangeLayer);
        }
        break;
            
        case DETAIL_ACTION_CANCEL_ORDER:
        {
            if(pOrderCheckInfo)
            {
                setCancelOrderCheckInfo(pOrderCheckInfo);
              
                ORDER_TYPE eOrderType = PixMenuOrderLayer::getOrderStatus(pOrderCheckInfo->getPay_status(),pOrderCheckInfo->getItem_status());
                
                // 제작 준비중이고 가상계좌 결재일때만 환불 계좌 입력하는 절차를 밟는다.
                // 그외는 바로 취소
                
                if(eOrderType == ORDER_PRODUCTION_PREPARING && pOrderCheckInfo->getPay_method().compare("vbank") == 0)
                {
                    mOrderCancelLayer = PixOrderCancelLayer::create(pOrderCheckInfo);
                    mOrderCancelLayer->setDelegate(this);
                    mOrderCancelLayer->setOrderCancelLayerDelegate(this);
                    mOrderCancelLayer->setTag(ORDER_CANCEL_LAYER_TAG);
                    
                    Director::getInstance()->getRunningScene()->addChild(mOrderCancelLayer);
                }
                else
                {
                    std::string msg = CODE_NAME("P47");
                    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixMenuOrderLayer::getOrderCancelCall));
                }
            }
        }
        break;
            
        case DETAIL_ACTION_TRACKING_NUMBER:
        {
            PixDeliveryTrackingLayer *deliveryTrackingLayer = PixDeliveryTrackingLayer::create(pOrderCheckInfo);
            deliveryTrackingLayer->setDelegate(this);
            Director::getInstance()->getRunningScene()->addChild(deliveryTrackingLayer);
        }
        break;
            
        default:
            break;
    }
}

void PixMenuOrderLayer::orderCancellation(const std::string &strBank_num, const std::string &strBank_code, const std::string &strBank_holder)
{
    getOrderCancelCall(strBank_num, strBank_code, strBank_holder);
}

#pragma mark - Network

void PixMenuOrderLayer::getOrderListCall(int nlimit, const std::string& strTag)
{
    PIXAPP->showLoading();
    
//    member_id	사용자 uniq ID	필수
//    vendor	밴더명	필수
//    page	페이지 수(1부터 시작)	필수
//    limit	불러올 목록 개수	필수
//    search_key	"상태 검색
//    (' :전체, 1: 입금확인중, 2: 제작준비중, 4: 제작중, 8: 배송중, 16: 배송완료, 32: 주문취소)"	필수
    
    std::string url = SREVERURL_MANAGER->getUrl("GET_ORDER_LIST");
    std::map<std::string, std::string> postParams;
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["page"] = StringUtils::format("%d", mn_Page);
    postParams["limit"] = StringUtils::format("%d", nlimit);
    postParams["search_key"] = getSearchkey();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, strTag, this, httpresponse_selector(PixMenuOrderLayer::getOrderListCallback), nullptr);
}

void PixMenuOrderLayer::getOrderListCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        float fOffset = 0;
        
        PixDictionary *results = root->dictionaryForKey("results");
        
        if(results)
        {
            if (strcmp(response->getHttpRequest()->getTag(), MORE_ORDER_SEARCH_TAG) == 0)
            {
                if(orderListView)
                {
                    fOffset = orderListView->getInnerContainerPosition().y;
                }
            }
            
            PixArray *keys = results->allKeys();
            
            if(keys)
            {
                for(int i = 0; i < keys->count(); ++i)
                {
                    std::string strDate = keys->getStringAtIndex(i);
                    
                    PixArray *monthlyCategory = results->arrayForKey(strDate);
                    
                    if(monthlyCategory)
                    {
                        MonthlyCategoryInfo *monthlyCategoryInfo = getMonthlyCategoryInfo(strDate);
                        
                        if(monthlyCategoryInfo == nullptr)
                        {
                            monthlyCategoryInfo = new MonthlyCategoryInfo(strDate);
                            addMonthlyCategoryInfo(monthlyCategoryInfo);
                            
                            // 리스트 뷰 날짜 카테고리 셀 추가
                            fOffset += addOrderMonthlyCategoryCell(strDate)->getContentSize().height;
                        }
                        
                        for (int j = 0; j < monthlyCategory->count(); ++j)
                        {
                            PixDictionary *order = monthlyCategory->getDictionaryAtIndex(j);
                            
                            if(order)
                            {
                                OrderCheckInfo *orderCheckInfo = new OrderCheckInfo();
                                
                                orderCheckInfo->setOrder_id(order->intForKey("order_id"));
                                orderCheckInfo->setOrder_addr_id(order->intForKey("order_addr_id"));
                                orderCheckInfo->setReg_date(order->stringForKey("reg_date"));
                                orderCheckInfo->setPay_method(order->stringForKey("pay_method"));
                                orderCheckInfo->setPay_status(order->intForKey("pay_status"));
                                orderCheckInfo->setItem_status(order->intForKey("item_status"));
                                orderCheckInfo->setGroup_date(order->stringForKey("group_date"));
                                orderCheckInfo->setAmount(order->intForKey("amount"));
                                orderCheckInfo->setDiscount_amount(order->intForKey("discount_amount"));
                                orderCheckInfo->setPay_amount(order->intForKey("pay_amount"));
                                orderCheckInfo->setPay_ship_amount(order->intForKey("pay_ship_amount"));
                                orderCheckInfo->setOrder_no(order->stringForKey("order_no"));
                                orderCheckInfo->setBackwoods_list_id(order->stringForKey("backwoods_list_id"));
                                orderCheckInfo->setInvoice_number(order->stringForKey("invoice_number"));
                                orderCheckInfo->setInvoice_url(order->stringForKey("invoice_url"));
                                orderCheckInfo->setStrUserName(order->stringForKey("strUserName"));
                                orderCheckInfo->setStrPhone01(order->stringForKey("strPhone01"));
                                orderCheckInfo->setStrEmail(order->stringForKey("strEmail"));
                                orderCheckInfo->setName(order->stringForKey("name"));
                                orderCheckInfo->setPhone1(order->stringForKey("phone1"));
                                orderCheckInfo->setPost(order->stringForKey("post"));
                                orderCheckInfo->setAddr1(order->stringForKey("addr1"));
                                orderCheckInfo->setAddr2(order->stringForKey("addr2"));
                                orderCheckInfo->setMemo(order->stringForKey("memo"));
                                orderCheckInfo->setVbank_name(order->stringForKey("vbank_name"));
                                orderCheckInfo->setVbank_num(order->stringForKey("vbank_num"));
                                orderCheckInfo->setVbank_holder(order->stringForKey("vbank_holder"));
                                orderCheckInfo->setCancel_date(order->stringForKey("cancel_date"));
                                
                                monthlyCategoryInfo->pushOrderCheckInfo(orderCheckInfo);
                                
                                // 리스트 뷰 주문 정보 셀 추가
                                fOffset += addOrderCell(orderCheckInfo)->getContentSize().height;
                                
                                PixArray *jArray = order->arrayForKey("goods");
                                
                                if(jArray)
                                {
                                    for (int k = 0; k < jArray->count(); ++k)
                                    {
                                        PixDictionary *goods = jArray->getDictionaryAtIndex(k);
                                        
                                        if(goods)
                                        {
                                            GoodsInfo *goodsInfo = new GoodsInfo();
                                            
                                            goodsInfo->setTitle(goods->stringForKey("title"));
                                            goodsInfo->setAlbum_image(goods->stringForKey("album_image"));
                                            goodsInfo->setTotal_cnt(goods->intForKey("total_cnt"));
                                            goodsInfo->setPay_amount(goods->intForKey("pay_amount"));
                                            goodsInfo->setItem_display_f(goods->stringForKey("item_display_f"));
                                            goodsInfo->setItem_display_s(goods->stringForKey("item_display_s"));
                                            goodsInfo->setItem_basic_price(goods->intForKey("item_basic_price"));
                                            goodsInfo->setItem_add_page(goods->intForKey("item_add_page"));
                                            goodsInfo->setItem_add_price(goods->intForKey("item_add_price"));
                                            goodsInfo->setItem_discount_price(goods->intForKey("item_discount_price"));
                                            goodsInfo->setAlbum_id(goods->intForKey("album_id"));

                                            orderCheckInfo->pushGoodsInfo(goodsInfo);
                                            
                                            // 리스트 뷰 생품 정보 셀 추가
                                            fOffset += addOrderGoodsCell(orderCheckInfo, goodsInfo)->getContentSize().height;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        mbln_Is_End = (root->intForKey("is_end") == 1) ? true : false;
        
        Size size = Size::ZERO;
        
        if(orderMoreCell)
        {
            size = orderMoreCell->getContentSize();
        }
        
        // 리스트 뷰 더보기 셀 추가
        addOrderMoreCell();
        
        if(orderMoreCell == nullptr)
        {
            fOffset -= size.height;
        }
        
        ++mn_Page;
        
        if (strcmp(response->getHttpRequest()->getTag(), MORE_ORDER_SEARCH_TAG) == 0)
        {
            if(orderListView)
            {
                orderListView->jumpToDestination(Vec2(0,-fOffset));
            }
        }
    }
}

void PixMenuOrderLayer::getOrderCancelCall()
{
    getOrderCancelCall("","","");
}

void PixMenuOrderLayer::getOrderCancelCall(const std::string &strBank_num, const std::string &strBank_code, const std::string &strBank_holder)
{
    if(getCancelOrderCheckInfo() == nullptr)
        return;
    
    PIXAPP->showLoading();
    
    //    order_id	주문 ID	필수
    //    vendor	벤더명	필수
    //    member_id	사용자 uniq ID	필수
    //    bank_num	계좌번호	가상계좌 입금후 환불시
    //    bank_code	은행코드	가상계좌 입금후 환불시
    //    bank_holder	예금주명	가상계좌 입금후 환불시
    
    std::string url = SREVERURL_MANAGER->getUrl("ORDER_CANCEL");
    std::map<std::string, std::string> postParams;
    
    postParams["order_id"] = StringUtils::format("%d", getCancelOrderCheckInfo()->getOrder_id());
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    
    if(getCancelOrderCheckInfo()->getPay_method().compare("vbank") == 0)
    {
        if(!strBank_num.empty() && !strBank_code.empty() && !strBank_holder.empty())
        {
            postParams["bank_num"] = strBank_num;
            postParams["bank_code"] = strBank_code;
            postParams["bank_holder"] = strBank_holder;
        }
    }
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ORDER_CANCEL", this, httpresponse_selector(PixMenuOrderLayer::getOrderCancelCallback), nullptr);
}

void PixMenuOrderLayer::getOrderCancelCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(getCancelOrderCheckInfo())
        {
            if(getCancelOrderCheckInfo()->getOrder_id() == root->intForKey("order_id"))
            {
                getCancelOrderCheckInfo()->setPay_status(root->intForKey("pay_status"));
                
                // 무통장 입금 레이어 삭제
                if(mOrderCancelLayer)
                {
                    Director::getInstance()->getRunningScene()->removeChildByTag(ORDER_CANCEL_LAYER_TAG);
                    mOrderCancelLayer = nullptr;
                }
                
                if(orderListView)
                {
                    orderListView->removeAllItems();
                    orderListView->stopAutoScroll();
                    // 더보기 셀 초기화
                    orderMoreCell = nullptr;
                }
                
                // 빈화면 연출
                if(Panel_1)
                {
                    Panel_1->setVisible(false);
                }
                
                if(Panel_2)
                {
                    Panel_2->setVisible(true);
                }
                
                // 리스트 뷰 아이템 info null 처리
                for (auto item : detailListView->getItems())
                {
                    PixOrderDetailStateCell *orderDetailStateCell = dynamic_cast<PixOrderDetailStateCell*>(item);
                    
                    if(orderDetailStateCell)
                    {
                        orderDetailStateCell->setOrderCheckInfo(nullptr);
                    }
                    
                    PixOrderDetailGoodsCell *orderDetailGoodsCell = dynamic_cast<PixOrderDetailGoodsCell*>(item);
                    
                    if(orderDetailGoodsCell)
                    {
                        orderDetailGoodsCell->setGoodsInfo(nullptr);
                    }
                    
                    PixOrderDetailOrdererInfoCell *orderDetailOrdererInfoCell = dynamic_cast<PixOrderDetailOrdererInfoCell*>(item);
                    
                    if(orderDetailOrdererInfoCell)
                    {
                        orderDetailOrdererInfoCell->setOrderCheckInfo(nullptr);
                    }
                    
                    PixOrderDetailDeliveryInfoCell *orderDetailDeliveryInfoCell = dynamic_cast<PixOrderDetailDeliveryInfoCell*>(item);
                    
                    if(orderDetailDeliveryInfoCell)
                    {
                        orderDetailDeliveryInfoCell->setOrderCheckInfo(nullptr);
                    }
                    
                    PixOrderDetailBillingInfoCell *orderDetailBillingInfoCell = dynamic_cast<PixOrderDetailBillingInfoCell*>(item);
                    
                    if(orderDetailBillingInfoCell)
                    {
                        orderDetailBillingInfoCell->setOrderCheckInfo(nullptr);
                    }
                }
                
                // 데이터 삭제
                removeAllMonthlyCategorys();
                
                // 재검색
                getOrderListCall(FIRST_ORDER_SEARCH_LIMIT, FIRST_ORDER_SEARCH_TAG);
                hideDetailListViewAction();
                
                std::string msg = CODE_NAME("T15");
                PixToast::showToast(msg.c_str(), Vec2(360,310));
            }
        }
        
        setCancelOrderCheckInfo(nullptr);
    }
}

#pragma mark - Android BackPressed

void PixMenuOrderLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
   
    if(mbln_IsAction == false)
    {
        if(me_OrderLayerType == ORDERLAYER_LIST)
        {
            if(getAndroidBackPressedAction() == false)
            {
                setAndroidBackPressedAction(true);
                hideMenuOrderLayerAction();
            }
            
            return;
        }
        else
        {
            hideDetailListViewAction();
            return;
        }
    }
}

#pragma mark - static method

std::string PixMenuOrderLayer::getPayMethodToUIString(const std::string &strPay_method)
{
    //    결제수단
    //    card (신용카드)
    //    trans (실시간계좌이체)
    //    vbank (가상계좌)
    //    phone (휴대폰 소액결제)
    //    samsung (삼성페이 / 이니시스전용)
    //    kpay (kPay 앱 직접호출 / 이니시스 전용)
    //    cultureland ( 문화상품권 / 이니시스 전용)
    //    smartculture ( 스마트 문화상품권 / 이니시스 전용)
    //    happymoney ( 해피머니 / 이니시스 전용)
    
    std::string strText;
    strText.clear();
    
    if(strPay_method.compare("card") == 0)
    {
        strText = CODE_NAME("Pay13");
    }
    else if(strPay_method.compare("trans") == 0)
    {
        strText = CODE_NAME("Pay34");
    }
    else if(strPay_method.compare("vbank") == 0)
    {
        strText = CODE_NAME("Pay15");
    }
    else if(strPay_method.compare("phone") == 0)
    {
        strText = CODE_NAME("Pay14");
    }
    else if(strPay_method.compare("samsung") == 0)
    {
        strText = "";
    }
    else if(strPay_method.compare("kpay") == 0)
    {
        strText = "";
    }
    else if(strPay_method.compare("cultureland") == 0)
    {
        strText = "";
    }
    else if(strPay_method.compare("smartculture") == 0)
    {
        strText = "";
    }
    else if(strPay_method.compare("happymoney") == 0)
    {
        strText = "";
    }
    
    return strText;
}

std::string PixMenuOrderLayer::getOrderStatusToUIString(int nPay_status, int nItem_status)
{
    //    * 각 주문 상태별 데이터
    //    입금확인중       : pay_status=1
    //    제작준비중       : pay_status=2 & item_status=2
    //    제작중             : pay_status=2 & item_status=4
    //    배송중             : pay_status=2 & item_status=8
    //    배송완료          : pay_status=2 & item_status=16
    //    주문취소처리중 : pay_status=4
    //    주문취소          : pay_status=8,16
    
    std::string strText;
    strText.clear();
    
    if(nPay_status == 1)
    {
        strText = CODE_NAME("Order10");
    }
    else if(nPay_status == 2 && nItem_status == 2)
    {
        strText = CODE_NAME("Order11");
    }
    else if(nPay_status == 2 && nItem_status == 4)
    {
        strText = CODE_NAME("Order12");
    }
    else if(nPay_status == 2 && nItem_status == 8)
    {
        strText = CODE_NAME("Order13");
    }
    else if(nPay_status == 2 && nItem_status == 16)
    {
        strText = CODE_NAME("Order14");
    }
    else if(nPay_status == 4)
    {
        strText = CODE_NAME("Order41");
    }
    else if(nPay_status == 8 || nPay_status == 16)
    {
        strText = CODE_NAME("Order15");
    }
    
    return strText;
}


ORDER_TYPE PixMenuOrderLayer::getOrderStatus(int nPay_status, int nItem_status)
{
    //    * 각 주문 상태별 데이터
    //    입금확인중       : pay_status=1
    //    제작준비중       : pay_status=2 & item_status=2
    //    제작중             : pay_status=2 & item_status=4
    //    배송중             : pay_status=2 & item_status=8
    //    배송완료          : pay_status=2 & item_status=16
    //    주문취소처리중 : pay_status=4
    //    주문취소          : pay_status=8,16
    

//    ORDER_DEPOSIT_CHECK = 1,                  // 입금 확인중
//    ORDER_PRODUCTION_PREPARING = 2,           // 제작 준비중
//    ORDER_IN_PRODUCTION = 4,                  // 제작중
//    ORDER_DELIVERY = 8,                       // 배송중
//    ORDER_COMPLETE_DELIVERY = 16,             // 배송완료
//    ORDER_CANCELLATION_PROCESSING = 100,      // 주문취소 처리중(UI에서만 사용 검색에서는 사용안함)
//    ORDER_CANCELLATION = 32,                  // 주문취소

    if(nPay_status == 1)
    {
        return ORDER_DEPOSIT_CHECK;
    }
    else if(nPay_status == 2 && nItem_status == 2)
    {
        return ORDER_PRODUCTION_PREPARING;
    }
    else if(nPay_status == 2 && nItem_status == 4)
    {
        return ORDER_IN_PRODUCTION;
    }
    else if(nPay_status == 2 && nItem_status == 8)
    {
        return ORDER_DELIVERY;
    }
    else if(nPay_status == 2 && nItem_status == 16)
    {
        return ORDER_COMPLETE_DELIVERY;
    }
    else if(nPay_status == 4)
    {
        return ORDER_CANCELLATION_PROCESSING;
    }
    else if(nPay_status == 8 || nPay_status == 16)
    {
        return ORDER_CANCELLATION;
    }

    return ORDER_NONE;
}
