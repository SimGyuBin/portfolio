//
//  PixPaymentCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 14..
//
//

#include "PixPaymentCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/CartInfo.h"
#include "../Info/CartOrderTotalInfo.h"
#include "../Info/CouponInfo.h"
#include "../Manager/PixMessageManager.h"
#include "../Layer/PixAlbumPreviewLayer.h"
#include "../Util/PixStringUtils.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

PixPaymentCell* PixPaymentCell::create(CartInfo *pCartInfo, CartOrderTotalInfo *pCartOrderTotalInfo)
{
    PixPaymentCell* widget = new (std::nothrow) PixPaymentCell(pCartInfo, pCartOrderTotalInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPaymentCell::PixPaymentCell(CartInfo *pCartInfo, CartOrderTotalInfo *pCartOrderTotalInfo)
: pixHttpClient(nullptr)
, _customItem(nullptr)
, imageLoader(nullptr)
, coverImageData(nullptr)
, MenuCartBookCell(nullptr)
, BookTitle(nullptr)
, btn_set_coupon(nullptr)
, btn_del_coupon(nullptr)
, pay_1(nullptr)
, pay_2(nullptr)
, pay_3(nullptr)
, pay_4(nullptr)
, coverImageButton(nullptr)
, mstr_Cart07("")
, mstr_Cart10("")
, mCartInfo(pCartInfo)
, mCartOrderTotalInfo(pCartOrderTotalInfo)
, mbln_Isloading(nullptr)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    downloaderQueue.clear();
}

PixPaymentCell::~PixPaymentCell()
{
    pixHttpClient->cancelRequest(this);
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    CC_SAFE_DELETE(imageLoader);
    CC_SAFE_DELETE(coverImageData);
}

bool PixPaymentCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixPaymentCell::onEnter()
{
    Widget::onEnter();
    scheduleUpdate();
}

void PixPaymentCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixPaymentCell::onExit()
{
    unscheduleUpdate();
    Widget::onExit();
}
void PixPaymentCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(_isTransitionFinished)
    {
        if(mCartInfo)
        {
            if(visible)
            {
                if(getLoading() == false)
                {
                    setLoading(true);
                    
                    sendGetCoverImageUrl(mCartInfo->getCover_image());
                }
            }
            else
            {
                
            }
        }
    }
}

void PixPaymentCell::update(float dt)
{
    Widget::update(dt);
    updatePaymentUI();
}

