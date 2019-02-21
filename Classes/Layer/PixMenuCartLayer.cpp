//
//  PixMenuCartLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 6..
//
//

#include "PixMenuCartLayer.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixMyAlbumManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/CartInfo.h"
#include "../Info/CartOrderTotalInfo.h"
#include "../Info/CouponInfo.h"
#include "../Info/DeliveryInfo.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUtils.h"
#include "PixMenuOrderLayer.h"
#include "../Common.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

PixMenuCartLayer* PixMenuCartLayer::create()
{
    PixMenuCartLayer *MenuCartLayer = new (std::nothrow) PixMenuCartLayer();
    if (MenuCartLayer && MenuCartLayer->init())
    {
        MenuCartLayer->autorelease();
        return MenuCartLayer;
    }
    else
    {
        CC_SAFE_DELETE(MenuCartLayer);
        return nullptr;
    }
}

PixMenuCartLayer::PixMenuCartLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, Top_01(nullptr)
, Top_02(nullptr)
, Top_03(nullptr)
, Top_04(nullptr)
, MenuCartEmptyLayer(nullptr)
, cartListView(nullptr)
, orderListView(nullptr)
, paymentListView(nullptr)
, orderCompletedListView(nullptr)
, Button_order(nullptr)
, Button_order_2(nullptr)
, Button_order_3(nullptr)
, mDelegate(nullptr)
, me_Type(CARTLAYER_NONE)
, mn_free_item_price(0)
, mn_price(0)
, mbln_IsAction(false)
, cartTotalInfo(nullptr)
, orderTotalInfo(nullptr)
, finalTotalInfo(nullptr)
, cartSelectAllCell(nullptr)
, cartTotalCell(nullptr)
, paymentCountCell(nullptr)
, paymentTotalCellCell(nullptr)
, payDeliveryInfoCell(nullptr)
, payNewDeliveryCell(nullptr)
, payDeliveryListCell(nullptr)
, payPaymentMethodCell(nullptr)
, payFinalPaymentAmountCell(nullptr)
, mbln_showOrderListView(false)
, mbln_showPaymentListView(false)
, mbln_showOrderCompletedListView(nullptr)
, deliveryVec(nullptr)
{
    deliveryVec = new std::vector<DeliveryInfo*>();
    
    cartVec.clear();
    orderVec.clear();
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMenuCartLayer::hideMenuCartLayerAction,this);
}

PixMenuCartLayer::~PixMenuCartLayer()
{
    pixHttpClient->cancelRequest(this);
    
    for (auto iter = cartVec.begin(); iter != cartVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    cartVec.clear();
    
    for (auto iter = orderVec.begin(); iter != orderVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    orderVec.clear();
    
    for (auto iter = deliveryVec->begin(); iter != deliveryVec->end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    deliveryVec->clear();
    
    CC_SAFE_DELETE(deliveryVec);
    
    CC_SAFE_DELETE(cartTotalInfo);
    CC_SAFE_DELETE(orderTotalInfo);
    CC_SAFE_DELETE(finalTotalInfo);
    
    CC_SAFE_RELEASE_NULL(payNewDeliveryCell);
    CC_SAFE_RELEASE_NULL(payDeliveryListCell);
}

void PixMenuCartLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuCartLayer.csb");
    
    if(rootNode)
    {
        Node *MenuCart_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuCart_Top"));
        
        if(MenuCart_Top)
        {
            // 결재 수단
            Top_01 = dynamic_cast<Node*>(MenuCart_Top->getChildByName("Top_01"));
            
            if(Top_01)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Top_01->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Top_01->getChildByName("Button_back"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixMenuCartLayer::buttonCancelPressed, this));
                }
            }
            
            // 주문 확인
            Top_02 = dynamic_cast<Node*>(MenuCart_Top->getChildByName("Top_02"));
            
            if(Top_02)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Top_02->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *Button_back = dynamic_cast<ui::Button*>(Top_02->getChildByName("Button_back"));
                
                if(Button_back)
                {
                    Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuCartLayer::buttonOrderBackPressed, this));
                }
            }
            
            // 주문 결재
            Top_03 = dynamic_cast<Node*>(MenuCart_Top->getChildByName("Top_03"));
            
            if(Top_03)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Top_03->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *Button_back = dynamic_cast<ui::Button*>(Top_03->getChildByName("Button_back"));
                
                if(Button_back)
                {
                    Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuCartLayer::buttonPaymentBackPressed, this));
                }
            }
            
            // 주문 완료
            Top_04 = dynamic_cast<Node*>(MenuCart_Top->getChildByName("Top_04"));
            
            if(Top_04)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Top_04->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *Button_back = dynamic_cast<ui::Button*>(Top_04->getChildByName("Button_back"));
                
                if(Button_back)
                {
                    Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuCartLayer::buttonCancelPressed, this));
                }
            }
        }
        
        MenuCartEmptyLayer = dynamic_cast<Node*>(rootNode->getChildByName("MenuCartEmptyLayer"));
        
        if(MenuCartEmptyLayer)
        {
            ui::Text *Text_empty = dynamic_cast<ui::Text*>(MenuCartEmptyLayer->getChildByName("Text_empty"));
            
            if(Text_empty)
            {
                Text_empty->setString(CODE_NAME(Text_empty->getString()));
            }
            
            ui::Text *Notice = dynamic_cast<ui::Text*>(MenuCartEmptyLayer->getChildByName("Notice"));
            
            if(Notice)
            {
                Notice->setString(CODE_NAME(Notice->getString()));
            }
            
            ui::Text *Text_notice = dynamic_cast<ui::Text*>(MenuCartEmptyLayer->getChildByName("Text_notice"));
            
            if(Text_notice)
            {
                Text_notice->setString(CODE_NAME(Text_notice->getString()));
            }

            if(PIXAPP->userInfo->getCartCount() > 0)
            {
                MenuCartEmptyLayer->setVisible(false);
            }
            else
            {
                MenuCartEmptyLayer->setVisible(true);
            }
        }
        
        ui::ListView *listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView"));
        
        if(listView)
        {
            cartListView = PixGalleryListView::create();
            cartListView->copyClonedListView(listView);
        }
        
        listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView_2"));
        
        if(listView)
        {
            orderListView = PixGalleryListView::create();
            orderListView->copyClonedListView(listView);
        }
        
        listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView_3"));
        
        if(listView)
        {
            paymentListView = PixGalleryListView::create();
            paymentListView->copyClonedListView(listView);
        }
        
        listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView_4"));
        
        if(listView)
        {
            orderCompletedListView = PixGalleryListView::create();
            orderCompletedListView->copyClonedListView(listView);
        }
        
        Button_order = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_order"));
        
        if(Button_order)
        {
            Button_order->setTitleText(CODE_NAME(Button_order->getTitleText()));
            Button_order->addTouchEventListener(CC_CALLBACK_2(PixMenuCartLayer::buttonOrderPressed, this));
            Button_order->setLocalZOrder(100);
        }
        
        Button_order_2 = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_order_2"));
        
        if(Button_order_2)
        {
            Button_order_2->setTitleText(CODE_NAME(Button_order_2->getTitleText()));
            Button_order_2->addTouchEventListener(CC_CALLBACK_2(PixMenuCartLayer::buttonInputShippingInfoPressed, this));
            Button_order_2->setLocalZOrder(101);
        }
        
        Button_order_3 = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_order_3"));
        
        if(Button_order_3)
        {
            Button_order_3->setTitleText(CODE_NAME(Button_order_3->getTitleText()));
            Button_order_3->addTouchEventListener(CC_CALLBACK_2(PixMenuCartLayer::buttonPaymentPressed, this));
            Button_order_3->setLocalZOrder(102);
        }

        setAnchorPoint(Vec2::ZERO);
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        addChild(rootNode);
        
        getCartCall();
        setLayerTypr(CARTLAYER_CART);
    }
}

void PixMenuCartLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuCartLayerAction();
}

void PixMenuCartLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(me_Type == CARTLAYER_CART)
    {
        updateCartOrderTotalInfo();
        
        if(MenuCartEmptyLayer)
        {
            if(PIXAPP->userInfo->getCartCount() > 0)
            {
                MenuCartEmptyLayer->setVisible(false);
            }
            else
            {
                MenuCartEmptyLayer->setVisible(true);
            }
        }
        
        if(cartListView)
        {
            if(cartVec.empty())
            {
                if(Button_order)
                {
                    Button_order->setEnabled(false);
                }
            }
            else
            {
                bool blnSelected = false;
                
                for (auto item : cartListView->getItems())
                {
                    PixCartCell *cartCell = dynamic_cast<PixCartCell*>(item);
                    
                    if(cartCell)
                    {
                        if(cartCell->isCheckSelected() == true)
                        {
                            blnSelected = true;
                            break;
                        }
                    }
                }
                
                if(Button_order)
                {
                    if(blnSelected)
                    {
                        Button_order->setEnabled(true);
                    }
                    else
                    {
                        Button_order->setEnabled(false);
                    }
                }
            }
        }
    }
    else if(me_Type == CARTLAYER_ORDER)
    {
        updateOrderTotalInfo();
    }
    else if(me_Type == CARTLAYER_PAYMENT)
    {
        updateFinalTotalInfo();
    }
    else
    {
        
    }
    
    if(rootNode)
    {
        Size size = rootNode->getContentSize();
        
        if(orderListView)
        {
            if(orderListView->getPosition().x >= size.width)
            {
                mbln_showOrderListView = false;
            }
            else
            {
                mbln_showOrderListView = true;
            }
        }
        
        if(paymentListView)
        {
            if(paymentListView->getPosition().x >= size.width)
            {
                mbln_showPaymentListView = false;
            }
            else
            {
                mbln_showPaymentListView = true;
            }
        }
        
        if(orderCompletedListView)
        {
            if(orderCompletedListView->getPosition().x >= size.width)
            {
                mbln_showOrderCompletedListView = false;
            }
            else
            {
                mbln_showOrderCompletedListView = true;
            }
        }
    }
}

