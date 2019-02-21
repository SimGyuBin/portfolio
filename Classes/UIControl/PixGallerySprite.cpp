//
//  PixGallerySprite.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 27..
//
//

#include "PixGallerySprite.h"

std::vector<PixGallerySprite*> PixGallerySprite::s_instanceVec;

PixGallerySprite::PixGallerySprite()
: _frameSize(Size::ZERO)
, mf_CenterCropScale(0.0f)
, _loading(nullptr)
, mRenderTextureSize(Size::ZERO)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    s_instanceVec.push_back(this);
#endif
}

PixGallerySprite::~PixGallerySprite()
{
    if(maskSprite)
    {
        Director::getInstance()->getTextureCache()->removeTexture(maskSprite->getTexture());
    }
    
    CC_SAFE_RELEASE(maskSprite);
    
    if(imageSprite)
    {
        Director::getInstance()->getTextureCache()->removeTexture(imageSprite->getTexture());
    }
    
    CC_SAFE_RELEASE(imageSprite);
    
    Director::getInstance()->getTextureCache()->removeTexture(getTexture());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::vector<PixGallerySprite*>::iterator it;
    it = find( s_instanceVec.begin(), s_instanceVec.end(), this);
    if ((*it))
    {
        s_instanceVec.erase(it);
    }
#endif
}

PixGallerySprite* PixGallerySprite::createWithTexture(Texture2D *pImageTexture, const std::string& maskFile)
{
    PixGallerySprite *sprite = new (std::nothrow) PixGallerySprite();
    if (sprite && sprite->init())
    {
        sprite->initGallerySprite(pImageTexture, maskFile);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void PixGallerySprite::initGallerySprite(Texture2D *pImageTexture, const std::string& maskFile)
{
    maskSprite = Sprite::create(maskFile);
    maskSprite->getTexture()->setAntiAliasTexParameters();
    maskSprite->setPosition(Vec2(maskSprite->getContentSize().width/2, maskSprite->getContentSize().height/2));
    maskSprite->setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
    maskSprite->setFlippedY(true);
    maskSprite->retain();
    
    setFrameSize(maskSprite->getContentSize());
    
    Texture2D *texture = pImageTexture;

    if(texture)
    {
        Size textureSize = texture->getContentSize();
        
        float tw = ((float) getFrameSize().width) / ((float) textureSize.width);
        float th = ((float) getFrameSize().height) / ((float) textureSize.height);
        
        float fScale = 1.0f;
        
        if (tw > th)
        {
            fScale = tw;
        }
        else
        {
            fScale = th;
        }
        
        if (fScale > 0)
        {
            
        }
        else
        {
            
        }
        
        imageSprite = Sprite::createWithTexture(texture);
        imageSprite->setScale(fScale);
        imageSprite->getTexture()->setAntiAliasTexParameters();
        imageSprite->setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});
        imageSprite->setFlippedY(true);
        imageSprite->retain();
        
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
        
        initWithTexture(rt->getSprite()->getTexture());
        rt->getSprite()->getTexture()->setAntiAliasTexParameters();
        setPosition(Vec2(maskSprite->getContentSize().width/2, maskSprite->getContentSize().height/2));
        setOpacity(0);
    }
}

void PixGallerySprite::onEnterTransitionDidFinish()
{
    Sprite::onEnterTransitionDidFinish();
    fadeInSpriteAction();
}

void PixGallerySprite::updateMaskSpriteRender()
{
    RenderTexture *rt = RenderTexture::create(mRenderTextureSize.width, mRenderTextureSize.height);
    
    rt->beginWithClear(0,0,0,0);
    maskSprite->visit();
    imageSprite->visit();
    rt->end();
    
    this->setTexture(rt->getSprite()->getTexture());
}

void PixGallerySprite::changeImageSprite(Sprite *pImageSprite)
{
    imageSprite = pImageSprite;
    imageSprite->setPosition(Vec2(maskSprite->getContentSize().width/2, maskSprite->getContentSize().height/2));
    
    Size textureSize = pImageSprite->getTexture()->getContentSize();
    
    float tw = ((float) getFrameSize().width) / ((float) textureSize.width);
    float th = ((float) getFrameSize().height) / ((float) textureSize.height);
    
    float fScale = 1.0f;
    
    if (tw > th)
    {
        fScale = tw;
    }
    else
    {
        fScale = th;
    }
    
//    float fPosX = -((((float) textureSize.width) * fScale) - ((float) getFrameSize().width)) / 2;
//    float fPosY = -((((float) textureSize.height) * fScale) - ((float) getFrameSize().height)) / 2;
//    imageSprite->setPosition(Vec2(fPosX + getFrameSize().width / 2,fPosY + getFrameSize().height / 2));
    
    imageSprite->setScale(fScale);
    updateMaskSpriteRender();
}

void PixGallerySprite::reloadTexture(float dt)
{
    unschedule(schedule_selector(PixGallerySprite::reloadTexture));
    
    if(maskSprite && imageSprite)
    {
        updateMaskSpriteRender();
    }
}

void PixGallerySprite::applicationWillEnterForeground()
{
    schedule(schedule_selector(PixGallerySprite::reloadTexture));
}

void PixGallerySprite::beforeReloadTexture()
{
    setTexture(nullptr);
}

#pragma mark - Action

void PixGallerySprite::fadeInSpriteAction()
{
    FadeIn *fadeIn = FadeIn::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixGallerySprite::fadeInSpriteActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);

    runAction(sequence);
}

void PixGallerySprite::fadeOutSpriteAction()
{
    FadeOut *fadeOut = FadeOut::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixGallerySprite::fadeOutSpriteActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeOut, callFuncN, NULL);
    
    runAction(sequence);
}

void PixGallerySprite::fadeInSpriteActionFinish(Node* node)
{
    if(getloading())
        getloading()->setVisible(false);
}

void PixGallerySprite::fadeOutSpriteActionFinish(Node* node)
{
   
}
