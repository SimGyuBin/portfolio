//
//  PixSmartalbumLevel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 17..
//
//

#include "PixSmartAlbumLevel.h"
#include "PixApp.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixUtils.h"
#include "../UIControl/PixMaskSprite.h"
#include "../Layer/PixSmartalbumSaveLayer.h"
#include "../Layer/PixSmartalbumIntro.h"
#include "../Manager/PixEditManager.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixUserDefault.h"
#include "../Common.h"

Scene* PixSmartAlbumLevel::scene()
{
    Scene *scene = Scene::create();
    PixSmartAlbumLevel *layer = new PixSmartAlbumLevel();
    
    scene->addChild(layer, 0, LAYER_LEVEL);
    layer->release();
    
    return scene;
}

PixSmartAlbumLevel::PixSmartAlbumLevel()
: pixHttpClient(nullptr)
, smartalbumLayer(nullptr)
, skin(nullptr)
, item(nullptr)
, mask(nullptr)
, smartalbumMenuLayer(nullptr)
, bottomMenu(nullptr)
, edit(nullptr)
, share(nullptr)
, save(nullptr)
, back(nullptr)
, gestureRecognizer(nullptr)
, touchDelegate(nullptr)
, completealbum(nullptr)
, curpagelabel(nullptr)
, mn_CurpageNum(1)
, mn_RequestImageCount(0)
, smartMaskBookModel(nullptr)
, album_effect(nullptr)
, albumcover_effect(nullptr)
, mbln_IsRequestImage(false)
, mbln_smartalbumMenuLayerVisible(true)
, mbln_IsFirst(true)
{
    for (int i = 0; i < EFFECT_COUNT; ++i)
    {
        prepagePos[i] = Vec2::ZERO;
        nextpagePos[i] = Vec2::ZERO;
    }
    
    pixHttpClient = PixHttpClient::sharedObject();
    downloaderQueue.clear();
    EDIT_MANAGER->clear();
}

PixSmartAlbumLevel::~PixSmartAlbumLevel()
{
    pixHttpClient->cancelRequest(this);
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    CC_SAFE_DELETE(smartMaskBookModel);
    CC_SAFE_DELETE(gestureRecognizer);
    
    PIXAPP->userInfo->localImageDataDelete();
    
    Director::getInstance()->getEventDispatcher()->removeEventListener(touchDelegate);
}

void PixSmartAlbumLevel::initLevel()
{
    smartalbumMenuLayer = dynamic_cast<Node*>(rootNode->getChildByName("smartalbumMenuLayer"));
    
    if(smartalbumMenuLayer)
    {
        rootNode->reorderChild(smartalbumMenuLayer, 11);
    }
    
    album_effect = dynamic_cast<Sprite*>(rootNode->getChildByName("album_effect"));
    
    if(album_effect)
    {
        album_effect->setVisible(false);
        rootNode->reorderChild(album_effect, 1);
        
        Node *layer = dynamic_cast<Node*>(rootNode->getChildByName("smartalbumLayer"));
    
        if(layer)
        {
            smartalbumLayer = ClippingNode::create();
            Sprite *stencil = Sprite::create("PixSmartalbumLevel/AlbumViewMask_1170X585.png");
            stencil->setAnchorPoint(Vec2::ZERO);
            smartalbumLayer->setStencil(stencil);
            smartalbumLayer->setContentSize(layer->getContentSize());
            smartalbumLayer->setScale(layer->getScale());
            smartalbumLayer->setPosition(layer->getPosition());
            rootNode->addChild(smartalbumLayer,0);
            
            ui::Widget *layout = dynamic_cast<ui::Widget*>(layer->getChildByName("skin"));
            
            if(layout)
            {
                skin = layout->clone();
                smartalbumLayer->addChild(skin, 1);
            }
            
            layout = dynamic_cast<ui::Widget*>(layer->getChildByName("item"));
            
            if(layout)
            {
                item = layout->clone();
                smartalbumLayer->addChild(item, 2);
            }
            
            layout = dynamic_cast<ui::Widget*>(layer->getChildByName("mask"));
            
            if(layout)
            {
                mask = layout->clone();
                smartalbumLayer->addChild(mask, 3);
            }
            
            rootNode->removeChild(layer);
        }
    }
    
    albumcover_effect = dynamic_cast<Sprite*>(rootNode->getChildByName("albumcover_effect"));
    
    if(albumcover_effect)
    {
        albumcover_effect->setVisible(false);
        rootNode->reorderChild(albumcover_effect, 2);
    }
    
    if (smartalbumMenuLayer)
    {
        bottomMenu = dynamic_cast<Sprite*>(smartalbumMenuLayer->getChildByName("bottom"));
        
        if(bottomMenu)
        {
            edit = dynamic_cast<ui::Button*>(bottomMenu->getChildByName("edit"));
            
            if (edit)
            {
                edit->addTouchEventListener(CC_CALLBACK_2(PixSmartAlbumLevel::editPressed, this));
            }
            
            share = dynamic_cast<ui::Button*>(bottomMenu->getChildByName("share"));
            
            if (share)
            {
                share->addTouchEventListener(CC_CALLBACK_2(PixSmartAlbumLevel::sharePressed, this));
            }
            
            save = dynamic_cast<ui::Button*>(bottomMenu->getChildByName("save"));
            
            if (save)
            {
                save->addTouchEventListener(CC_CALLBACK_2(PixSmartAlbumLevel::savePressed, this));
            }
            
            back = dynamic_cast<ui::Button*>(bottomMenu->getChildByName("back"));
            
            if(back)
            {
                back->addTouchEventListener(CC_CALLBACK_2(PixSmartAlbumLevel::backPressed, this));
            }
        }
    }
    
    completealbum = dynamic_cast<Sprite*>(rootNode->getChildByName("completealbum"));
    
    if (completealbum)
    {
        completealbum->setOpacity(0);
        
        ui::Text *label = dynamic_cast<ui::Text*>(completealbum->getChildByName("label"));
        
        if(label)
        {
            label->setString(CODE_NAME(label->getString()));
        }
    }
    
    curpagelabel = dynamic_cast<ui::Text*>(rootNode->getChildByName("curpagelabel"));
    
    smartalbumMenuLayerVisible(true);
    PIXAPP->showSmartAlbumLoading();
}