void PixMenuCartLayer::createCartListView()
{
    if(cartListView == nullptr)
        return;
    
    if(cartVec.empty())
        return;
    
    cartSelectAllCell = PixCartSelectAllCell::create();
    cartSelectAllCell->setDelegate(this);
    cartSelectAllCell->setCheckSelected(true);
    cartListView->pushBackCustomItem(cartSelectAllCell);
    
    for (int i = 0; i < cartVec.size(); ++i)
    {
        PixCartCell *cartCell = PixCartCell::create(cartVec.at(i), cartTotalInfo);
        cartCell->setDelegate(this);
        cartCell->setCheckSelected(true);
        cartListView->pushBackCustomItem(cartCell);
    }
    
    PixCartProductGuideCell *cartProductGuideCell = PixCartProductGuideCell::create();
    cartListView->pushBackCustomItem(cartProductGuideCell);
    
    cartTotalCell = PixCartOrderTotalCell::create(cartTotalInfo);
    cartListView->pushBackCustomItem(cartTotalCell);
}

void PixMenuCartLayer::checkGetCartCallback(float dt)
{
    if(cartListView)
    {
        // 애니메이션중 에디트 필드가 검은색으로 보이는
        // 현상으로 애니 매이션 종료후 Visible(true)
     
        if(!cartListView->getItems().empty())
        {
            this->unschedule(schedule_selector(PixMenuCartLayer::checkGetCartCallback));
            
            for (auto item : cartListView->getItems())
            {
                PixCartCell *cartCell = dynamic_cast<PixCartCell*>(item);
                
                if(cartCell)
                {
                    cartCell->setCountVisible(true);
                }
            }
        }
    }
}

void PixMenuCartLayer::updateCartOrderTotalInfo()
{
    if(cartListView == nullptr)
        return;
    
    if(cartTotalInfo == nullptr)
        return;
    
    int nTotal_price = 0;
    int nTotal_pay_price = 0;
    int nTotal_discount_price = 0;
    int nShip_price = 0;
    
    if(!cartListView->getItems().empty())
    {
        for (auto item : cartListView->getItems())
        {
            PixCartCell *cartCell = dynamic_cast<PixCartCell*>(item);
            
            if(cartCell)
            {
                if(cartCell->isCheckSelected())
                {
                    CartInfo* cartInfo = cartCell->getCartInfo();
                    
                    if(cartInfo)
                    {
                        CartItem *cartItem = cartInfo->getSelectedCartItem();
                        
                        if(cartItem)
                        {
                            nTotal_price += cartItem->getItem_price();
                            nTotal_discount_price += (cartItem->getItem_price() - cartItem->getItem_pay_price());
                        }
                    }
                }
            }
        }
    }
    
    cartTotalInfo->setTotal_price(nTotal_price);
    cartTotalInfo->setTotal_discount_price(nTotal_discount_price);
    
    if((nTotal_price - nTotal_discount_price) < mn_free_item_price)
    {
        nShip_price = mn_price;
    }
    
    cartTotalInfo->setShip_price(nShip_price);
    
    nTotal_pay_price = (nTotal_price - nTotal_discount_price) + nShip_price;
    cartTotalInfo->setTotal_pay_price(nTotal_pay_price);
}

void PixMenuCartLayer::setLayerTypr(CARTLAYER_TYPE eType)
{
    if(me_Type == eType)
        return;
    
    me_Type = eType;
    
    switch (me_Type)
    {
        case CARTLAYER_CART:
        {
            if(Top_01)
            {
                Top_01->setVisible(true);
            }
            
            if(cartListView)
            {
                cartListView->setVisible(true);
                cartListView->stopAutoScroll();
            }
            
            if(Button_order)
            {
                Button_order->setVisible(true);
            }
            
            if(Top_02)
            {
                Top_02->setVisible(false);
            }
            
            if(orderListView)
            {
                orderListView->setVisible(false);
                orderListView->stopAutoScroll();
            }
            
            if(Button_order_2)
            {
                Button_order_2->setVisible(false);
            }
            
            if(Top_03)
            {
                Top_03->setVisible(false);
            }
            
            if(paymentListView)
            {
                paymentListView->setVisible(false);
                paymentListView->stopAutoScroll();
            }
            
            if(Button_order_3)
            {
                Button_order_3->setVisible(false);
            }
            
            if(Top_04)
            {
                Top_04->setVisible(false);
            }
            
            if(orderCompletedListView)
            {
                orderCompletedListView->setVisible(false);
                orderCompletedListView->stopAutoScroll();
            }
        }
        break;
            
        case CARTLAYER_ORDER:
        {
            if(Top_01)
            {
                Top_01->setVisible(false);
            }
            
            if(cartListView)
            {
                cartListView->setVisible(false);
                cartListView->stopAutoScroll();
            }
            
            if(Button_order)
            {
                Button_order->setVisible(false);
            }
            
            if(Top_02)
            {
                Top_02->setVisible(true);
            }
            
            if(orderListView)
            {
                orderListView->setVisible(true);
                orderListView->stopAutoScroll();
            }
            
            if(Button_order_2)
            {
                Button_order_2->setVisible(true);
            }
            
            if(Top_03)
            {
                Top_03->setVisible(false);
            }
            
            if(paymentListView)
            {
                paymentListView->setVisible(false);
                paymentListView->stopAutoScroll();
            }
            
            if(Button_order_3)
            {
                Button_order_3->setVisible(false);
            }
            
            if(Top_04)
            {
                Top_04->setVisible(false);
            }
            
            if(orderCompletedListView)
            {
                orderCompletedListView->setVisible(false);
                orderCompletedListView->stopAutoScroll();
            }
        }
        break;
            
        case CARTLAYER_PAYMENT:
        {
            if(Top_01)
            {
                Top_01->setVisible(false);
            }
            
            if(cartListView)
            {
                cartListView->setVisible(false);
                cartListView->stopAutoScroll();
            }
            
            if(Button_order)
            {
                Button_order->setVisible(false);
            }
            
            if(Top_02)
            {
                Top_02->setVisible(false);
            }
            
            if(orderListView)
            {
                orderListView->setVisible(false);
                orderListView->stopAutoScroll();
            }
            
            if(Button_order_2)
            {
                Button_order_2->setVisible(false);
            }
            
            if(Top_03)
            {
                Top_03->setVisible(true);
            }
            
            if(paymentListView)
            {
                paymentListView->setVisible(true);
                paymentListView->stopAutoScroll();
            }
            
            if(Button_order_3)
            {
                Button_order_3->setVisible(true);
            }
            
            if(Top_04)
            {
                Top_04->setVisible(false);
            }
            
            if(orderCompletedListView)
            {
                orderCompletedListView->setVisible(false);
                orderCompletedListView->stopAutoScroll();
            }
        }
        break;
        
        case CARTLAYER_ORDER_COMPLETED:
        {
            if(Top_01)
            {
                Top_01->setVisible(false);
            }
            
            if(cartListView)
            {
                cartListView->setVisible(false);
                cartListView->stopAutoScroll();
            }
            
            if(Button_order)
            {
                Button_order->setVisible(false);
            }
            
            if(Top_02)
            {
                Top_02->setVisible(false);
            }
            
            if(orderListView)
            {
                orderListView->setVisible(false);
                orderListView->stopAutoScroll();
            }
            
            if(Button_order_2)
            {
                Button_order_2->setVisible(false);
            }
            
            if(Top_03)
            {
                Top_03->setVisible(false);
            }
            
            if(paymentListView)
            {
                paymentListView->setVisible(false);
                paymentListView->stopAutoScroll();
            }
            
            if(Button_order_3)
            {
                Button_order_3->setVisible(false);
            }
            
            if(Top_04)
            {
                Top_04->setVisible(true);
            }
            
            if(orderCompletedListView)
            {
                orderCompletedListView->setVisible(true);
                orderCompletedListView->stopAutoScroll();
            }
        }
        break;
            
        default:
            break;
    }
}

