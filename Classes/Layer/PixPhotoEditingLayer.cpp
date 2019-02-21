//
//  PixPhotoEditingLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 1..
//
//

#include "PixPhotoEditingLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Network/PixHttpClient.h"
#include "../Scene/PixSmartAlbumLevel.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUtils.h"
#include "../Util/PixMath.h"
#include "../Common.h"


PixPhotoEditingInfo* PixPhotoEditingInfo::showInfo(Node* Layer, const char *message, const Vec2 &position, float duration, const char *fontName, int fontSize, Color4B textColor)
{
    Node *beforeToast = dynamic_cast<Node*>(Layer->getChildByTag(EDITINGINFO_TAG));
    if (beforeToast) beforeToast->removeFromParentAndCleanup(true);
    
    PixPhotoEditingInfo *info = new PixPhotoEditingInfo();
    info->init(Layer, message, position, duration, fontName, fontSize, textColor);
    Layer->addChild(info, 1000, EDITINGINFO_TAG);
    info->release();
    
    return info;
}

PixPhotoEditingInfo::~PixPhotoEditingInfo()
{
    
}

void PixPhotoEditingInfo::init(Node* Layer, const char *message, const Vec2 &position, float duration,
                    const char *fontName, int fontSize, Color4B textColor)
{
    m_Layer = Layer;
    m_duration = duration;
    
    label = ui::Text::create(message, fontName, fontSize);
    
    Sprite::init();
    setTextureRect(Rect(0, 0, label->getContentSize().width, label->getContentSize().height));
    setPosition(position);
    setOpacity(0);
    
    label->setPosition(Vec2(getContentSize().width * 0.5f,
                            getContentSize().height * 0.5f));
    
    label->setTextColor(textColor);
    label->setOpacity(0);
    addChild(label);
}


void PixPhotoEditingInfo::onEnter()
{
    Sprite::onEnter();
    
    if (Director::getInstance()->isPaused())
    {
        setOpacity(255);
        label->setOpacity(255);
    }
    else
    {
        runAction(FadeTo::create(EDITINGINFO_ANI_SPEED, 255));
        label->runAction(FadeTo::create(EDITINGINFO_ANI_SPEED, 255));
    }
    
    scheduleUpdate();
}

void PixPhotoEditingInfo::update(float dt)
{
    m_duration -= dt;
    
    if (m_duration <= 0)
    {
        unscheduleUpdate();
        
        label->runAction(FadeTo::create(EDITINGINFO_ANI_SPEED, 0));
        
        
        FadeTo *fade = FadeTo::create(EDITINGINFO_ANI_SPEED, 0);
        CallFunc *func = CallFunc::create(CC_CALLBACK_0(PixPhotoEditingInfo::remove, this));
        
        runAction(Sequence::create(fade, func, NULL));
    }
}

void PixPhotoEditingInfo::remove()
{
    PixPhotoEditingInfo *beforeInfo = dynamic_cast<PixPhotoEditingInfo*>(m_Layer->getChildByTag(EDITINGINFO_TAG));
    if (beforeInfo) beforeInfo->removeFromParentAndCleanup(true);
}

PixPhotoEditingLayer* PixPhotoEditingLayer::create(PixSmartMaskPageModel *pModel, int nIndex)
{
    PixPhotoEditingLayer *photoEditingLayer = new (std::nothrow) PixPhotoEditingLayer(pModel, nIndex);
    if (photoEditingLayer && photoEditingLayer->init())
    {
        photoEditingLayer->autorelease();
        return photoEditingLayer;
    }
    else
    {
        CC_SAFE_DELETE(photoEditingLayer);
        return nullptr;
    }
}