void PixPaymentCell::createCustomItem()
{
    if(mCartInfo == nullptr)
        return;
    
    CartItem *cartItem = mCartInfo->getSelectedCartItem();
    
    if(cartItem == nullptr)
        return;
    
    _customItem = CSLoader::createNode("12_Oder/MenuPaymentItem_02.csb");
    
    if(_customItem)
    {
        mstr_Cart07 = CODE_NAME("Cart07");
        mstr_Cart10 = CODE_NAME("Cart10");
        
        MenuCartBookCell = dynamic_cast<Node*>(_customItem->getChildByName("MenuCartBookCell"));
        
        if(MenuCartBookCell)
        {
            std::string normalImg = "Common/BookCoverMask_200X200_1.png";
            std::string pressedImg = "Common/BookCoverMask_200X200_2.png";
            std::string disabledImg = "";
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg);
            menuItem->setCallback(CC_CALLBACK_1(PixPaymentCell::coverImagePressed, this));
            
            coverImageButton = PixScrollMenu::create(menuItem, nullptr);
            coverImageButton->setPosition(Vec2(MenuCartBookCell->getContentSize().width / 2, MenuCartBookCell->getContentSize().height / 2));
            coverImageButton->setAnchorPoint(Vec2::ZERO);
            coverImageButton->setContentSize(MenuCartBookCell->getContentSize());
            MenuCartBookCell->addChild(coverImageButton, 1000);
        }
        
        BookTitle = dynamic_cast<ui::Text*>(_customItem->getChildByName("BookTitle"));
        
        if(BookTitle)
        {
            BookTitle->setString(mCartInfo->getTitle());
        }
        
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("btn_set_coupon"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixPaymentCell::ButtonSetCouponPressed, this));
            
            btn_set_coupon = PixScrollMenu::create(menuItem, nullptr);
            btn_set_coupon->setPosition(pos);
            
            btn_set_coupon->setContentSize(button->getContentSize());
            _customItem->addChild(btn_set_coupon);
            
            ui::Text *text = dynamic_cast<ui::Text*>(button->getChildByName("coupon"));
            
            if(text)
            {
                text->setString(CODE_NAME(text->getString()));
                
                ui::Text *clone =  dynamic_cast<ui::Text*>(text->clone());
                
                if(clone)
                {
                    clone->setFontSize(text->getFontSize());
                    clone->setTextColor(text->getTextColor());
                    clone->setString(text->getString());
                    menuItem->addChild(clone);
                }
            }
            
            button->removeFromParentAndCleanup(true);
        }
        
        button = dynamic_cast<ui::Button*>(_customItem->getChildByName("btn_del_coupon"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixPaymentCell::ButtonDelCouponPressed, this));
            
            btn_del_coupon = PixScrollMenu::create(menuItem, nullptr);
            btn_del_coupon->setPosition(pos);
            
            btn_del_coupon->setContentSize(button->getContentSize());
            _customItem->addChild(btn_del_coupon);
            
            ui::Text *text = dynamic_cast<ui::Text*>(button->getChildByName("coupon"));
            
            if(text)
            {
                text->setString(CODE_NAME(text->getString()));
                
                ui::Text *clone =  dynamic_cast<ui::Text*>(text->clone());
                
                if(clone)
                {
                    clone->setFontSize(text->getFontSize());
                    clone->setTextColor(text->getTextColor());
                    clone->setString(text->getString());
                    menuItem->addChild(clone);
                }
            }
            
            button->removeFromParentAndCleanup(true);
        }
        
        ui::Text *Quantity = dynamic_cast<ui::Text*>(_customItem->getChildByName("Quantity"));
        
        if(Quantity)
        {
            Quantity->setString(CODE_NAME(Quantity->getString()));
        }

        ui::Text *Number = dynamic_cast<ui::Text*>(_customItem->getChildByName("Number"));
        
        if(Number)
        {
            Number->setString(StringUtils::format("%d",mCartInfo->getCnt()));
            
            if(Quantity)
            {
                Size size = Quantity->getContentSize();
                Vec2 pos = Quantity->getPosition();
                
                pos = Vec2((pos.x + size.width + 15.0f), pos.y);
                
                Number->setPosition(pos);
            }
        }
        
        ui::Text *Infor_01 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Infor_01"));
        
        if(Infor_01)
        {
            Infor_01->setString(cartItem->getItem_display_f());
        }
        
        ui::Text *Infor_02 = dynamic_cast<ui::Text*>(_customItem->getChildByName("Infor_02"));
        
        if(Infor_02)
        {
            Infor_02->setString(cartItem->getItem_display_s());
        }
        
        ui::Text *Item = dynamic_cast<ui::Text*>(_customItem->getChildByName("Item"));
        
        if(Item)
        {
            Item->setString(CODE_NAME(Item->getString()));
        }

        ui::Text *Discounted = dynamic_cast<ui::Text*>(_customItem->getChildByName("Discounted"));
        
        if(Discounted)
        {
            Discounted->setString(CODE_NAME(Discounted->getString()));
        }

        ui::Text *Total = dynamic_cast<ui::Text*>(_customItem->getChildByName("Total"));
        
        if(Total)
        {
            Total->setString(CODE_NAME(Total->getString()));
        }
        
        ui::Text *PagePrice = dynamic_cast<ui::Text*>(_customItem->getChildByName("PagePrice"));
        
        if(PagePrice)
        {
            std::string strText = CODE_NAME(PagePrice->getString());
            strText = StringUtils::format(strText.c_str(), cartItem->getItem_add_page(), cartItem->getItem_add_page() * 2);
            PagePrice->setString(strText);
        }
        
        pay_1 = dynamic_cast<ui::Text*>(_customItem->getChildByName("pay_1"));
        
        if(pay_1)
        {
            pay_1->setString(StringUtils::format(mstr_Cart07.c_str(),"0"));
        }
        
        pay_2 = dynamic_cast<ui::Text*>(_customItem->getChildByName("pay_2"));
        
        if(pay_2)
        {
            pay_2->setString(StringUtils::format(mstr_Cart10.c_str(),"0"));
        }
        
        pay_3 = dynamic_cast<ui::Text*>(_customItem->getChildByName("pay_3"));
        
        if(pay_3)
        {
            pay_3->setString(StringUtils::format(mstr_Cart07.c_str(),"0"));
        }
        
        pay_4 = dynamic_cast<ui::Text*>(_customItem->getChildByName("pay_4"));
        
        if(pay_4)
        {
            pay_4->setString(StringUtils::format(mstr_Cart07.c_str(),"0"));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixPaymentCell::updatePaymentUI()
{
    if(mCartInfo == nullptr)
        return;
    
    CartItem *cartItem = mCartInfo->getSelectedCartItem();
    
    if(cartItem == nullptr)
        return;
    
    if(mCartInfo->getCouponInfo() == nullptr)
    {
        if((cartItem->getItem_price() - cartItem->getItem_pay_price()) > 0)
        {
            // 이벤트 할인
            if(btn_set_coupon)
            {
                btn_set_coupon->setVisible(true);
            }
            
            if(btn_del_coupon)
            {
                btn_del_coupon->setVisible(false);
            }
        }
        else
        {
            if(btn_set_coupon)
            {
                btn_set_coupon->setVisible(true);
            }
            
            if(btn_del_coupon)
            {
                btn_del_coupon->setVisible(false);
            }
        }
    }
    else
    {
        // 쿠폰 적용 상태
        if(btn_set_coupon)
        {
            btn_set_coupon->setVisible(false);
        }
        
        if(btn_del_coupon)
        {
            btn_del_coupon->setVisible(true);
        }
    }
    
    if(pay_1)
    {
        std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_basic_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        pay_1->setString(strText);
    }
    
    if(pay_2)
    {
        int nMoney = 0;
        
        CouponInfo *couponInfo = mCartInfo->getCouponInfo();
        
        if(couponInfo)
        {
            nMoney = couponInfo->getCoupon_price();
        }
        else
        {
            nMoney = cartItem->getItem_price() - cartItem->getItem_pay_price();
        }
        
        std::string strMoney = PixStringUtils::commaMoney(nMoney);
        std::string strText = StringUtils::format(mstr_Cart10.c_str(), strMoney.c_str());
        pay_2->setString(strText);
    }
    
    if(pay_3)
    {
        int nMoney = 0;
        
        CouponInfo *couponInfo = mCartInfo->getCouponInfo();
        
        if(couponInfo)
        {
            nMoney = cartItem->getItem_price() - couponInfo->getCoupon_price();
        }
        else
        {
            nMoney = cartItem->getItem_pay_price();
        }
        
        std::string strMoney = PixStringUtils::commaMoney(nMoney);
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        pay_3->setString(strText);
    }
    
    if(pay_4)
    {
        std::string strMoney = PixStringUtils::commaMoney(cartItem->getItem_add_price());
        std::string strText = StringUtils::format(mstr_Cart07.c_str(), strMoney.c_str());
        pay_4->setString(strText);
    }
}

void PixPaymentCell::sendGetCoverImageUrl(const std::string &url)
{
    PixDownloader *pixDownloader = PixDownloader::create();
    pixDownloader->setDelegate(this);
    pixDownloader->setWrite(false);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->setDownloadeUrl(url.c_str());
    pixDownloader->createDownloadDataTask("CartCellCoverImage");
    
    downloaderQueue.push_back(pixDownloader);
}

void PixPaymentCell::createCoverImage()
{
    if(mCartInfo == nullptr)
        return;
    
    if(coverImageData == nullptr)
        return;
    
    if(imageLoader == nullptr)
    {
        imageLoader = new PixImageLoader();
    }
    
    imageLoader->addImageAsync(mCartInfo->getCover_image(), coverImageData, this, imageloader_selector(PixPaymentCell::addCoverImageAsyncCallBack), nullptr);
}

void PixPaymentCell::addCoverImageAsyncCallBack(Texture2D *pTexture, void* pUserData)
{
    if(pTexture == nullptr)
        return;
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        if(MenuCartBookCell)
        {
            PixGallerySprite *sprite = PixGallerySprite::createWithTexture(pTexture, "Common/BookCoverMask_200X200.png");
            
            sprite->setPosition(Vec2(MenuCartBookCell->getContentSize().width / 2,
                                     MenuCartBookCell->getContentSize().height / 2));
            
            MenuCartBookCell->addChild(sprite, 10, 1000);
            
            CC_SAFE_DELETE(coverImageData);
        }
        
    });

}