void PixMenuCartLayer::clearOrderListView()
{
    if(orderListView)
    {
        orderListView->removeAllItems();
    }
    
    paymentCountCell = nullptr;
    paymentTotalCellCell = nullptr;
    
    for (auto iter = orderVec.begin(); iter != orderVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    orderVec.clear();
    
    CC_SAFE_DELETE(orderTotalInfo);
}

void PixMenuCartLayer::createOrderListView()
{
    if(orderListView == nullptr)
        return;
    
    if(orderVec.empty())
        return;
    
    paymentCountCell = PixPaymentCountCell::create((int)orderVec.size());
    orderListView->pushBackCustomItem(paymentCountCell);
    
    for (int i = 0; i < orderVec.size(); ++i)
    {
        PixPaymentCell *paymentCell = PixPaymentCell::create(orderVec.at(i), orderTotalInfo);
        paymentCell->setDelegate(this);
        orderListView->pushBackCustomItem(paymentCell);
    }
    
    paymentTotalCellCell = PixPaymentTotalCell::create(orderTotalInfo);
    orderListView->pushBackCustomItem(paymentTotalCellCell);
}

void PixMenuCartLayer::updateOrderTotalInfo()
{
    if(orderListView == nullptr)
        return;
    
    if(orderTotalInfo == nullptr)
        return;
    
    if(orderVec.empty())
        return;
    
    int nTotal_price = 0;
    int nTotal_discount_price = 0;
    int nShip_price = 0;
    int nTotal_pay_price = 0;;
    
    for (auto item : orderListView->getItems())
    {
        PixPaymentCell *child = dynamic_cast<PixPaymentCell*>(item);
        
        if(child)
        {
            CartInfo *cartInfo = child->getCartInfo();
            
            if(cartInfo)
            {
                CartItem *cartItem = cartInfo->getSelectedCartItem();
                
                if(cartItem)
                {
                    nTotal_price += cartItem->getItem_price();
                    
                    CouponInfo *couponInfo = cartInfo->getCouponInfo();
                    
                    if(couponInfo)
                    {
                        nTotal_discount_price += couponInfo->getCoupon_price();
                    }
                    else
                    {
                        nTotal_discount_price += (cartItem->getItem_price() - cartItem->getItem_pay_price());
                    }
                }
            }
        }
    }
    
    orderTotalInfo->setTotal_price(nTotal_price);
    orderTotalInfo->setTotal_discount_price(nTotal_discount_price);

    if((nTotal_price - nTotal_discount_price) < mn_free_item_price)
    {
        nShip_price = mn_price;
    }
    
    orderTotalInfo->setShip_price(nShip_price);
    
    nTotal_pay_price = (nTotal_price - nTotal_discount_price) + nShip_price;
    orderTotalInfo->setTotal_pay_price(nTotal_pay_price);
}

void PixMenuCartLayer::createPaymentListView()
{
    if(paymentListView == nullptr)
        return;
    
    paymentListView->removeAllItems();
    
    // 주문자 정보
    paymentListView->pushBackCustomItem(PixPayBuyerInfoCell::create());
    
    // 배송지 정보 (버튼)
    payDeliveryInfoCell = PixPayDeliveryInfoCell::create();
    payDeliveryInfoCell->setDelegate(this);
    paymentListView->pushBackCustomItem(payDeliveryInfoCell);
    
    // 신규배송지
    CC_SAFE_RELEASE_NULL(payNewDeliveryCell);
    payNewDeliveryCell = PixPayNewDeliveryCell::create();
    payNewDeliveryCell->retain();

    // 배송지리스트
    CC_SAFE_RELEASE_NULL(payDeliveryListCell);
    payDeliveryListCell = PixPayDeliveryListCell::create();
    payDeliveryListCell->setDelegate(this);
    payDeliveryListCell->retain();
    
    if(deliveryVec->empty())
    {
        paymentListView->pushBackCustomItem(payNewDeliveryCell);
        payDeliveryInfoCell->setDeliveryType(CARTLAYER_ONLY_NEW);
    }
    else
    {
        DeliveryInfo *pInfo = nullptr;
        
        for (auto iter = deliveryVec->begin(); iter != deliveryVec->end(); ++iter)
        {
            if((*iter)->getLast() == true)
            {
                pInfo = (*iter);
                break;
            }
        }
        
        if(pInfo)
        {
            pInfo->setSelected(true);
        }
        
        payDeliveryListCell->setDeliveryInfo(pInfo);
        paymentListView->pushBackCustomItem(payDeliveryListCell);
        payDeliveryInfoCell->setDeliveryType(CARTLAYER_LIST);
    }
    
    // 결재 방법
    payPaymentMethodCell = PixPayPaymentMethodCell::create();
    paymentListView->pushBackCustomItem(payPaymentMethodCell);
    
    if(orderTotalInfo)
    {
        CC_SAFE_DELETE(finalTotalInfo);
        finalTotalInfo = orderTotalInfo->clone();
    }
    
    // 최종 결재 금액
    payFinalPaymentAmountCell = PixPayFinalPaymentAmountCell::create(finalTotalInfo);
    paymentListView->pushBackCustomItem(payFinalPaymentAmountCell);
}

void PixMenuCartLayer::clearPaymentListView()
{
    if(paymentListView == nullptr)
        return;
    
    paymentListView->removeAllItems();
    
    for (auto iter = deliveryVec->begin(); iter != deliveryVec->end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    deliveryVec->clear();
    
    payDeliveryInfoCell = nullptr;
    payPaymentMethodCell = nullptr;
    payFinalPaymentAmountCell = nullptr;
    
    CC_SAFE_RELEASE_NULL(payNewDeliveryCell);
    CC_SAFE_RELEASE_NULL(payDeliveryListCell);
    
    CC_SAFE_DELETE(finalTotalInfo);
}

void PixMenuCartLayer::updateFinalTotalInfo()
{
    if(paymentListView == nullptr)
        return;
    
    if(finalTotalInfo == nullptr)
        return;
    
    int nTotal_price = 0;
    int nTotal_pay_price = 0;
    int nTotal_discount_price = 0;
    int nShip_price = 0;
    int nPay_add_ship_amount = 0;
    
    if(!orderListView->getItems().empty())
    {
        for (auto item : orderListView->getItems())
        {
            PixPaymentCell *child = dynamic_cast<PixPaymentCell*>(item);
            
            if(child)
            {
                CartInfo *cartInfo = child->getCartInfo();
                
                if(cartInfo)
                {
                    CartItem *cartItem = cartInfo->getSelectedCartItem();
                    
                    if(cartItem)
                    {
                        nTotal_price += cartItem->getItem_price();
                        
                        CouponInfo *couponInfo = cartInfo->getCouponInfo();
                        
                        if(couponInfo)
                        {
                            nTotal_discount_price += couponInfo->getCoupon_price();
                        }
                        else
                        {
                            nTotal_discount_price += (cartItem->getItem_price() - cartItem->getItem_pay_price());
                        }
                    }
                }
            }
        }
    }
    
    finalTotalInfo->setTotal_price(nTotal_price);
    finalTotalInfo->setTotal_discount_price(nTotal_discount_price);
    
    
    
    DeliveryInfo *pInfo = nullptr;
    
    if(payDeliveryInfoCell)
    {
        DELIVERY_TYPE eTYpe = payDeliveryInfoCell->getDeliveryType();
        
        if(eTYpe == CARTLAYER_LIST)
        {
            for (auto iter = deliveryVec->begin(); iter != deliveryVec->end(); ++iter)
            {
                if((*iter)->getSelected())
                {
                    pInfo = (*iter);
                    break;
                }
            }
        }
        else if(eTYpe == CARTLAYER_NEW || eTYpe == CARTLAYER_ONLY_NEW)
        {
            if(payNewDeliveryCell)
            {
                nPay_add_ship_amount = payNewDeliveryCell->getPay_add_ship_amount();
            }
        }
    }
    
    finalTotalInfo->setShip_price(nShip_price);
    finalTotalInfo->setPay_add_ship_amount(nPay_add_ship_amount);
    
    // mn_free_item_price 이상 배송비 무료
    // 산간 지방일경우 추가배송비 + mn_price(기본배송비)
    
    if(pInfo)
    {
        nPay_add_ship_amount = pInfo->getPay_add_ship_amount();
        
        if(payFinalPaymentAmountCell)
        {
            if(nPay_add_ship_amount > 0)
            {
                payFinalPaymentAmountCell->setDeliveryChargeType(DELIVERY_CHARGE_ADD);
            }
            else
            {
                payFinalPaymentAmountCell->setDeliveryChargeType(DELIVERY_CHARGE_NONE);
            }
        }
    }
    // 배송지 리스트가 없을떄
    else
    {
        nPay_add_ship_amount = payNewDeliveryCell->getPay_add_ship_amount();
        
        if(payFinalPaymentAmountCell)
        {
            if(nPay_add_ship_amount > 0)
            {
                payFinalPaymentAmountCell->setDeliveryChargeType(DELIVERY_CHARGE_ADD);
            }
            else
            {
                payFinalPaymentAmountCell->setDeliveryChargeType(DELIVERY_CHARGE_NONE);
            }
        }
    }
    
    if(nPay_add_ship_amount == 0)
    {
        if(payFinalPaymentAmountCell)
        {
            payFinalPaymentAmountCell->setDeliveryChargeType(DELIVERY_CHARGE_NONE);
        }
    }
    
    if((nTotal_price - nTotal_discount_price) < mn_free_item_price)
    {
        nShip_price = mn_price;
    }
    else
    {
        nShip_price = 0;
        nPay_add_ship_amount = 0;
    }
    
    nTotal_pay_price = (nTotal_price - nTotal_discount_price) + nShip_price + nPay_add_ship_amount;
    
    finalTotalInfo->setShip_price(nShip_price);
    finalTotalInfo->setPay_add_ship_amount(nPay_add_ship_amount);
    finalTotalInfo->setTotal_pay_price(nTotal_pay_price);
}

void PixMenuCartLayer::createOrderCompletedListView(int nAmount, const std::string& strPay_method, const std::string& strPay_method_text, const std::string& strVbank_name, const std::string& strVbank_num,const std::string& strVbank_holder, const std::string& strVbank_date, const std::string& strOrder_id, const std::string& strReg_date)
{
    if(orderCompletedListView == nullptr)
        return;
    
    orderCompletedListView->removeAllItems();
    
    PixPurchaseCompletedTitleCell *titleCell = PixPurchaseCompletedTitleCell::create(strOrder_id, strReg_date);
    orderCompletedListView->pushBackCustomItem(titleCell);
    
    PixPurchaseCompletedInfoCell *infoCell = PixPurchaseCompletedInfoCell::create(nAmount, strPay_method, strPay_method_text, strVbank_name, strVbank_num, strVbank_holder, strVbank_date);
    orderCompletedListView->pushBackCustomItem(infoCell);
    
    PixPurchaseCompletedBottomCell *bottomCell = PixPurchaseCompletedBottomCell::create();
    bottomCell->setDelegate(this);
    orderCompletedListView->pushBackCustomItem(bottomCell);
}

bool PixMenuCartLayer::checkPurchase()
{
    if(payDeliveryInfoCell)
    {
        DELIVERY_TYPE eTYpe = payDeliveryInfoCell->getDeliveryType();
        
        if(eTYpe == CARTLAYER_LIST)
        {
            
        }
        else if(eTYpe == CARTLAYER_NEW || eTYpe == CARTLAYER_ONLY_NEW)
        {
            if(payNewDeliveryCell)
            {
                if(strlen(payNewDeliveryCell->getName()) == 0)
                {
                    std::string msg = CODE_NAME("P15");
                    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
                    return false;
                }
                
                if(strlen(payNewDeliveryCell->getPhoneNumber()) == 0)
                {
                    std::string msg = CODE_NAME("P11");
                    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
                    return false;
                }
                
                if(PixNativeUtils::isPhoneNumberPatternMatching(payNewDeliveryCell->getPhoneNumber()) == false)
                {
                    std::string msg = CODE_NAME("P22");
                    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
                    return false;
                }

                if(strlen(payNewDeliveryCell->getZipCode()) == 0)
                {
                    std::string msg = CODE_NAME("P48");
                    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
                    return false;
                }
                
                if(strlen(payNewDeliveryCell->getDetailedAddress()) == 0)
                {
                    std::string msg = CODE_NAME("P49");
                    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
                    return false;
                }
            }
        }
    }
    
    if(payFinalPaymentAmountCell)
    {
        if(payFinalPaymentAmountCell->isAgreeSelected() == false)
        {
            std::string msg = CODE_NAME("P50");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
            return false;
        }
    }
    
    return true;
}

const char* PixMenuCartLayer::purchaseDataParsing(const std::string& strData, const std::string& strKey)
{
    std::string data = strData;
    std::string code = "";
    std::string key = strKey;
    
    auto pos = data.find(key);
    
    if ((pos != std::string::npos) && (0 < pos))
    {
        code = data.substr(pos + key.size(), data.size());
        
        auto end = code.find("&");
        
        if((end != std::string::npos) && (0 < end))
        {
            code = code.substr(0, end);
        }
    }
    
    return code.c_str();
}

#pragma mark - Button

void PixMenuCartLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hideMenuCartLayerAction();
    }
}

