//
//  PixPreviewCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 16..
//
//

#include "PixPreviewCell.h"
#include "../UIControl/PixGallerySprite.h"
#include "../Model/MyAlbumPreviewModel.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

PixPreviewCell* PixPreviewCell::create()
{
    PixPreviewCell* widget = new (std::nothrow) PixPreviewCell(false);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPreviewCell* PixPreviewCell::create(bool isWrite)
{
    PixPreviewCell* widget = new (std::nothrow) PixPreviewCell(isWrite);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPreviewCell::PixPreviewCell(bool isWrite)
: _customItem(nullptr)
, smartalbumLayer(nullptr)
, loding_myalbum(nullptr)
, loadingBar(nullptr)
, albumcover_effect(nullptr)
, album_effectl(nullptr)
, m_MyAlbumPreviewModel(nullptr)
, mbln_loading(false)
, previewSprite(nullptr)
, mbln_Write(isWrite)
{
    
}

PixPreviewCell::~PixPreviewCell()
{
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    if(previewSprite)
    {
        Director::getInstance()->getTextureCache()->removeTexture(previewSprite->getTexture());
    }
}

bool PixPreviewCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}
void PixPreviewCell::onEnter()
{
    Widget::onEnter();
}

void PixPreviewCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
    
    if(_customItem)
    {
        FadeIn *fadeIn = FadeIn::create(0.3f);
        _customItem->runAction(fadeIn);
    }
}

void PixPreviewCell::onExit()
{
    Widget::onExit();
}

void PixPreviewCell::setVisible(bool visible)
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

void PixPreviewCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("07_My_AlbumList/AlbumImegeLayer.csb");
    
    smartalbumLayer = dynamic_cast<Node*>(_customItem->getChildByName("smartalbumLayer"));
    loding_myalbum = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("loding_myalbum"));
    loadingBar = dynamic_cast<ui::LoadingBar*>(_customItem->getChildByName("loadingBar"));
    
    if(loadingBar)
    {
        loadingBar->setPercent(0);
    }
    
    albumcover_effect = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("albumcover_effect"));
    album_effectl = dynamic_cast<ui::ImageView*>(_customItem->getChildByName("album_effect"));
    
    setContentSize(_customItem->getContentSize());
    
    this->addChild(_customItem);
    
    _customItem->setOpacity(0);
}

void PixPreviewCell::loadImage()
{
    if(getloading() == false)
    {
        setloading(true);
        sendGetPreviewImageUrl_Call();
    }
}

void PixPreviewCell::addPreviewSprite(Texture2D *pTexture)
{
    if(loding_myalbum)
    {
        _customItem->removeChild(loding_myalbum);
    }
    
    if(loadingBar)
    {
        _customItem->removeChild(loadingBar);
    }
    
    previewSprite = Sprite::createWithTexture(pTexture);
    
    Size previewSize = previewSprite->getContentSize();
    Size frameSize = smartalbumLayer->getContentSize();
    
    float fScaleX = ((float) frameSize.width) / ((float) previewSize.width);
    float fScaleY = ((float) frameSize.height) / ((float) previewSize.height);
    previewSprite->setPosition(Vec2(frameSize.width * 0.5f, frameSize.height * 0.5f));
    previewSprite->setScale(fScaleX, fScaleY);
    previewSprite->setOpacity(0);
    
    smartalbumLayer->addChild(previewSprite);
    
    FadeIn *fadeIn = FadeIn::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixPreviewCell::previewActionFinish, this));
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);

    previewSprite->runAction(sequence);
}

void PixPreviewCell::previewActionFinish(Node* node)
{
    ui::Layout *background = dynamic_cast<ui::Layout*>(_customItem->getChildByName("background"));
    
    if(background)
    {
        background->setVisible(false);
    }
}

void PixPreviewCell::setAlbumcoverEffectVisible(bool visible)
{
    if(albumcover_effect)
    {
        albumcover_effect->setVisible(visible);
    }
}

void PixPreviewCell::setAlbumEffectlVisible(bool visible)
{
    if(album_effectl)
    {
        album_effectl->setVisible(visible);
    }
}

void PixPreviewCell::addPreviewAsyncCallBack(Texture2D *pTexture, void *pUserData)
{
    if(pTexture == nullptr)
        return;
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
    
        if(getMyAlbumPreviewModel())
        {
            addPreviewSprite(pTexture);
            //            getMyAlbumPreviewModel()->deleteImageLoader();
            //            getMyAlbumPreviewModel()->destroyPreviewData();
        }
    });
}

#pragma mark - Network

void PixPreviewCell::sendGetPreviewImageUrl_Call()
{
    
    PixDownloader *pixDownloader = nullptr;
    
    if(getWrite())
    {
        std::string image_path = getMyAlbumPreviewModel()->getThumb_url().c_str();
        
        std::vector<std::string> tokens;
        PixStringUtils::getTokens(tokens, image_path, "/");
        
        std::string filename;
        filename.clear();
        
        if(!tokens.empty())
        {
            filename = tokens[tokens.size() - 1];
        }
        
        std::string fullPath = std::string(THEME_PREVIEW_PATH).append(filename);
        bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
        
        if(!isFileExist)
        {
            pixDownloader = PixDownloader::create(image_path.c_str(), THEME_PREVIEW_PATH, this);
            pixDownloader->setWrite(true);
        }
        else
        {
            if(getMyAlbumPreviewModel())
            {
                getMyAlbumPreviewModel()->addImageAsync(this ,imageloader_selector(PixPreviewCell::addPreviewAsyncCallBack), fullPath, getMyAlbumPreviewModel());
            }
            
            return;
        }
    }
    else
    {
        pixDownloader = PixDownloader::create();
        pixDownloader->setDelegate(this);
        pixDownloader->setWrite(false);
    }
    
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->setDownloadeUrl(getMyAlbumPreviewModel()->getThumb_url().c_str());
    pixDownloader->createDownloadDataTask("PreviewImage");
    
    downloaderQueue.push_back(pixDownloader);
}

#pragma mark - Downloade Delegate

void PixPreviewCell::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
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

void PixPreviewCell::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    if(loadingBar)
    {
        loadingBar->setPercent(percent);
    }
}

void PixPreviewCell::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    if(getMyAlbumPreviewModel())
    {
        unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * data.size());
        std::copy(data.begin(), data.end(), buff);
        
        Data *previewData = new Data();
        previewData->fastSet(buff, data.size());
        
        getMyAlbumPreviewModel()->destroyPreviewData();
        getMyAlbumPreviewModel()->setPreviewData(previewData);
        getMyAlbumPreviewModel()->addImageAsync(this ,imageloader_selector(PixPreviewCell::addPreviewAsyncCallBack), getMyAlbumPreviewModel());
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
