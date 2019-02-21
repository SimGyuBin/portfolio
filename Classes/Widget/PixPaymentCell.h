//
//  PixPaymentCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 14..
//
//

#ifndef PixPaymentCell_h
#define PixPaymentCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Network/PixHttpClient.h"
#include "../Network/PixDownloader.h"
#include "../UIControl/PixGallerySprite.h"
#include "../Util/PixImageLoader.h"

USING_NS_CC;

class CartInfo;
class CartOrderTotalInfo;
class PixPaymentCellDelegate;

class PixPaymentCell : public ui::Widget, public PixDownloaderDelegate
{
public:
    
    static PixPaymentCell* create(CartInfo *pCartInfo, CartOrderTotalInfo *pCartOrderTotalInfo);
    
    PixPaymentCell(CartInfo *pCartInfo, CartOrderTotalInfo *pCartOrderTotalInfo);
    virtual ~PixPaymentCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    virtual void update(float dt) override;
    
    void createCustomItem();

private:
    
    PixHttpClient *pixHttpClient;
    Node *_customItem;
    PixImageLoader *imageLoader;
    Data *coverImageData;
    Node *MenuCartBookCell;
    ui::Text *BookTitle;
    PixScrollMenu *btn_set_coupon;
    PixScrollMenu *btn_del_coupon;
    ui::Text *pay_1;
    ui::Text *pay_2;
    ui::Text *pay_3;
    ui::Text *pay_4;
    PixScrollMenu *coverImageButton;
    std::string mstr_Cart07;
    std::string mstr_Cart10;
    
    std::deque<PixDownloader*> downloaderQueue;
    
    CC_SYNTHESIZE(CartInfo*, mCartInfo, CartInfo)
    CC_SYNTHESIZE(CartOrderTotalInfo*, mCartOrderTotalInfo, CartOrderTotalInfo)
    CC_SYNTHESIZE(bool, mbln_Isloading, Loading)
    CC_SYNTHESIZE(PixPaymentCellDelegate*, mDelegate, Delegate)
    
    void updatePaymentUI();
    void sendGetCoverImageUrl(const std::string &url);
    void createCoverImage();
    void addCoverImageAsyncCallBack(Texture2D *pTexture, void* pUserData);

#pragma mark - Button

    void ButtonSetCouponPressed(cocos2d::Ref *pSender);
    void ButtonDelCouponPressed(cocos2d::Ref *pSender);
    void coverImagePressed(cocos2d::Ref *pSender);
    
#pragma mark - Network
    
    void getCouponCall();
    void getCouponCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode) override;
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent) override;
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data) override;

};

class PixPaymentCellDelegate
{
public:
    virtual void paymentCellCouponPressed(cocos2d::Ref *pWidget, bool blnSet) {};
};

#endif /* PixPaymentCell_h */