PixPhotoEditingLayer::PixPhotoEditingLayer(PixSmartMaskPageModel *pModel, int nIndex)
: mModel(nullptr)
, rootNode(nullptr)
, mEditScreen(nullptr)
, mEditSprite(nullptr)
, Button_before(nullptr)
, Button_next(nullptr)
, Button_complete(nullptr)
, Button_cancel(nullptr)
, Button_reset(nullptr)
, Button_reversal(nullptr)
, Button_rotation(nullptr)
, Button_expansion(nullptr)
, Button_reduction(nullptr)
, photonumber(nullptr)
, infor(nullptr)
, mDelegate(nullptr)
, mbln_isComplete(false)
, mbln_IsAction(false)
{
    if(pModel)
    {
        this->mModel = pModel->clone();
    }
    
    this->mn_Index = nIndex;
    
    hideLayerFunction = std::bind(&PixPhotoEditingLayer::hideLayerAction,this);
}

PixPhotoEditingLayer::~PixPhotoEditingLayer()
{
    CC_SAFE_DELETE(mModel);
    CC_SAFE_DELETE(gestureRecognizer);
}

void PixPhotoEditingLayer::initLayer()
{
    rootNode = CSLoader::createNode("06_EditAlbum/EditAPhotoLayer.csb");
    
    if(rootNode)
    {
        mEditScreen = dynamic_cast<Sprite*>(rootNode->getChildByName("edit_photo_back_3"));
        
        Button_before = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_before"));
        if(Button_before)
        {
            Button_before->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditingLayer::btnBeforePressed, this));
        }
        
        Button_next = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_next"));
        if(Button_next)
        {
            Button_next->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditingLayer::btnNextPressed, this));
        }
        
        Button_complete = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_complete"));
        if(Button_complete)
        {
            Button_complete->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditingLayer::btnCompletePressed, this));
        }
        
        Button_cancel = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_cancel"));
        if(Button_cancel)
        {
            Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditingLayer::btnCancelPressed, this));
        }
        
        Button_reset = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_reset"));
        if(Button_reset)
        {
            Button_reset->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditingLayer::btnResetPressed, this));
        }
        
        Button_reversal = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_reversal"));
        if(Button_reversal)
        {
            Button_reversal->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditingLayer::btnReversalPressed, this));
        }
        
        Button_rotation = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_rotation"));
        if(Button_rotation)
        {
            Button_rotation->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditingLayer::btnRotationPressed, this));
        }
        
        Button_expansion = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_expansion"));
        if(Button_expansion)
        {
            Button_expansion->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditingLayer::btnExpansionPressed, this));
        }
        
        Button_reduction = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_reduction"));
        if(Button_reduction)
        {
            Button_reduction->addTouchEventListener(CC_CALLBACK_2(PixPhotoEditingLayer::btnReductionPressed, this));
        }
        
        photonumber = dynamic_cast<ui::Text*>(rootNode->getChildByName("number"));
        
        infor = dynamic_cast<ui::Text*>(rootNode->getChildByName("infor"));
        
        if(infor)
        {
            infor->setString("");
        }
        
        setAnchorPoint(Vec2::ZERO);
        
        float fheight = Director::getInstance()->getVisibleSize().height;
        setPosition(Vec2(0,-fheight));
        
        addChild(rootNode);
        
        setEditScreen();
    }
}

void PixPhotoEditingLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
    initGestureRecognizer();
}

void PixPhotoEditingLayer::update(float delta)
{
    PixModalLayer::update(delta);
    
    updateEditPhotoNumberLabel();
}