void PixMenuCartLayer::buttonOrderBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hideOrderListViewAction();
    }
}

void PixMenuCartLayer::buttonPaymentBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hidePaymentListViewAction();
    }
}

void PixMenuCartLayer::buttonOrderPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        getPayStop01();
    }
}

void PixMenuCartLayer::buttonInputShippingInfoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        getAddre();
    }
}

void PixMenuCartLayer::buttonPaymentPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        getPayStop02();
    }
}

#pragma mark - Action

void PixMenuCartLayer::showMenuCartLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCartLayer::showMenuCartLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuCartLayer::hideMenuCartLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCartLayer::hideMenuCartLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }

}

void PixMenuCartLayer::showMenuCartLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
    
    this->schedule(schedule_selector(PixMenuCartLayer::checkGetCartCallback));
}

void PixMenuCartLayer::hideMenuCartLayerActionFinish(Node* node)
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

void PixMenuCartLayer::runCartListViewDeleteAction()
{
    if(cartListView == nullptr)
        return;
    
    Vec2 movePos = Vec2(cartListView->getPositionX() + cartListView->getContentSize().width,
                        cartListView->getPositionY());
    MoveTo *moveTo = MoveTo::create(0.3f,movePos);
    EaseSineIn *easeSineIn = EaseSineIn::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixMenuCartLayer::deleteCartListViewActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineIn, callFuncN, NULL);
    cartListView->runAction(sequence);
}

void PixMenuCartLayer::deleteCartListViewActionFinish()
{
    if(cartListView)
    {
        for (auto iter = cartVec.begin(); iter != cartVec.end(); ++iter)
        {
            MyAlbumModel *pAlbumModel = MYALBUM_MANAGER->getMyAlbumModelByCartID((*iter)->getCart_id());
            
            if(pAlbumModel)
            {
                pAlbumModel->setCart_id(0);
            }
            
            CC_SAFE_DELETE((*iter));
        }
        
        cartListView->removeAllChildrenWithCleanup(true);
        cartSelectAllCell = nullptr;
        cartTotalCell = nullptr;
        cartListView = nullptr;
        CC_SAFE_DELETE(cartTotalInfo);
    }
}

void PixMenuCartLayer::showOrderListViewAction()
{
    if(orderListView == nullptr)
        return;
    
    if(mbln_showOrderListView == true)
        return;
    
    setLayerTypr(CARTLAYER_ORDER);
    
    orderListView->setEnabled(false);
    
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0, orderListView->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCartLayer::showOrderListViewActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    orderListView->runAction(sequence);
}

void PixMenuCartLayer::hideOrderListViewAction()
{
    if(orderListView == nullptr)
        return;
    
    if(mbln_showOrderListView == false)
        return;
    
    orderListView->stopAutoScroll();
    orderListView->setEnabled(false);
    
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth, orderListView->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCartLayer::hideOrderListViewActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    orderListView->stopAllActions();
    orderListView->runAction(sequence);
}

void PixMenuCartLayer::showOrderListViewActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(orderListView)
    {
        orderListView->setEnabled(true);
    }
}

void PixMenuCartLayer::hideOrderListViewActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    setLayerTypr(CARTLAYER_CART);
    clearOrderListView();
    
    if(orderListView)
    {
        orderListView->setEnabled(true);
    }
}

void PixMenuCartLayer::showPaymentListViewAction()
{
    if(paymentListView == nullptr)
        return;
    
    if(mbln_showPaymentListView == true)
        return;
    
    setLayerTypr(CARTLAYER_PAYMENT);
    
    paymentListView->setEnabled(false);
    
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0, paymentListView->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCartLayer::showPaymentListViewActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    paymentListView->runAction(sequence);
}

void PixMenuCartLayer::hidePaymentListViewAction()
{
    if(paymentListView == nullptr)
        return;
    
    if(mbln_showPaymentListView == false)
        return;
    
    paymentListView->stopAutoScroll();
    paymentListView->setEnabled(false);
    
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth, paymentListView->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCartLayer::hidePaymentListViewActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    paymentListView->stopAllActions();
    paymentListView->runAction(sequence);
}

void PixMenuCartLayer::showPaymentListViewActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(paymentListView)
    {
        paymentListView->setEnabled(true);
    }
}

void PixMenuCartLayer::hidePaymentListViewActionFinish(Node* node)
{
    mbln_IsAction = false;

    setLayerTypr(CARTLAYER_ORDER);
    
    if(paymentListView)
    {
        paymentListView->setEnabled(true);
    }
}

void PixMenuCartLayer::showOrderCompletedListViewAction()
{
    if(orderCompletedListView == nullptr)
        return;
    
    if(mbln_showOrderCompletedListView == true)
        return;
    
    setLayerTypr(CARTLAYER_ORDER_COMPLETED);
    
    orderCompletedListView->setEnabled(false);
    
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0, orderCompletedListView->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCartLayer::showOrderCompletedListViewActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    orderCompletedListView->runAction(sequence);
}

void PixMenuCartLayer::hideOrderCompletedListViewAction()
{
    if(orderCompletedListView == nullptr)
        return;
    
    if(mbln_showOrderCompletedListView == false)
        return;
    
    orderCompletedListView->stopAutoScroll();
    orderCompletedListView->setEnabled(false);
    
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth, orderCompletedListView->getPositionY()));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCartLayer::hideOrderCompletedListViewActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    orderCompletedListView->stopAllActions();
    orderCompletedListView->runAction(sequence);
}

void PixMenuCartLayer::showOrderCompletedListViewActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(orderCompletedListView)
    {
        orderCompletedListView->setEnabled(true);
    }
}

void PixMenuCartLayer::hideOrderCompletedListViewActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    setLayerTypr(CARTLAYER_PAYMENT);
    
    if(orderCompletedListView)
    {
        orderCompletedListView->setEnabled(true);
    }
}

#pragma mark - Delegate

void PixMenuCartLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMenuCartLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuCartLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuCartLayer::hideLayerActionFinish(Ref *pTarget)
{
   
}

void PixMenuCartLayer::cartCellDeleteActionStart(Ref *pTarget)
{
    if(cartListView)
    {
        cartListView->setBounceEnabled(false);
        cartListView->setEnabled(false);
    }
    
    if(PIXAPP->userInfo->getCartCount() == 0)
    {
        runCartListViewDeleteAction();
    }
}

void PixMenuCartLayer::cartCellDeleteActionFinish(Ref *pTarget)
{
    PixCartCell *cartCell = dynamic_cast<PixCartCell*>(pTarget);
    
    if(cartCell)
    {
        CartInfo *cartInfo = cartCell->getCartInfo();
        
        if(cartInfo)
        {
            MyAlbumModel *pAlbumModel = MYALBUM_MANAGER->getMyAlbumModelByCartID(cartInfo->getCart_id());
            
            if(pAlbumModel)
            {
                pAlbumModel->setCart_id(0);
            }
            
            for (auto iter = cartVec.begin(); iter != cartVec.end(); ++iter)
            {
                if((*iter) == cartInfo)
                {
                    CC_SAFE_DELETE((*iter));
                    cartVec.erase(iter);
                    break;
                }
            }
        }
  
        if(cartListView)
        {
            ssize_t nIndex = cartListView->getIndex(cartCell);
            
            cartListView->removeItem(nIndex);
            
            --nIndex;
            
            if(nIndex > 0)
            {
                cartListView->jumpToItem(nIndex, Vec2::ZERO, Vec2::ZERO);
            }
            
            cartListView->setBounceEnabled(true);
            cartListView->setEnabled(true);
            cartListView->stopAutoScroll();
        }
    }
    
    if(cartVec.empty())
    {
        cartSelectAllCell = nullptr;
        cartTotalCell = nullptr;
    }
}

