//
//  PixEditSprite.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 27..
//
//

#ifndef PixEditSprite_h
#define PixEditSprite_h

#include "cocos2d.h"
#include "PixMaskSprite.h"

static const Color4B SELECT_COLOR = {170,200,250,166};
static const Color4B CHANGE_COLOR = {80,120,230,153};
static const Color4B LONG_PRESS_COLOR = {255,255,255,178};

USING_NS_CC;

class PixSmartMaskImageModel;
class PixEditAlbumLayer;

class PixEditSprite : public PixMaskSprite
{
public :
    
    PixEditSprite();
    virtual ~PixEditSprite();
    
    static PixEditSprite* create(PixNetworkImageModel *pModel, int nSmartIndex, Size &albumSize, Node *pEditAlbumImageLayer, bool fadeIn = false);
    
    bool initEditSpriteSprite(PixNetworkImageModel *pModel, int nSmartIndex, Size &albumSize);
    virtual void initSmartAlbumMaskSprite();
    
    void selectLayerVisible(bool visible);
    void changeLayerVisible(bool visible);
    void longPressLayerVisible(bool visible);
    
    void setChangeMode(bool changeMode);
    void changeImageSprite(Sprite *pImageSprite);
    void changeImageTexture(Texture2D *pTexture);
    
    inline bool isChange() { return mbln_IsChange; }
    
    virtual void onEnterTransitionDidFinish();
    
    CC_SYNTHESIZE(Node*, mEditAlbumImageLayer, EditAlbumImageLayer)
    CC_SYNTHESIZE(PixSmartMaskImageModel*, mSmartMaskImageModel, SmartMaskImageModel)
    CC_SYNTHESIZE(Size, albumSize, AlbumSize)

private:
    
    Sprite *selectSprite;
    Sprite *changeSprite;
    Sprite *longPressSprite;
    Sprite *changeSymbol;
    
    bool mbln_IsChange;
};

#endif /* PixEditSprite_h */
