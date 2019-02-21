//
//  PixSelectedImageCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 3..
//
//

// 커스텀 위젯 클래스
// 선택된 이미지 리스트뷰 에서 사용

#ifndef PixSelectedImageCell_h
#define PixSelectedImageCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Model/PixImageModel.h"
#include "../UIControl/PixAdjustableHeightSprite.h"

class PixSelectedRemoveButtonDelegate;
class PixGalleryListView;

class PixSelectedImageCell : public ui::Widget
{
public :
    
    PixSelectedImageCell();
    virtual ~PixSelectedImageCell();
    
    static PixSelectedImageCell* create(PixImageModel *pModel, PixSelectedRemoveButtonDelegate *pDelegate, PixGalleryListView *ListView);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem(PixImageModel *pModel, PixSelectedRemoveButtonDelegate *pDelegate, PixGalleryListView *ListView);
    void loadImage();
    void createAdjustableHeightSprite();
    void createAdjustableHeightSprite(Texture2D *pTexture);
    void addThumbnailImageAsyncCallBack(Texture2D *pTexture, void *pUserData);
    void loadFullScreenImageLoad(float dt);
    void addFullScreenImageAsyncCallBack(Texture2D *pTexture, void *pUserData);
    void addFullScreenDataloadComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate);
    void createImageLoader();
    void deleteImageLoader();
    
private:
    
    PixSelectedRemoveButtonDelegate *mDelegate;
    PixImageModel *mModel;
    PixGalleryListView *mParent;
    Node *_RemoveButtonLayer;
    PixAdjustableHeightSprite *_sprite;
    PixImageLoader *imageLoader;
    Texture2D *mFullScreenImage;
    bool mbln_IsRemoveAction;
    bool mbln_IsThumbnail;
    
    CC_SYNTHESIZE(bool, mbln_Isloading, Loading)
    
#pragma mark - Touch

    void removePressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType type);
    void removeActionFinish();
};


class PixSelectedRemoveButtonDelegate
{
public:
    virtual void clickRemoveButton(ui::Widget* pWidget) {};
};


#endif /* PixSelectedImageCell_h */