void PixPhotoEditingLayer::setEditScreen()
{
    if(mEditScreen)
    {
        mEditScreen->removeAllChildren();
        
        PixSmartMaskImageModel *smartMaskImageModel = mModel->getSmartMaskImageModel(mn_Index);
        
        if(smartMaskImageModel)
        {
            PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
            
            if(smartImageModel)
            {
                autolayout* pAutolayout = mModel->getAutolayout();
                
                int nZOrder = 0;
                PixNetworkImageModel *pMaskData = PixNetworkImageModel::create();
                pMaskData->retain();
                
                pMaskData->size.width = atof(smartMaskImageModel->getItem_width().c_str());
                pMaskData->size.height = atof(smartMaskImageModel->getItem_height().c_str());
                pMaskData->parentSize.width = atof(pAutolayout->getPage_size_real_width().c_str());
                pMaskData->parentSize.height = atof(pAutolayout->getPage_size_real_height().c_str());
                pMaskData->pos.x = atof(smartMaskImageModel->getItem_x().c_str());
                pMaskData->pos.y = atof(smartMaskImageModel->getItem_y().c_str());
                pMaskData->rotate = atof(smartMaskImageModel->getItem_rotate().c_str());
                pMaskData->zOrder = nZOrder++;
                pMaskData->nTag = smartImageModel->getSmartIndex();
                pMaskData->path = smartMaskImageModel->getLayout_res_mask_mask();
                pMaskData->nType = CLIPPING_MASK;
                
                std::vector<std::string> tokens;
                PixStringUtils::getTokens(tokens, pMaskData->path, "/");
                
                std::string filename;
                filename.clear();
                
                if(!tokens.empty())
                {
                    filename = tokens[tokens.size() - 1];
                }
                
                std::string path = FileUtils::getInstance()->getWritablePath();
                std::string fullPath =  path + filename;
                
                bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
                
                if(isFileExist)
                {
                    pMaskData->path = fullPath;
                    pMaskData->addImageAsync(CC_CALLBACK_2(PixPhotoEditingLayer::completeTexture, this));
                }
                
                PixNetworkImageModel *pModel = PixNetworkImageModel::create();
                pModel->retain();
                
                pModel->size.width = atof(smartMaskImageModel->getItem_width().c_str());
                pModel->size.height = atof(smartMaskImageModel->getItem_height().c_str());
                pModel->parentSize.width = atof(pAutolayout->getPage_size_real_width().c_str());
                pModel->parentSize.height = atof(pAutolayout->getPage_size_real_height().c_str());
                pModel->pos.x = atof(smartMaskImageModel->getItem_x().c_str());
                pModel->pos.y = atof(smartMaskImageModel->getItem_y().c_str());
                pModel->rotate = atof(smartMaskImageModel->getItem_rotate().c_str());
                pModel->path = smartImageModel->getPath();
                pModel->nOrientation = smartImageModel->getOrientation();
                pModel->nType = CLIPPING_IMAGE;
                pModel->zOrder = nZOrder++;
                pModel->nTag = smartImageModel->getSmartIndex();
                pModel->date = smartImageModel->getDate();
                
                pModel->addLocalImageAsync(CC_CALLBACK_2(PixPhotoEditingLayer::completeTexture, this));
            }
        }
    }
}

