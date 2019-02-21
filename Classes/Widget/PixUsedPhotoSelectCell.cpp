//
//  PixUsedPhotoSelectCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 22..
//
//

#include "PixUsedPhotoSelectCell.h"
#include "../Model/MyAlbumUsedImageModel.h"
#include "../UIControl/PixScrollMenu.h"

PixUsedPhotoSelectCell* PixUsedPhotoSelectCell::create(MyAlbumUsedImageModel *pModel)
{
    PixUsedPhotoSelectCell* widget = new (std::nothrow) PixUsedPhotoSelectCell(pModel);
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}


PixUsedPhotoSelectCell::PixUsedPhotoSelectCell(MyAlbumUsedImageModel *pModel)
: mModel(nullptr)
, mSelectImageSprite(nullptr)
, mPhotoRefresh(nullptr)
, mLodingBack(nullptr)
, mLoding(nullptr)
, mbln_Loading(false)
, mbln_Check_M_Texture(false)
, mDelegat(nullptr)
{
    this->mModel = pModel;
    downloaderQueue.clear();
    
    if(mSelectImageSprite)
    {
        Director::getInstance()->getTextureCache()->removeTexture(mSelectImageSprite->getTexture());
    }
    
    functionsToPerform.clear();
}

PixUsedPhotoSelectCell::~PixUsedPhotoSelectCell()
{
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    if(mModel)
    {
        mModel->delete_M_ImageLoader();
    }
    
    functionsToPerform.clear();
}

bool PixUsedPhotoSelectCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixUsedPhotoSelectCell::onEnter()
{
    Widget::onEnter();
}

void PixUsedPhotoSelectCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixUsedPhotoSelectCell::onExit()
{
    Widget::onExit();
}

void PixUsedPhotoSelectCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(visible)
    {
        if(getLoading() == false)
        {
            imageLoad();
        }
        else
        {
            if(!functionsToPerform.empty())
            {
                auto temp = functionsToPerform;
                functionsToPerform.clear();
                
                for( const auto &function : temp )
                {
                    function();
                }
            }
        }
    }
}

void PixUsedPhotoSelectCell::createCustomItem()
{
    mLodingBack = Sprite::create("Common/loding_UsedPhotoList.png");
    mLodingBack->setPosition(Vec2(getContentSize().width / 2,
                                  getContentSize().height / 2));
    addChild(mLodingBack,10);
    
    Sprite *sprite = Sprite::create("Common/loding_UsedPhotoList_2.png");
    
    mLoding = ProgressTimer::create(sprite);
    mLoding->setPosition(Vec2(getContentSize().width / 2,
                              getContentSize().height / 2));
    mLoding->setType(ProgressTimer::Type::RADIAL);
    mLoding->setPercentage(0.0f);
    addChild(mLoding,11);
    
    std::string normalImg = "Common/button_720x1077.png";
    std::string pressedImg = "Common/button_720x1077.png";
    std::string disabledImg = "";
    
    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                          CC_CALLBACK_1(PixUsedPhotoSelectCell::ButtonCellPressed, this));
    
    PixScrollMenu *scrollMenu = PixScrollMenu::create(menuItem, nullptr);
    scrollMenu->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
    scrollMenu->setAnchorPoint(Vec2::ZERO);
    scrollMenu->setContentSize(getContentSize());
    scrollMenu->setLocalZOrder(2);
    addChild(scrollMenu,12);
}

void PixUsedPhotoSelectCell::imageLoad()
{
    if(getLoading())
        return;
    
    if(mSelectImageSprite)
        return;
    
    setLoading(true);
    
    createSelectImageSprite();
}

void PixUsedPhotoSelectCell::createSelectImageSprite()
{
//    if(mModel->get_S_Texture() && mModel->get_M_Texture() == nullptr)
//    {
//        mbln_Check_M_Texture = true;
//        
//        Sprite *sprite = Sprite::createWithTexture(mModel->get_S_Texture());
//        runSelectImageSpriteAction(sprite);
//
//        std::string mUrl = mModel->get_m_image();
//        sendGetUsedImageUrl(mUrl, mModel);
//    }
//    else
    
    std::string mUrl = mModel->get_m_image();
    sendGetUsedImageUrl(mUrl, mModel);
}

