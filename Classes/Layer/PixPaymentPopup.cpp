//
//  PixPaymentPopup.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 15..
//
//

#include "PixPaymentPopup.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Scene/PixApp.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"
#include "../Info/CartInfo.h"
#include "../Info/CouponInfo.h"

PixPaymentPopup* PixPaymentPopup::create(CartInfo *pCartInfo, const std::string &strParamCoupon_group_id, const std::string &strParamCoupon_id)
{
    PixPaymentPopup *paymentPopup = new (std::nothrow) PixPaymentPopup(pCartInfo, strParamCoupon_group_id, strParamCoupon_id);
    if (paymentPopup && paymentPopup->init())
    {
        paymentPopup->autorelease();
        return paymentPopup;
    }
    else
    {
        CC_SAFE_DELETE(paymentPopup);
        return nullptr;
    }
}
    
PixPaymentPopup::PixPaymentPopup(CartInfo *pCartInfo, const std::string &strParamCoupon_group_id, const std::string &strParamCoupon_id)
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MenuPaymen_Pop(nullptr)
, Button_complete(nullptr)
, paymentPopupListView(nullptr)
, couponPopupEventDiscountCell(nullptr)
, mCartInfo(pCartInfo)
, mstr_ParamCoupon_group_id(strParamCoupon_group_id)
, mstr_ParamCoupon_id(strParamCoupon_id)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixPaymentPopup::hideLayerAction,this);
}

