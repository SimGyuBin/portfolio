//
//  PixSmartSkinSprite.h
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 1..
//
//

#ifndef PixSmartSkinSprite_h
#define PixSmartSkinSprite_h

#include "cocos2d.h"

USING_NS_CC;

class PixSmartSkinSprite : public Sprite
{
public:
    
    static PixSmartSkinSprite* createWithTexture(Texture2D *texture, bool fadeIn = false);
   
    PixSmartSkinSprite(bool fadeIn);
    virtual ~PixSmartSkinSprite();
    virtual void onEnterTransitionDidFinish();
    
    inline bool isTransitionFinished()
    {
        return _isTransitionFinished;
    }
    
    inline bool isInitWithTexture()
    {
        return _isInitWithTexture;
    }
    
private:
    
    bool _isInitWithTexture;
    
    CC_SYNTHESIZE(bool, mbln_FadeIn, FadeIn)
    
    bool initWithTexture(Texture2D *texture);
    
#pragma mark - Action
    
    void fadeInSpriteAction();
    void fadeOutSpriteAction();
    void fadeInSpriteActionFinish(Node* node);
    void fadeOutSpriteActionFinish(Node* node);
    
};

#endif /* PixSmartSkinSprite_h */