void PixMenuCartLayer::cartCellCheckBoxPressed(bool var)
{
    bool blnSelected = true;
    
    for (auto item : cartListView->getItems())
    {
        PixCartCell *cartCell = dynamic_cast<PixCartCell*>(item);
        
        if(cartCell)
        {
            if(cartCell->isCheckSelected() == false)
            {
                blnSelected = false;
                break;
            }
        }
    }
    
    if(cartSelectAllCell)
    {
        if(blnSelected)
        {
            cartSelectAllCell->setCheckSelected(true);
        }
        else
        {
            cartSelectAllCell->setCheckSelected(false);
        }
    }
}

void PixMenuCartLayer::cartSelectAllCellCheckBoxPressed(bool var)
{
    for (auto item : cartListView->getItems())
    {
        PixCartCell *cartCell = dynamic_cast<PixCartCell*>(item);
        
        if(cartCell)
        {
            cartCell->setCheckSelected(var);
        }
    }
}

void PixMenuCartLayer::paymentCellCouponPressed(cocos2d::Ref *pWidget, bool blnSet)
{
    if(pWidget == nullptr)
        return;
    
    PixPaymentCell *paymentCell = dynamic_cast<PixPaymentCell*>(pWidget);
    
    if(paymentCell)
    {
        std::string strParamCoupon_group_id;
        std::string strParamCoupon_id;
        
        strParamCoupon_group_id.clear();
        strParamCoupon_id.clear();
        
        CartInfo *curCartInfo = paymentCell->getCartInfo();
        
        if(curCartInfo)
        {
            CouponInfo *curCouponInfo = curCartInfo->getCouponInfo();
            
            for (auto item : orderListView->getItems())
            {
                PixPaymentCell *child = dynamic_cast<PixPaymentCell*>(item);
                
                if(child)
                {
                    CartInfo *cartInfo = child->getCartInfo();
                    
                    if(cartInfo)
                    {
                        CouponInfo *couponInfo = cartInfo->getCouponInfo();
                        
                        if(couponInfo)
                        {
                            if(curCouponInfo)
                            {
                                int nCurCoupon_group_id = curCouponInfo->getCoupon_group_id();
                                std::string strCurCoupon_id = curCouponInfo->getCoupon_id();
                                
                                if(nCurCoupon_group_id == couponInfo->getCoupon_group_id())
                                {
                                    if(strCurCoupon_id.compare(strCurCoupon_id) == 0)
                                    {
                                        continue;
                                    }
                                }
                            }
                            
                            strParamCoupon_group_id.append(StringUtils::format("%d",couponInfo->getCoupon_group_id()));
                            
                            strParamCoupon_group_id.append(",");
                            
                            if(!couponInfo->getCoupon_id().empty())
                            {
                                strParamCoupon_id.append(couponInfo->getCoupon_id());
                                strParamCoupon_id.append(",");
                            }
                        }
                    }
                }
            }
        }
        
        if(!strParamCoupon_group_id.empty())
        {
           strParamCoupon_group_id = strParamCoupon_group_id.substr(0, strParamCoupon_group_id.find_last_of(","));
        }
        
        if(!strParamCoupon_id.empty())
        {
            strParamCoupon_id = strParamCoupon_id.substr(0, strParamCoupon_id.find_last_of(","));
        }
        
        PixPaymentPopup *paymentPopup = PixPaymentPopup::create(paymentCell->getCartInfo(), strParamCoupon_group_id, strParamCoupon_id);
        Director::getInstance()->getRunningScene()->addChild(paymentPopup);
    }
}

void PixMenuCartLayer::payDeliveryInfoCellListPressed()
{
    ssize_t nIndex = 0;
    
    if(paymentListView)
    {
        float fOffset = paymentListView->getInnerContainerPosition().y;
        
        if(payNewDeliveryCell)
        {
            nIndex = paymentListView->getIndex(payNewDeliveryCell);
            
            if(nIndex > 0)
            {
                paymentListView->removeChild(payNewDeliveryCell);
                
                if(payDeliveryListCell)
                {
                    paymentListView->insertCustomItem(payDeliveryListCell, nIndex);
                }
            }
        }
        
        // 잘작동 안함
        float fHeight = payNewDeliveryCell->getContentSize().height;
        
        if(payDeliveryListCell)
        {
            fHeight = fHeight - payDeliveryListCell->getContentSize().height;
        }
        else
        {
            fHeight = 0;
        }
        
        paymentListView->jumpToDestination(Vec2(0,(fOffset + fHeight)));
    }
}

void PixMenuCartLayer::payDeliveryInfoCellNewPressed()
{
    ssize_t nIndex = 0;
    
    if(paymentListView)
    {
        if(payDeliveryListCell)
        {
            float fOffset = paymentListView->getInnerContainerPosition().y;
            
            nIndex = paymentListView->getIndex(payDeliveryListCell);
            
            if(nIndex > 0)
            {
                paymentListView->removeChild(payDeliveryListCell);
                
                if(payNewDeliveryCell)
                {
                    paymentListView->insertCustomItem(payNewDeliveryCell, nIndex);
                }
            }
            
            float fHeight = payDeliveryListCell->getContentSize().height;
            
            if(payNewDeliveryCell)
            {
                fHeight = payNewDeliveryCell->getContentSize().height - fHeight;
            }
            else
            {
                fHeight = 0;
            }
            
            paymentListView->jumpToDestination(Vec2(0,(fOffset - fHeight)));
        }
    }
}

void PixMenuCartLayer::payDeliveryListCellMoerPressed()
{
    if(mbln_IsAction == true)
        return;
    
    PixPayDeliveryListLayer *payDeliveryListLayer = PixPayDeliveryListLayer::create(deliveryVec);
    payDeliveryListLayer->setDelegate(this);
    Director::getInstance()->getRunningScene()->addChild(payDeliveryListLayer);
}

void PixMenuCartLayer::payDeliveryListLayerCompletePressed(DeliveryInfo *pInfo)
{
    if(pInfo == nullptr)
        return;
    
    for (auto iter = deliveryVec->begin(); iter != deliveryVec->end(); ++iter)
    {
        if((*iter) == pInfo)
        {
            (*iter)->setSelected(true);
        }
        else
        {
            (*iter)->setSelected(false);
        }
    }
    
    if(payDeliveryListCell)
    {
        payDeliveryListCell->setDeliveryInfo(pInfo);
    }
}

void PixMenuCartLayer::payDeliveryListLayerNoAddressAction()
{
    payDeliveryInfoCell->setDeliveryType(CARTLAYER_ONLY_NEW);
    payDeliveryInfoCellNewPressed();
}

void PixMenuCartLayer::purchaseCompile(const std::string& str)
{
    CCLOG("simsimsim purchaseCompile : %s",str.c_str());
    
    std::string strImp_uid = purchaseDataParsing(str,"imp_uid=");
    std::string strMerchant_uid = purchaseDataParsing(str,"merchant_uid=");
    std::string strImp_success = purchaseDataParsing(str,"imp_success=");
    std::string strError_msg =  purchaseDataParsing(str,"error_msg=");
    
    payDoneCall(strImp_uid, strMerchant_uid, strImp_success);
}

void PixMenuCartLayer::purchaseCompletedBottomCellMoerPressed()
{
    PixMenuOrderLayer *menuOrderLayer = PixMenuOrderLayer::create();
    Director::getInstance()->getRunningScene()->addChild(menuOrderLayer);
    
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionStart(this);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

void PixMenuCartLayer::cellDeleteActionFinish(ui::Widget *pWidget)
{
    if(pWidget == nullptr)
        return;
    
    if(cartListView)
    {
        ssize_t nIndex = cartListView->getIndex(pWidget);
        cartListView->removeItem(nIndex);
    }
}

#pragma mark - Network

void PixMenuCartLayer::getCartCall()
{
    PIXAPP->showLoading();

//    member_id	사용자 uniq ID
//    vendor	밴더명
    
    std::string url = SREVERURL_MANAGER->getUrl("GET_CART");
    std::map<std::string, std::string> postParams;
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "GET_CART", this, httpresponse_selector(PixMenuCartLayer::getCartCallback), nullptr);
}

