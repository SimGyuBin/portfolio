//
//  PixEditAlbumLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 23..
//
//

#include "PixEditAlbumLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixImageCreateManager.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Network/PixHttpClient.h"
#include "../Scene/PixSmartAlbumLevel.h"
#include "../Widget/PixEditAlbumCell.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUtils.h"
#include "../UIControl/PixToast.h"
#include "../UIControl/PixEditSprite.h"
#include "../Scene/PixApp.h"
#include "../Util/PixTime.h"
#include "../Util/PixMath.h"
#include "../Common.h"

PixEditAlbumLayer* PixEditAlbumLayer::create(PixSmartMaskBookModel *pModel, int nPage)
{
    PixEditAlbumLayer *editAlbumLayer = new (std::nothrow) PixEditAlbumLayer(pModel,nPage);
    if (editAlbumLayer && editAlbumLayer->init())
    {
        editAlbumLayer->autorelease();
        return editAlbumLayer;
    }
    else
    {
        CC_SAFE_DELETE(editAlbumLayer);
        return nullptr;
    }
}

PixEditAlbumLayer::PixEditAlbumLayer(PixSmartMaskBookModel *pModel, int nPage)
: mDelegate(nullptr)
, mModel(nullptr)
, mn_Page(-1)
, mn_RequestImageCount(0)
, mbln_IsRequestImage(false)
, mbln_IsPlace(false)
, pixHttpClient(nullptr)
, rootNode(nullptr)
, album_effect(nullptr)
, albumcover_effect(nullptr)
, EditAlbumFunctionLayer(nullptr)
, btn_complet(nullptr)
, btn_layout_change(nullptr)
, btn_layout_remove(nullptr)
, EditAlbumImageLayer(nullptr)
, skin(nullptr)
, item(nullptr)
, mask(nullptr)
, photoEditMenuLayer(nullptr)
, photoEditSprite(nullptr)
, changeSprite(nullptr)
, Button_before(nullptr)
, Button_next(nullptr)
, curpagelabel(nullptr)
, gestureRecognizer(nullptr)
{
    this->mModel = pModel;
    this->mn_Page = nPage;
    
    pixHttpClient = PixHttpClient::sharedObject();
    downloaderQueue.clear();
    
    hideLayerFunction = std::bind(&PixEditAlbumLayer::hideEditAlbumLayerAction,this);
}

PixEditAlbumLayer::~PixEditAlbumLayer()
{
    pixHttpClient->cancelRequest(this);
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    EDIT_MANAGER->clear();
    IMAGECREATE_MANAGER->clear();
    
    CC_SAFE_DELETE(gestureRecognizer);
}

void PixEditAlbumLayer::initLayer()
{
    rootNode = CSLoader::createNode("06_EditAlbum/EditAlbumLayer.csb");
    
    if(rootNode)
    {
        album_effect = dynamic_cast<Sprite*>(rootNode->getChildByName("album_effect"));
        
        if(album_effect)
        {
            album_effect->setVisible(false);
            rootNode->reorderChild(album_effect, 100);
        }
        
        albumcover_effect = dynamic_cast<Sprite*>(rootNode->getChildByName("albumcover_effect"));

        if(albumcover_effect)
        {
            albumcover_effect->setVisible(false);
            rootNode->reorderChild(albumcover_effect, 101);
        }
        
        EditAlbumFunctionLayer = dynamic_cast<Node*>(rootNode->getChildByName("EditAlbumFunctionLayer"));
       
        if(EditAlbumFunctionLayer)
        {
            btn_complet = dynamic_cast<ui::Button*>(EditAlbumFunctionLayer->getChildByName("btn_complet"));
            if(btn_complet)
            {
                 btn_complet->addTouchEventListener(CC_CALLBACK_2(PixEditAlbumLayer::btnCompletPressed, this));
            }
            
            btn_layout_change = dynamic_cast<ui::Button*>(EditAlbumFunctionLayer->getChildByName("btn_layout_change"));
  
            if(btn_layout_change)
            {
                btn_layout_change->addTouchEventListener(CC_CALLBACK_2(PixEditAlbumLayer::btnLayoutChangePressed, this));
            }
            
            btn_layout_remove = dynamic_cast<ui::Button*>(EditAlbumFunctionLayer->getChildByName("btn_layout_remove"));
  
            if(btn_layout_remove)
            {
                btn_layout_remove->addTouchEventListener(CC_CALLBACK_2(PixEditAlbumLayer::btnLayoutRemovePressed, this));
            }
        }
        
        Node *layer = dynamic_cast<Node*>(rootNode->getChildByName("EditAlbumImageLayer"));
        
        if(layer)
        {
            EditAlbumImageLayer = ClippingNode::create();
            Sprite *stencil = Sprite::create("EditAlbumLevel/AlbumViewMask_1020X510.png");
            stencil->setAnchorPoint(Vec2::ZERO);
            EditAlbumImageLayer->setStencil(stencil);
            EditAlbumImageLayer->setContentSize(layer->getContentSize());
            EditAlbumImageLayer->setScale(layer->getScale());
            EditAlbumImageLayer->setPosition(layer->getPosition());
            rootNode->addChild(EditAlbumImageLayer,0);
           
            ui::Widget *layout = dynamic_cast<ui::Widget*>(layer->getChildByName("skin"));
            
            if(layout)
            {
                skin = layout->clone();
                EditAlbumImageLayer->addChild(skin, 1);
            }
            
            layout = dynamic_cast<ui::Widget*>(layer->getChildByName("item"));
            
            if(layout)
            {
                item = layout->clone();
                EditAlbumImageLayer->addChild(item, 2);
            }
            
            layout = dynamic_cast<ui::Widget*>(layer->getChildByName("mask"));
            
            if(layout)
            {
                mask = layout->clone();
                EditAlbumImageLayer->addChild(mask, 3);
            }
            
            rootNode->removeChild(layer);
            
            setEditAlbumImageLayer();
            setAlbumEffect();
        }
        
        Button_before = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_before"));
        
        if(btn_complet)
        {
            Button_before->addTouchEventListener(CC_CALLBACK_2(PixEditAlbumLayer::btnbeforePressed, this));
        }
        
        Button_next = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_next"));
        
        if(btn_complet)
        {
            Button_next->addTouchEventListener(CC_CALLBACK_2(PixEditAlbumLayer::btnNextPressed, this));
        }
        
        curpagelabel = dynamic_cast<ui::Text*>(rootNode->getChildByName("curpagelabel"));
        
        setAnchorPoint(Vec2::ZERO);
        
        float fheight = Director::getInstance()->getVisibleSize().height;
        setPosition(Vec2(0,-fheight));
        
        addChild(rootNode);
        
    }
}

void PixEditAlbumLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showEditAlbumLayerAction();
    initGestureRecognizer();
}

void PixEditAlbumLayer::update(float delta)
{
    PixModalLayer::update(delta);

    updateCurPagelabel();
    
    if(mbln_IsRequestImage == true)
    {
        if(mn_RequestImageCount == 0)
        {
            mbln_IsRequestImage = false;
            sendGetImageCompleted();
        }
    }
    
    if(btn_layout_remove)
    {
        int nCurNum = mn_Page - 1;
        
        PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
        
        if(smartMaskPageModel)
        {
            if(smartMaskPageModel->getType() == ITEM_COVER)
            {
                btn_layout_remove->setEnabled(false);
            }
            else
            {
                btn_layout_remove->setEnabled(true);
            }
        }
    }
    
    if(Button_before)
    {
        if(mModel->getSmartMaskPageModeCount() <= 1)
        {
            Button_before->setVisible(false);
        }
        else
        {
            Button_before->setVisible(true);
        }
    }
    
    if(Button_next)
    {
        if(mModel->getSmartMaskPageModeCount() <= 1)
        {
            Button_next->setVisible(false);
        }
        else
        {
            Button_next->setVisible(true);
        }
    }
}

void PixEditAlbumLayer::initGestureRecognizer()
{
    gestureRecognizer = new PixSimpleGestureRecognizer();
    gestureRecognizer->init();
    gestureRecognizer->setGestureHandler(this);
    
    // Enable all gesture kinds
    gestureRecognizer->setTapEnabled(true);
    gestureRecognizer->setDoubleTapEnabled(false);
    gestureRecognizer->setLongPressEnabled(true);
    gestureRecognizer->setPanEnabled(false);
    gestureRecognizer->setPinchEnabled(false);
    gestureRecognizer->setRotationEnabled(false);
    gestureRecognizer->setSwipeEnabled(true);
    
    // Taps will be fired immediately without waiting for double tap
    gestureRecognizer->setTapRequiresDoubleTapRecognitionToFail(false);
    
}

void PixEditAlbumLayer::setEditAlbumImageLayer()
{
    if(mModel == nullptr)
        return;

    PIXAPP->showSmartAlbumLoading();
    EDIT_MANAGER->photoUsedVecClear();
    
    skin->removeAllChildren();
    item->removeAllChildren();
    mask->removeAllChildren();
    
    Director::getInstance()->getTextureCache()->removeAllTextures();
    
    mbln_IsRequestImage = true;
    
    loadSmartLayout();
    loadLocalImage();
}

void PixEditAlbumLayer::loadSmartLayout()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        autolayout* pAutolayout = smartMaskPageModel->getAutolayout();
        
        double nAddPosX = 0;
        double nAddPosY = 0;
        pAutolayout->getAutoLayoutPageModelAddPos(nAddPosX, nAddPosY);
        const double nPageSizeRealWidth = atof(pAutolayout->getPage_size_real_width().c_str());
        const double nPageSizeRealHeight = atof(pAutolayout->getPage_size_real_height().c_str());
        
        int nZOrder = 0;
        
        PixNetworkImageModel *pSkinData = PixNetworkImageModel::create();
        pSkinData->retain();
        
        pSkinData->pos.x = -atof(pAutolayout->getSkin_crop_left_right().c_str());
        pSkinData->pos.y = -atof(pAutolayout->getSkin_crop_top_bottom().c_str());
        pSkinData->size.width = atof(pAutolayout->getSkin_width().c_str());
        pSkinData->size.height = atof(pAutolayout->getSkin_height().c_str());
        pSkinData->parentSize.width = nPageSizeRealWidth;
        pSkinData->parentSize.height = nPageSizeRealHeight;
        
        pSkinData->zOrder = nZOrder++;
        pSkinData->path = smartMaskPageModel->getSkin_url();
        pSkinData->nType = CLIPPING_SKIN;
        ++mn_RequestImageCount;
        sendGetImageUrl_Call(smartMaskPageModel->getSkin_url(), pSkinData);
        
        
        PixNetworkImageModel *pMasklayoutData = PixNetworkImageModel::create();
        pMasklayoutData->retain();
        
        pMasklayoutData->pos.x = 0;
        pMasklayoutData->pos.y = 0;
        pMasklayoutData->size.width = nPageSizeRealWidth;
        pMasklayoutData->size.height = nPageSizeRealHeight;
        pMasklayoutData->parentSize.width = nPageSizeRealWidth;
        pMasklayoutData->parentSize.height = nPageSizeRealHeight;
        
        pMasklayoutData->zOrder = 10000 + nZOrder++;
        pMasklayoutData->nTag = pMasklayoutData->zOrder;
        pMasklayoutData->path = smartMaskPageModel->getLayout_res_mask_layout();
        pMasklayoutData->nType = CLIPPING_MASKLAYOUT;
        ++mn_RequestImageCount;
        sendGetImageUrl_Call(smartMaskPageModel->getLayout_res_mask_layout(), pMasklayoutData);
        
        int nCount = smartMaskPageModel->getSmartMaskImageModelCount();
        
        nZOrder = 100;
        
        for(int i = 0; i < nCount; ++i)
        {
            PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModel(i);
            
            if(smartMaskImageModel)
            {
                
                PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                
                if(smartImageModel)
                {
                    PixNetworkImageModel *pModel = PixNetworkImageModel::create();
                    pModel->retain();
                    
                    pModel->size.width = atof(smartMaskImageModel->getItem_width().c_str());
                    pModel->size.height = atof(smartMaskImageModel->getItem_height().c_str());
                    pModel->parentSize.width = nPageSizeRealWidth;
                    pModel->parentSize.height = nPageSizeRealHeight;
                    pModel->pos.x = atof(smartMaskImageModel->getItem_x().c_str()) + nAddPosX;
                    pModel->pos.y = atof(smartMaskImageModel->getItem_y().c_str()) + nAddPosY;
                    pModel->rotate = atof(smartMaskImageModel->getItem_rotate().c_str());
                    pModel->zOrder = nZOrder++;
                    pModel->nTag = smartImageModel->getSmartIndex();
                    pModel->path = smartMaskImageModel->getLayout_res_mask_mask();
                    pModel->nType = CLIPPING_MASK;
                    
                    ++mn_RequestImageCount;
                    sendGetImageUrl_Call(smartMaskImageModel->getLayout_res_mask_mask(), pModel);
                }
            }
        }
    }
}
void PixEditAlbumLayer::loadLocalImage()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);

    if(smartMaskPageModel)
    {
        autolayout* pAutolayout = smartMaskPageModel->getAutolayout();
        
        double nAddPosX = 0;
        double nAddPosY = 0;
        pAutolayout->getAutoLayoutPageModelAddPos(nAddPosX, nAddPosY);
        const double nPageSizeRealWidth = atof(pAutolayout->getPage_size_real_width().c_str());
        const double nPageSizeRealHeight = atof(pAutolayout->getPage_size_real_height().c_str());
        
        int nCount = smartMaskPageModel->getSmartMaskImageModelCount();
        
        int nZOrder = 100;
        
        for(int i = 0; i < nCount; ++i)
        {
            PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModel(i);
            
            if(smartMaskImageModel)
            {
                PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                
                if(smartImageModel)
                {
                    PixNetworkImageModel *pModel = PixNetworkImageModel::create();
                    pModel->retain();
                    
                    pModel->size.width = atof(smartMaskImageModel->getItem_width().c_str());
                    pModel->size.height = atof(smartMaskImageModel->getItem_height().c_str());
                    pModel->parentSize.width = nPageSizeRealWidth;
                    pModel->parentSize.height = nPageSizeRealHeight;
                    pModel->pos.x = atof(smartMaskImageModel->getItem_x().c_str()) + nAddPosX;
                    pModel->pos.y = atof(smartMaskImageModel->getItem_y().c_str()) + nAddPosY;
                    pModel->rotate = atof(smartMaskImageModel->getItem_rotate().c_str());
                    pModel->path = smartImageModel->getPath();
                    pModel->nOrientation = smartImageModel->getOrientation();
                    pModel->nType = CLIPPING_IMAGE;
                    pModel->zOrder = nZOrder++;
                    pModel->nTag = smartImageModel->getSmartIndex();
                    pModel->date = smartImageModel->getDate();
                    ++mn_RequestImageCount;
                    
                    pModel->addLocalImageAsync(CC_CALLBACK_2(PixEditAlbumLayer::completeTexture, this));
                }
            }
        }
    }
}

