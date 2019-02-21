//
//  PixSelectFolderCell.hpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 13..
//
//

#ifndef PixSelectFolderCell_h
#define PixSelectFolderCell_h

#include "cocos2d.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixGallerySprite.h"

USING_NS_CC;

#define FOLDER_CELL_TEXT_LIMITWIDTH 490

using namespace ui;
using namespace cocostudio;

class PixImageModel;
class PixSelectFolderCellDelegate;
class PixGalleryListView;

class PixSelectFolderCell : public ui::Widget
{
    
public:
    
    static PixSelectFolderCell* create(PixImageModel *pModel, const std::string &folderName, int nCount);
    
    PixSelectFolderCell(PixImageModel *pModel, const std::string &folderName, int nCount);
    virtual ~PixSelectFolderCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    
private:

    Node *_customItem;
    ui::Text *folderName;
    ui::Text *count;
    ui::Layout *frame;
    PixGallerySprite *sprite;
    
    PixImageModel *mModel;
    int mn_Count;
    
    CC_SYNTHESIZE(PixSelectFolderCellDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_FolderName, FolderName)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_FolderPath, FolderPath)
    CC_SYNTHESIZE(bool, mbln_ViewAll, ViewAll)
    CC_SYNTHESIZE(PixGalleryListView*, _galleryListView, GalleryListView)
    
    void addThumbnailImageAsyncCallBack(Texture2D *pTexture, void *pUserData);
    
#pragma mark - Button
    
    void cellPressed(cocos2d::Ref *pSender);

};

class PixSelectFolderCellDelegate
{
public:
    virtual void selectFolderCellViewAllPressed() {};
    virtual void selectFolderCellPressed(const std::string &strFolderPath, const std::string &setFolderName) {};
};

#endif /* PixSelectFolderCell_h */

