//
//  PixSmartalbumSaveLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 23..
//
//

#ifndef PixSmartalbumSaveLayer_h
#define PixSmartalbumSaveLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditBox.h"
#include "../Network/PixHttpClient.h"
#include "PixSaveProgressLayer.h"

USING_NS_CC;

#define SVAE_CENCEL_ALERT_TAG 32227

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixSmartMaskBookModel;
class PixSmartMaskPageModel;
class PixSmartMaskCoverPageModel;

class PixUploadImageInfo
{
public:
    
    PixUploadImageInfo()
    {
        _path.clear();
        _smart_image_id.clear();
        _Seq = -1;
        _AlbumSeq = -1;
        _Cover = false;
        
    }
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _path ,Path)
    CC_SYNTHESIZE(int, _Seq, Seq)
    CC_SYNTHESIZE(int, _AlbumSeq, AlbumSeq)
    CC_SYNTHESIZE(bool, _Cover, Cover)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _smart_image_id, Smart_image_id)
};

class PixSmartalbumSaveLayer : public PixModalLayer, public ui::EditBoxDelegate
{
public:
    
    static PixSmartalbumSaveLayer* create(PixSmartMaskBookModel *pModel);
    
    PixSmartalbumSaveLayer(PixSmartMaskBookModel *pModel);
    virtual ~PixSmartalbumSaveLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    virtual void onExit();
    
private:
    
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
    ui::Button *btn_back;
    ui::Button *Button_Save;
    PixEditBox *covertitle;
        
    PixSaveProgressLayer *renderingProgress;
    
    PixSmartMaskBookModel *smartMaskBookModel;
    
    int nUploadImagesCount;
    int nUploadImagesTotalCount;
    int nUploadPreviewCount;
    int nUploadPreviewTotalCount;
    
    std::deque<PixSmartMaskCoverPageModel*> albumImagesH600Queue;
    std::deque<PixSmartMaskPageModel*> albumImagesF1200Queue;
    std::vector<PixUploadImageInfo*> uploadImageInfotVec;
    std::string mstr_FileRenderXML;
    
    bool mbln_IsAction;
    int mn_SaveDoneRetry;

    CC_SYNTHESIZE(int, mn_Album_id, Album_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_AlbumPhotoSaveURL, AlbumPhotoSaveURL)
    
    void uploadImageInfoVecClear();
    PixUploadImageInfo* getUploadImageInfoByPath(const std::string &path);
    
    void createPreview();
    void previewRenderTextureCallBack(PixSmartMaskPageModel *pModel);
    
    void createUploadImages();
    void createUploadImageComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate);
    void createFileRenderXML();
    
    void createRenderingProgress();
    
    float getRenderingProgressPercent();
    
    void updateImageUpload(const std::string &strMessage);
    float getImageUploadPercent();
    
    void updatePreviewUpload(const std::string &strMessage);
    float getPreviewUploadPercent();
    
    void showRenderingProgress();
    void resetRenderingProgress();
    void hideRenderingProgress();
    bool checkCovertitle();
    bool checkAlbumSaveDone();
    
#pragma mark - Button
    
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonSavePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonSaveCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(ui::EditBox* editBox);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Network
    
    void albumBasicSave();
    void albumPhotoSave(const std::string &fileRender, const std::string &strWidth, const std::string &strHeight, const std::string &strDate, PixUploadImageInfo* pInfo);
    void albumImageSave(PixSmartMaskPageModel *pModel);
    
    void albumSaveDone();
    
    void albumBasicSaveCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumPhotoSaveCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumImageSaveCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumSaveDoneCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumBasicSaveError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumPhotoSaveError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumImageSaveError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumSaveDoneError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void albumSaveDoneError();
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};


#endif /* PixSmartalbumSaveLayer_h */