void PixEditAlbumLayer::completeTexture(Texture2D *pTexture, void *pUserData)
{
    PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
    
    if(pModel == nullptr)
    {
        --mn_RequestImageCount;
        return;
    }
    
    if(pTexture == nullptr)
    {
        --mn_RequestImageCount;
        pModel->release();
        return;
    }
    
    if(EditAlbumImageLayer == nullptr)
    {
        --mn_RequestImageCount;
        pModel->release();
        return;
    }
    
    float fScaleX = 1.0f;
    float fScaleY = 1.0f;
    
    Size albumSize = EditAlbumImageLayer->getContentSize();
    Vec2 ratio = Vec2 ( albumSize.width / pModel->parentSize.width, albumSize.height / pModel->parentSize.height );
    Vec2 pos = Vec2( pModel->pos.x * ratio.x, albumSize.height - (pModel->pos.y * ratio.y) );
    Size maskSize = Size(pModel->size.width * ratio.x, pModel->size.height * ratio.y );
    
    if (pModel->nType == CLIPPING_MASK)
    {
        pos.x += maskSize.width / 2;
        pos.y -= maskSize.height / 2;
        
        fScaleX = maskSize.width / pTexture->getContentSize().width;
        fScaleY = maskSize.height / pTexture->getContentSize().height;
        
        PixEditSprite *editSprite = dynamic_cast<PixEditSprite*>(item->getChildByTag(pModel->nTag));
        
        if(editSprite)
        {
            editSprite->setMaskSpriteTexture(pTexture, fScaleX, fScaleY);
        }
        else
        {
            editSprite = PixEditSprite::create(pModel, pModel->nSmartIndex, albumSize, EditAlbumImageLayer);
            editSprite->setMaskSpriteTexture(pTexture, fScaleX, fScaleY);
            editSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            editSprite->setPosition(pos);
            editSprite->setRotation(pModel->rotate);
            editSprite->setTag(pModel->nTag);
            
            item->addChild(editSprite, pModel->zOrder);
        }
    }
    else if(pModel->nType == CLIPPING_IMAGE)
    {
        Size size = pTexture->getContentSize();
        
        float fScale = 1.0f;
        fScaleX = maskSize.width / size.width;
        fScaleY = maskSize.height / size.height;
        if (fScaleX > fScaleY)
        {
            fScale = fScaleX;
        }
        else
        {
            fScale = fScaleY;
        }
        
        pos.x += maskSize.width / 2;
        pos.y -= maskSize.height / 2;
        
        PixEditSprite *editSprite = dynamic_cast<PixEditSprite*>(item->getChildByTag(pModel->nTag));
        
        if(editSprite)
        {
            editSprite->setImageSpriteTexture(pTexture, fScale, fScale);
        }
        else
        {
            editSprite = PixEditSprite::create(pModel, pModel->nSmartIndex, albumSize, EditAlbumImageLayer);
            editSprite->setImageSpriteTexture(pTexture, fScale, fScale);
            editSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            editSprite->setPosition(pos);
            editSprite->setRotation(pModel->rotate);
            editSprite->setTag(pModel->nTag);
            
            item->addChild(editSprite, pModel->zOrder);
        }
        
        int nCurNum = mn_Page - 1;
        
        PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
        
        if(smartMaskPageModel)
        {
            PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModelByPath(pModel->path);
            
            if(smartMaskImageModel)
            {
                PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                
                if(smartImageModel)
                {
                    editSprite->setSmartMaskImageModel(smartMaskImageModel);
                    
                    PhotoEditInfo *info = new PhotoEditInfo();
                    info->setSmartImageModel(smartImageModel);
                    EDIT_MANAGER->pushPhotoUsed(info);
                }
            }
        }
    }
    else
    {
        Sprite *sprite = Sprite::createWithTexture(pTexture);
        sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        Size size = sprite->getContentSize();
        
        if( pModel->parentSize.width == 0 || pModel->parentSize.height == 0 )
        {
            fScaleX = albumSize.width / size.width;
            fScaleY = albumSize.height / size.height;
            
            sprite->setPosition(Vec2(albumSize.width * 0.5f, albumSize.height * 0.5f));
            sprite->setScale(fScaleX, fScaleY);
            sprite->setRotation(pModel->rotate);
            sprite->setTag(pModel->nTag);
            
            if (pModel->nType == CLIPPING_SKIN)
            {
                skin->addChild(sprite, pModel->zOrder);
            }
            else if (pModel->nType == CLIPPING_MASKLAYOUT)
            {
                mask->addChild(sprite, pModel->zOrder);
            }
            else
            {
//                EditAlbumImageLayer->addChild(sprite);
            }
        }
        else
        {
            Size albumSize = EditAlbumImageLayer->getContentSize();
            
            pos.x += maskSize.width / 2;
            pos.y -= maskSize.height / 2;
            
            fScaleX = maskSize.width / pTexture->getContentSize().width;
            fScaleY = maskSize.height / pTexture->getContentSize().height;
            
            sprite->setPosition(pos);
            sprite->setScale(fScaleX, fScaleY);
            sprite->setRotation(pModel->rotate);
            sprite->setTag(pModel->nTag);
            
            if (pModel->nType == CLIPPING_SKIN)
            {
                skin->addChild(sprite, pModel->zOrder);
            }
            else if (pModel->nType == CLIPPING_MASKLAYOUT)
            {
                mask->addChild(sprite, pModel->zOrder);
            }
            else
            {
//                EditAlbumImageLayer->addChild(sprite);
            }
        }
    }
    
    --mn_RequestImageCount;
    pModel->release();
}