void PixMenuCartLayer::getCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        PixArray *cart_list = root->arrayForKey("cart_list");
        
        if(cart_list)
        {
            for (int i = 0; i < cart_list->count(); ++i)
            {
                PixDictionary *cart = cart_list->getDictionaryAtIndex(i);
                
                if(cart)
                {
                    CartInfo *cartInfo = new CartInfo();
 
                    int nCart_id = cart->intForKey("cart_id");
                    int nSmart_album_id = cart->intForKey("smart_album_id");
                    int nCnt = cart->intForKey("cnt");
                    std::string strTitle = cart->stringForKey("title");
                    int nTotal_page = cart->intForKey("total_page");
                    int nP_theme_id = cart->intForKey("p_theme_id");
                    std::string strCover_image = cart->stringForKey("cover_image");
                
                    cartInfo->setCart_id(nCart_id);
                    cartInfo->setSmart_album_id(nSmart_album_id);
                    cartInfo->setCnt(nCnt);
                    cartInfo->setTitle(strTitle);
                    cartInfo->setTotal_page(nTotal_page);
                    cartInfo->setP_theme_id(nP_theme_id);
                    cartInfo->setCover_image(strCover_image);
                  
                    PixArray *item_list = cart->arrayForKey("item");
                    
                    if(item_list)
                    {
                        for (int j = 0; j < item_list->count(); ++j)
                        {
                            PixDictionary *item = item_list->getDictionaryAtIndex(j);
                            
                            if(item)
                            {
                                CartItem *cartItem = new CartItem();
                                
                                int nItem_id = item->intForKey("item_id");
                                std::string strCover = item->stringForKey("cover");
                                std::string strItem_display_f = item->stringForKey("item_display_f");
                                std::string strItem_display_s = item->stringForKey("item_display_s");
                                int nItem_price = item->intForKey("item_price");
                                int nItem_pay_price = item->intForKey("item_pay_price");
                                int nItem_basic_page = item->intForKey("item_basic_page");
                                int nItem_add_page = item->intForKey("item_add_page");;
                                int nItem_basic_price = item->intForKey("item_basic_price");
                                int nItem_basic_event_price = item->intForKey("item_basic_event_price");
                                int nItem_add_price = item->intForKey("item_add_price");
                                
                                cartItem->setItem_id(nItem_id);
                                cartItem->setCover(strCover);
                                cartItem->setItem_display_f(strItem_display_f);
                                cartItem->setItem_display_s(strItem_display_s);
                                cartItem->setItem_price(nItem_price);
                                cartItem->setItem_pay_price(nItem_pay_price);
                                cartItem->setItem_basic_page(nItem_basic_page);
                                cartItem->setItem_add_page(nItem_add_page);
                                cartItem->setItem_basic_price(nItem_basic_price);
                                cartItem->setItem_basic_event_price(nItem_basic_event_price);
                                cartItem->setItem_add_price(nItem_add_price);
                                
                                cartInfo->pushCartItem(cartItem);
                            }
                        }
                    }
                    
                    // 첫번째 아이템 선택 상태로 초기화
                    cartInfo->setSelectedCartItem(0);
                    
                    cartVec.push_back(cartInfo);
                }
            }
            
            PixDictionary *delivery = root->dictionaryForKey("delivery");
            
            if(delivery)
            {
                mn_free_item_price = delivery->intForKey("free_item_price");
                mn_price = delivery->intForKey("price");
            }
            
            cartTotalInfo = new CartOrderTotalInfo();
            
            cartTotalInfo->setTotal_price(0);
            cartTotalInfo->setTotal_pay_price(0);
            cartTotalInfo->setTotal_discount_price(0);
            cartTotalInfo->setShip_price(0);
        }
        
        createCartListView();
    }
}

void PixMenuCartLayer::getPayStop01()
{
    if(cartListView == nullptr)
        return;
    
    if(cartListView->getItems().empty())
        return;
    
    std::string strCart_id;
    std::string strSvalue;
    
    Vector<ui::Widget*>& items = cartListView->getItems();
    
    ssize_t nCount = items.size();
    
    for (ssize_t i = 0; i < nCount; ++i)
    {
        PixCartCell *cartCell = dynamic_cast<PixCartCell*>(items.at(i));
        
        if(cartCell)
        {
            if(cartCell->isCheckSelected())
            {
                CartInfo* cartInfo = cartCell->getCartInfo();
                strCart_id.append(StringUtils::format("%d",cartInfo->getCart_id()));
                strCart_id.append(",");
                
                CartItem *cartItem = cartInfo->getSelectedCartItem();
                
                if(cartItem)
                {
                    strSvalue.append(StringUtils::format("%d",cartInfo->getCart_id()));
                    strSvalue.append(",");
                    strSvalue.append(StringUtils::format("%d",cartItem->getItem_id()));
                    strSvalue.append("^");
                }
            }
        }
    }
    
    if(strCart_id.empty())
        return;
    
    if(strSvalue.empty())
        return;
    
    strCart_id = strCart_id.substr(0, strCart_id.find_last_of(","));
    strSvalue = strSvalue.substr(0, strSvalue.find_last_of("^"));
    
    PIXAPP->showLoading();
    
//    member_id	사용자 uniq ID
//    vendor	밴더명
//    cart_id	"사용자가 선택한 장바구니 ID (,로 연결)
//    만약 전체 데이터가 넘어올경우 null로 넘겨도 된다.(키값은 세팅)"
//    svalue	사용자가 선택한 아이템 정보 배열 (cart_id,item_id^.....)
    
    std::string url = SREVERURL_MANAGER->getUrl("PAY_STEP01");
    std::map<std::string, std::string> postParams;
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["cart_id"] = strCart_id;
    postParams["svalue"] = strSvalue;
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "PAY_STEP01", this, httpresponse_selector(PixMenuCartLayer::getPayStop01Callback), nullptr);
}

void PixMenuCartLayer::getPayStop01Callback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    std::vector<char>* data = response->getResponseData();
    std::string strResponseData(data->begin(), data->end());
    
    strResponseData = PixStringUtils::replaceAll(strResponseData,".","+");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"-","=");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"_","/");
    strResponseData = PixNativeUtils::nativeCallAES256Decrypt(strResponseData);
    
    rapidjson::Document document;
    document.Parse<0>(strResponseData.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        PixDictionary *root = pixHttpClient->getDictionary(document);
        
        if(root)
        {
            clearOrderListView();
            
            PixArray *cart_list = root->arrayForKey("cart_list");
            
            if(cart_list)
            {
                for (int i = 0; i < cart_list->count(); ++i)
                {
                    PixDictionary *cart = cart_list->getDictionaryAtIndex(i);
                    
                    if(cart)
                    {
                        CartInfo *cartInfo = new CartInfo();
                        
                        int nCart_id = cart->intForKey("cart_id");
                        int nItem_id = cart->intForKey("item_id");
                        int nSmart_album_id = cart->intForKey("smart_album_id");
                        std::string strTitle = cart->stringForKey("title");
                        int nTotal_page = cart->intForKey("total_page");
                        std::string strItem_display_f = cart->stringForKey("item_display_f");
                        std::string strItem_display_s = cart->stringForKey("item_display_s");
                        int nCnt = cart->intForKey("cnt");
                        std::string strCover_image = cart->stringForKey("cover_image");
                        int nItem_each_price = cart->intForKey("item_each_price");
                        int nItem_price = cart->intForKey("item_price");
                        int nItem_pay_price = cart->intForKey("item_pay_price");
                        std::string strThumb_name = cart->stringForKey("thumb_name");
                        std::string strThumb_path = cart->stringForKey("thumb_path");
                        int nItem_basic_price = cart->intForKey("item_basic_price");
                        int nItem_add_price = cart->intForKey("item_add_price");
                        int nItem_add_page = cart->intForKey("item_add_page");
                        
                        cartInfo->setCart_id(nCart_id);
                        cartInfo->setSmart_album_id(nSmart_album_id);
                        cartInfo->setTitle(strTitle);
                        cartInfo->setTotal_page(nTotal_page);
                        cartInfo->setCnt(nCnt);
                        cartInfo->setCover_image(strCover_image);
                        cartInfo->setThumb_name(strThumb_name);
                        cartInfo->setThumb_path(strThumb_path);

                        // getCart 와 자료구조를 동일하게 가져가기위해 선택된 아이템으로 CartItem 생성
                        
                        CartItem *cartItem = new CartItem();
                        
                        cartItem->setItem_id(nItem_id);
                        cartItem->setSelected(true);
                        cartItem->setItem_display_f(strItem_display_f);
                        cartItem->setItem_display_s(strItem_display_s);
                        cartItem->setItem_each_price(nItem_each_price);
                        cartItem->setItem_price(nItem_price);
                        cartItem->setItem_pay_price(nItem_pay_price);
                        cartItem->setItem_basic_price(nItem_basic_price);
                        cartItem->setItem_add_price(nItem_add_price);
                        cartItem->setItem_add_page(nItem_add_page);
                        
                        cartInfo->pushCartItem(cartItem);
                        
                        orderVec.push_back(cartInfo);
                    }
                }
            }
            
            PixDictionary *data = root->dictionaryForKey("data");
            
            if(data)
            {
                orderTotalInfo = new CartOrderTotalInfo();
                
                orderTotalInfo->setTotal_price(data->intForKey("total_price"));
                orderTotalInfo->setTotal_pay_price(data->intForKey("total_pay_price"));
                orderTotalInfo->setTotal_discount_price(data->intForKey("total_discount_price"));
                orderTotalInfo->setShip_price(data->intForKey("ship_price"));
            }
            
            createOrderListView();
            showOrderListViewAction();
        }
    }
}

void PixMenuCartLayer::getAddre()
{
    PIXAPP->showLoading();

//    member_id	사용자 uniq ID	필수
//    vendor	밴더명	필수
    
    std::string url = SREVERURL_MANAGER->getUrl("GET_ADDR");
    std::map<std::string, std::string> postParams;
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "GET_ADDR", this, httpresponse_selector(PixMenuCartLayer::getAddreCallback), nullptr);
}