void PixSmartAlbumLevel::onEnter()
{
    GbLevel::onEnter();
}

void PixSmartAlbumLevel::onEnterTransitionDidFinish()
{
    GbLevel::onEnterTransitionDidFinish();
    initGestureRecognizer();
    createSmartAlbum();
}

void PixSmartAlbumLevel::update(float dt)
{
    GbLevel::update(dt);

    if(mbln_IsRequestImage == true)
    {
        if(mn_RequestImageCount == 0)
        {
            bool isItemLayout = true;
            int nCount = (int)item->getChildrenCount();
            
            for(int i = 0; i < nCount; ++i)
            {
                PixMaskSprite *maskSprite = dynamic_cast<PixMaskSprite*>(item->getChildren().at(i));
                
                if(maskSprite)
                {
                    if(maskSprite->getDataloadSuccess() == false)
                    {
                        isItemLayout = false;
                        break;
                    }
                }
            }
            
            if(isItemLayout)
            {
                mbln_IsRequestImage = false;
                sendGetImageCompleted();
            }
        }
    }
}

void PixSmartAlbumLevel::createSmartAlbum()
{   
    PixDictionary* root = SMARTALBUM_MANAGER->getSmartAlbumDictionary();
    
    if(root)
    {
        smartMaskBookModel = new PixSmartMaskBookModel();
        smartMaskBookModel->createSmartAlbum(root);
    }
    
    setCurPage();
    setAlbumEffect();
}

void PixSmartAlbumLevel::setCurPagelabel()
{
    curpagelabel->setString(StringUtils::format("%d / %d",mn_CurpageNum,smartMaskBookModel->getSmartMaskPageModeCount()));
}

void PixSmartAlbumLevel::setCurPage()
{
    PIXAPP->showSmartAlbumLoading();
    mbln_IsRequestImage = true;

    smartalbumLayer->setVisible(false);
    skin->removeAllChildren();
    item->removeAllChildren();
    mask->removeAllChildren();

    Director::getInstance()->getTextureCache()->removeAllTextures();
    
    loadSmartLayout();
    loadLocalImage();
}

