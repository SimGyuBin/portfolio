//
//  PixUsedPhotoSelectCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 22..
//
//

#ifndef PixUsedPhotoSelectCell_h
#define PixUsedPhotoSelectCell_h

#include "cocos2d.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixDownloader.h"
#include "../UIControl/PixPanZoomLayer.h"

#define SELECTCELL_MARGIN   10

USING_NS_CC;

class MyAlbumUsedImageModel;
class PixPhotoSelectCellDelegate;

class PixUsedPhotoSelectCell : public ui::Widget, public PixDownloaderDelegate
{
    
public :
    
    static PixUsedPhotoSelectCell* create(MyAlbumUsedImageModel *pModel);
    
    PixUsedPhotoSelectCell(MyAlbumUsedImageModel *pModel);
    virtual ~PixUsedPhotoSelectCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    void imageLoad();
    
    inline MyAlbumUsedImageModel* getModel()
    {
        return mModel;
    }
    
private :

    MyAlbumUsedImageModel *mModel;
    Sprite *mSelectImageSprite;
    Sprite *mLodingBack;
    ui::Button *mPhotoRefresh;
    ProgressTimer *mLoding;
    bool mbln_Check_M_Texture;
    std::deque<PixDownloader*> downloaderQueue;
    std::vector<std::function<void()>> functionsToPerform;
    
    CC_SYNTHESIZE(bool, mbln_Loading, Loading)
    CC_SYNTHESIZE(PixPhotoSelectCellDelegate*, mDelegat, Delegat)
    
    void createSelectImageSprite();
    void runSelectImageSpriteAction(Sprite *pSprite);
    void selectImageSpriteActionFinish();
    void addUsedImageAsyncCallBack(Texture2D *pTexture, void *pUserData);

#pragma mark - Button
    
    void ButtonCellPressed(cocos2d::Ref *pSender);
    void ButtonPhotoRefreshPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Network
    
    void sendGetUsedImageUrl(const std::string &url, MyAlbumUsedImageModel *pModel);
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode) override;
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent) override;
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data) override;
    
};

class PixPhotoSelectCellDelegate
{
public:
    virtual void photoSelectCellPressed(cocos2d::Ref *pTaget) {};
    
};

#endif /* PixUsedPhotoSelectCell_h */
