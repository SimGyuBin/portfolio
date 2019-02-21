//
//  PixOrderGoodsCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 12..
//
//

#ifndef PixOrderGoodsCell_h
#define PixOrderGoodsCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixDownloader.h"
#include "../Util/PixImageLoader.h"
#include "../UIControl/PixGallerySprite.h"

USING_NS_CC;

class OrderCheckInfo;
class GoodsInfo;

class PixOrderGoodsCell : public ui::Widget, public PixDownloaderDelegate
{
public:
    
    static PixOrderGoodsCell* create(OrderCheckInfo *pOrderCheckInfo, GoodsInfo *pGoodsInfo);
    
    PixOrderGoodsCell(OrderCheckInfo *pOrderCheckInfo, GoodsInfo *pGoodsInfo);
    virtual ~PixOrderGoodsCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    Node *Node_MenuCartBookCell;
    
    PixImageLoader *imageLoader;
    Data *coverImageData;
    std::deque<PixDownloader*> downloaderQueue;
    
    CC_SYNTHESIZE(OrderCheckInfo*, mOrderCheckInfo, OrderCheckInfo)
    CC_SYNTHESIZE(GoodsInfo*, mGoodsInfo, GoodsInfo)
    CC_SYNTHESIZE(bool, mbln_Isloading, Loading)
    
    void sendGetAlbumImageUrl(const std::string &url);
    void createAlbumImage();
    void addAlbumImageAsyncCallBack(Texture2D *pTexture, void* pUserData);
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode) override;
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent) override;
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data) override;
    
};

#endif /* PixOrderGoodsCell_h */