void PixUsedPhotoSelectCell::runSelectImageSpriteAction(Sprite *pSprite)
{
    if(pSprite == nullptr)
        return;
    
    if(mSelectImageSprite)
    {
        removeChild(mSelectImageSprite);
        mSelectImageSprite = nullptr;
    }
    
    if(mLodingBack)
    {
        mLodingBack->setVisible(false);
    }
    
    if(mLoding)
    {
        mLoding->setVisible(false);
    }
    
    Size size = Size(getContentSize().width - SELECTCELL_MARGIN,
                     getContentSize().height - SELECTCELL_MARGIN);
    
    mSelectImageSprite = pSprite;
    mSelectImageSprite->setPosition((size.width * 0.5f) + (SELECTCELL_MARGIN * 0.5f),
                                    (size.height * 0.5f) + (SELECTCELL_MARGIN * 0.5f));
    
    float fWidth = mSelectImageSprite->getContentSize().width;
    float fHeight = mSelectImageSprite->getContentSize().height;
    
    Size imageSize = mSelectImageSprite->getContentSize();
    
    float fScale = 0.0f;
    
    if (imageSize.width > imageSize.height)
    {
        fScale = ((float) size.width) / ((float) imageSize.width);
        
        float fReSizeheight = fHeight * fScale;
        
        if(fReSizeheight > size.height)
        {
            fScale = ((float) size.width) / ((float) fHeight);
        }
    }
    else
    {
        fScale = ((float) size.height) / ((float) imageSize.height);
        
        float fReSizeWidth = fWidth * fScale;
        
        if(fReSizeWidth > size.width)
        {
            fScale = ((float) size.width) / ((float) fWidth);
        }
    }

    mSelectImageSprite->setScale(fScale);
    
    mSelectImageSprite->setOpacity(0);
    FadeIn *fadeIn = FadeIn::create(0.3f);
    CallFunc *callFunc = CallFunc::create(CC_CALLBACK_0(PixUsedPhotoSelectCell::selectImageSpriteActionFinish, this));
    
    addChild(mSelectImageSprite);
    
    mSelectImageSprite->runAction(Sequence::create(fadeIn, callFunc, NULL));
}

void PixUsedPhotoSelectCell::selectImageSpriteActionFinish()
{

}

void PixUsedPhotoSelectCell::addUsedImageAsyncCallBack(Texture2D *pTexture, void *pUserData)
{
    if(pTexture == nullptr)
        return;
    
    functionsToPerform.push_back([=](void) -> void {
        
        if(mModel)
        {
            Sprite *sprite = Sprite::createWithTexture(pTexture);
            runSelectImageSpriteAction(sprite);
            
            mModel->delete_M_ImageLoader();
        }
        
    });
    
}

#pragma mark - Button

void PixUsedPhotoSelectCell::ButtonCellPressed(cocos2d::Ref *pSender)
{
    if(mDelegat)
    {
        mDelegat->photoSelectCellPressed(this);
    }
}

void PixUsedPhotoSelectCell::ButtonPhotoRefreshPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mSelectImageSprite)
        {
            removeChild(mSelectImageSprite);
            mSelectImageSprite = nullptr;
        }
        
        setLoading(false);
        
        if(mPhotoRefresh)
        {
            removeChild(mPhotoRefresh);
            mPhotoRefresh = nullptr;
        }
        
        if(mLodingBack)
        {
            mLodingBack->setVisible(true);
        }
        
        if(mLoding)
        {
            mLoding->setVisible(true);
            mLoding->setPercentage(0);
        }
    }
}


#pragma mark - Network

void PixUsedPhotoSelectCell::sendGetUsedImageUrl(const std::string &url, MyAlbumUsedImageModel *pModel)
{
    PixDownloader *pixDownloader = PixDownloader::create();
    pixDownloader->setDelegate(this);
    pixDownloader->setWrite(false);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->setDownloadeUrl(url.c_str());
    pixDownloader->setUserData(pModel);
    pixDownloader->createDownloadDataTask("UsedImage");
    
    downloaderQueue.push_back(pixDownloader);
}

#pragma mark - Downloade Delegate

void PixUsedPhotoSelectCell::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
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
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        std::string normalImg = "Common/loding_UsedPhotoRefresh.png";
        std::string pressedImg = "Common/loding_UsedPhotoRefresh_on.png";
        
        mPhotoRefresh = ui::Button::create(normalImg, pressedImg, normalImg, ui::Button::TextureResType::LOCAL);
        mPhotoRefresh->setPosition(Vec2(getContentSize().width / 2, 485));
        mPhotoRefresh->addTouchEventListener(CC_CALLBACK_2(PixUsedPhotoSelectCell::ButtonPhotoRefreshPressed, this));
        addChild(mPhotoRefresh, 13);
        
    });
}

void PixUsedPhotoSelectCell::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    if(mLoding)
    {
        mLoding->setPercentage(percent);
    }
}

void PixUsedPhotoSelectCell::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    MyAlbumUsedImageModel *pModel = static_cast<MyAlbumUsedImageModel*>(downloader->getUserData());
    
    if(pModel)
    {
        unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * data.size());
        std::copy(data.begin(), data.end(), buff);
        
        Data *mData = new Data();
        mData->fastSet(buff, data.size());
        
        pModel->destroy_M_Data();
        pModel->set_M_Data(mData);
        pModel->add_M_ImageAsync(this, imageloader_selector(PixUsedPhotoSelectCell::addUsedImageAsyncCallBack), pModel);
    }
}
