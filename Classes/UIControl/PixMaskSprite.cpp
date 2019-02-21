//
//  PixMaskSprite.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 18..
//
//

#include "PixMaskSprite.h"
#include "../Network/PixHttpClient.h"

std::vector<PixMaskSprite*> PixMaskSprite::s_instanceVec;

PixMaskSprite* PixMaskSprite::create(bool fadeIn)
{
    PixMaskSprite *maskSprite = new PixMaskSprite();
    
    if(maskSprite && maskSprite->init())
    {
        maskSprite->setFadeIn(fadeIn);
        maskSprite->autorelease();
        return maskSprite;
    }
    else
    {
        CC_SAFE_DELETE(maskSprite);
        return nullptr;
    }
}

PixMaskSprite* PixMaskSprite::create(const char *imageFile, const char *maskFile, bool fadeIn)
{
    PixMaskSprite *maskSprite = new PixMaskSprite();
    maskSprite->setFadeIn(fadeIn);
    maskSprite->initMaskSprite(imageFile, maskFile);
    maskSprite->autorelease();
    
    return maskSprite;
}

PixMaskSprite::PixMaskSprite()
: maskSprite(nullptr)
, imageSprite(nullptr)
, maskSpriteTexture(nullptr)
, imageSpriteTexture(nullptr)
, networkImageModel(nullptr)
, mbln_DataloadSuccess(false)
, mRenderTextureSize(Size::ZERO)
, _isInitWithTexture(false)
, mbln_FadeIn(false)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    s_instanceVec.push_back(this);
#endif
}

PixMaskSprite::~PixMaskSprite()
{
    if(maskSprite)
    {
        Director::getInstance()->getTextureCache()->removeTexture(maskSprite->getTexture());
    }

    if(imageSprite)
    {
        Director::getInstance()->getTextureCache()->removeTexture(imageSprite->getTexture());
    }
    
    CC_SAFE_RELEASE(maskSprite);
    CC_SAFE_RELEASE(imageSprite);
    
    CC_SAFE_RELEASE_NULL(networkImageModel);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::vector<PixMaskSprite*>::iterator it;
    it = find( s_instanceVec.begin(), s_instanceVec.end(), this);
    if ((*it))
    {
        s_instanceVec.erase(it);
    }
#endif
}

void PixMaskSprite::setMaskSpriteTexture(Texture2D *pTexture, float scaleX, float scaleY)
{
    maskSpriteTexture = pTexture;
    
    maskSprite = Sprite::createWithTexture(pTexture);
    maskSprite->getTexture()->setAntiAliasTexParameters();
    maskSprite->setScale(scaleX, scaleY);
    maskSprite->setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
    maskSprite->setFlippedY(true);
    maskSprite->retain();
    
    if(maskSprite && imageSprite)
    {
        initSmartAlbumMaskSprite();
    }
}

void PixMaskSprite::setImageSpriteTexture(Texture2D *pTexture, float scaleX, float scaleY)
{
    imageSpriteTexture = pTexture;
    
    imageSprite = Sprite::createWithTexture(pTexture);
    imageSprite->getTexture()->setAntiAliasTexParameters();
    imageSprite->setScale(scaleX, scaleY);
    imageSprite->setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});
    imageSprite->setFlippedY(true);
    imageSprite->retain();
    
    if(maskSprite && imageSprite)
    {
        initSmartAlbumMaskSprite();
    }
}

void PixMaskSprite::initSmartAlbumMaskSprite()
{
    Size masksize = Size(maskSprite->getContentSize().width * maskSprite->getScaleX(),
                         maskSprite->getContentSize().height * maskSprite->getScaleY());

    maskSprite->setPosition(Vec2(masksize.width / 2, masksize.height / 2));
    imageSprite->setPosition(Vec2(masksize.width / 2, masksize.height / 2));

    RenderTexture *rt = RenderTexture::create(masksize.width, masksize.height);
    mRenderTextureSize = masksize;
    
    rt->begin();
    maskSprite->visit();
    imageSprite->visit();
    rt->end();
    
    if(rt->getSprite()->getTexture() == nullptr)
    {
        CCLOG("simsimssim PixMaskSprite::initSmartAlbumMaskSprite rt->getSprite()->getTexture() == nullptr ");
    }
    
    initWithTexture(rt->getSprite()->getTexture());
    rt->getSprite()->getTexture()->setAntiAliasTexParameters();
    
    if(networkImageModel)
    {
        networkImageModel->cocos2dScale = imageSprite->getScale();
        networkImageModel->cocos2dPos = imageSprite->getPosition();
    }
    
    setDataloadSuccess(true);
    
    setContentSize(Size(getContentSize().width * getScale(),
                        getContentSize().height * getScale()));
    
    if(getFadeIn())
    {
        setFadeIn(false);
        setOpacity(0);
        fadeInSpriteAction();
    }
}

