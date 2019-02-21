#ifndef __PIXGALLERY_LEVEL_H__
#define __PIXGALLERY_LEVEL_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Core/GbLevel.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Widget/PixSelectDateCell.h"
#include "../Widget/PixSelectImageCell.h"
#include "../Widget/PixSelectedImageCell.h"
#include "../Widget/PixSelectFolderCell.h"
#include "../Model/PixImageModel.h"
#include "../Network/PixHttpClient.h"
#include "../Info/LocalImageDateInfo.h"
#include "../Layer/PixSelectedPhotoAmountLayer.h"
#include "../Layer/PixAlbumViewLayer.h"
#include "../Layer/PixGalleryTouchControlLayer.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Info/CategoryInfo.h"
#include "PixApp.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

#define FOLDER_TEXT_LIMITWIDTH 255

typedef enum
{
    GALLERY_NONE = 0,
    GALLERY_SELECT,
    GALLERY_SELECT_VIEW,
    GALLERY_REPALACE,
    GALLERY_ADD_PHOTO,
    GALLERY_ADD_PAGE,
    GALLERY_USER_PHOTO,
} GALLERY_TYPE;

typedef enum
{
    MOVE_SELECT_PHOTOS_LAYER_NORMAL,
    MOVE_SELECT_PHOTOS_LAYER_PRESSED,
    MOVE_SELECT_PHOTOS_LAYER_COUNT,
} MOVE_SELECT_PHOTOS_LAYER_TYPE;

typedef enum
{
    PHOTOS_FOLDER_LAYER_NONE,
    PHOTOS_FOLDER_LAYER_WHITE,
    PHOTOS_FOLDER_LAYER_COUNT,
} PHOTOS_FOLDER_LAYER_TYPE;

typedef enum
{
    PHOTOS_LISTVIEW_ALL,
    PHOTOS_LISTVIEW_FOLDER_CONTENTS,
} PHOTOS_LISTVIEW_TYPE;

typedef enum
{
    ADD_NONE,
    ADD_PREPAGE,
    ADD_NEXTPAGE,
    ADD_ENDPAGE,
} ADDPAGE_TYPE;

class PixGalleryLevel : public GbLevel, public PixSelectedPhotoAmountLayerDelegate, public PixLayerActionDelegate, public PixSelectFolderCellDelegate
{
    
public:
    
    static Scene* scene();
    PixGalleryLevel();
    virtual ~PixGalleryLevel();
    
    inline GALLERY_TYPE getGalleryType()
    {
        return galleryType;
    }
    
    void setGalleryType(GALLERY_TYPE type)
    {
        galleryType = type;
    }
    
    void settingLayer();
    
    inline PixAlbumViewLayer *getAlbumViewLayer()
    {
        return albumViewLayer;
    }
    
    inline Node* getAddPhotoTopLayer()
    {
        return addPhotoTopLayer;
    }
    
    inline Node* getMoveSelectPhotosLayer()
    {
        return moveSelectPhotosLayer;
    }
    
    inline MOVE_SELECT_PHOTOS_LAYER_TYPE getMoveSelectPhotosLayerType()
    {
        return moveSelectPhotosLayerType;
    }
    
    void setMoveSelectPhotosLayerType(MOVE_SELECT_PHOTOS_LAYER_TYPE type)
    {
        moveSelectPhotosLayerType = type;
    }
    
    void moveSelectPhotosLayerEffect(float fPercent);
    void updateSelectedEffect();
    void cancelEvent();
    void setSelectTextureVec();
    std::vector<PixImageModel*>& getSelectImageModelVec();
    void createUserPhotoListView();
    void addUserPhotoImageModel(PixImageModel *pModel);
    void showUserPhotoListView();
    void showSelectListView();
    void setAddPageTag(ADDPAGE_TYPE eType);
    
private:
    
    PixHttpClient *pixHttpClient;
    
    GALLERY_TYPE galleryType;
    PHOTOS_LISTVIEW_TYPE listViewType;
    
    Node *selectPhotosLayer;
    Node *selectPhotosTopLayer;
    Node *addPhotoTopLayer;
    Node *moveSelectPhotosLayer;
    Node *userPhotosLayer;
    Node *folderNameNode[PHOTOS_FOLDER_LAYER_COUNT];
    Sprite *moveSelectPhotosBack[MOVE_SELECT_PHOTOS_LAYER_COUNT];
    Sprite *moveSelectPhotosSymbol[MOVE_SELECT_PHOTOS_LAYER_COUNT];
    MOVE_SELECT_PHOTOS_LAYER_TYPE moveSelectPhotosLayerType;
    
