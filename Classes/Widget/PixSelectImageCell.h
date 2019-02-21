//
//  PixSelectImageCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 26..
//
//

// 커스텀 위젯 틀래스
// 이미지 선랙 리스트뷰에서 사용

#ifndef PixSelectCell_h
#define PixSelectCell_h

#include "cocos2d.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "editor-support/cocostudio/CSParseBinary_generated.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Model/PixImageModel.h"
#include "../UIControl/PixScrollMenu.h"
#include "../UIControl/PixQuickNavigation.h"

USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocostudio;
using namespace cocostudio::timeline;

class PixSelectDateCell;
class PixGalleryListView;
class LocalImageInfo;

class PixSelectImageCell : public ui::Widget, public PixQuickNavigationUpdateLabel
{
public :

    static PixSelectImageCell* create();
    
    PixSelectImageCell();
    virtual ~PixSelectImageCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    virtual void update(float dt) override;
    
    void addThumbnailImageAsyncCallBack(Texture2D *pTexture, void *pUserData);
    void createCustomItem();
    Texture2D* createSelectImage();
    
    void pushImageModel(PixImageModel *pModel, int nIndex);
    PixImageModel* pushImageModel(LocalImageInfo *info, int nIndex);
    
    Node* getCellItem(const std::string &name);
    void removeCellItem(Node *pItme);
    void setCellSelected(int nIndex, bool isSelected);
    void setCellSelectImage(int nIndex, bool isVisible);
    void setCellSelectedEffect(int nIndex, bool isVisible);
    bool isCellSelected(int nIndex);
    void updateSelected();
    
    int getImageModelCount();
    PixImageModel* getImageModel(int nIndex);
    PixImageModel* getImageModelByPath(const std::string &path);
    Texture2D* getImageModelTexture(int nIndex);
    
    void deleteImageLoader();
    void cellTouchCancelled();
    
    virtual void setQuickNavigationUpdateLabel(ui::Text *pLabel) override;
    
    CC_SYNTHESIZE(PixSelectDateCell*, _dateCell, DateCell)
    CC_SYNTHESIZE(PixGalleryListView*, _galleryListView, GalleryListView)
    
private:
    
    Node *_customItem;
    PixScrollMenu *menu;
    std::vector<PixImageModel*> imageModelVec;
    
#pragma mark - Button
    
    void itemPressed(cocos2d::Ref *pSender);
    
};


#endif /* PixSelectCell_h */
