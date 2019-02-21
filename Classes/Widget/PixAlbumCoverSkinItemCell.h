//
//  PixAlbumCoverSkinItemCell.h
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 22..
//
//

#ifndef PixAlbumCoverSkinItemCell_h
#define PixAlbumCoverSkinItemCell_h

#include "cocos2d.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixGalleryListView;
class CardSkinInfo;

class PixAlbumCoverSkinItemCell : public ui::Widget
{
  
public :
    
    static PixAlbumCoverSkinItemCell* create();
    
    PixAlbumCoverSkinItemCell();
    virtual ~PixAlbumCoverSkinItemCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    Node* getCellItem(const std::string &name);
    void removeCellItem(Node *pItme);
    void setCellSelected(int nIndex, bool isSelected);
    bool isCellSelected(int nIndex);
    void pushCardSkinInfo(CardSkinInfo *pInfo);
    CardSkinInfo* getCardSkinInfo(int nIndex);
    
private:
    
    Node *_customItem;
    
    std::vector<CardSkinInfo*> cardSkinInfoVec;
    
    CC_SYNTHESIZE(PixGalleryListView*, _galleryListView, GalleryListView)
    
};

#endif /* PixAlbumCoverSkinItemCell_h */
