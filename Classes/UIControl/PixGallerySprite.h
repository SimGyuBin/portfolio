//
//  PixGallerySprite.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 27..
//
//

// 캘러리 씬에서 사용되고 있는 커스텀 Sprite
// 프레임 사이즈에 기반으로 텍스처 생성

#ifndef PixGallerySprite_h
#define PixGallerySprite_h

#include "cocos2d.h"

USING_NS_CC;

class PixGallerySprite : public Sprite
{
    
public :
  
    PixGallerySprite();
    virtual ~PixGallerySprite();
  
    static PixGallerySprite* createWithTexture(Texture2D *pImageTexture, const std::string& maskFile);
    
    void initGallerySprite(Texture2D *pImageTexture, const std::string& maskFile);
    
    virtual void onEnterTransitionDidFinish();
    void updateMaskSpriteRender();
    void changeImageSprite(Sprite *pImageSprite);

#pragma mark android
    
    static std::vector<PixGallerySprite*> s_instanceVec;
    void reloadTexture(float dt);
    void applicationWillEnterForeground();
    void beforeReloadTexture();
    
private:

    Size mRenderTextureSize;
    
    CC_SYNTHESIZE(Sprite*, maskSprite, MaskSprite)
    CC_SYNTHESIZE(Sprite*, imageSprite, ImageSprite)
    CC_SYNTHESIZE(float, mf_CenterCropScale, CenterCropScale)
    CC_SYNTHESIZE_PASS_BY_REF(Size, _frameSize, FrameSize)
    CC_SYNTHESIZE(Node*, _loading, loading);
    
#pragma mark - Action
    
    void fadeInSpriteAction();
    void fadeOutSpriteAction();
    void fadeInSpriteActionFinish(Node* node);
    void fadeOutSpriteActionFinish(Node* node);

};

#endif /* PixGallerySprite_h */