void PixEditAlbumLayer::sendGetImageUrl_Call(const std::string &url, void *pUserData)
{
    std::vector<std::string> tokens;
    PixStringUtils::getTokens(tokens, url, "/");
    
    std::string filename;
    filename.clear();
    
    if(!tokens.empty())
    {
        filename = tokens[tokens.size() - 1];
    }
    
    std::string path = FileUtils::getInstance()->getWritablePath();
    std::string fullPath =  path + "Smartlayout/" + filename;
    
    bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
    
    if(isFileExist)
    {
        PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
        
        if(pModel)
        {
            pModel->path = fullPath;
            pModel->addImageAsync(CC_CALLBACK_2(PixEditAlbumLayer::completeTexture, this));
        }
    }
    else
    {
        PixDownloader *pixDownloader = PixDownloader::create(url.c_str(), SMARTLAYOUT_PATH, this);
        pixDownloader->setConnectionTimeout(3);
        pixDownloader->setUserData(pUserData);
        pixDownloader->createDownloadDataTask(fullPath);
    }
}

void PixEditAlbumLayer::sendGetImageCompleted()
{
    PIXAPP->hideLoading();
    setAlbumEffect();
}

PixEditSprite* PixEditAlbumLayer::addLocalImageSprite(PixNetworkImageModel *networkImageModel, ClippingNode* clippingNode, int nSmartIndex)
{
    if(clippingNode == nullptr)
        return nullptr;
    
    if(mModel == nullptr)
        return nullptr;
    
    PixNetworkImageModel *pModel = networkImageModel;
    
    Size albumSize = Size::ZERO;
    
    if(EditAlbumImageLayer)
    {
        albumSize = EditAlbumImageLayer->getContentSize();
    }
    
    PixEditSprite *editSprite = PixEditSprite::create(pModel, nSmartIndex, albumSize, EditAlbumImageLayer);
    
    if(editSprite)
    {
        if (pModel->nType == CLIPPING_MASK)
        {
            clippingNode->setStencil(editSprite);
            clippingNode->addChild(editSprite, 1);
        }
        else if(pModel->nType == CLIPPING_IMAGE)
        {
            clippingNode->addChild(editSprite, 2);
        }
        else
        {
            clippingNode->addChild(editSprite, 2);
        }
    }
    
    return editSprite;
}

void PixEditAlbumLayer::addEditMenuLayer(PixEditSprite *pSprite)
{
    if(pSprite == nullptr)
        return;
    
    int nCurNum = mn_Page - 1;
    
    EDIT_MENU_TYPE type = MENU_NONE;
    
    PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        if(smartMaskPageModel->getSmartMaskImageModelCount() <= 1)
        {
            type = MENU_ONE_BUTTON;
        }
        else
        {
            type = MENU_TWO_BUTTON;
        }
    }
    
    Vec2 pos = pSprite->getPosition();
    photoEditMenuLayer = PixPhotoEditMenuLayer::create(pSprite, type);
    photoEditMenuLayer->setDelegate(this);
    photoEditMenuLayer->setTag(MENU_TAG);
    
    Size menuSize = photoEditMenuLayer->getContentSize();
    
    Vec2 menuPos = pos;
    menuPos.x += EditAlbumImageLayer->getPosition().x;
    menuPos.y += EditAlbumImageLayer->getPosition().y;
    menuPos.x -= menuSize.width / 2;
    menuPos.y -= menuSize.height;
    
    Rect r1 = EditAlbumImageLayer->getBoundingBox();
    Rect r2 = Rect(menuPos,menuSize);
    
    if(r1.getMaxX() < r2.getMaxX())
    {
        menuPos.x -= menuSize.width / 3;
        photoEditMenuLayer->setRight();
    }
    else if(r1.getMinX() > r2.getMinX())
    {
        menuPos.x += menuSize.width / 3;
        photoEditMenuLayer->setLeft();
    }
    else
    {
        photoEditMenuLayer->setCenter();
    }
    
    photoEditMenuLayer->setPosition(menuPos);
    addChild(photoEditMenuLayer);
    
}

void PixEditAlbumLayer::layoutRemove()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        mModel->removeSmartMaskPageModel(nCurNum);
    }
    
    goPreEditPage(false);
 
    mbln_IsPlace = true;
    photoEditSprite = nullptr;
}

void PixEditAlbumLayer::reomveEditSprite(PhotoEditInfo *info)
{
    if(info == nullptr)
        return;

    EDIT_MANAGER->pushPhotoUnused(info);
}

void PixEditAlbumLayer::goPreEditPage(bool blnCheckEmpty)
{
    if(blnCheckEmpty)
    {
        if(checkPhotoEmpty())
        {
            return;
        }
    }
    
    if(photoEditMenuLayer)
    {
        removeChild(photoEditMenuLayer);
    }
    
    if(photoEditSprite)
    {
//        photoEditSprite->selectLayerVisible(false);
//        photoEditSprite->setChangeMode(false);
    }
    
    photoEditSprite = nullptr;
    
    EDIT_MANAGER->photoUsedVecClear();
    
    if(mn_Page > 1)
    {
        --mn_Page;
    }
    else
    {
        mn_Page = mModel->getSmartMaskPageModeCount();
    }
    
    mbln_IsPlace = true;
    setEditAlbumImageLayer();
}

void PixEditAlbumLayer::goNextEditPage(bool blnCheckEmpty)
{
    if(blnCheckEmpty)
    {
        if(checkPhotoEmpty())
        {
            return;
        }
    }
    
    if(photoEditMenuLayer)
    {
        removeChild(photoEditMenuLayer);
    }
    
    if(photoEditSprite)
    {
//        photoEditSprite->selectLayerVisible(false);
//        photoEditSprite->setChangeMode(false);
    }
    
    photoEditSprite = nullptr;
    
    EDIT_MANAGER->photoUsedVecClear();
    
    if(mn_Page < mModel->getSmartMaskPageModeCount())
    {
        ++mn_Page;
    }
    else
    {
        mn_Page = 1;
    }
    
    mbln_IsPlace = true;
    setEditAlbumImageLayer();
}