void PixSmartAlbumLevel::loadSmartLayout()
{
    int nCurNum = mn_CurpageNum - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
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
                    pModel->rotate = atof(smartMaskImageModel->getItem_rotate().c_str());
                    pModel->path = smartMaskImageModel->getLayout_res_mask_mask();
                    pModel->nType = CLIPPING_MASK;
                    
                    ++mn_RequestImageCount;
                    sendGetImageUrl_Call(smartMaskImageModel->getLayout_res_mask_mask(), pModel);
                }
            }
        }
    }
}
void PixSmartAlbumLevel::loadLocalImage()
{
    int nCurNum = mn_CurpageNum - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
    if(smartMaskPageModel)
    {
        autolayout* pAutolayout = smartMaskPageModel->getAutolayout();
        
        double nAddPosX = 0;
        double nAddPosY = 0;
        pAutolayout->getAutoLayoutPageModelAddPos(nAddPosX, nAddPosY);
        const double nPageSizeRealWidth = atof(pAutolayout->getPage_size_real_width().c_str());
        const double nPageSizeRealHeight = atof(pAutolayout->getPage_size_real_height().c_str());
        
        int nCount = smartMaskPageModel->getSmartMaskImageModelCount();
        
        int nZOrder = 1000;
        
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
                    
                    pModel->addLocalImageAsync(CC_CALLBACK_2(PixSmartAlbumLevel::completeTexture, this));
                }
            }
        }
    }
}

void PixSmartAlbumLevel::completeTexture(Texture2D *pTexture, void *pUserData)
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
    
    if(smartalbumLayer == nullptr)
    {
        --mn_RequestImageCount;
        pModel->release();
        return;
    }
    
    float fScaleX = 1.0f;
    float fScaleY = 1.0f;
    
    Size albumSize = smartalbumLayer->getContentSize();
    Vec2 ratio = Vec2 ( albumSize.width / pModel->parentSize.width, albumSize.height / pModel->parentSize.height );
    Vec2 pos = Vec2( pModel->pos.x * ratio.x, albumSize.height - (pModel->pos.y * ratio.y) );
    Size maskSize = Size(pModel->size.width * ratio.x, pModel->size.height * ratio.y );
    
    if (pModel->nType == CLIPPING_MASK)
    {
        pos.x += maskSize.width / 2;
        pos.y -= maskSize.height / 2;
        
        fScaleX = maskSize.width / pTexture->getContentSize().width;
        fScaleY = maskSize.height / pTexture->getContentSize().height;
        
        PixMaskSprite *maskSprite = dynamic_cast<PixMaskSprite*>(item->getChildByTag(pModel->nTag));
        
        if(maskSprite)
        {
            maskSprite->setMaskSpriteTexture(pTexture, fScaleX, fScaleY);
        }
        else
        {
            maskSprite = PixMaskSprite::create();
            maskSprite->setMaskSpriteTexture(pTexture, fScaleX, fScaleY);
            maskSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            maskSprite->setPosition(pos);
            maskSprite->setRotation(pModel->rotate);
            maskSprite->setTag(pModel->nTag);
            
            item->addChild(maskSprite, pModel->zOrder);
            
//            auto label = Label::createWithSystemFont(pModel->date, "", 20,
//                                                     Size(300, 100), TextHAlignment::CENTER, TextVAlignment::CENTER);
//            
//            label->setColor(Color3B::ORANGE);
//            label->setAnchorPoint(Vec2(0.5f, 0.5f));
//            label->setRotation(pModel->rotate);
//            label->setPosition(pos);
//            item->addChild(label, pModel->zOrder + 100000);
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
        
        PixMaskSprite *maskSprite = dynamic_cast<PixMaskSprite*>(item->getChildByTag(pModel->nTag));
        if(maskSprite)
        {
            maskSprite->setImageSpriteTexture(pTexture, fScale, fScale);
        }
        else
        {
            maskSprite = PixMaskSprite::create();
            maskSprite->setImageSpriteTexture(pTexture, fScale, fScale);
            maskSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            maskSprite->setPosition(pos);
            maskSprite->setRotation(pModel->rotate);
            maskSprite->setTag(pModel->nTag);
            
            item->addChild(maskSprite, pModel->zOrder);
            
//            auto label = Label::createWithSystemFont(pModel->date, "", 20,
//                                                     Size(300, 100), TextHAlignment::CENTER, TextVAlignment::CENTER);
//            
//            label->setColor(Color3B::ORANGE);
//            label->setAnchorPoint(Vec2(0.5f, 0.5f));
//            label->setRotation(pModel->rotate);
//            label->setPosition(pos);
//            item->addChild(label, pModel->zOrder + 100000);
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
//                smartalbumLayer->addChild(sprite);
            }
        }
        else
        {
            Size albumSize = smartalbumLayer->getContentSize();
            
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
//                smartalbumLayer->addChild(sprite);
            }
        }
    }
    
    --mn_RequestImageCount;
    pModel->release();
    
}

