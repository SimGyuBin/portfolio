//
//  PixMaskSprite.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 18..
//
//

#ifndef PixMaskSprite_h
#define PixMaskSprite_h

#include "cocos2d.h"

USING_NS_CC;

class PixNetworkImageModel;

class PixMaskSprite : public Sprite
{
public:

    static PixMaskSprite* create(bool fadeIn = false);
    static PixMaskSprite* create(const char *imageFile, const char *maskFile, bool fadeIn = false);
    PixMaskSprite();
    virtual ~PixMaskSprite();
    
    void setMaskSpriteTexture(Texture2D *pTexture, float scaleX, float scaleY);
    void setImageSpriteTexture(Texture2D *pTexture, float scaleX, float scaleY);
    void updateMaskSpriteRender();
    void moveMaskSprite(const Vec2 &pos);
    void moveImageSprite(const Vec2 &pos);
    
    void setImageSpriteFlippedX(bool flippedX);
    void setImageSpriteRotation(float rotation);
    void setImageSpriteScale(float fScale);
    
    inline bool isTransitionFinished()
    {
        return _isTransitionFinished;
    }
    
    inline bool isInitWithTexture()
    {
        return _isInitWithTexture;
    }
    
#pragma mark - Action
    
    void fadeInSpriteAction();
    void fadeOutSpriteAction();
    void fadeInSpriteActionFinish(Node* node);
    void fadeOutSpriteActionFinish(Node* node);
    
#pragma mark android
    
    static std::vector<PixMaskSprite*> s_instanceVec;
    void reloadTexture(float dt);
    void applicationWillEnterForeground();
    void beforeReloadTexture();
    
    Size mRenderTextureSize;
    
protected:
    
    bool _isInitWithTexture;
    
    CC_SYNTHESIZE(Sprite*, maskSprite, MaskSprite)
    CC_SYNTHESIZE(Sprite*, imageSprite, ImageSprite)
    CC_SYNTHESIZE(Texture2D*, maskSpriteTexture, MaskSpriteTexture)
    CC_SYNTHESIZE(Texture2D*, imageSpriteTexture, ImageSpriteTexture)
    CC_SYNTHESIZE(PixNetworkImageModel*, networkImageModel, NetworkImageModel)
    CC_SYNTHESIZE(bool, mbln_DataloadSuccess, DataloadSuccess)
    CC_SYNTHESIZE(bool, mbln_FadeIn, FadeIn)
    
    virtual void initSmartAlbumMaskSprite();
    virtual void initMaskSprite(const char *imageFile, const char *maskFile);
    
};
#endif /* PixMaskSprite_h */