void PixPhotoEditingLayer::completeTexture(Texture2D *pTexture, void *pUserData)
{
    PixNetworkImageModel *pModel = static_cast<PixNetworkImageModel*>(pUserData);
    
    if(pModel == nullptr)
    {
        return;
    }
    
    if(pTexture == nullptr)
    {
        pModel->release();
        return;
    }
    
    if(mEditScreen == nullptr)
    {
        pModel->release();
        return;
    }
    
    Size editScreenSize = mEditScreen->getContentSize();
    
    
    if (pModel->nType == CLIPPING_MASK)
    {
        float fScaleX = 1.0f;
        float fScaleY = 1.0f;
        float fScale = 1.0f;

        Size size = Size(pTexture->getContentSize().width,
                         pTexture->getContentSize().height);
        
        float fRotation = fabs(pModel->rotate);
       
        if(fRotation != 0)
        {
            if(fRotation >= 180)
            {
                fRotation -= 180;
            }
            
            float fSin = PixMath::sharedObject()->fast_sinf(fRotation);
            Size rotationSize = size;
            
            size.width += fabs((rotationSize.height - rotationSize.width)) * fSin;
            size.height += fabs((rotationSize.width - rotationSize.height)) * fSin;
        }
       
        
        fScaleX = mEditScreen->getContentSize().width / size.width;
        fScaleY = mEditScreen->getContentSize().height / size.height;
        
        if (fScaleX > fScaleY)
        {
            fScale = fScaleY;
        }
        else
        {
            fScale = fScaleX;
        }

        mEditSprite = dynamic_cast<PixMaskSprite*>(mEditScreen->getChildByTag(pModel->nTag));
        
        if(mEditSprite)
        {
            mEditSprite->setMaskSpriteTexture(pTexture, fScale, fScale);
        }
        else
        {
            mEditSprite = PixMaskSprite::create();
            mEditSprite->setMaskSpriteTexture(pTexture, fScale, fScale);
            mEditSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            mEditSprite->setPosition(Vec2(mEditScreen->getContentSize().width / 2,
                                         mEditScreen->getContentSize().height / 2));
            mEditSprite->setRotation(pModel->rotate);
            mEditSprite->setLocalZOrder(pModel->zOrder);
            mEditSprite->setRotation(pModel->rotate);
            mEditSprite->setTag(pModel->nTag);
            
            mEditScreen->addChild(mEditSprite);
        }
    }
    else if(pModel->nType == CLIPPING_IMAGE)
    {
        float fScaleX = 1.0f;
        float fScaleY = 1.0f;
        float fScale = 1.0f;
        
        Size size = Size(pTexture->getContentSize().width,
                         pTexture->getContentSize().height);
        
        float fRotation = fabs(pModel->rotate);
        
        if(fRotation != 0)
        {
            if(fRotation >= 180)
            {
                fRotation -= 180;
            }
            
            float fSin = PixMath::sharedObject()->fast_sinf(fRotation);
            Size rotationSize = size;
            
            size.width += fabs((rotationSize.height - rotationSize.width)) * fSin;
            size.height += fabs((rotationSize.width - rotationSize.height)) * fSin;
        }

        fScaleX = mEditScreen->getContentSize().width  / size.width;
        fScaleY = mEditScreen->getContentSize().height / size.height;
        
        if (fScaleX > fScaleY)
        {
            fScale = fScaleY;
        }
        else
        {
            fScale = fScaleX;
        }

        mEditSprite = dynamic_cast<PixMaskSprite*>(mEditScreen->getChildByTag(pModel->nTag));
        
        if(mEditSprite)
        {
            mEditSprite->setImageSpriteTexture(pTexture, fScale, fScale);
        }
        else
        {
            mEditSprite = PixMaskSprite::create();
            mEditSprite->setImageSpriteTexture(pTexture, fScale, fScale);
            mEditSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            mEditSprite->setPosition(Vec2(mEditScreen->getContentSize().width / 2,
                                         mEditScreen->getContentSize().height / 2));
            mEditSprite->setRotation(pModel->rotate);
            mEditSprite ->setLocalZOrder(pModel->zOrder);
            mEditSprite->setRotation(pModel->rotate);
            mEditSprite->setTag(pModel->nTag);
            
            mEditScreen->addChild(mEditSprite);
        }
        
        mEditSprite->setNetworkImageModel(pModel);
        pModel->retain();
    }
    
    pModel->release();
}

void PixPhotoEditingLayer::initGestureRecognizer()
{
    // Init gesture recognizer
    gestureRecognizer = new PixSimpleGestureRecognizer();
    gestureRecognizer->init();
    gestureRecognizer->setGestureHandler(this);
    
    // Enable all gesture kinds
    gestureRecognizer->setTapEnabled(false);
    gestureRecognizer->setDoubleTapEnabled(false);
    gestureRecognizer->setLongPressEnabled(false);
    gestureRecognizer->setPanEnabled(true);
    gestureRecognizer->setPinchEnabled(false);
    gestureRecognizer->setRotationEnabled(false);
    gestureRecognizer->setSwipeEnabled(false);
    
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

}

void PixPhotoEditingLayer::updateEditPhotoNumberLabel()
{
    if(photonumber)
    {
        int nItemNum = mn_Index + 1;
        
        photonumber->setString(StringUtils::format("%d/%d",nItemNum, mModel->getSmartMaskImageModelCount()));
    }
}