void PixSmartAlbumLevel::sendGetImageCompleted()
{
    setCurPagelabel();
    smartalbumLayer->setVisible(true);
    
    PIXAPP->hideLoading();
    
    setAlbumEffect();
    
    if(mbln_IsFirst)
    {
        mbln_IsFirst = false;
        
        if(PixUserDefault::GetSmartAlbumTutorial())
        {
            Scene *scene = PIXAPP->isScene;
        
            if(scene)
            {
                scene->addChild(PixSmartalbumIntro::create());
            }
        }
    }
}

void PixSmartAlbumLevel::showCompleteAlbum(bool blnShow)
{

    if(blnShow)
    {
        FadeIn *fadeIn = FadeIn::create(0.3f);
        DelayTime *delayTime = DelayTime::create(1.0f);
        FadeOut *fadeOut = FadeOut::create(0.3f);
        Sequence *sequence = Sequence::create(fadeIn, delayTime, fadeOut, NULL);
        completealbum->runAction(sequence);
    }
    else
    {
        completealbum->stopAllActions();
        completealbum->setOpacity(0);
    }
    
}

void PixSmartAlbumLevel::initGestureRecognizer()
{
    // Init gesture recognizer
    gestureRecognizer = new PixSimpleGestureRecognizer();
    gestureRecognizer->init();
    gestureRecognizer->setGestureHandler(this);
    
    // Enable all gesture kinds
    gestureRecognizer->setTapEnabled(true);
    gestureRecognizer->setDoubleTapEnabled(false);
    gestureRecognizer->setLongPressEnabled(false);
    gestureRecognizer->setPanEnabled(false);
    gestureRecognizer->setPinchEnabled(false);
    gestureRecognizer->setRotationEnabled(false);
    gestureRecognizer->setSwipeEnabled(true);
    
    // Taps will be fired immediately without waiting for double tap
    gestureRecognizer->setTapRequiresDoubleTapRecognitionToFail(false);
    
    // Other config
    // _gestureRecognizer->setTapThreshold(1.0f);
    // _gestureRecognizer->setLongPressThreshold(1.0f);
    // _gestureRecognizer->setDoubleTapInterval(0.3f);
    // _gestureRecognizer->setPinchFingersDistanceChangeTolerance(0.1f);
    // _gestureRecognizer->setRotationFingersDistanceChangeTolerance(0.5f);
    // _gestureRecognizer->setSwipeThreshold(0.3f);
    
    //
    // IMPORTANT:
    // For multiple touch gestures on iOS (pinch, rotation), always remember tu put
    // the below line of code right after creating the CCEAGLView in AppController.mm
    // [eaglView setMultipleTouchEnabled:YES];
    // For Android, there no need to do this.
    //
    
    // Create touch listener and register it with cocos2d to receive touch events
    touchDelegate = EventListenerTouchOneByOne::create();
    
    touchDelegate->onTouchBegan = CC_CALLBACK_2(PixSmartAlbumLevel::TouchBegan, this);
    touchDelegate->onTouchMoved = CC_CALLBACK_2(PixSmartAlbumLevel::TouchMoved, this);
    touchDelegate->onTouchCancelled = CC_CALLBACK_2(PixSmartAlbumLevel::TouchCancelled, this);
    touchDelegate->onTouchEnded = CC_CALLBACK_2(PixSmartAlbumLevel::TouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchDelegate, 100);
}


void PixSmartAlbumLevel::goPrePage()
{
    if(mn_CurpageNum > 1)
    {
        --mn_CurpageNum;
        setCurPage();
    }
    else
    {
        mn_CurpageNum = smartMaskBookModel->getSmartMaskPageModeCount();
        setCurPage();
    }
}

void PixSmartAlbumLevel::goNextPage()
{
    if(mn_CurpageNum < smartMaskBookModel->getSmartMaskPageModeCount())
    {
        ++mn_CurpageNum;
        setCurPage();
    }
    else
    {
        mn_CurpageNum = 1;
        setCurPage();
    }
}

