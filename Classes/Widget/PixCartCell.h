//
//  PixCartCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 9..
//
//

#ifndef PixCartCell_h
#define PixCartCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Network/PixHttpClient.h"
#include "../Network/PixDownloader.h"
#include "../UIControl/PixGallerySprite.h"
#include "../UIControl/PixEditBox.h"
#include "../Util/PixImageLoader.h"

USING_NS_CC;

class CartInfo;
class CartOrderTotalInfo;
class PixCartCellDelegate;

class PixCartCell : public ui::Widget, public PixDownloaderDelegate, public ui::EditBoxDelegate
{
public:
    
    static PixCartCell* create(CartInfo *pCartInfo, CartOrderTotalInfo *pCartOrderTotalInfo);
    
    PixCartCell(CartInfo *pCartInfo, CartOrderTotalInfo *pCartOrderTotalInfo);
    virtual ~PixCartCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    bool isCheckSelected();
    void setCheckSelected(bool var);
    void setCountVisible(bool var);
    
private:
    
    PixHttpClient *pixHttpClient;
    Node *_customItem;
    ui::CheckBox *check;
    PixScrollMenu *Button_remove;
    Node *MenuCartBookCell;
    ui::Text *BookTitle;
    PixEditBox *Count;
    PixScrollMenu *Button_down;
    PixScrollMenu *Button_up;
    ui::Text *eventDiscount;
    ui::Text *Text_Pay_0;
    ui::Text *Text_Pay_1;
    ui::Text *Text_Pay_2;
    ui::Text *Text_Pay_3;
    ui::ImageView* ImageView_Price;
    PixImageLoader *imageLoader;
    Data *coverImageData;
    PixScrollMenu *coverImageButton;
    std::string mstr_Cart07;
    
    std::deque<PixDownloader*> downloaderQueue;
    
    CC_SYNTHESIZE(CartInfo*, mCartInfo, CartInfo)
    CC_SYNTHESIZE(CartOrderTotalInfo*, mCartOrderTotalInfo, CartOrderTotalInfo)
    CC_SYNTHESIZE(bool, mbln_Isloading, Loading)
    CC_SYNTHESIZE(PixCartCellDelegate*, mDelegate, Delegate)
    
    void updateCartUI();
    void sendGetCoverImageUrl(const std::string &url);
    void createCoverImage();
    void addCoverImageAsyncCallBack(Texture2D *pTexture, void* pUserData);
    
#pragma mark - Action
    
    void runDeleteAction();
    void deleteActionFinish();
    
#pragma mark - Button
    
    void checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void ButtonRemoveePressed(cocos2d::Ref *pSender);
    void ButtonDownPressed(cocos2d::Ref *pSender);
    void ButtonUpPressed(cocos2d::Ref *pSender);
    void coverImagePressed(cocos2d::Ref *pSender);
  
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(ui::EditBox* editBox) override;
    
#pragma mark - Network
    
    void delCartCall();
    void delCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void updCarCall(int nItem_cnt);
    void updCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void updCarCallError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode) override;
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent) override;
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data) override;
};

class PixCartCellDelegate
{
public :
    virtual void cartCellDeleteActionStart(Ref *pTarget) {};
    virtual void cartCellDeleteActionFinish(Ref *pTarget) {};
    virtual void cartCellCheckBoxPressed(bool var) {};
};

#endif /* PixCartCell_h */
