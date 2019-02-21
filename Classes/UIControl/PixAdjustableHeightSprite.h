//
//  PixAdjustableHeightSprite.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 3..
//
//

// 캘러리 씬에서 사용되고 있는 커스텀 Sprite
// 프레임 사이즈에 기반으로 텍스처의 Height 크기가 결정 된다

#ifndef PixAdjustableHeightSprite_h
#define PixAdjustableHeightSprite_h

#include "cocos2d.h"

USING_NS_CC;

class PixAdjustableHeightSprite : public Sprite
{
    
public :
    
    PixAdjustableHeightSprite();
    virtual ~PixAdjustableHeightSprite();
    
    static PixAdjustableHeightSprite* create(const std::string& filename, Size frameSize);
    static PixAdjustableHeightSprite* createWithTexture(Texture2D *texture, Size frameSize);
    
    void initFrame(Size frameSize);
    
    virtual void onEnterTransitionDidFinish();
    
private:
    
    CC_SYNTHESIZE_PASS_BY_REF(Size, _frameSize, FrameSize)
};



#endif /* PixAdjustableHeightSprite_h */