void PixSmartAlbumLevel::goMyAlbum()
{
    UserInfo *pUserInfo = PIXAPP->userInfo;
    pUserInfo->removeAllImageModel();
    
    PIXAPP->goLevel(0, PT_MYALBUM, TR_FADE);
}

void PixSmartAlbumLevel::goThemeLevel()
{
    UserInfo *pUserInfo = PIXAPP->userInfo;
    pUserInfo->removeAllImageModel();
    
    PIXAPP->goLevel(0, PT_MYALBUM_THEME, TR_FADE);
}

void PixSmartAlbumLevel::setAlbumEffect()
{
    int nCurNum = mn_CurpageNum - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
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

#pragma mark - Action

void PixSmartAlbumLevel::smartalbumMenuLayerVisible(bool isFirst)
{
    mbln_smartalbumMenuLayerVisible = !mbln_smartalbumMenuLayerVisible;
    
    float fTime = 0.3f;
    
    if(isFirst)
    {
        fTime = 0.0f;
    }
    else
    {

    }
    
    if(mbln_smartalbumMenuLayerVisible)
    {
        if (back)
        {
            back->stopAllActions();
            FadeIn *fadeIn = FadeIn::create(fTime);
            EaseSineOut *easeSineOut = EaseSineOut::create(fadeIn);
            back->runAction(easeSineOut);
        }
        
        if(bottomMenu)
        {
            bottomMenu->stopAllActions();
            FadeIn *fadeIn = FadeIn::create(fTime);
            EaseSineOut *easeSineOut = EaseSineOut::create(fadeIn);
            bottomMenu->runAction(easeSineOut);
        }
    }
    else
    {
        if (back)
        {
            back->stopAllActions();
            FadeOut *fadeOut = FadeOut::create(fTime);
            EaseSineOut *easeSineOut = EaseSineOut::create(fadeOut);
            back->runAction(easeSineOut);
        }
                
        if(bottomMenu)
        {
            bottomMenu->stopAllActions();
            FadeOut *fadeOut = FadeOut::create(fTime);
            EaseSineOut *easeSineOut = EaseSineOut::create(fadeOut);
            bottomMenu->runAction(easeSineOut);
        }
    }
}

void PixSmartAlbumLevel::showSmartalbumLayerAction()
{
    if(smartalbumLayer == nullptr || album_effect == nullptr)
        return;
    
    smartalbumLayer->setVisible(true);
    album_effect->setVisible(true);
    
    
    FadeIn *fadeIn = FadeIn::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSmartAlbumLevel::showSmartalbumLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);
    
    smartalbumLayer->runAction(sequence);
    
    fadeIn = FadeIn::create(0.3f);
    
    album_effect->runAction(sequence);
}

void PixSmartAlbumLevel::hideSmartalbumLayerAction()
{
    if(smartalbumLayer == nullptr || album_effect == nullptr)
        return;
    
    FadeOut *fadeOut = FadeOut::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSmartAlbumLevel::hideSmartalbumLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeOut, callFuncN, NULL);
    
    smartalbumLayer->runAction(sequence);
    
    fadeOut = FadeOut::create(0.3f);
    
    album_effect->runAction(sequence);
}

void PixSmartAlbumLevel::showSmartalbumLayerActionFinish(Node* node)
{
    
}

void PixSmartAlbumLevel::hideSmartalbumLayerActionFinish(Node* node)
{
    smartalbumLayer->setVisible(false);
    album_effect->setVisible(false);
}


#pragma mark - Touch

bool PixSmartAlbumLevel::TouchBegan(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    gestureRecognizer->onTouchBegan(touch, event);
    return true;
}

void PixSmartAlbumLevel::TouchMoved(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    gestureRecognizer->onTouchMoved(touch, event);
}

void PixSmartAlbumLevel::TouchEnded(Touch* touch, Event* event)
{
    gestureRecognizer->onTouchEnded(touch, event);
}

void PixSmartAlbumLevel::TouchCancelled(Touch* touch, Event* event)
{
    gestureRecognizer->onTouchCancelled(touch, event);
}

#pragma mark - GestureHandler

bool PixSmartAlbumLevel::onGestureTap(PixGestureTap* gesture)
{
    if (NodeContainsPoint(smartalbumMenuLayer, gesture->getLocation()))
    {
        if (gesture->getNumClicks() == 1)
        {
            CCLOG("Tap detected");
            
            smartalbumMenuLayerVisible();
        }
        else
        {
            CCLOG("Double Tap detected");
        }
    }
    
    return false;
}