void PixMenuCartLayer::getAddreCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        clearPaymentListView();
        
        PixArray *addr_list = root->arrayForKey("addr");
        
        if(addr_list)
        {
            for (int i = 0; i < addr_list->count(); ++i)
            {
                PixDictionary *addr = addr_list->getDictionaryAtIndex(i);
                
                if(addr)
                {
                    DeliveryInfo *deliveryInfo = new DeliveryInfo();
                    
                    int nMember_addr_id = addr->intForKey("member_addr_id");
                    std::string strName = addr->stringForKey("name");
                    std::string strPost = addr->stringForKey("post");
                    std::string strAddr1 = addr->stringForKey("addr1");
                    std::string strAddr2 = addr->stringForKey("addr2");
                    std::string strPhone1 = addr->stringForKey("phone1");
                    std::string strPhone2 = addr->stringForKey("phone2");
                    bool blnLast = addr->intForKey("last") == 1 ? true : false;
                    int nBackwoods_list_id = addr->intForKey("backwoods_list_id");
                    int nPay_add_ship_amount(addr->intForKey("pay_add_ship_amount"));
                    
                    deliveryInfo->setMember_addr_id(nMember_addr_id);
                    deliveryInfo->setName(strName);
                    deliveryInfo->setPost(strPost);
                    deliveryInfo->setAddr1(strAddr1);
                    deliveryInfo->setAddr2(strAddr2);
                    deliveryInfo->setPhone1(strPhone1);
                    deliveryInfo->setPhone2(strPhone2);
                    deliveryInfo->setLast(blnLast);
                    deliveryInfo->setBackwoods_list_id(nBackwoods_list_id);
                    deliveryInfo->setPay_add_ship_amount(nPay_add_ship_amount);
                    
                    deliveryVec->push_back(deliveryInfo);
                }
            }
        }
        
        
        
        createPaymentListView();
        showPaymentListViewAction();
    }
}

void PixMenuCartLayer::getPayStop02()
{
    if(checkPurchase() == false)
        return;
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("PAY_STEP02");
    std::map<std::string, std::string> postParams;
    
    std::string postData = getPayStop02JsonString();
    
    CCLOG("simsimsim postData : %s", postData.c_str());
    
    postData = PixNativeUtils::nativeCallAES256Encrypt(postData);
    postData = PixStringUtils::replaceAll(postData,"+",".");
    postData = PixStringUtils::replaceAll(postData,"=","-");
    postData = PixStringUtils::replaceAll(postData,"/","_");
    
    postParams.clear();
    postParams["u"] = postData;
    
    pixHttpClient->getFormData(url, postParams, "PAY_STEP02", this, httpresponse_selector(PixMenuCartLayer::getPayStop02Callback), nullptr);
}

void PixMenuCartLayer::getPayStop02Callback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    std::vector<char>* data = response->getResponseData();
    std::string strResponseData(data->begin(), data->end());
    
    strResponseData = PixStringUtils::replaceAll(strResponseData,".","+");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"-","=");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"_","/");
    strResponseData = PixNativeUtils::nativeCallAES256Decrypt(strResponseData);
    
    rapidjson::Document document;
    document.Parse<0>(strResponseData.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        PixDictionary *root = pixHttpClient->getDictionary(document);
        
        if(root)
        {
            int nOrder_id = root->intForKey("order_id");
            purchaseCall(nOrder_id);
        }
    }
}

void PixMenuCartLayer::payDoneCall(const std::string& imp_uid, const std::string& merchant_uid, const std::string& imp_success)
{
    if(orderVec.empty())
        return;
 
    PIXAPP->showLoading();
    
//    imp_uid	아임포트에서 부여하는 거래건 당 고유한 번호	필수
//    merchant_uid	우리가 부여한 주문번호	필수
//    imp_success	결제처리가 성공여부	필수
//    cart_id	연결된 장바구니 ID (쉼표로 연결)	필수
//    member_id	회원 SEQ	필수
//    vendor	벤더명	필수
    
    std::string url = SREVERURL_MANAGER->getUrl("PAY_DONE");
    std::map<std::string, std::string> postParams;
    
    std::string strCart_id;
    strCart_id.clear();
    
    for(auto iter = orderVec.begin(); iter != orderVec.end(); ++iter)
    {
        strCart_id.append(StringUtils::format("%d",(*iter)->getCart_id()));
        strCart_id.append(",");
    }
    
    if(strCart_id.empty())
        return;
    
    strCart_id = strCart_id.substr(0, strCart_id.find_last_of(","));
    
    postParams["imp_uid"] = imp_uid;
    postParams["merchant_uid"] = merchant_uid;
    postParams["imp_success"] = imp_success;
    postParams["cart_id"] = strCart_id;
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "PAY_DONE", this, httpresponse_selector(PixMenuCartLayer::payDoneCallback), nullptr);
}

void PixMenuCartLayer::payDoneCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();

    std::vector<char>* data = response->getResponseData();
    std::string strResponseData(data->begin(), data->end());
    
    strResponseData = PixStringUtils::replaceAll(strResponseData,".","+");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"-","=");
    strResponseData = PixStringUtils::replaceAll(strResponseData,"_","/");
    strResponseData = PixNativeUtils::nativeCallAES256Decrypt(strResponseData);
    
    rapidjson::Document document;
    document.Parse<0>(strResponseData.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        PixDictionary *root = pixHttpClient->getDictionary(document);
        
        if(root)
        {
            std::string strFail_reason = root->stringForKey("fail_reason");
            
            if(!strFail_reason.empty())
            {
                PixPurchaseFailedLayer *purchaseFailedLayer = PixPurchaseFailedLayer::create(strFail_reason);
                Director::getInstance()->getRunningScene()->addChild(purchaseFailedLayer);
                return;
            }
            
            int nAmount = root->intForKey("amount");
            std::string strPay_method = root->stringForKey("pay_method");
            std::string strPay_method_text = root->stringForKey("pay_method_text");
            std::string strVbank_name = root->stringForKey("vbank_name");
            std::string strVbank_num = root->stringForKey("vbank_num");
            std::string strVbank_holder = root->stringForKey("vbank_holder");
            std::string strVbank_date = root->stringForKey("vbank_date");
            
            std::string strOrder_id = root->stringForKey("order_id");
            std::string strReg_date = root->stringForKey("reg_date");
            
            int nCartCount = root->intForKey("cartCount");
            PIXAPP->userInfo->setCartCount(nCartCount);
            
            for(auto iter = orderVec.begin(); iter != orderVec.end(); ++iter)
            {
                MyAlbumModel *pAlbumModel = MYALBUM_MANAGER->getMyAlbumModelByCartID((*iter)->getCart_id());
                
                if(pAlbumModel)
                {
                    pAlbumModel->setCart_id(0);
                }
            }

            createOrderCompletedListView(nAmount, strPay_method, strPay_method_text, strVbank_name, strVbank_num, strVbank_holder, strVbank_date, strOrder_id, strReg_date);
            showOrderCompletedListViewAction();
        }
    }
}

void PixMenuCartLayer::purchaseCall(int nOrder_id)
{
//    member_id	사용자 uniq ID	필수
//    vendor	밴더명	필수
//    order_id	주문번호	필수
    
    std::string url = SREVERURL_MANAGER->getUrl("PURCHASE");
    std::map<std::string, std::string> postParams;
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["order_id"] = StringUtils::format("%d",nOrder_id);
    
    std::ostringstream postBuf;
    pixHttpClient->getPostData(postBuf, postParams);
    std::string postData = postBuf.str();
    
    postData = PixNativeUtils::nativeCallAES256Encrypt(postData);
    postData = PixStringUtils::replaceAll(postData,"+",".");
    postData = PixStringUtils::replaceAll(postData,"=","-");
    postData = PixStringUtils::replaceAll(postData,"/","_");
    
    url.append(postData);
    
    PixPurchaseLayer *purchaseLayer = PixPurchaseLayer::create(url);
    purchaseLayer->setDelegate(this);
    Director::getInstance()->getRunningScene()->addChild(purchaseLayer);
}