#pragma mark - Button

void PixPaymentCell::ButtonSetCouponPressed(cocos2d::Ref *pSender)
{
    if(mDelegate)
    {
        mDelegate->paymentCellCouponPressed(this, true);
    }
}

void PixPaymentCell::ButtonDelCouponPressed(cocos2d::Ref *pSender)
{
    if(mDelegate)
    {
        mDelegate->paymentCellCouponPressed(this, false);
    }
}

void PixPaymentCell::coverImagePressed(cocos2d::Ref *pSender)
{
    if(mCartInfo)
    {
        PixAlbumPreviewLayer *albumPreviewLayer = PixAlbumPreviewLayer::create(mCartInfo->getSmart_album_id());
        Director::getInstance()->getRunningScene()->addChild(albumPreviewLayer);
    }
}

#pragma mark - Network

void PixPaymentCell::getCouponCall()
{
    
}

void PixPaymentCell::getCouponCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
}

#pragma mark - Downloade Delegate

void PixPaymentCell::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
{
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        if((*iter) == downloader)
        {
            (*iter)->release();
            downloaderQueue.erase(iter);
            break;
        }
    }
}

void PixPaymentCell::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixPaymentCell::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * data.size());
    std::copy(data.begin(), data.end(), buff);
    
    CC_SAFE_DELETE(coverImageData);
    
    coverImageData = new Data();
    coverImageData->fastSet(buff, data.size());
    
    createCoverImage();
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        if((*iter) == downloader)
        {
            (*iter)->release();
            downloaderQueue.erase(iter);
            break;
        }
    }
}