bool PixSmartAlbumLevel::onGestureLongPress(PixGestureLongPress* gesture)
{
    if (NodeContainsPoint(this, gesture->getLocation()))
    {
        CCLOG("Long Press detected");
    }
    
    return false;
}

bool PixSmartAlbumLevel::onGesturePan(PixGesturePan* gesture)
{
    static int lastPanId = -1;
    static bool panInsideNode = false;
    
    // A new pan
    if (gesture->getID() != lastPanId)
    {
        lastPanId = gesture->getID();
        panInsideNode = NodeContainsPoint(this, gesture->getLocation());
    }
    
    if (panInsideNode)
    {
        this->setPosition(this->getPosition() + gesture->getTranslation());
    }
    
    return false;
}

bool PixSmartAlbumLevel::onGesturePinch(PixGesturePinch* gesture)
{
    static int lastPinchId = -1;
    static bool pinchInsideNode = false;
    static float originalScale;
    
    // A new pinch
    if (gesture->getID() != lastPinchId)
    {
        lastPinchId = gesture->getID();
        pinchInsideNode = NodeContainsPoint(this, gesture->getLocation());
        originalScale = this->getScale();
    }
    
    if (pinchInsideNode)
    {
        this->setScale(originalScale * gesture->getScale());
        
        if (gesture->getTouch0() == nullptr || gesture->getTouch1() == nullptr)
            return false;
        
        Vec2 curPosTouch0 = Director::getInstance()->convertToGL(gesture->getTouch0()->getLocationInView());
        Vec2 curPosTouch1 = Director::getInstance()->convertToGL(gesture->getTouch1()->getLocationInView());
        Vec2 prevPosTouch0 = Director::getInstance()->convertToGL(gesture->getTouch0()->getPreviousLocationInView());
        Vec2 prevPosTouch1 = Director::getInstance()->convertToGL(gesture->getTouch1()->getPreviousLocationInView());
        
        Vec2 curPosLayer = ccpMidpoint(curPosTouch0, curPosTouch1);
        Vec2 prevPosLayer = ccpMidpoint(prevPosTouch0, prevPosTouch1);
        
        float prevScale = this->getScale();
        float curScale = this->getScale() * ccpDistance(curPosTouch0, curPosTouch1) / ccpDistance(prevPosTouch0, prevPosTouch1);
        
        if (this->getScale() != prevScale)
        {
            Vec2 realCurPosLayer = this->convertToNodeSpaceAR(curPosLayer);
            float deltaX = (realCurPosLayer.x) * (this->getScale() - prevScale);
            float deltaY = (realCurPosLayer.y) * (this->getScale() - prevScale);
            
            this->setPosition(Vec2(this->getPosition().x - deltaX, this->getPosition().y - deltaY));
        }
        
        // If current and previous position of the multitouch's center aren't equal -> change position of the layer
        if (!prevPosLayer.equals(curPosLayer))
        {
            this->setPosition(Vec2(this->getPosition().x + curPosLayer.x - prevPosLayer.x,
                                    this->getPosition().y + curPosLayer.y - prevPosLayer.y));
        }
    }
    
    return false;
}

bool PixSmartAlbumLevel::onGestureRotation(PixGestureRotation* gesture)
{
    static int lastRotationId = -1;
    static bool rotationInsideNode = false;
    static float originalRotation;
    
    // A new rotation
    if (gesture->getID() != lastRotationId)
    {
        lastRotationId = gesture->getID();
        rotationInsideNode = NodeContainsPoint(this, gesture->getLocation());
        originalRotation = this->getRotation();
    }
    
    if (rotationInsideNode)
    {
        this->setRotation(originalRotation + gesture->getRotation());
    }
    
    return false;
}