bool PixEditAlbumLayer::checkPhotoEmpty()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        int nCount = smartMaskPageModel->getSmartMaskImageModelCount();
        
        for (int i = 0; i < nCount; ++i)
        {
            PixSmartMaskImageModel *smartMaskImageModel = smartMaskPageModel->getSmartMaskImageModel(i);
            
            if(smartMaskImageModel)
            {
                if(smartMaskImageModel->getSmartImageModel() == nullptr)
                {
                    std::string msg = "현재 페이지에 빈 사진틀이 있습니다.\n사진을 넣거나 레이아웃을 변경해주세요.";
                    PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
                    return true;
                }
            }
        }
    }
    
    return false;
}

void PixEditAlbumLayer::updateCurPagelabel()
{
    if(curpagelabel)
    {
        curpagelabel->setString(StringUtils::format("%d / %d",mn_Page,mModel->getSmartMaskPageModeCount()));
    }
}

void PixEditAlbumLayer::setAlbumEffect()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        if(smartMaskPageModel->getType() == ITEM_COVER)
        {
            album_effect->setVisible(false);
            albumcover_effect->setVisible(true);
        }
        else
        {
            album_effect->setVisible(true);
            albumcover_effect->setVisible(false);
        }
    }
}

void PixEditAlbumLayer::removeChangeSprite()
{
    if(changeSprite)
    {
        removeChild(changeSprite);
        changeSprite = nullptr;
    }
}

void PixEditAlbumLayer::editSpriteChangeEvent(PixEditSprite *pSprite)
{
    if(pSprite)
    {
        if(photoEditSprite)
        {
            if(photoEditSprite == pSprite)
            {
                mbln_IsPlace = true;
                photoEditSprite->setChangeMode(false);
                photoEditSprite = nullptr;
                return;
            }
            
            if(photoEditSprite->isChange())
            {
                Sprite *changeSprite = pSprite->getImageSprite();
                pSprite->changeImageSprite(photoEditSprite->getImageSprite());
                photoEditSprite->changeImageSprite(changeSprite);
                
                PixSmartMaskImageModel *modelA = photoEditSprite->getSmartMaskImageModel();
                PixSmartMaskImageModel *modelB = pSprite->getSmartMaskImageModel();
                
                if(modelA && modelB)
                {
                    PixSmartImageModel *changeImageModel = modelA->getSmartImageModel();
                    modelA->setSmartImageModel(modelB->getSmartImageModel());
                    modelB->setSmartImageModel(changeImageModel);
                }
                
                mbln_IsPlace = true;
                photoEditSprite->setChangeMode(false);
                photoEditSprite = nullptr;
            }
        }
    }
}

#pragma mark - Button

void PixEditAlbumLayer::btnCompletPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
     
        if(checkPhotoEmpty())
            return;
        
        if(photoEditSprite)
        {
            if(photoEditMenuLayer)
            {
//                photoEditSprite->selectLayerVisible(false);
                removeChild(photoEditMenuLayer);
            }
            
//            photoEditSprite->setChangeMode(false);
        }
        
        hideEditAlbumLayerAction();
    }
}

void PixEditAlbumLayer::btnLayoutChangePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {

        if(EDIT_MANAGER->getPhotoUsedCount() <= 0)
        {
            std::string msg = "자동 레이아웃을 구성할 사진이 없습니다.\n사진을 넣어주세요.";
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        else
        {
            if(photoEditMenuLayer)
            {
                removeChild(photoEditMenuLayer);
            }
            
            layoutChange();
            photoEditSprite = nullptr;
        }
    }
}

void PixEditAlbumLayer::btnLayoutRemovePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        std::string msg = CODE_NAME("P40");
        
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixEditAlbumLayer::layoutRemove));
    }
}

void PixEditAlbumLayer::btnbeforePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        goPreEditPage();
    }
}

void PixEditAlbumLayer::btnNextPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        goNextEditPage();
    }
}

#pragma mark - Action

void PixEditAlbumLayer::showEditAlbumPhotoLayerAction()
{
    if(EditAlbumImageLayer == nullptr || album_effect == nullptr)
        return;
    
    FadeIn *fadeIn = FadeIn::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixEditAlbumLayer::showEditAlbumPhotoLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);
    
    EditAlbumImageLayer->runAction(sequence);
    
    fadeIn = FadeIn::create(0.3f);
    
    album_effect->runAction(sequence);
}

void PixEditAlbumLayer::hideEditAlbumPhotoLayerAction()
{
    if(EditAlbumImageLayer == nullptr || album_effect == nullptr)
        return;
    
    FadeOut *fadeOut = FadeOut::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixEditAlbumLayer::hideEditAlbumPhotoLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeOut, callFuncN, NULL);
    
    EditAlbumImageLayer->runAction(sequence);
    
    fadeOut = FadeOut::create(0.3f);
    
    album_effect->runAction(sequence);
}

void PixEditAlbumLayer::showEditAlbumPhotoLayerActionFinish(Node* node)
{
    
}

void PixEditAlbumLayer::hideEditAlbumPhotoLayerActionFinish(Node* node)
{
    
}

