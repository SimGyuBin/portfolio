//
//  PixSelectedImageCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 3..
//
//

#include "PixSelectedImageCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "PixSelectDateCell.h"
#include "../Info/UserInfo.h"
#include "../Util/PixImageLoader.h"
#include "../Util/PixNativeUtils.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Manager/PixImageCreateManager.h"

PixSelectedImageCell::PixSelectedImageCell()
: mDelegate(nullptr)
, mModel(nullptr)
, _RemoveButtonLayer(nullptr)
, _sprite(nullptr)
, mbln_IsRemoveAction(false)
, imageLoader(nullptr)
, mFullScreenImage(nullptr)
, mParent(nullptr)
, mbln_Isloading(false)
, mbln_IsThumbnail(false)
{
    
}

PixSelectedImageCell::~PixSelectedImageCell()
{
    deleteImageLoader();

    if(mFullScreenImage)
    {
        Director::getInstance()->getTextureCache()->removeTexture(mFullScreenImage);
        mFullScreenImage = nullptr;
      
//        if(mModel->getLocalImageInfo())
//        {
//            mModel->getLocalImageInfo()->destroyFullScreenImageData();
//        }
    }
    
    this->unschedule(schedule_selector(PixSelectedImageCell::loadFullScreenImageLoad));
}

PixSelectedImageCell* PixSelectedImageCell::create(PixImageModel *pModel, PixSelectedRemoveButtonDelegate *pDelegate, PixGalleryListView *ListView)
{
    PixSelectedImageCell* widget = new (std::nothrow) PixSelectedImageCell();
    if (widget && widget->init())
    {
        widget->createCustomItem(pModel, pDelegate, ListView);
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool PixSelectedImageCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}
void PixSelectedImageCell::onEnter()
{
    Widget::onEnter();    
}

void PixSelectedImageCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixSelectedImageCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(visible)
    {
        if(_sprite == nullptr && _isTransitionFinished)
        {
            loadImage();
        }
    }
    else
    {

    }
}

void PixSelectedImageCell::createCustomItem(PixImageModel *pModel, PixSelectedRemoveButtonDelegate *pDelegate, PixGalleryListView *ListView)
{
    if(pModel)
    {
        this->mModel = pModel;
        this->mDelegate = pDelegate;
        this->mParent = ListView;
        
        if(mModel && mModel->getLocalImageInfo())
        {
            LocalImageInfo *localImageInfoinfo = mModel->getLocalImageInfo();
            
            Size frameSize = Size(700, atoi(localImageInfoinfo->getHeight().c_str()));
            Size imageSize = Size(atoi(localImageInfoinfo->getWidth().c_str()),
                                  atoi(localImageInfoinfo->getHeight().c_str()));
            
            float tw = ((float) frameSize.width) / ((float) imageSize.width);
            
            float fScale = 1.0f;
            
            fScale = tw;
            
            float fHeight = (frameSize.height * fScale) + 10;
            
            setContentSize(Size(720, fHeight));
        }
    }
}

void PixSelectedImageCell::loadImage()
{
    if(mModel == nullptr)
        return;
    
    if(mModel->getLocalImageInfo()  == nullptr)
        return;
    
    if(getLoading())
        return;
     
    Data* pThumbnaildata = mModel->getLocalImageInfo()->getThumbnailData();
    Data* pFullScreenData = mModel->getLocalImageInfo()->getFullScreenData();
    
    if(pFullScreenData)
    {
        if(!pFullScreenData->isNull())
        {
            if(imageLoader == nullptr)
            {
                createImageLoader();
            }
            
            std::string strKey = "FullScreen" + mModel->getLocalImageInfo()->getPath();
            imageLoader->addImageAsync(strKey, pFullScreenData, this, imageloader_selector(PixSelectedImageCell::addFullScreenImageAsyncCallBack));
            
            return;
        }
    }
    else if(mModel->getTexture())
    {
        createAdjustableHeightSprite();
        mbln_IsThumbnail = true;
    }
   
    if(mbln_IsThumbnail || pThumbnaildata == nullptr)
    {
         this->schedule(schedule_selector(PixSelectedImageCell::loadFullScreenImageLoad));
    }
  
    //        // 테스트 라벨
    //        ui::Text *text = ui::Text::create();
    //        text->setString(StringUtils::format("%d", pModel->getOrientation()));
    //        text->setFontSize(50);
    //        text->setColor(cocos2d::Color3B::RED);
    //        text->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
    //        this->addChild(text);
}

void PixSelectedImageCell::createImageLoader()
{
    imageLoader = new PixImageLoader();
}

void PixSelectedImageCell::deleteImageLoader()
{
    CC_SAFE_DELETE(imageLoader);
}