void PixMaskSprite::initMaskSprite(const char *imageFile, const char *maskFile)
{
    maskSprite = Sprite::create(maskFile);
    maskSprite->getTexture()->setAntiAliasTexParameters();
    maskSprite->setPosition(Vec2(maskSprite->getContentSize().width/2, maskSprite->getContentSize().height/2));
    maskSprite->setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
    maskSprite->setFlippedY(true);
    maskSprite->retain();
    
    imageSprite = Sprite::create(imageFile);
    imageSprite->getTexture()->setAntiAliasTexParameters();
    imageSprite->setPosition(Vec2(maskSprite->getContentSize().width/2, maskSprite->getContentSize().height/2));
    imageSprite->setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});
    imageSprite->setFlippedY(true);
    imageSprite->retain();
    
    RenderTexture *rt = RenderTexture::create(maskSprite->getContentSize().width, maskSprite->getContentSize().height);
    
    mRenderTextureSize = maskSprite->getContentSize();
    
    rt->begin();
    maskSprite->visit();
    imageSprite->visit();
    rt->end();
    
    if(rt->getSprite()->getTexture() == nullptr)
    {
        CCLOG("simsimssim PixMaskSprite::initMaskSprite rt->getSprite()->getTexture() == nullptr ");
    }
    
    initWithTexture(rt->getSprite()->getTexture());
    rt->getSprite()->getTexture()->setAntiAliasTexParameters();
    
    setDataloadSuccess(true);
    
    if(getFadeIn())
    {
        setFadeIn(false);
        setOpacity(0);
        fadeInSpriteAction();
    }
}

void PixMaskSprite::updateMaskSpriteRender()
{
    RenderTexture *rt = RenderTexture::create(mRenderTextureSize.width, mRenderTextureSize.height);
    
    rt->beginWithClear(0,0,0,0);
    maskSprite->visit();
    imageSprite->visit();
    rt->end();
    
    this->setTexture(rt->getSprite()->getTexture());   
}

void PixMaskSprite::moveMaskSprite(const Vec2 &pos)
{
    if (maskSprite == nullptr)
        return;
    
    maskSprite->setPosition(pos);
    
    RenderTexture *rt = RenderTexture::create(maskSprite->getContentSize().width, maskSprite->getContentSize().height);
    
    rt->beginWithClear(0,0,0,0);
    maskSprite->visit();
    imageSprite->visit();
    rt->end();
    
    this->setTexture(rt->getSprite()->getTexture());
}

void PixMaskSprite::moveImageSprite(const Vec2 &pos)
{
    if (imageSprite == nullptr)
        return;
    
    imageSprite->setPosition(pos);
    
    RenderTexture *rt = RenderTexture::create(maskSprite->getContentSize().width, maskSprite->getContentSize().height);
    
    rt->beginWithClear(0,0,0,0);
    maskSprite->visit();
    imageSprite->visit();
    rt->end();
    
    this->setTexture(rt->getSprite()->getTexture());
}

void PixMaskSprite::setImageSpriteFlippedX(bool flippedX)
{
    imageSprite->setFlippedX(flippedX);
    updateMaskSpriteRender();
}

void PixMaskSprite::setImageSpriteRotation(float rotation)
{
    imageSprite->setRotation(rotation);
    updateMaskSpriteRender();
}

void PixMaskSprite::setImageSpriteScale(float fScale)
{
    imageSprite->setScale(fScale);
    updateMaskSpriteRender();
}

#pragma mark - Action

void PixMaskSprite::fadeInSpriteAction()
{
    FadeIn *fadeIn = FadeIn::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMaskSprite::fadeInSpriteActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);
    
    runAction(sequence);
}

void PixMaskSprite::fadeOutSpriteAction()
{
    FadeOut *fadeOut = FadeOut::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMaskSprite::fadeOutSpriteActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeOut, callFuncN, NULL);
    
    runAction(sequence);
}

void PixMaskSprite::fadeInSpriteActionFinish(Node* node)
{
    _isInitWithTexture = true;
}

void PixMaskSprite::fadeOutSpriteActionFinish(Node* node)
{
    
}

void PixMaskSprite::reloadTexture(float dt)
{
    unschedule(schedule_selector(PixMaskSprite::reloadTexture));
    
    if(maskSprite && imageSprite)
    {
        updateMaskSpriteRender();
    }
}

void PixMaskSprite::applicationWillEnterForeground()
{
    schedule(schedule_selector(PixMaskSprite::reloadTexture));
}

void PixMaskSprite::beforeReloadTexture()
{
    setTexture(nullptr);
}