std::string PixMenuCartLayer::getPayStop02JsonString()
{
    std::string strJsonString;
    strJsonString.clear();
    
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    
    document.AddMember("member_id", rapidjson::Value(PIXAPP->userInfo->getMember_id().c_str(),allocator), allocator);
    document.AddMember("vendor", rapidjson::Value(PIXAPP->userInfo->getVendor().c_str(),allocator), allocator);
    
    if(payPaymentMethodCell)
    {
         document.AddMember("pay_method", rapidjson::Value(payPaymentMethodCell->getPay_method(),allocator), allocator);
    }
    
    if(finalTotalInfo)
    {
        int nPay_coupon_amount = 0;
        int nPay_event_amount = 0;
        int nItem_cnt = 0;
        int nTotal_price = 0;
        int nTotal_discount_price = 0;
        int nShip_price = 0;
        
        if(!orderListView->getItems().empty())
        {
            for (auto item : orderListView->getItems())
            {
                PixPaymentCell *child = dynamic_cast<PixPaymentCell*>(item);
                
                if(child)
                {
                    CartInfo *cartInfo = child->getCartInfo();
                    
                    if(cartInfo)
                    {
                        CouponInfo *couponInfo = cartInfo->getCouponInfo();
                        
                        if(couponInfo)
                        {
                            nPay_coupon_amount += couponInfo->getCoupon_price();
                        }
                        else
                        {
                            CartItem *cartItem = cartInfo->getSelectedCartItem();
                            
                            if(cartItem)
                            {
                                nTotal_price += cartItem->getItem_price();
                                
                                nPay_event_amount += (cartItem->getItem_price() - cartItem->getItem_pay_price());
                                
                                CouponInfo *couponInfo = cartInfo->getCouponInfo();
                                
                                if(couponInfo)
                                {
                                    nTotal_discount_price += couponInfo->getCoupon_price();
                                }
                                else
                                {
                                    nTotal_discount_price += (cartItem->getItem_price() - cartItem->getItem_pay_price());
                                }
                            }
                        }
                    }
                    
                    ++nItem_cnt;
                }
            }
        }
        
        int nMember_addr_id = -1;
        int nBackwoods_list_id = 0;
        int nPay_add_ship_amount = 0;
        std::string strPost = "";
        std::string strAddr1 = "";
        std::string strAddr2 = "";
        std::string strPhone1 = "";
        std::string strPhone2 = "";
        std::string strMemo = "";
        std::string strName = "";
        
        if(payDeliveryInfoCell)
        {
            DELIVERY_TYPE eTYpe = payDeliveryInfoCell->getDeliveryType();
            
            if(eTYpe == CARTLAYER_LIST)
            {
                DeliveryInfo *pInfo = nullptr;
                
                for (auto iter = deliveryVec->begin(); iter != deliveryVec->end(); ++iter)
                {
                    if((*iter)->getSelected())
                    {
                        pInfo = (*iter);
                        break;
                    }
                }
                
                if(pInfo)
                {
                    nMember_addr_id = pInfo->getMember_addr_id();
                    nBackwoods_list_id = pInfo->getBackwoods_list_id();
                    strPost = pInfo->getPost();
                    strAddr1 = pInfo->getAddr1();
                    strAddr2 = pInfo->getAddr2();
                    strPhone1 = pInfo->getPhone1();
                    strPhone2 = pInfo->getPhone2();
                    strName = pInfo->getName();
                    
                    if(payDeliveryListCell)
                    {
                        strMemo = payDeliveryListCell->getMemo();
                    }
                }
            }
            else if(eTYpe == CARTLAYER_NEW || eTYpe == CARTLAYER_ONLY_NEW)
            {
                if(payNewDeliveryCell)
                {
                    nMember_addr_id = -1;
                    nBackwoods_list_id = payNewDeliveryCell->getBackwoods_list_id();
                    strPost = payNewDeliveryCell->getZipCode();
                    strAddr1 = payNewDeliveryCell->getAddress();
                    strAddr2 = payNewDeliveryCell->getDetailedAddress();
                    strPhone1 = payNewDeliveryCell->getPhoneNumber();
                    strPhone2 = "";
                    strMemo = payNewDeliveryCell->getMemo();
                    strName = payNewDeliveryCell->getName();
                }
            }
        }
        
        DeliveryInfo *pInfo = nullptr;
        
        if(payDeliveryInfoCell)
        {
            DELIVERY_TYPE eTYpe = payDeliveryInfoCell->getDeliveryType();
            
            if(eTYpe == CARTLAYER_LIST)
            {
                for (auto iter = deliveryVec->begin(); iter != deliveryVec->end(); ++iter)
                {
                    if((*iter)->getSelected())
                    {
                        pInfo = (*iter);
                        break;
                    }
                }
            }
            else if(eTYpe == CARTLAYER_NEW || eTYpe == CARTLAYER_ONLY_NEW)
            {
                if(payNewDeliveryCell)
                {
                    nPay_add_ship_amount = payNewDeliveryCell->getPay_add_ship_amount();
                }
            }
        }
        
        if(pInfo)
        {
            nPay_add_ship_amount = pInfo->getPay_add_ship_amount();
        }
        
        if((nTotal_price - nTotal_discount_price) < mn_free_item_price)
        {
            nShip_price = mn_price;
        }
        else
        {
            nShip_price = 0;
            nPay_add_ship_amount = 0;
        }
            
        document.AddMember("amount", finalTotalInfo->getTotal_price(), allocator);
        document.AddMember("pay_amount", finalTotalInfo->getTotal_pay_price(), allocator);
        document.AddMember("pay_event_amount", nPay_event_amount, allocator);
        document.AddMember("pay_coupon_amount", nPay_coupon_amount, allocator);
        document.AddMember("pay_ship_amount", nShip_price, allocator);
        document.AddMember("pay_add_ship_amount", nPay_add_ship_amount, allocator);
        document.AddMember("item_cnt", nItem_cnt, allocator);
        
        document.AddMember("member_addr_id", nMember_addr_id, allocator);
        
        if(nBackwoods_list_id == 0)
        {
            document.AddMember("backwoods_list_id", rapidjson::Value("null",allocator), allocator);
        }
        else
        {
            document.AddMember("backwoods_list_id", nBackwoods_list_id, allocator);
        }
       
        document.AddMember("name", rapidjson::Value(strName.c_str(),allocator), allocator);
        document.AddMember("email", rapidjson::Value(PIXAPP->userInfo->getEmail().c_str(),allocator), allocator);
        document.AddMember("post", rapidjson::Value(strPost.c_str(),allocator), allocator);
        document.AddMember("addr1", rapidjson::Value(strAddr1.c_str(),allocator), allocator);
        document.AddMember("addr2", rapidjson::Value(strAddr2.c_str(),allocator), allocator);
        document.AddMember("phone1", rapidjson::Value(strPhone1.c_str(),allocator), allocator);
        document.AddMember("phone2", rapidjson::Value(strPhone2.c_str(),allocator), allocator);
        document.AddMember("memo", rapidjson::Value(strMemo.c_str(),allocator), allocator);
        document.AddMember("click_date", rapidjson::Value(getCurrentDate().c_str(),allocator), allocator);
        
        rapidjson::Value carts(rapidjson::kArrayType);
        
        if(!orderListView->getItems().empty())
        {
            for (auto item : orderListView->getItems())
            {
                PixPaymentCell *child = dynamic_cast<PixPaymentCell*>(item);
                
                if(child)
                {
                    CartInfo *cartInfo = child->getCartInfo();
                    
                    if(cartInfo)
                    {
                        rapidjson::Value cart(rapidjson::kObjectType);
                        
                        int nPay_event_amount = 0;
                        int nPay_coupon_amount = 0;
                        int nTotal_pay_price = 0;
                        int nCoupon_group_id = 0;
                        std::string strCoupon_id = "";
                        int nTotal_price = 0;
                        int nItem_id = 0;
                        std::string strItem_display_f = "";
                        int nAmount = 0;
                        
                        CartItem *cartItem = cartInfo->getSelectedCartItem();
                        
                        if(cartItem)
                        {
                            nTotal_price = cartItem->getItem_price();
                            nItem_id = cartItem->getItem_id();
                            strItem_display_f = cartItem->getItem_display_f();
                            nAmount = cartItem->getItem_price();
                            
                            CouponInfo *couponInfo = cartInfo->getCouponInfo();
                            
                            if(couponInfo)
                            {
                                nPay_event_amount = 0;
                                nPay_coupon_amount = couponInfo->getCoupon_price();
                                nCoupon_group_id = couponInfo->getCoupon_group_id();
                                strCoupon_id = couponInfo->getCoupon_id();
                                nTotal_pay_price = nTotal_price - nPay_coupon_amount;
                            }
                            else
                            {
                                nPay_event_amount = (cartItem->getItem_price() - cartItem->getItem_pay_price());
                                nPay_coupon_amount = 0;
                                nCoupon_group_id = 0;
                                strCoupon_id = "";
                                nTotal_pay_price = nTotal_price - nPay_event_amount;
                            }
                        }
                        
                        cart.AddMember("smart_album_id", cartInfo->getSmart_album_id(), allocator);
                        cart.AddMember("title", rapidjson::Value(cartInfo->getTitle().c_str(),allocator), allocator);
                        cart.AddMember("item_id", nItem_id, allocator);
                        cart.AddMember("item_name", rapidjson::Value(strItem_display_f.c_str(),allocator), allocator);
                        cart.AddMember("total_page", cartInfo->getTotal_page(), allocator);
                        cart.AddMember("item_cnt", cartInfo->getCnt(), allocator);
                        cart.AddMember("amount", nAmount, allocator);
                        cart.AddMember("pay_amount", nTotal_pay_price, allocator);
                        cart.AddMember("pay_event_amount", nPay_event_amount, allocator);
                        cart.AddMember("pay_coupon_amount", nPay_coupon_amount, allocator);
                        
                        if(nCoupon_group_id == 0)
                        {
                            cart.AddMember("coupon_group_id", rapidjson::Value("",allocator), allocator);
                        }
                        else
                        {
                            cart.AddMember("coupon_group_id", nCoupon_group_id, allocator);
                        }
                        
                        cart.AddMember("coupon_id", rapidjson::Value(strCoupon_id.c_str(),allocator), allocator);
                        cart.AddMember("thumb_path", rapidjson::Value(cartInfo->getThumb_path().c_str(),allocator), allocator);
                        cart.AddMember("thumb_name", rapidjson::Value(cartInfo->getThumb_name().c_str(),allocator), allocator);
                        
                        carts.PushBack(cart, allocator);
                    }
                }
            }
        }
        
        document.AddMember("item", carts, allocator);
    }
    
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    document.Accept(writer);
    
    strJsonString = strbuf.GetString();
    
    return strJsonString;
}

#pragma mark - Android BackPressed

void PixMenuCartLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(me_Type == CARTLAYER_CART || me_Type == CARTLAYER_ORDER_COMPLETED)
        {
            if(getAndroidBackPressedAction() == false)
            {
                setAndroidBackPressedAction(true);
                hideMenuCartLayerAction();
            }
            
            return;
        }
        else if(me_Type == CARTLAYER_ORDER)
        {
            hideOrderListViewAction();
            return;
        }
        else if(me_Type == CARTLAYER_PAYMENT)
        {
            hidePaymentListViewAction();
            return;
        }
    }
}