void PixSelectedImageCell::addThumbnailImageAsyncCallBack(Texture2D *pTexture, void *pUserData)
{
    mModel->setTexture(pTexture);
    createAdjustableHeightSprite();

//    if(mParent)
//    {
//        mParent->requestDoLayout();
//    }
}
void PixSelectedImageCell::loadFullScreenImageLoad(float dt)
{
    this->unschedule(schedule_selector(PixSelectedImageCell::loadFullScreenImageLoad));
    
    LocalImageInfo *localImageInfo = mModel->getLocalImageInfo();
    
    std::string strPath = localImageInfo->getPath();
    
    int nWidth = atoi(mModel->getLocalImageInfo()->getWidth().c_str());
    int nHeight = atoi(mModel->getLocalImageInfo()->getHeight().c_str());
    
//    PixNativeUtils::nativeCallGetFullScreenImage(strPath, nWidth, nHeight);
    
    IMAGECREATE_MANAGER->imageCreate(strPath, nWidth, nHeight, IMAGECREATE_FULLSCREEN, IMAGECREATE_CALLBACK(PixSelectedImageCell::addFullScreenDataloadComplete, this));
}
void PixSelectedImageCell::addFullScreenImageAsyncCallBack(Texture2D *pTexture, void *pUserData)
{
    mFullScreenImage = pTexture;
    createAdjustableHeightSprite(pTexture);
    
//    if(mParent)
//    {
//        mParent->requestDoLayout();
//    }
}

void PixSelectedImageCell::addFullScreenDataloadComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate)
{
    if(mModel && mModel->getLocalImageInfo())
    {
        Data *pFullScreenData = mModel->getLocalImageInfo()->getFullScreenData();
        
        if(pFullScreenData && !pFullScreenData->isNull())
        {
            if(imageLoader == nullptr)
            {
                createImageLoader();
            }
            
            std::string strKey = "FullScreenData" + mModel->getLocalImageInfo()->getPath();
            imageLoader->addImageAsync(strKey, pFullScreenData, this, imageloader_selector(PixSelectedImageCell::addFullScreenImageAsyncCallBack));
        }
    }
}

void PixSelectedImageCell::createAdjustableHeightSprite()
{
    Texture2D *texture = mModel->getTexture();
    createAdjustableHeightSprite(texture);
}

void PixSelectedImageCell::createAdjustableHeightSprite(Texture2D *pTexture)
{
    float fHeight = 0;
    
    if(pTexture)
    {
        if(_sprite)
        {
            removeChild(_sprite);
            _sprite = nullptr;
        }
        
        if(_RemoveButtonLayer)
        {
            removeChild(_RemoveButtonLayer);
            _RemoveButtonLayer = nullptr;
        }
        
        Size size = Size(700, pTexture->getContentSize().height);
        
        _sprite = PixAdjustableHeightSprite::createWithTexture(pTexture, size);
        _sprite->setPosition(10,10);
        this->addChild(_sprite, -10);
        
        fHeight = (size.height * _sprite->getScale()) + 10;
        
        setContentSize(Size(720, fHeight));
        
        _RemoveButtonLayer = CSLoader::createNode("04_SelectPhoto/SelectedRemoveButtonLayer.csb");
        
        if(_RemoveButtonLayer)
        {
            ui::Button *removeButton = dynamic_cast<ui::Button*>(_RemoveButtonLayer->getChildByName("remove"));
            
            if(removeButton)
            {
                removeButton->addTouchEventListener(CC_CALLBACK_2(PixSelectedImageCell::removePressed, this));
            }
            
            fHeight = fHeight - _RemoveButtonLayer->getContentSize().height;
            
            _RemoveButtonLayer->setPositionY(fHeight);
            addChild(_RemoveButtonLayer);
        }
        
        setLoading(true);
    }
}

void PixSelectedImageCell::removePressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        
        if(mbln_IsRemoveAction)
            return;
        
        mbln_IsRemoveAction = true;
        
        this->removeChild(_RemoveButtonLayer);
        
        if(_sprite == nullptr)
            return;

        FadeTo *fadeTo = FadeTo::create(0.3f,0);
        EaseSineOut *easeSineOut_1 = EaseSineOut::create(fadeTo);
        ScaleTo *scaleTo = ScaleTo::create(0.3f, 0.0f);
        EaseSineOut *easeSineOut_2 = EaseSineOut::create(scaleTo);
        CallFunc *callFunc = CallFunc::create(CC_CALLBACK_0(PixSelectedImageCell::removeActionFinish, this));
        
        Sequence *sequence = Sequence::create(easeSineOut_2, callFunc, NULL);
        
        _sprite->runAction(easeSineOut_1);
        this->runAction(sequence);
    }
}

void PixSelectedImageCell::removeActionFinish()
{
    if(mModel)
    {
        mModel->setSelected(false);
        
        if(mModel->getDateCell())
        {
            PixSelectDateCell *dateCell = dynamic_cast<PixSelectDateCell*>(mModel->getDateCell());
            
            if(dateCell)
            {
                dateCell->checkSelectAlldateItem();
            }
        }
    }
    
    if(mDelegate)
    {
        mDelegate->clickRemoveButton(this);
    }
}