void PixPhotoEditingLayer::showInfo(const std::string &message)
{
    
    if(infor)
    {
        Vec2 pos = infor->getPosition();
        std::string fontName = infor->getFontName();
        float fontSize = infor->getFontSize();
        Color4B textColor = infor->getTextColor();
        
        PixPhotoEditingInfo::showInfo(rootNode, message.c_str(), pos, 3.0f, fontName.c_str(), fontSize, textColor);
        
    }
}

#pragma mark - Button

void PixPhotoEditingLayer::btnBeforePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mn_Index > 0)
        {
            --mn_Index;
            setEditScreen();
        }
    }
}

void PixPhotoEditingLayer::btnNextPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mn_Index < (mModel->getSmartMaskImageModelCount() - 1))
        {
            ++mn_Index;
            setEditScreen();
        }
    }
}

void PixPhotoEditingLayer::btnCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        mbln_isComplete = true;
        hideLayerAction();
    }
}

void PixPhotoEditingLayer::btnCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixPhotoEditingLayer::btnResetPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        setEditScreen();
        
        std::string msg = "사진을 원본으로 되돌립니다.";
        showInfo(msg);
    }
}

void PixPhotoEditingLayer::btnReversalPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mEditSprite)
        {
            Sprite *imageSprite = mEditSprite->getImageSprite();
            
            if(imageSprite)
            {
                bool flippedX = imageSprite->isFlippedX();
                mEditSprite->setImageSpriteFlippedX(!flippedX);
                
                std::string msg = "사진을 좌우 반전합니다.";
                showInfo(msg);

            }
           
        }
    }
}

void PixPhotoEditingLayer::btnRotationPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        Sprite *imageSprite = mEditSprite->getImageSprite();
        
        float fRotation = imageSprite->getRotation();
        
        if(fRotation < -360)
        {
            fRotation += 360;
        }
        
        fRotation -= 90;
        
        mEditSprite->setImageSpriteRotation(fRotation);
        
        std::string msg = "우측으로 90도 씩 회전합니다";
        showInfo(msg);
    }
}

void PixPhotoEditingLayer::btnExpansionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mEditSprite)
        {
            PixNetworkImageModel* pModel = mEditSprite->getNetworkImageModel();
            Sprite *imageSprite = mEditSprite->getImageSprite();
            
            if(pModel && imageSprite)
            {
                float fExpansion = pModel->cocos2dScale * 0.1;
                float fScale = imageSprite->getScale() + fExpansion;;
                float fMaxinScale = pModel->cocos2dScale * 2;
                
                if (fScale <= fMaxinScale)
                {
                    mEditSprite->setImageSpriteScale(fScale);
                    
                    std::string msg = "10%씩 확대합니다.";
                    showInfo(msg);
                }
                else
                {
                    std::string msg = "사진을 더이상 확대할 수 없습니다.";
                    PixToast::showToast(msg.c_str(), Vec2(986,270));
                }
            }
        }
    }
}

void PixPhotoEditingLayer::btnReductionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(mEditSprite)
        {
            PixNetworkImageModel* pModel = mEditSprite->getNetworkImageModel();
            Sprite *imageSprite = mEditSprite->getImageSprite();
            
            if(pModel && imageSprite)
            {
                float fReduction = pModel->cocos2dScale * 0.1;
                float fScale = imageSprite->getScale() - fReduction;
                float fMinScale = pModel->cocos2dScale;
                
                if(fScale >= fMinScale)
                {
                    mEditSprite->setImageSpriteScale(fScale);
                    
                    std::string msg = "10%씩 축소합니다.";
                    showInfo(msg);
                }
                else
                {
                    std::string msg = "사진을 더이상 축소할 수 없습니다.";
                    PixToast::showToast(msg.c_str(), Vec2(986,270));
                }
            }
        }
    }
}

#pragma mark - Touch & Touches

bool PixPhotoEditingLayer::onTouchBegan(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    gestureRecognizer->onTouchBegan(touch, event);
    
    return PixModalLayer::onTouchBegan(touch, event);
}

