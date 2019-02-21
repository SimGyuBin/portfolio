//
//  PixAlbumCoverCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 2..
//
//

#ifndef PixAlbumCoverCell_h
#define PixAlbumCoverCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Network/PixHttpClient.h"
#include "../Network/PixDownloader.h"
#include "../UIControl/PixGallerySprite.h"
#include "../Model/MyAlbumModel.h"
#include "../Layer/PixMyAlbumPopup.h"

#define LAYOUT_ITEM_TAG         5000
#define COVERCELL_POPUP_TAG     1159
#define COVERCELL_TITLE_MARGIN  30.0f

USING_NS_CC;

class PixLayerActionDelegate;
class PixAlbumCoverCellDeleteActionDelegate;
class PixSlideLeftMenu;
class PixGalleryListView;

class PixAlbumCoverCell : public ui::Widget, public PixMyAlbumPopupDelegate, public PixDownloaderDelegate
{
public :
    
    static PixAlbumCoverCell* create(MyAlbumModel *pModel);
    
    PixAlbumCoverCell(MyAlbumModel *pModel);
    virtual ~PixAlbumCoverCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    
    bool albumcoverItemContainsPoint(const std::vector<Vec2>&touchVec);
    
    MyAlbumModel* getMyAlbumModel();
    void setMyAlbumModel(MyAlbumModel *pModel);

        
    void runDeleteAction();
    void deleteActionFinish();
    void createSkin();
    
private:
    
    PixHttpClient *pixHttpClient;
    
    MyAlbumModel *mMyAlbumModel;
    Node *_customItem;
    
    Node *AlbumcoverItem;
    Node *AlbumCoverItemLayer;
    Node *AlbumcoverItem_Bottom;
    ui::Layout *panel;
    ui::Text *date;
    ui::Text *title;
    ui::Text *period;
    PixScrollMenu *btn_more;
    PixScrollMenu *btn_share;
    PixScrollMenu *btn_set_cart;
    PixScrollMenu *btn_del_cart;
    PixScrollMenu *btn_dim_cart;
    
    bool mbln_IsVisible;
 
    CC_SYNTHESIZE(bool, mbln_loading, Loading)
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(PixAlbumCoverCellDeleteActionDelegate*, mDeleteActionDelegate, DeleteActionDelegate)
    CC_SYNTHESIZE(PixSlideLeftMenu* ,mSlideLeftMenu, SlideLeftMenu)
    CC_SYNTHESIZE(PixGalleryListView*, mGalleryListView, GalleryListView)
    
    void loadMyAlbumImages();
    void createMaskSprite(PixMyAlbumImageModel *pModel, Texture2D *pTexture, int nIndex);
    void removeAlbumImages();
    void addAlbumImageAsyncCallBack(Texture2D *pTexture, void *pUserData);
    void updateTitle();
    void moveCart();
    void addTipLayer();
    
#pragma mark - Button
    
    void ButtonSharePressed(cocos2d::Ref *pSender);
    void ButtonMorePressed(cocos2d::Ref *pSender);
    void ButtonSetCartPressed(cocos2d::Ref *pSender);
    void ButtonDelCartPressed(cocos2d::Ref *pSender);
    void ButtonDimCartPressed(cocos2d::Ref *pSender);
    
#pragma mark - Delegate
    
    virtual void editMyAlbumTitle() override;
    virtual void skinChangeMyAlbum() override;
    virtual void removeMyAlbum() override;
    
#pragma mark - Network
    
    void albumDelete();
    void setCart();
    void delCart();
    void albumDeleteCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void setCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void delCartCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode) override;
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent) override;
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data) override;
};

class PixAlbumCoverCellDeleteActionDelegate
{
public :
    virtual void albumCoverCellDeleteActionFinish(Ref *pTarget) {};
};


#endif /* PixAlbumCoverCell_h */