    PixGalleryListView *selectListView;
    PixGalleryListView *folderListView;
    std::unordered_map<std::string, PixGalleryListView*> folderContentsListViewMap;
    PixGalleryListView *curfolderContentsListView;
    PixGalleryListView *userPhotoListView;
    
    ui::Button *btn_selectback;
    ui::CheckBox *PhotoCheckBox;
    ui::Button *Photoamount;
    ui::Button *createalbum;
    ui::Button *addpage;
    ui::Button *addphoto;
    ui::Button *replacephoto;
    ui::Button *usedPhoto;
    ui::Text *addphoto_label;
    ui::Text *addpage_label;
    ui::Text *usedPhoto_label;
    
    ui::Button *Button_cancel;
    ui::Button *Button_complete;
    ui::Button *Button_close;
    ui::Text *galleryTitle;
    
    PixAlbumViewLayer *albumViewLayer;
    
    virtual void initLevel();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    virtual void update(float dt);
    
    void settingSelectScrollView(std::vector<LocalImageInfo*> &localImageInfoVec);
    void settingStepByStepSelectScrollView(float dt = 0);
    void settingfolderContentsListView();
    void settingfolderContentsListView(std::vector<PixImageModel*> &imageModelVec, bool blnIsEnd);
    void settingStepByStepfolderContentsListView(float dt = 0);
    
    void setSelectPhotosTopLayerLabelString(int nSelectCount = 0);
    void createCategoryVec();
    void createFolderContentsListView();
    void createFolderListView();
    void setfolderContentsListView();
    void showFolderListView();
    void setListViewType(PHOTOS_LISTVIEW_TYPE type);
    void updateFolderSymbol();
    void updateFolderLabel();
    
    void goSmamrtAlbum();
    void checkSelectImage();
    void setCellSelected(PixSelectImageCell *pImageCell, const std::vector<Vec2>&touchVec);
    void selectListViewLongTouchEvent(PixSelectImageCell *pImageCell, const std::vector<Vec2>&touchVec);
    void createAlbumViewLayer();
    void addPhotoTopLayerVisible(bool visible);
    void addMoveSelectPhotosLayer(bool visible);
    void addUserPhotosLayer(bool visible);
    const std::string& getGalleryTitleString(GALLERY_TYPE type);
    std::vector<PixImageModel*>& getImageModelVec();
    
    CC_SYNTHESIZE(PixImageModel*, mSelectModel, SelectModel)
    CC_SYNTHESIZE(int, mn_LocalImageCount, LocalImageCount)
    CC_SYNTHESIZE(int, mn_SelectImageCount, SelectImageCount)
    CC_SYNTHESIZE(int, mn_StepByStepIndex, StepByStepIndex)
    CC_SYNTHESIZE(int, mn_MinPicCount, MinPicCount)
    CC_SYNTHESIZE(int, mn_MaxPicCount, MaxPicCount)
    CC_SYNTHESIZE(PixGalleryTouchControlLayer*, mGalleryTouchControlLayer, GalleryTouchControlLayer)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_RepalaceTitle, RepalaceTitle)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_AddPhotoTitle, AddPhotoTitle)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_ViewAllPhotos, ViewAllPhotos)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_FolderName, FolderName)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_FolderPath, FolderPath)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_AddPhotoNumString, AddPhotoNumString)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_AddPageNumString, AddPageNumString)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_UsedPhotoNumString, UsedPhotoNumString)
    CC_SYNTHESIZE(bool, mbln_PhotoGuide, PhotoGuide)
    
    std::vector<PixImageModel*> selectImageModelVec;
    std::vector<PixImageModel*> folderContentsVec;
    std::vector<PixImageModel*> userImageModelVec;
    std::vector<PixImageModel*> usedPhotolVec;
    
    std::vector<CategoryInfo*> categoryVec;
  
    std::vector<LocalImageInfo*> *localImages;
    std::vector<LocalImageInfo*>::iterator localImagesIter;
    
#pragma mark - Button
    
    void createalbumPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void selectbackPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void photoCheckBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void photoamountPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void btnCancelPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void btnCompletePressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void btnFolder(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void addpagePressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void addphotoPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void replacephotoPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void usedPhotoPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    
#pragma mark - Touch
    
    void listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
    
#pragma mark - Delegate
    
    virtual void buttonAmountPressed(int nAmount);
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    virtual void selectFolderCellViewAllPressed();
    virtual void selectFolderCellPressed(const std::string &strFolderPath, const std::string &strFolderName);
    
#pragma mark - Network
    
    void albumNew();
    void albumNewCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void division();
    void divisionCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};

#endif // __PIXGALLERY_LEVEL_H__