void PixPhotoEditingLayer::onTouchMoved(Touch* touch, Event* event)
{
    PixModalLayer::onTouchMoved(touch, event);
    
    // Let the gesture recognizer to do its work
    gestureRecognizer->onTouchMoved(touch, event);
}

void PixPhotoEditingLayer::onTouchEnded(Touch* touch, Event* event)
{
    PixModalLayer::onTouchEnded(touch, event);
    
    gestureRecognizer->onTouchEnded(touch, event);
}

void PixPhotoEditingLayer::onTouchCancelled(Touch* touch, Event* event)
{
    PixModalLayer::onTouchCancelled(touch, event);
    
    gestureRecognizer->onTouchCancelled(touch, event);
}

#pragma mark - GestureHandler

bool PixPhotoEditingLayer::onGestureTap(PixGestureTap* gesture)
{
    if (NodeContainsPoint(this, gesture->getLocation()))
    {
        if (gesture->getNumClicks() == 1)
        {
            CCLOG("Tap detected");
        }
        else
        {
            CCLOG("Double Tap detected");
        }
    }
    
    return false;
}

bool PixPhotoEditingLayer::onGestureLongPress(PixGestureLongPress* gesture)
{
    if (NodeContainsPoint(this, gesture->getLocation()))
    {
        CCLOG("Long Press detected");
    }
    
    return false;
}

bool PixPhotoEditingLayer::onGesturePan(PixGesturePan* gesture)
{
    if(mEditSprite == nullptr)
        return false;
    
    PixNetworkImageModel *pModel = mEditSprite->getNetworkImageModel();
    
    if(pModel == nullptr)
        return false;
    
    static int lastPanId = -1;
    static bool panInsideNode = false;
    
    // A new pan
    if (gesture->getID() != lastPanId)
    {
        lastPanId = gesture->getID();
        panInsideNode = NodeContainsPoint(mEditSprite, gesture->getLocation());
    }
    
    if (panInsideNode)
    {
        if(mEditSprite)
        {
            Sprite *maskSprite = mEditSprite->getMaskSprite();
            Sprite *imageSprite = mEditSprite->getImageSprite();

            if(maskSprite && imageSprite)
            {
                PixNetworkImageModel *pModel = mEditSprite->getNetworkImageModel();
                
                if(pModel)
                {
                    Vec2 pos = Vec2(imageSprite->getPosition().x + gesture->getTranslation().x,
                                    imageSprite->getPosition().y - gesture->getTranslation().y);
                    
                    Size maskSize = Size(maskSprite->getContentSize().width * maskSprite->getScale(),
                                         maskSprite->getContentSize().height * maskSprite->getScale());
                    
                    Size imageSize = Size(imageSprite->getContentSize().width * imageSprite->getScale(),
                                          imageSprite->getContentSize().height * imageSprite->getScale());
                    
                    Vec2 panPos = Vec2(fabs(maskSize.width - imageSize.width) * 0.5f,
                                       fabs(maskSize.height - imageSize.height) * 0.5f);
                    
                    float fMoveX = fabs(pos.x - pModel->cocos2dPos.x);
                    float fMoveY = fabs(pos.y - pModel->cocos2dPos.y);
                    
                    if (panPos.x > fMoveX && panPos.y > fMoveY)
                    {
                        mEditSprite->moveImageSprite(pos);
                    }
                }
            }
        }
    }
    
    return false;
}

