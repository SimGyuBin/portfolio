//
//  PixAdjustableHeightSprite.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 3..
//
//

#include "PixAdjustableHeightSprite.h"

PixAdjustableHeightSprite::PixAdjustableHeightSprite()
: _frameSize(Size::ZERO)
{
    
}

PixAdjustableHeightSprite::~PixAdjustableHeightSprite()
{
    
}

PixAdjustableHeightSprite* PixAdjustableHeightSprite::create(const std::string& filename, Size frameSize)
{
    PixAdjustableHeightSprite *sprite = new (std::nothrow) PixAdjustableHeightSprite();
    if (sprite && sprite->init())
    {
        sprite->initFrame(frameSize);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

PixAdjustableHeightSprite* PixAdjustableHeightSprite::createWithTexture(Texture2D *texture, Size frameSize)
{
    PixAdjustableHeightSprite *sprite = new (std::nothrow) PixAdjustableHeightSprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->initFrame(frameSize);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void PixAdjustableHeightSprite::initFrame(Size frameSize)
{
    setFrameSize(frameSize);
    
    Texture2D *texture = getTexture();
    
    if(texture)
    {
        Size textureSize = texture->getContentSize();
        
        float tw = ((float) frameSize.width) / ((float) textureSize.width);
        
        float fScale = 1.0f;
        
        fScale = tw;
        
        setScale(fScale);
        setAnchorPoint(Vec2(0, 0));
            
//        setOpacity(0);
    }
}

void PixAdjustableHeightSprite::onEnterTransitionDidFinish()
{
    Sprite::onEnterTransitionDidFinish();
    
//    FadeIn *fadeIn = FadeIn::create(0.3f);
//    this->runAction(fadeIn);
}
