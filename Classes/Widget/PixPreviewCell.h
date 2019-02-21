//
//  PixPreviewCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 16..
//
//

#ifndef PixPreviewCell_h
#define PixPreviewCell_h

#include "cocos2d.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../Network/PixDownloader.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class MyAlbumPreviewModel;

class PixPreviewCell : public ui::Widget, public PixDownloaderDelegate
{
    
public :
    
    static PixPreviewCell* create();
    static PixPreviewCell* create(bool isWrite);
    
    PixPreviewCell(bool isWrite);
    virtual ~PixPreviewCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    void loadImage();
    void addPreviewSprite(Texture2D *pTexture);
    void previewActionFinish(Node* node);
    void setAlbumcoverEffectVisible(bool visible);
    void setAlbumEffectlVisible(bool visible);
    void addPreviewAsyncCallBack(Texture2D *pTexture, void *pUserData);
    
private:
    
    Node *_customItem;
    Node *smartalbumLayer;
    ui::ImageView *albumcover_effect;
    ui::ImageView *album_effectl;
    Sprite *previewSprite;
    
    ui::ImageView *loding_myalbum;
    ui::LoadingBar *loadingBar;
    
    CC_SYNTHESIZE(MyAlbumPreviewModel*, m_MyAlbumPreviewModel, MyAlbumPreviewModel)
    CC_SYNTHESIZE(bool, mbln_loading, loading)
    CC_SYNTHESIZE(bool, mbln_Write, Write)
    
    std::deque<PixDownloader*> downloaderQueue;
    
#pragma mark - Network
    
    void sendGetPreviewImageUrl_Call();
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode) override;
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent) override;
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data) override;
};

#endif /* PixPreviewCell_h */