bool PixSmartAlbumLevel::onGestureSwipe(PixGestureSwipe* gesture)
{
    SwipeDirection dir = gesture->getDirection();
 
    if (dir == SwipeDirectionNorth)
    {
        CCLOG("Swipe detected: NORTH");
        
        if (bottomMenu->getOpacity() == 0)
        {
            layoutChange();
        }
    }
    else if (dir == SwipeDirectionNorthEast)
    {
        CCLOG("Swipe detected: NORTH EAST");
    }
    else if (dir == SwipeDirectionEast)
    {
        CCLOG("Swipe detected: EAST");
        
        if(mbln_smartalbumMenuLayerVisible == false && bottomMenu->getNumberOfRunningActions() == 0)
        {
            if(smartMaskBookModel->getSmartMaskPageModeCount() <= 1)
                return false;
            
            goPrePage();
        }
    }
    else if (dir == SwipeDirectionSouthEast)
    {
        CCLOG("Swipe detected: SOUTH EAST");
    }
    else if (dir == SwipeDirectionSouth)
    {
        CCLOG("Swipe detected: SOUTH");
        if (bottomMenu->getOpacity() == 0)
        {
            layoutChange();
        }
    }
    else if (dir == SwipeDirectionSouthWest)
    {
        CCLOG("Swipe detected: SOUTH WEST");
    }
    else if (dir == SwipeDirectionWest)
    {
        CCLOG("Swipe detected: WEST");
        
        if(mbln_smartalbumMenuLayerVisible == false && bottomMenu->getNumberOfRunningActions() == 0)
        {
            if(smartMaskBookModel->getSmartMaskPageModeCount() <= 1)
                return false;
            
            goNextPage();
        }
    }
    else if (dir == SwipeDirectionNorthWest)
    {
        CCLOG("Swipe detected: NORTH WEST");
    }
    
//    bool panEnabled = true; // If pan is not enabled, use false
//    bool swipeFromNode = NodeContainsPoint(this, panEnabled ? gesture->getEndLocation() : gesture->getLocation());
//    
//    if (swipeFromNode)
//    {
//        float swipeLength = gesture->getLength();
//        float moveDistance = swipeLength * 2;
//        
//        // Move the sprite along the swipe direction
//        Vec2 targetPos = this->getPosition() + (gesture->getDirectionVec().getNormalized() * moveDistance);
//        
//        MoveTo* move = MoveTo::create(1.5f, targetPos);
//        EaseOut* easeout = EaseOut::create(move, 5);
//        
//        this->runAction(easeout);
//    }
    
    return false;
}


#pragma mark - Button

void PixSmartAlbumLevel::editPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && bottomMenu->getNumberOfRunningActions() == 0)
    {
        if (bottomMenu->getOpacity() == 0)
        {
            smartalbumMenuLayerVisible();
            return;
        }
        
        PixEditAlbumLayer *editAlbumLayer = PixEditAlbumLayer::create(smartMaskBookModel,mn_CurpageNum);
        editAlbumLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(editAlbumLayer);
    }
}

void PixSmartAlbumLevel::sharePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && bottomMenu->getNumberOfRunningActions() == 0)
    {
        if (bottomMenu->getOpacity() == 0)
        {
            smartalbumMenuLayerVisible();
            return;
        }

        PixSmartalbumSharePopupLayer *smartalbumSharePopupLayer = PixSmartalbumSharePopupLayer::create();
        smartalbumSharePopupLayer->setShareDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(smartalbumSharePopupLayer);
    }
}

void PixSmartAlbumLevel::savePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && bottomMenu->getNumberOfRunningActions() == 0)
    {
        if (bottomMenu->getOpacity() == 0)
        {
            smartalbumMenuLayerVisible();
            return;
        }

        PixSmartalbumSaveLayer *smartalbumSaveLayer = PixSmartalbumSaveLayer::create(smartMaskBookModel);
        Director::getInstance()->getRunningScene()->addChild(smartalbumSaveLayer);
    }
}

void PixSmartAlbumLevel::backPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && back->getNumberOfRunningActions() == 0)
    {
        if (bottomMenu->getOpacity() == 0)
        {
            smartalbumMenuLayerVisible();
            return;
        }

        if(!smartMaskBookModel->getSmartalbum().empty())
        {
            goThemeLevel();
        }
        else
        {
            PixSmartalbumNonSavePopupLayer::show(this);
        }
    }
}

#pragma mark - Network


void PixSmartAlbumLevel::sendGetImageUrl_Call(const std::string &url, void *pUserData)
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
            pModel->addImageAsync(CC_CALLBACK_2(PixSmartAlbumLevel::completeTexture, this));
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

void PixSmartAlbumLevel::layoutChange()
{
    int nCurNum = mn_CurpageNum - 1;
    
    PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
    
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
        }
        
        PixDictionary *childen = SMARTALBUM_MANAGER->getChildenDictionary(leaf);
        
        if(childen)
        {
            item_size = childen->stringForKey("title");
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

    pixHttpClient->getFormData(url, postParams, "smart_client_change_layout", this, httpresponse_selector(PixSmartAlbumLevel::layoutChangeCallback), nullptr);
}

