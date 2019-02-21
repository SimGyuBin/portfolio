//
//  PixSmartSkinSprite.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 1..
//
//

#include "PixSmartSkinSprite.h"

PixSmartSkinSprite* PixSmartSkinSprite::createWithTexture(Texture2D *texture, bool fadeIn)
{
    PixSmartSkinSprite *sprite = new (std::nothrow) PixSmartSkinSprite(fadeIn);
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

PixSmartSkinSprite::PixSmartSkinSprite(bool fadeIn)
: _isInitWithTexture(false)
, mbln_FadeIn(false)
{
    setFadeIn(fadeIn);
}

PixSmartSkinSprite:: ~PixSmartSkinSprite()
{

}

void PixSmartSkinSprite::onEnterTransitionDidFinish()
{
    Sprite::onEnterTransitionDidFinish();
}

bool PixSmartSkinSprite::initWithTexture(Texture2D *texture)
{
    if(Sprite::initWithTexture(texture))
    {
        if(getFadeIn())
        {
            setFadeIn(false);
            setOpacity(0);
            fadeInSpriteAction();
        }
        else
        {
            _isInitWithTexture = true;
        }
    }
    else
    {
        _isInitWithTexture = false;
    }
    
    return true;
}

#pragma mark - Action

void PixSmartSkinSprite::fadeInSpriteAction()
{
    FadeIn *fadeIn = FadeIn::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSmartSkinSprite::fadeInSpriteActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeIn, callFuncN, NULL);
    
    runAction(sequence);
}

void PixSmartSkinSprite::fadeOutSpriteAction()
{
    FadeOut *fadeOut = FadeOut::create(0.3f);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSmartSkinSprite::fadeOutSpriteActionFinish, this));
    
    Sequence *sequence = Sequence::create(fadeOut, callFuncN, NULL);
    
    runAction(sequence);
}

void PixSmartSkinSprite::fadeInSpriteActionFinish(Node* node)
{
  
    _isInitWithTexture = true;
}

void PixSmartSkinSprite::fadeOutSpriteActionFinish(Node* node)
{
    
}