void PixEditAlbumLayer::showEditAlbumLayerAction()
{
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixEditAlbumLayer::showEditAlbumLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixEditAlbumLayer::hideEditAlbumLayerAction()
{
    float fheight = Director::getInstance()->getVisibleSize().height;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-fheight));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixEditAlbumLayer::hideEditAlbumLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixEditAlbumLayer::showEditAlbumLayerActionFinish(Node* node)
{
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixEditAlbumLayer::hideEditAlbumLayerActionFinish(Node* node)
{
    if(getParent())
    {
        if(mDelegate)
        {
            if(mbln_IsPlace)
            {
                mDelegate->hideLayerActionFinish(this);
            }
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Touch & Touches

bool PixEditAlbumLayer::onTouchBegan(Touch* touch, Event* event)
{
    if(gestureRecognizer)
    {
        gestureRecognizer->onTouchBegan(touch, event);
    }
    
    if(item)
    {
        ssize_t count = item->getChildren().size();
        
        for(ssize_t i = 0; i < count; ++i)
        {
            PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
            
            if(pSprite)
            {
                if(NodeContainsPoint(pSprite, touch->getLocation()))
                {
                    pSprite->selectLayerVisible(true);
                }
            }
        }
    }
    
    return PixModalLayer::onTouchBegan(touch,event);
}

void PixEditAlbumLayer::onTouchMoved(Touch* touch, Event* event)
{
    PixModalLayer::onTouchMoved(touch,event);
    
    if(gestureRecognizer)
    {
        gestureRecognizer->onTouchMoved(touch, event);
    }
    
    if(changeSprite)
    {
        Vec2 pos = this->convertToNodeSpace(touch->getLocation());
        pos = Vec2(pos.x - (changeSprite->getContentSize().width * 0.5f),
                   pos.y - (changeSprite->getContentSize().height * 0.5f));
        changeSprite->setPosition(pos);
        
        if(item)
        {
            std::deque<photoChangeInfo> photoChangeInfoVec;
            
            ssize_t count = item->getChildren().size();
            
            for(ssize_t i = 0; i < count; ++i)
            {
                PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
                
                if(pSprite)
                {
                    pSprite->selectLayerVisible(false);
                    
                    Rect rect = pSprite->getBoundingBox();
                    
                    if(rect.intersectsRect(changeSprite->getBoundingBox()))
                    {
                        if(photoEditSprite != pSprite)
                        {
                            photoChangeInfo info;
                            info.editSprite = pSprite;
                            info.unionWithRect = rect.unionWithRect(changeSprite->getBoundingBox());
                            photoChangeInfoVec.push_back(info);
                        }
                    }
                }
            }
            
            
            if(!photoChangeInfoVec.empty())
            {
                photoChangeInfoSort(photoChangeInfoVec,compareUnionWithRectHigh);
                photoChangeInfo info = photoChangeInfoVec.front();
                info.editSprite->selectLayerVisible(true);
            }
        }
    }
    else
    {
        if(item)
        {
            ssize_t count = item->getChildren().size();
            
            for(ssize_t i = 0; i < count; ++i)
            {
                PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
                
                if(pSprite)
                {
                    if(NodeContainsPoint(pSprite, touch->getLocation()))
                    {
                        if(pSprite->isChange() == false)
                        {
                            pSprite->selectLayerVisible(true);
                        }
                    }
                    else
                    {
                        if(photoEditSprite)
                        {
                            if(pSprite != photoEditSprite)
                            {
                                pSprite->selectLayerVisible(false);
                            }
                        }
                        else
                        {
                            pSprite->selectLayerVisible(false);
                        }
                    }
                }
            }
        }
    }
}

void PixEditAlbumLayer::onTouchEnded(Touch* touch, Event* event)
{
    PixModalLayer::onTouchEnded(touch,event);
    
    if(gestureRecognizer)
    {
        gestureRecognizer->onTouchEnded(touch, event);
    }
    
    if(changeSprite && photoEditSprite)
    {
        Vec2 pos = this->convertToNodeSpace(touch->getLocation());
        pos = Vec2(pos.x - (changeSprite->getContentSize().width * 0.5f),
                   pos.y - (changeSprite->getContentSize().height * 0.5f));
        changeSprite->setPosition(pos);
        
        if(item)
        {
            std::deque<photoChangeInfo> photoChangeInfoVec;
            
            ssize_t count = item->getChildren().size();
            
            for(ssize_t i = 0; i < count; ++i)
            {
                PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
                
                if(pSprite)
                {
                    pSprite->selectLayerVisible(false);
                    
                    Rect rect = pSprite->getBoundingBox();
                    
                    if(rect.intersectsRect(changeSprite->getBoundingBox()))
                    {
                        if(photoEditSprite != pSprite)
                        {
                            photoChangeInfo info;
                            info.editSprite = pSprite;
                            info.unionWithRect = rect.unionWithRect(changeSprite->getBoundingBox());
                            photoChangeInfoVec.push_back(info);
                        }
                    }
                }
            }
            
            
            if(!photoChangeInfoVec.empty())
            {
                photoChangeInfoSort(photoChangeInfoVec,compareUnionWithRectHigh);
                photoChangeInfo info = photoChangeInfoVec.front();
                editSpriteChangeEvent(info.editSprite);
            }
        }
    }
    
    if(photoEditSprite)
    {
        photoEditSprite->setChangeMode(false);
    }
    
    removeChangeSprite();
}

void PixEditAlbumLayer::onTouchCancelled(Touch* touch, Event* event)
{
    PixModalLayer::onTouchCancelled(touch,event);
    
    if(gestureRecognizer)
    {
        gestureRecognizer->onTouchCancelled(touch, event);
    }
    
    removeChangeSprite();
}

#pragma mark - GestureHandler

bool PixEditAlbumLayer::onGestureTap(PixGestureTap* gesture)
{
    bool blnIsAddEditMenuLayer = false;
    
    if(item)
    {
        ssize_t count = item->getChildren().size();
        
        for(ssize_t i = 0; i < count; ++i)
        {
            PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
            
            if(pSprite)
            {
                if(NodeContainsPoint(pSprite, gesture->getLocation()))
                {
                    bool blnIsAdd = true;
                    
                    if(photoEditSprite)
                    {
                        if(photoEditSprite->isChange())
                        {
                            editSpriteChangeEvent(pSprite);
                            pSprite->selectLayerVisible(false);
                            blnIsAdd = false;
                        }
                    }
                    
                    if(blnIsAdd)
                    {
                        if(photoEditSprite != pSprite)
                        {
                            if(photoEditSprite)
                            {
                                if(photoEditSprite->isChange())
                                {
                                    photoEditSprite->setChangeMode(false);
                                }
                                
                                if(photoEditMenuLayer)
                                {
                                    photoEditSprite->selectLayerVisible(false);
                                    removeChild(photoEditMenuLayer);
                                    photoEditSprite = nullptr;
                                }
                            }
                            
                            pSprite->selectLayerVisible(true);
                            addEditMenuLayer(pSprite);
                            photoEditSprite = pSprite;
                            blnIsAddEditMenuLayer = true;
                        }
                    }
                }
            }
        }
    }
    
    if(photoEditSprite && blnIsAddEditMenuLayer == false)
    {
        if(photoEditSprite->isChange())
        {
            photoEditSprite->setChangeMode(false);
        }
        
        if(photoEditMenuLayer)
        {
            photoEditSprite->selectLayerVisible(false);
            removeChild(photoEditMenuLayer);
            photoEditSprite = nullptr;
        }
    }
    
    if(btn_layout_remove && NodeContainsPoint(btn_layout_remove, gesture->getLocation()))
    {
        if(btn_layout_remove->isEnabled() == false)
        {
            int nCurNum = mn_Page - 1;
            
            PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
            
            if(smartMaskPageModel)
            {
                if(smartMaskPageModel->getType() == ITEM_COVER)
                {
                    std::string msg = CODE_NAME("T06");
                    PixToast::showToast(msg.c_str(), PixToast::toastPos(), 3.0f);
                    return false;
                }
            }
        }
    }

    return false;
}

bool PixEditAlbumLayer::onGestureLongPress(PixGestureLongPress* gesture)
{
    if(item)
    {
        ssize_t count = item->getChildren().size();
        
        if(count <= 1)
        {
            return false;
        }
        
        for(ssize_t i = 0; i < count; ++i)
        {
            PixEditSprite *pSprite = dynamic_cast<PixEditSprite*>(item->getChildren().at(i));
            
            if(pSprite)
            {
                if(NodeContainsPoint(pSprite, gesture->getLocation()))
                {
                    if(pSprite->getImageSprite() == nullptr)
                        return false;
                    
                    changeSprite = Sprite::createWithTexture(pSprite->getImageSprite()->getTexture());
                    
                    
                    Size size = Size(CHANGESPRITE_SIZE,CHANGESPRITE_SIZE);
                   
                    float fWidth = changeSprite->getContentSize().width;
                    float fHeight = changeSprite->getContentSize().height;
                    
                    Size imageSize = changeSprite->getContentSize();
                    
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
                    
                    changeSprite->setScale(fScale);
                    changeSprite->setContentSize(Size(fWidth * fScale, fHeight * fScale));
                    changeSprite->setOpacity(255.0f * 0.9f);
                    changeSprite->setAnchorPoint(Vec2(0.5f,0.5f));
                    
                    Vec2 pos = this->convertToNodeSpace(gesture->getLocation());
                    pos = Vec2(pos.x - (changeSprite->getContentSize().width * 0.5f),
                               pos.y - (changeSprite->getContentSize().height * 0.5f));
                    changeSprite->setPosition(pos);
                    
                    addChild(changeSprite,10000);

                    photoEditSprite = pSprite;
                    photoEditSprite->selectLayerVisible(false);
                    photoEditSprite->setChangeMode(true);
                }
            }
        }
    }
    
    return false;
}

bool PixEditAlbumLayer::onGesturePan(PixGesturePan* gesture)
{
     return false;
}

bool PixEditAlbumLayer::onGesturePinch(PixGesturePinch* gesture)
{
     return false;
}

bool PixEditAlbumLayer::onGestureRotation(PixGestureRotation* gesture)
{
     return false;
}

bool PixEditAlbumLayer::onGestureSwipe(PixGestureSwipe* gesture)
{
    if(gesture->getLength() < 50.0f)
        return false;
    
    SwipeDirection dir = gesture->getDirection();
    
    if (dir == SwipeDirectionNorth)
    {
        CCLOG("Swipe detected: NORTH");
        
        if(EDIT_MANAGER->getPhotoUsedCount() <= 0)
        {
            std::string msg = "자동 레이아웃을 구성할 사진이 없습니다.\n사진을 넣어주세요.";
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        else
        {
            if(photoEditMenuLayer)
            {
                removeChild(photoEditMenuLayer);
            }
            
            layoutChange();
            photoEditSprite = nullptr;
        }
    }
    else if (dir == SwipeDirectionNorthEast)
    {
        CCLOG("Swipe detected: NORTH EAST");
    }
    else if (dir == SwipeDirectionEast)
    {
        CCLOG("Swipe detected: EAST");
        goPreEditPage();
    }
    else if (dir == SwipeDirectionSouthEast)
    {
        CCLOG("Swipe detected: SOUTH EAST");
    }
    else if (dir == SwipeDirectionSouth)
    {
        CCLOG("Swipe detected: SOUTH");
       
        if(EDIT_MANAGER->getPhotoUsedCount() <= 0)
        {
            std::string msg = "자동 레이아웃을 구성할 사진이 없습니다.\n사진을 넣어주세요.";
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
        else
        {
            if(photoEditMenuLayer)
            {
                removeChild(photoEditMenuLayer);
            }
            
            layoutChange();
            photoEditSprite = nullptr;
        }
        
    }
    else if (dir == SwipeDirectionSouthWest)
    {
        CCLOG("Swipe detected: SOUTH WEST");
    }
    else if (dir == SwipeDirectionWest)
    {
        CCLOG("Swipe detected: WEST");
        goNextEditPage();
        
    }
    else if (dir == SwipeDirectionNorthWest)
    {
        CCLOG("Swipe detected: NORTH WEST");
    }

    return false;
}

#pragma mark - Delegate

void PixEditAlbumLayer::photoEditMenuLayerChangePressed(PixEditSprite* pSprite)
{
    if(pSprite == nullptr)
        return;
    
    photoEditMenuLayer = nullptr;
    
    if(photoEditSprite)
    {
        photoEditSprite->selectLayerVisible(false);
        photoEditSprite->setChangeMode(true);
    }
}

void PixEditAlbumLayer::photoEditMenuLayerReplacePressed(PixEditSprite *pSprite)
{
    if(pSprite == nullptr)
        return;
    
    photoEditMenuLayer = nullptr;
    
    if(photoEditSprite)
    {
        photoEditSprite->selectLayerVisible(false);
        
        PixAddPhotoSelectLayer *addPhotoSelectLayer = PixAddPhotoSelectLayer::create();
        addPhotoSelectLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(addPhotoSelectLayer);
    }

    
    // 편집 레이어 넘어가는 소스
    
//    if(pSprite == nullptr)
//        return;
//    
//    photoEditMenuLayer = nullptr;
//    
//    if(photoEditSprite)
//    {
//        photoEditSprite->selectLayerVisible(false);
//    }
//    
//    photoEditSprite = nullptr;
//    
//    int nCurNum = mn_Page - 1;
//    
//    PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
//    
//    if(smartMaskPageModel)
//    {
//        int nIndex = -1;
//        
//        PixSmartMaskImageModel *smartMaskImageModel = pSprite->getSmartMaskImageModel();
//                    
//        if(smartMaskImageModel)
//        {
//            PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
//            
//            if(smartImageModel)
//            {
//                nIndex = smartMaskPageModel->getSmartMaskImageSeqByPath(smartImageModel->getPath());
//            }
//        }
//
//        PixPhotoEditingLayer *photoEditingLayer = PixPhotoEditingLayer::create(smartMaskPageModel, nIndex);
//        photoEditingLayer->setDelegate(this);
//        
//        Director::getInstance()->getRunningScene()->addChild(photoEditingLayer);
//    }
}

void PixEditAlbumLayer::photoEditMenuLayerDeletePressed(PixEditSprite *pSprite)
{
    if(pSprite == nullptr)
        return;
    
    photoEditMenuLayer = nullptr;
    
    if(photoEditSprite)
    {
        photoEditSprite->selectLayerVisible(false);
    }
    
//    pSprite->createEmptyTexture();
    
    PixSmartMaskImageModel *smartMaskImageModel = pSprite->getSmartMaskImageModel();
            
    if(smartMaskImageModel)
    {
        PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
        
        if(smartImageModel)
        {
            PixSmartImageModel *smartImageModelClone = smartImageModel->clone();
            
            PhotoEditInfo *reomveInfo = new PhotoEditInfo();
            reomveInfo->setSmartImageModel(smartImageModelClone);
            
            int nCount = EDIT_MANAGER->getPhotoUsedCount();
            
            for(int i = 0; i < nCount; ++i)
            {
                PhotoEditInfo *usedInfo = EDIT_MANAGER->getPhotoUsed(i);
                
                if(usedInfo)
                {
                    PixSmartImageModel *smartImageModel = usedInfo->getSmartImageModel();
                    
                    if(smartImageModel)
                    {
                        if(smartImageModel->getPath().compare(smartImageModelClone->getPath()) == 0
                           && smartImageModel->getSmartIndex() == smartImageModelClone->getSmartIndex())
                        {
                            EDIT_MANAGER->reomvePhotoUsed(usedInfo);
                            break;
                        }
                    }
                }
            }
            
//            reomveEditSprite(reomveInfo);
            EDIT_MANAGER->pushPhotoUnused(reomveInfo);
            CC_SAFE_DELETE(smartImageModel);
            smartMaskImageModel->setSmartImageModel(nullptr);
            layoutChange();
        }
        
        mbln_IsPlace = true;
    }
    
//    runActionLayoutChange();
//    std::string msg = "사진 수와 맞는 레이아웃으로 변경해 보세요.";
//    PixToast::showToast(msg.c_str());
    
    photoEditSprite = nullptr;
}

void PixEditAlbumLayer::photoInsertLayerPressed(PixEditSprite* pSprite)
{
    photoEditSprite->setChangeMode(true);
}

void PixEditAlbumLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixEditAlbumLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixEditAlbumLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
    
    if(pTarget)
    {
        PixAddPhotoSelectLayer *addPhotoSelectLayer = dynamic_cast<PixAddPhotoSelectLayer*>(pTarget);
        
        if(addPhotoSelectLayer)
        {
            PixImageModel *pModel = addPhotoSelectLayer->getSelectModel();
            
            if(pModel && pModel->getLocalImageInfo())
            {
                LocalImageInfo *localImageInfo = pModel->getLocalImageInfo();
                
                if(photoEditSprite)
                {
                    PixSmartMaskImageModel *smartMaskImageModel = photoEditSprite->getSmartMaskImageModel();
                    
                    if(smartMaskImageModel)
                    {
                        PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                        
                        if(smartImageModel)
                        {
                            smartImageModel->setPath(localImageInfo->getPath());
                            smartImageModel->setDate(localImageInfo->getFullDate());
                            smartImageModel->setWidth(localImageInfo->getWidth());
                            smartImageModel->setHeight(localImageInfo->getHeight());
                            smartImageModel->setOption(pModel->getOption());
                            smartImageModel->setOrientation(localImageInfo->getOrientation());
                            
                            PixImageModel *imageModel = PIXAPP->userInfo->getImageModelBySmartIndex(smartImageModel->getSmartIndex());
                            
                            if(imageModel)
                            {
                                PIXAPP->userInfo->removeImageModel(imageModel);
                            }
                            
                            pModel->setSmartIndex(smartImageModel->getSmartIndex());
                            PIXAPP->userInfo->addImageModel(pModel->clone());
                        }
                    }
                }
            }
        }
    }
}

void PixEditAlbumLayer::hideLayerActionFinish(Ref *pTarget)
{
    if(pTarget)
    {
        PixAddPhotoSelectLayer *addPhotoSelectLayer = dynamic_cast<PixAddPhotoSelectLayer*>(pTarget);
        
        if(addPhotoSelectLayer)
        {
            PixImageModel *pModel = addPhotoSelectLayer->getSelectModel();
            
            if(pModel && pModel->getLocalImageInfo())
            {
                mbln_IsPlace = true;
                photoEditSprite = nullptr;
                setEditAlbumImageLayer();
            }
        } 
    }
}

#pragma mark - Network

void PixEditAlbumLayer::layoutChange()
{
    int nCurNum = mn_Page - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel == nullptr)
        return;
    
    PIXAPP->showSmartAlbumLoading(true);
        
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_LAYOUT_CHANGE");
    std::map<std::string, std::string> postParams;
    
    std::string theme_id = "";
    std::string member_id = PIXAPP->userInfo->getMember_id();
    std::string layout_group_id = "";
    std::string vendor = PIXAPP->userInfo->getVendor();
    std::string resource_id = smartMaskPageModel->getLayout_res_id();
    std::string img = "";
    std::string item_size = "";
    
    PixDictionary *themeInfo = PIXAPP->userInfo->getSelectThemeInfo();
    
    if(themeInfo)
    {
        PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(themeInfo);
        
        if(leaf)
        {
            theme_id = leaf->stringForKey("theme_id");
            layout_group_id = leaf->stringForKey("layout_group_id");
            
            PixDictionary *childen =  SMARTALBUM_MANAGER->getChildenDictionary(leaf);
            
            if(childen)
            {
                item_size = childen->stringForKey("title");
            }
        }
    }
    
    ParamInfo *paramInfo  = SMARTALBUM_MANAGER->getLayoutChangeImgParam(smartMaskPageModel);
    img = paramInfo->img;
    paramInfo->clear();
    
    postParams["theme_id"] = theme_id;
    postParams["member_id"] = member_id;
    postParams["layout_group_id"] = layout_group_id;
    postParams["vendor"] = vendor;
    postParams["resource_id"] = resource_id;
    postParams["img"] = img;
    postParams["item_size"] = item_size;
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "smart_client_change_layout", this, httpresponse_selector(PixEditAlbumLayer::layoutChangeCallback), nullptr);
}

void PixEditAlbumLayer::layoutChangeCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        int nCurNum = mn_Page - 1;
        
        PixSmartMaskPageModel *smartMaskPageModel = mModel->getSmartMaskPageModel(nCurNum);
        
        if(smartMaskPageModel)
        {
            smartMaskPageModel->changelayout(root);
            setEditAlbumImageLayer();
            setAlbumEffect();
            mbln_IsPlace = true;
            
            GOOGLEANALYTICS_MANAGER->logEvent("smart", "change_layout", PIXAPP->userInfo->getUserID(), 1);
        }
    }
}

#pragma mark - Downloade Delegate

void PixEditAlbumLayer::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
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

void PixEditAlbumLayer::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixEditAlbumLayer::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    std::string fullPath = identifier;
    
    void *pUserData = downloader->getUserData();
    
    PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
    
    if(pModel)
    {
        pModel->path = fullPath;
        pModel->addImageAsync(CC_CALLBACK_2(PixEditAlbumLayer::completeTexture, this));
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


#pragma mark - Android BackPressed

void PixEditAlbumLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(getAndroidBackPressedAction() == false)
    {
        setAndroidBackPressedAction(true);
        hideEditAlbumLayerAction();
    }
}