bool PixPhotoEditingLayer::onGesturePinch(PixGesturePinch* gesture)
{
    
//    if(mEditSprite == nullptr)
//        return false;
//
//    PixNetworkImageModel *pModel = mEditSprite->getNetworkImageModel();
//    
//    if(pModel == nullptr)
//        return false;
//    
//    static int lastPinchId = -1;
//    static bool pinchInsideNode = false;
//    static float originalScale;
//    static float mf_PinchSize = - 1;
//
//    // A new pinch
//    if (gesture->getID() != lastPinchId)
//    {
//        lastPinchId = gesture->getID();
//        pinchInsideNode = NodeContainsPoint(mEditSprite, gesture->getLocation());
//        originalScale = mEditSprite->getScale();
//    }
//    
//    if (pinchInsideNode)
//    {
//        float fMinScale = pModel->cocos2dScale;
//        float fMaxinScale = pModel->cocos2dScale * 2;
//        float fScale = mEditSprite->getScale();
//        
//        float fLength = PixMath::sharedObject()->getLength(gesture->getTouch0()->getLocation(), gesture->getTouch1()->getLocation());
//        
//        
//        if (mf_PinchSize == -1)
//        {
//            mf_PinchSize = fLength;
//        }
//        else
//        {
//            float percent = fLength / mf_PinchSize;
//            
//            fScale = fScale * percent;
//            
//            if (fScale >= fMinScale && fScale <= fMaxinScale)
//            {
//                mEditSprite->setScale(fScale);
//                
//                if (gesture->getTouch0() == nullptr || gesture->getTouch1() == nullptr)
//                    return false;
//                
//                Vec2 curPosTouch0 = Director::getInstance()->convertToGL(gesture->getTouch0()->getLocationInView());
//                Vec2 curPosTouch1 = Director::getInstance()->convertToGL(gesture->getTouch1()->getLocationInView());
//                Vec2 prevPosTouch0 = Director::getInstance()->convertToGL(gesture->getTouch0()->getPreviousLocationInView());
//                Vec2 prevPosTouch1 = Director::getInstance()->convertToGL(gesture->getTouch1()->getPreviousLocationInView());
//                
//                
//                Vec2 curPosLayer = curPosTouch0 + curPosTouch1 / 2.0f;
//                Vec2 prevPosLayer = prevPosTouch0 + prevPosTouch1 / 2.0f;
//                
//                if (!prevPosLayer.equals(curPosLayer))
//                {
//                    
//                    Vec2 pinchPos = curPosLayer - prevPosLayer;
//                    
//                    Vec2 pos = Vec2(mEditSprite->getPosition().x + curPosLayer.x - prevPosLayer.x,
//                                    mEditSprite->getPosition().y + curPosLayer.y - prevPosLayer.y);
//                    
//                    moveEditSprite(pos);
//                }
//            }
//
//            mf_PinchSize = fLength;
//        }
//    }
    
    return false;
}

bool PixPhotoEditingLayer::onGestureRotation(PixGestureRotation* gesture)
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

bool PixPhotoEditingLayer::onGestureSwipe(PixGestureSwipe* gesture)
{
    SwipeDirection dir = gesture->getDirection();
    
    if (dir == SwipeDirectionNorth)
    {
        CCLOG("Swipe detected: NORTH");
    }
    else if (dir == SwipeDirectionNorthEast)
    {
        CCLOG("Swipe detected: NORTH EAST");
    }
    else if (dir == SwipeDirectionEast)
    {
        CCLOG("Swipe detected: EAST");
    }
    else if (dir == SwipeDirectionSouthEast)
    {
        CCLOG("Swipe detected: SOUTH EAST");
    }
    else if (dir == SwipeDirectionSouth)
    {
        CCLOG("Swipe detected: SOUTH");
    }
    else if (dir == SwipeDirectionSouthWest)
    {
        CCLOG("Swipe detected: SOUTH WEST");
    }
    else if (dir == SwipeDirectionWest)
    {
        CCLOG("Swipe detected: WEST");
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

#pragma mark - Action

void PixPhotoEditingLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixPhotoEditingLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixPhotoEditingLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fheight = Director::getInstance()->getVisibleSize().height;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-fheight));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixPhotoEditingLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}
    
void PixPhotoEditingLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixPhotoEditingLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        if(mDelegate)
        {
            if(mbln_isComplete)
            {
                mDelegate->hideLayerActionFinish(mModel->clone());
            }
            else
            {
                mDelegate->hideLayerActionFinish(nullptr);
            }
        }

        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixPhotoEditingLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}
