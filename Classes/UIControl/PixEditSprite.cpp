//
//  PixEditSprite.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 27..
//
//

#include "PixEditSprite.h"
#include "../Network/PixHttpClient.h"
#include "../Util/PixUtils.h"
#include "../Layer/PixEditAlbumLayer.h"


PixEditSprite* PixEditSprite::create(PixNetworkImageModel *pModel, int nSmartIndex, Size &albumSize, Node *pEditAlbumImageLayere, bool fadeIn)
{
    PixEditSprite *sprite = new (std::nothrow)PixEditSprite();
    if (sprite && sprite->init())
    {
        sprite->setFadeIn(fadeIn);
        sprite->setEditAlbumImageLayer(pEditAlbumImageLayere);
        if(sprite->initEditSpriteSprite(pModel, nSmartIndex, albumSize) == false)
        {
            
        }
        
        sprite->autorelease();
        return sprite;
        
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}


PixEditSprite::PixEditSprite()
: selectSprite(nullptr)
, changeSprite(nullptr)
, longPressSprite(nullptr)
, mbln_IsChange(false)
, albumSize(Size::ZERO)
, mSmartMaskImageModel(nullptr)
, mEditAlbumImageLayer(nullptr)
, changeSymbol(nullptr)
{

}

PixEditSprite::~PixEditSprite()
{
    if(getEditAlbumImageLayer())
    {
        if(changeSymbol)
        {
            getEditAlbumImageLayer()->removeChild(changeSymbol);
        }
    }
}

bool PixEditSprite::initEditSpriteSprite(PixNetworkImageModel *pModel, int nSmartIndex, Size &albumSize)
{
    if(pModel == nullptr)
        return false;
    
    networkImageModel = pModel;
    networkImageModel->retain();
    
    setAlbumSize(albumSize);
    
    return true;
}

void PixEditSprite::initSmartAlbumMaskSprite()
{
    PixMaskSprite::initSmartAlbumMaskSprite();
    
    Size size = Size(maskSprite->getContentSize().width * maskSprite->getScaleX(),
                     maskSprite->getContentSize().height * maskSprite->getScaleY());
    
    // 사진 선택 이펙트
    LayerColor *selectLayer = LayerColor::create(SELECT_COLOR);
    selectLayer->setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});
    selectLayer->setScale(1.5f);
    RenderTexture *render = RenderTexture::create(size.width, size.height);
    
    render->beginWithClear(0,0,0,0);
    maskSprite->visit();
    selectLayer->visit();
    render->end();
    
    selectSprite = Sprite::createWithTexture(render->getSprite()->getTexture());
    selectSprite->setPosition(Vec2(size.width / 2, size.height / 2));
    selectSprite->setVisible(false);
    addChild(selectSprite);
    
    // 사진 교체 이펙트
    LayerColor *changeLayer = LayerColor::create(CHANGE_COLOR);
    changeLayer->setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});
    changeLayer->setScale(1.5f);
    render = RenderTexture::create(size.width, size.height);
    
    render->beginWithClear(0,0,0,0);
    maskSprite->visit();
    changeLayer->visit();
    render->end();
    
    changeSprite = Sprite::createWithTexture(render->getSprite()->getTexture());
    changeSprite->setPosition(Vec2(size.width / 2, size.height / 2));
    changeSprite->setVisible(false);
    addChild(changeSprite);
    
    // 롱 탭 이팩트
    LayerColor *longPressLayer = LayerColor::create(LONG_PRESS_COLOR);
    longPressLayer->setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});
    longPressLayer->setScale(1.5f);
    render = RenderTexture::create(size.width, size.height);
    
    render->beginWithClear(0,0,0,0);
    maskSprite->visit();
    longPressLayer->visit();
    render->end();
    
    longPressSprite = Sprite::createWithTexture(render->getSprite()->getTexture());
    longPressSprite->setPosition(Vec2(size.width / 2, size.height / 2));
    longPressSprite->setVisible(false);
    addChild(longPressSprite);
    
    if(getEditAlbumImageLayer())
    {
        changeSymbol = Sprite::create("EditAlbumLevel/Photochange.png");
        changeSymbol->setPosition(getPosition());
        changeSymbol->setVisible(false);
        getEditAlbumImageLayer()->addChild(changeSymbol, 100);
    }
}

void PixEditSprite::selectLayerVisible(bool visible)
{
    if(selectSprite)
        selectSprite->setVisible(visible);
}

void PixEditSprite::changeLayerVisible(bool visible)
{
    if(changeSprite)
        changeSprite->setVisible(visible);
    
    if(changeSymbol)
        changeSymbol->setVisible(visible);
}

void PixEditSprite::longPressLayerVisible(bool visible)
{
    if(longPressSprite)
        longPressSprite->setVisible(visible);
}

void PixEditSprite::setChangeMode(bool changeMode)
{
    mbln_IsChange = changeMode;
    
    longPressLayerVisible(changeMode);
//    changeLayerVisible(mbln_IsChange);
}

void PixEditSprite::changeImageSprite(Sprite *pImageSprite)
{
    Size size = pImageSprite->getTexture()->getContentSize();
    
    imageSprite = pImageSprite;
    
    Size maskSize = Size(maskSprite->getContentSize().width * maskSprite->getScaleX(),
                         maskSprite->getContentSize().height * maskSprite->getScaleY());
    
    imageSprite->setPosition(Vec2(maskSize.width / 2, maskSize.height / 2));

    float fScaleX = 1.0f;
    float fScaleY = 1.0f;
    float fScale = 1.0f;

    fScaleX = (networkImageModel->size.width * (albumSize.width / networkImageModel->parentSize.width)) / size.width;
    fScaleY = (networkImageModel->size.height * (albumSize.height / networkImageModel->parentSize.height)) / size.height;
    
    if (fScaleX > fScaleY)
    {
        fScale = fScaleX;
    }
    else
    {
        fScale = fScaleY;
    }

    imageSprite->setScale(fScale);
    updateMaskSpriteRender();
}

void PixEditSprite::changeImageTexture(Texture2D *pTexture)
{
    Size size = pTexture->getContentSize();
    
    imageSprite->setTexture(pTexture);
    
    Size maskSize = Size(maskSprite->getContentSize().width * maskSprite->getScaleX(),
                         maskSprite->getContentSize().height * maskSprite->getScaleY());
    
    imageSprite->setPosition(Vec2(maskSize.width / 2, maskSize.height / 2));
    
    float fScaleX = 1.0f;
    float fScaleY = 1.0f;
    float fScale = 1.0f;
    
    fScaleX = (networkImageModel->size.width * (albumSize.width / networkImageModel->parentSize.width)) / size.width;
    fScaleY = (networkImageModel->size.height * (albumSize.height / networkImageModel->parentSize.height)) / size.height;
    
    if (fScaleX > fScaleY)
    {
        fScale = fScaleX;
    }
    else
    {
        fScale = fScaleY;
    }
    
    imageSprite->setScale(fScale);
    updateMaskSpriteRender();
}

void PixEditSprite::onEnterTransitionDidFinish()
{
    PixMaskSprite::onEnterTransitionDidFinish();
    
}
