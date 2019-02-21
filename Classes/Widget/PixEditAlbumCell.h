//
//  PixEditAlbumCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 24..
//
//

#ifndef PixEditAlbumCell_h
#define PixEditAlbumCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixGallerySprite.h"

USING_NS_CC;

class PhotoEditInfo;

class PixEditAlbumCell : public ui::Widget
{
public :
    
    static PixEditAlbumCell* create(PhotoEditInfo *info);
    
    PixEditAlbumCell(PhotoEditInfo *info);
    virtual ~PixEditAlbumCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    void setVisiblEeffect(bool visible);
    bool isVisiblEeffect();
    void spriteFadeOutAction();
    void changeTexture(const std::string& path);
    
    CC_SYNTHESIZE(PhotoEditInfo*, mEditInfo, EditInfo)
    CC_SYNTHESIZE(Texture2D*, mTexture, mTexture)
    
private:
    
    Node *customItem;
    Sprite *thum_photoselect_effect;
    
    PixGallerySprite *mSprite;
};

#endif /* PixEditAlbumCell_h */
