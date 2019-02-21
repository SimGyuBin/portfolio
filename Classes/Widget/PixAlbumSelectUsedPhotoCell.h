//
//  PixAlbumSelectUsedPhotoCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 14..
//
//

#ifndef PixAlbumSelectUsedPhotoCell_h
#define PixAlbumSelectUsedPhotoCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixDownloader.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

class MyAlbumModel;
class MyAlbumUsedImageModel;

class PixAlbumSelectUsedPhotoCell : public ui::Widget, public PixDownloaderDelegate
{
    
public :
    
    static PixAlbumSelectUsedPhotoCell* create(MyAlbumModel *pModel);
    
    PixAlbumSelectUsedPhotoCell(MyAlbumModel *pModel);
    virtual ~PixAlbumSelectUsedPhotoCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    
    void pushMyAlbumUsedImageModel(MyAlbumUsedImageModel* pModel);
    void createUsedPhoto();
    void addUsedThumbImageAsyncCallBack(Texture2D *pTexture, void* pUserData);
    Node* getCellItem(const std::string &name);
    void removeCellItem(Node *pItme);
    MyAlbumUsedImageModel* getMyAlbumUsedImageModel(int nIndex);
    bool isItemIndex(int nIndex);
    
    CC_SYNTHESIZE(bool, mbln_Isloading, Loading)
    
private:
    
    Node *_customItem;
    PixScrollMenu *menu;
    
    std::vector<MyAlbumUsedImageModel*> albumUsedImageModelVec;
    std::deque<PixDownloader*> downloaderQueue;
    
    CC_SYNTHESIZE(MyAlbumModel*, mAlbumModel, AlbumModel)

#pragma mark - Button
    
    void cellPressed(cocos2d::Ref *pSender);
    
#pragma mark - Network
    
    void sendGetUsedThumbImageUrl(const std::string &url, MyAlbumUsedImageModel *pModel);
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode) override;
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent) override;
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data) override;
};

#endif /* PixAlbumSelectUsedPhotoCell_h */