PixPaymentPopup::~PixPaymentPopup()
{
    pixHttpClient->cancelRequest(this);
    
    for (auto iter = couponVec.begin(); iter != couponVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
}
    
void PixPaymentPopup::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuPaymen_PopLayer.csb");
    
    if(rootNode)
    {
        MenuPaymen_Pop = dynamic_cast<Node*>(rootNode->getChildByName("MenuPaymen_Pop"));
        
        if(MenuPaymen_Pop)
        {
            Node *MenuPaymen_popup_Top = dynamic_cast<Node*>(MenuPaymen_Pop->getChildByName("MenuPaymen_popup_Top"));
            
            if(MenuPaymen_popup_Top)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(MenuPaymen_popup_Top->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *Button_cancel = dynamic_cast<ui::Button*>(MenuPaymen_popup_Top->getChildByName("Button_cancel"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixPaymentPopup::ButtonCancelPressed, this));
                }
            }
            
            ui::ListView *listView = dynamic_cast<ui::ListView*>(MenuPaymen_Pop->getChildByName("ListView"));
            
            if(listView)
            {
                paymentPopupListView = PixGalleryListView::create();
                paymentPopupListView->copyClonedListView(listView);
            }
            
            Button_complete = dynamic_cast<ui::Button*>(MenuPaymen_Pop->getChildByName("Button_Complete"));
            
            if(Button_complete)
            {
                Button_complete->addTouchEventListener(CC_CALLBACK_2(PixPaymentPopup::ButtonCompletePressed, this));
                
                Button_complete->setEnabled(true);
                
                ui::Text *Text = dynamic_cast<ui::Text*>(Button_complete->getChildByName("Text"));
                
                if(Text)
                {
                    Text->setString(CODE_NAME(Text->getString()));
                }
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixPaymentPopup::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    getCouponCall();
}

void PixPaymentPopup::update(float delta)
{
    PixModalLayer::update(delta);
    
    if(paymentPopupListView)
    {
        bool blnSelected = false;
        
        for (auto item : paymentPopupListView->getItems())
        {
            PixCouponPopupItemCell *couponPopupItemCell = dynamic_cast<PixCouponPopupItemCell*>(item);
            
            if(couponPopupItemCell)
            {
                if(couponPopupItemCell->isCheckSelected() == true)
                {
                    blnSelected = true;
                    break;
                }
            }
        }
        
        if(blnSelected == false)
        {
            if(couponPopupEventDiscountCell)
            {
                couponPopupEventDiscountCell->setCheckSelected(true);
                couponPopupEventDiscountCell->setCheckEnabled(false);
            }
        }
    }
}

void PixPaymentPopup::createPaymentPopupListView()
{
    if(paymentPopupListView == nullptr)
        return;
    
    if(mCartInfo == nullptr)
        return;
    
    int nEventDiscount = 0;
    
    CartItem *cartItem = mCartInfo->getSelectedCartItem();
    
    if(cartItem)
    {
        nEventDiscount = cartItem->getItem_price() - cartItem->getItem_pay_price();
    }
    
    couponPopupEventDiscountCell = PixCouponPopupEventDiscountCell::create(nEventDiscount, !couponVec.empty());
    couponPopupEventDiscountCell->setDelegate(this);
    paymentPopupListView->pushBackCustomItem(couponPopupEventDiscountCell);
    
    PixCouponPopupTitleCell *couponPopupTitleCell = PixCouponPopupTitleCell::create((int)couponVec.size());
    paymentPopupListView->pushBackCustomItem(couponPopupTitleCell);
    
    if(couponVec.empty())
    {
        PixCouponPopupEmptyCell *couponPopupEmptyCell = PixCouponPopupEmptyCell::create();
        paymentPopupListView->pushBackCustomItem(couponPopupEmptyCell);
    }
    else
    {
        for(ssize_t i = 0; i < couponVec.size(); ++i)
        {
            CouponInfo *couponInfo = couponVec.at(i);
            
            PixCouponPopupItemCell *couponPopupItemCell = PixCouponPopupItemCell::create(couponInfo);
            couponPopupItemCell->setDelegate(this);
            
            if(mCartInfo->getCouponInfo())
            {
                int nCoupon_group_id = mCartInfo->getCouponInfo()->getCoupon_group_id();
                std::string strCoupon_id = mCartInfo->getCouponInfo()->getCoupon_id();
                
                if(couponInfo->getCoupon_group_id() == nCoupon_group_id)
                {
                    if(couponInfo->getCoupon_id().compare(strCoupon_id) == 0)
                    {
                        couponPopupItemCell->setCheckSelected(true);
                        
                        if(couponPopupEventDiscountCell)
                        {
                            couponPopupEventDiscountCell->setCheckSelected(false);
                            couponPopupEventDiscountCell->setCheckEnabled(true);
                        }
                    }
                }
            }
            
            paymentPopupListView->pushBackCustomItem(couponPopupItemCell);
        }
    }
    
    PixCouponPopupBottomCell *couponPopupBottomCell = PixCouponPopupBottomCell::create();
    paymentPopupListView->pushBackCustomItem(couponPopupBottomCell);
}

void PixPaymentPopup::hideLayerAction()
{
    this->removeFromParentAndCleanup(true);
}

#pragma mark - Button

void PixPaymentPopup::ButtonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixPaymentPopup::ButtonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        
        if(couponPopupEventDiscountCell)
        {
            if(couponPopupEventDiscountCell->isCheckSelected())
            {
                if(mCartInfo)
                {
                    mCartInfo->deldteCouponInfo();
                }
            }
            else
            {
                for (auto item : paymentPopupListView->getItems())
                {
                    PixCouponPopupItemCell *couponPopupItemCell = dynamic_cast<PixCouponPopupItemCell*>(item);
                    
                    if(couponPopupItemCell)
                    {
                        if(couponPopupItemCell->isCheckSelected() == true)
                        {
                            if(mCartInfo)
                            {
                                CouponInfo *pCouponInfo = couponPopupItemCell->getCouponInfo();
                                
                                if(pCouponInfo)
                                {
                                    mCartInfo->deldteCouponInfo();
                                    mCartInfo->setCouponInfo(pCouponInfo->clone());
                                }
                            }
                            
                            break;
                        }
                    }
                }
            }
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Delegate

void PixPaymentPopup::couponPopupItemCellCheckBoxPressed(cocos2d::Ref* pWidget, bool var)
{
    if(pWidget == nullptr)
        return;
 
    PixCouponPopupItemCell *pCell = dynamic_cast<PixCouponPopupItemCell*>(pWidget);
    
    if(pCell)
    {
        if(var)
        {
            for (auto item : paymentPopupListView->getItems())
            {
                PixCouponPopupItemCell *couponPopupItemCell = dynamic_cast<PixCouponPopupItemCell*>(item);
                
                if(couponPopupItemCell)
                {
                    if(pCell != couponPopupItemCell)
                    {
                        couponPopupItemCell->setCheckSelected(false);
                    }
                }
            }
            
            if(couponPopupEventDiscountCell)
            {
                couponPopupEventDiscountCell->setCheckSelected(false);
                couponPopupEventDiscountCell->setCheckEnabled(true);
            }
        }
    }
}

void PixPaymentPopup::couponPopupEventDiscountCellCheckBoxPressed(bool var)
{
    if(var)
    {
        for (auto item : paymentPopupListView->getItems())
        {
            PixCouponPopupItemCell *couponPopupItemCell = dynamic_cast<PixCouponPopupItemCell*>(item);
            
            if(couponPopupItemCell)
            {
                couponPopupItemCell->setCheckSelected(false);
            }
        }
    }
}

#pragma mark - Network

void PixPaymentPopup::getCouponCall()
{
    if(mCartInfo == nullptr)
        return;
    
    PIXAPP->showLoading();
    
    
    
    int nitem_price = 0;
    
    CartItem *cartItem = mCartInfo->getSelectedCartItem();
    
    if(cartItem)
    {
        nitem_price = cartItem->getItem_price();
    }
    
//    member_id	사용자 uniq ID
//    vendor	밴더명
//    cart_id	사용자가 선택한 장바구니 ID
//    item_pay_price	상품 결제 금액
//    coupon_group_id	다른 상품에 적용된 쿠폰 그룹 ID (  쉼표[,]로 연결, 없으면 null)
//    coupon_id	다른 상품에 적용된 쿠폰 ID ( 쉼표[,]로 연결, 없으면 null)
//    target	쿠폰 사용처 (1: 상품, 2: 배송비)
    
    std::string url = SREVERURL_MANAGER->getUrl("GET_COUPON");
    std::map<std::string, std::string> postParams;
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["cart_id"] = StringUtils::format("%d",mCartInfo->getCart_id());
    postParams["item_price"] = StringUtils::format("%d",nitem_price);

    if(!getParamCoupon_group_id().empty())
    {
        postParams["coupon_group_id"] = getParamCoupon_group_id();
    }

    if(!getParamCoupon_id().empty())
    {
        postParams["coupon_id"] = getParamCoupon_id();
    }
    
    postParams["target"] = "1";
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "GET_COUPON", this, httpresponse_selector(PixPaymentPopup::getCouponCallback), nullptr);
}

void PixPaymentPopup::getCouponCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
        PixDictionary *root = PixHttpClient::sharedObject()->getDictionary(document);
        
        if(root)
        {
            PixArray *coupon = root->arrayForKey("coupon");
            
            if(coupon)
            {
                for (int i = 0; i < coupon->count(); ++i)
                {
                    PixDictionary *dic = coupon->getDictionaryAtIndex(i);
                 
                    if(dic)
                    {
                        CouponInfo *couponInfo = new CouponInfo();
                        
                        couponInfo->setCoupon_group_id(dic->intForKey("coupon_group_id"));
                        couponInfo->setCoupon_id(dic->stringForKey("coupon_id"));
                        couponInfo->setSubject(dic->stringForKey("subject"));
                        couponInfo->setMethod(dic->intForKey("method"));
                        couponInfo->setCoupon_value(dic->intForKey("coupon_value"));
                        couponInfo->setCoupon_price(dic->intForKey("coupon_price"));
                        
                        couponVec.push_back(couponInfo);
                    }
                }
            }
            
            createPaymentPopupListView();
        }
    }
}

#pragma mark - Android BackPressed

void PixPaymentPopup::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    PIXAPP->hideLoading();
    
    setAndroidBackPressedAction(true);
    hideLayerAction();
}