void PixSmartAlbumLevel::layoutChangeCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        int nCurNum = mn_CurpageNum - 1;
        
        PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(nCurNum);
        
        if(smartMaskPageModel)
        {
            smartMaskPageModel->changelayout(root);
            setCurPage();
                     
            GOOGLEANALYTICS_MANAGER->logEvent("smart", "change_layout", PIXAPP->userInfo->getUserID(), 1);
        }
    }
}

#pragma mark - Downloade Delegate

void PixSmartAlbumLevel::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
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

void PixSmartAlbumLevel::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixSmartAlbumLevel::PixSmartAlbumLevel::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    std::string fullPath = identifier;
    
    void *pUserData = downloader->getUserData();
    
    PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
    
    if(pModel)
    {
        pModel->path = fullPath;
        pModel->addImageAsync(CC_CALLBACK_2(PixSmartAlbumLevel::completeTexture, this));
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

#pragma mark - Delegate

void PixSmartAlbumLevel::showLayerActionStart(Ref *pTarget)
{
    
}

void PixSmartAlbumLevel::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixSmartAlbumLevel::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixSmartAlbumLevel::hideLayerActionFinish(Ref *pTarget)
{
    
    PixEditAlbumLayer *editAlbumLayer = dynamic_cast<PixEditAlbumLayer*>(pTarget);
    
    if(editAlbumLayer)
    {
        if(mn_CurpageNum != editAlbumLayer->getCurPage())
        {
            mn_CurpageNum = editAlbumLayer->getCurPage();
        }
    }
    
    setCurPage();
}

void PixSmartAlbumLevel::clickedButtonInNonSavePopup(PixSmartalbumNonSavePopupLayer *popup, NONSAVE_POPUP_TYPE buttonType)
{
    if(buttonType == NONSAVE_POPUP_EXIT)
    {
        goMyAlbum();
    }
    else if(buttonType == NONSAVE_POPUP_NEW)
    {
        goThemeLevel();
    }
    else if(buttonType == NONSAVE_POPUP_SAVE)
    {
        PixSmartalbumSaveLayer *smartalbumSaveLayer = PixSmartalbumSaveLayer::create(smartMaskBookModel);
        Director::getInstance()->getRunningScene()->addChild(smartalbumSaveLayer);
    }
}

void PixSmartAlbumLevel::smartAlbumShare()
{
    
    Size size = Size(smartalbumLayer->getContentSize().width * smartalbumLayer->getScaleX(),
                     smartalbumLayer->getContentSize().height * smartalbumLayer->getScaleY());
    
    int nCurNum = mn_CurpageNum - 1;
    
    RenderTexture *render = RenderTexture::create(size.width, size.height);
    render->begin();
    
    smartalbumLayer->setPosition(Vec2::ZERO);
    smartalbumLayer->Node::visit();
    
    if(nCurNum == 0)
    {
        albumcover_effect->setVisible(true);
        album_effect->setVisible(false);
        albumcover_effect->setAnchorPoint(Vec2(0.5f,0.5f));
        albumcover_effect->setPosition(Vec2(size.width
                                            * 0.5f,size.height * 0.5f));
        albumcover_effect->Node::visit();
    }
    else
    {
        album_effect->setVisible(true);
        albumcover_effect->setVisible(false);
        album_effect->setAnchorPoint(Vec2(0.5f,0.5f));
        album_effect->setPosition(Vec2(size.width
                                            * 0.5f,size.height * 0.5f));
        album_effect->Node::visit();
    }
    
    render->end();
    
    std::string filename = StringUtils::format("SmartAlbumShare_%d.jpg",nCurNum);
    
    render->saveToFile(filename, Image::Format::JPG, false, CC_CALLBACK_2(PixSmartAlbumLevel::smartAlbumShareSave, this) );
}

void PixSmartAlbumLevel::smartAlbumShareSave(RenderTexture* pRenderTexture, const std::string& filename)
{
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
    
    CC_ASSERT(FileUtils::getInstance()->isFileExist(fullPath));
    
    Data buf = FileUtils::getInstance()->getDataFromFile(fullPath);
    
    if (!buf.isNull())
    {
        PixNativeUtils::nativeCallSaveImage(&buf, filename, CODE_NAME("P42"));
    }
}
