//
//  PixEventCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 26..
//
//

#ifndef PixEventCell_h
#define PixEventCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixDownloader.h"
#include "../Util/PixImageLoader.h"

USING_NS_CC;

class EventInfo;

class PixEventCell : public ui::Widget, public PixDownloaderDelegate
{
public:
    
    static PixEventCell* create(EventInfo *pEventInfo);
    
    PixEventCell(EventInfo *pEventInfo);
    virtual ~PixEventCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    void loadImage();
    void destroyEventBannerData();
    void createImageLoader();
    void deleteImageLoader();
    
private:
    
    Node *_customItem;
    
    ui::Layout *Panel_EventBanner;
    ui::ImageView *loding_myalbum;
    ui::LoadingBar *loadingBar;
    ui::ImageView *Event_finish;
    Sprite *eventBannerSprite;
    
    Data* eventBannerData;
    PixImageLoader *imageLoader;
    
    CC_SYNTHESIZE(bool, mbln_loading, loading)
    CC_SYNTHESIZE(EventInfo*, mEventInfo, EventInfo)
    
    std::deque<PixDownloader*> downloaderQueue;
    
    void addEventBannerCallBack(Texture2D *pTexture, void *pUserData);
    void addEventBannerSprite(Texture2D *pTexture);
    void eventBannerActionFinish(Node* node);
    
#pragma mark - Network
    
    void sendGetEventBannerImageUrl_Call();
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode) override;
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent) override;
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data) override;
    
};

#endif /* PixEventCell_h */
