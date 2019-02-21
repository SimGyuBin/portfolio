//
//  PixEventCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 26..
//
//

#include "PixEventCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Info/EventInfo.h"
#include "../Common.h"

PixEventCell* PixEventCell::create(EventInfo *pEventInfo)
{
    PixEventCell* widget = new (std::nothrow) PixEventCell(pEventInfo);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;

}

PixEventCell::PixEventCell(EventInfo *pEventInfo)
: _customItem(nullptr)
, Panel_EventBanner(nullptr)
, loding_myalbum(nullptr)
, loadingBar(nullptr)
, Event_finish(nullptr)
, eventBannerSprite(nullptr)
, mbln_loading(false)
, eventBannerData(nullptr)
, imageLoader(nullptr)
, mEventInfo(pEventInfo)
{
    downloaderQueue.clear();
}

PixEventCell::~PixEventCell()
{
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    destroyEventBannerData();
    deleteImageLoader();
}

bool PixEventCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixEventCell::onEnter()
{
    Widget::onEnter();
}

void PixEventCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
    
    if(_customItem)
    {
        FadeIn *fadeIn = FadeIn::create(0.3f);
        _customItem->runAction(fadeIn);
    }
}

void PixEventCell::onExit()
{
    Widget::onExit();
}

void PixEventCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(visible)
    {
        if(getloading() == false)
        {
            loadImage();
        }
    }
}

void PixEventCell::createCustomItem()
{
    if(mEventInfo == nullptr)
        return;
    
    _customItem = CSLOADER_MANAGER->getCustomItem("11_Menu/MenuEventItem.csb");
    
    if(_customItem)
    {
        Panel_EventBanner = dynamic_cast<ui::Layout*>(_customItem->getChildByName("Panel_EventBanner"));
        
        if(Panel_EventBanner)
        {
            
        }
        
        ui::Text *Text_Title = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Title"));
        
        if(Text_Title)
        {
            Text_Title->setString(mEventInfo->getTitle());
        }
        
        ui::Text *Text_Date = dynamic_cast<ui::Text*>(_customItem->getChildByName("Text_Date"));
        
        if(Text_Date)
        {
            std::string strDate = mEventInfo->getSdate();
            strDate.append(" - ");
            strDate.append(mEventInfo->getEdate());
            
            Text_Date->setString(strDate);
        }
        
        loding_myalbum = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("loding_myalbum"));
        
        if(loding_myalbum)
        {
            
        }
        
        loadingBar = dynamic_cast<ui::LoadingBar*>(_customItem->getChildByName("loadingBar"));
        
        if(loadingBar)
        {
            loadingBar->setPercent(0);
        }
        
        Event_finish = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("Event_finish"));
        
        if(Event_finish)
        {
            ui::Text *Text_END = dynamic_cast<ui::Text*>(Event_finish->getChildByName("Text_END"));
            
            if(Text_END)
            {
                Text_END->setString(CODE_NAME(Text_END->getString()));
            }
            
            Event_finish->setVisible(mEventInfo->getEnd());
        }

        _customItem->setOpacity(0);
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixEventCell::loadImage()
{
    if(getloading() == false)
    {
        setloading(true);
        sendGetEventBannerImageUrl_Call();
    }
}

void PixEventCell::destroyEventBannerData()
{
    CC_SAFE_DELETE(eventBannerData);
}

void PixEventCell::createImageLoader()
{
    imageLoader = new PixImageLoader();
}

void PixEventCell::deleteImageLoader()
{
    CC_SAFE_DELETE(imageLoader);
}

void PixEventCell::addEventBannerCallBack(Texture2D *pTexture, void *pUserData)
{
    if(pTexture == nullptr)
        return;
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        addEventBannerSprite(pTexture);
        destroyEventBannerData();
        deleteImageLoader();

    });
}

void PixEventCell::addEventBannerSprite(Texture2D *pTexture)
{
    if(Panel_EventBanner == nullptr)
        return;
    
    if(loding_myalbum)
    {
        _customItem->removeChild(loding_myalbum);
    }
    
    if(loadingBar)
    {
        _customItem->removeChild(loadingBar);
    }
    
    eventBannerSprite = Sprite::createWithTexture(pTexture);
    
    Size previewSize = eventBannerSprite->getContentSize();
    Size frameSize = Panel_EventBanner->getContentSize();
    
    float fScaleX = ((float) frameSize.width) / ((float) previewSize.width);
    float fScaleY = ((float) frameSize.height) / ((float) previewSize.height);
    eventBannerSprite->setPosition(Vec2(frameSize.width * 0.5f, frameSize.height * 0.5f));
    eventBannerSprite->setScale(fScaleX, fScaleY);
    eventBannerSprite->setOpacity(0);
    
    Panel_EventBanner->addChild(eventBannerSprite);
    
    FadeIn *fadeIn = FadeIn::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixEventCell::eventBannerActionFinish, this));
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);
    
    eventBannerSprite->runAction(sequence);
}

void PixEventCell::eventBannerActionFinish(Node* node)
{
    
}

#pragma mark - Network

void PixEventCell::sendGetEventBannerImageUrl_Call()
{
    if(mEventInfo == nullptr)
        return;
    
    PixDownloader *pixDownloader = PixDownloader::create();
    pixDownloader->setDelegate(this);
    pixDownloader->setWrite(false);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->setDownloadeUrl(mEventInfo->getBanner_path().c_str());
    pixDownloader->createDownloadDataTask("PreviewImage");
    
    downloaderQueue.push_back(pixDownloader);
}

#pragma mark - Downloade Delegate

void PixEventCell::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
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

void PixEventCell::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    if(loadingBar)
    {
        loadingBar->setPercent(percent);
    }
}

void PixEventCell::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    if(mEventInfo)
    {
        unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * data.size());
        std::copy(data.begin(), data.end(), buff);
        
        destroyEventBannerData();
        
        eventBannerData = new Data();
        eventBannerData->fastSet(buff, data.size());

        if(imageLoader == nullptr)
        {
            createImageLoader();
        }
        
        imageLoader->addImageAsync(mEventInfo->getBanner_path(), eventBannerData, this, imageloader_selector(PixEventCell::addEventBannerCallBack), nullptr);
    }
    
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
