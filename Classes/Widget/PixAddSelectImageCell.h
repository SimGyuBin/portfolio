//
//  PixAddSelectImageCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 21..
//
//

#ifndef PixAddSelectImageCell_h
#define PixAddSelectImageCell_h

#include "cocos2d.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Model/PixImageModel.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixSelectDateCell;
class PixGalleryListView;
class LocalImageInfo;

class PixAddSelectImageCell : public ui::Widget
{
public :
    
    static PixAddSelectImageCell* create();
    
    PixAddSelectImageCell();
    virtual ~PixAddSelectImageCell();
    void clear();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    
    void addThumbnailImageAsyncCallBack(Texture2D *pTexture, void *pUserData);
    
    void createCustomItem();
    PixImageModel* pushImageModel(LocalImageInfo *info, int nIndex);
    
    Node* getCellItem(const std::string &name);
    void removeCellItem(Node *pItme);
    void setCellSelected(int nIndex, bool isSelected);
    bool isCellSelected(int nIndex);
    
    PixImageModel* getImageModel(int nIndex);
    PixImageModel* getImageModelByPath(const std::string &path);
    Texture2D* getImageModelTexture(int nIndex);
    
    void deleteImageLoader();
    
    CC_SYNTHESIZE(PixGalleryListView*, _galleryListView, GalleryListView)
    
private:
    
    Node *_customItem;
    std::vector<PixImageModel*> imageModelVec;
};


#endif /* PixAddSelectImageCell_h */
