//
//  PixImageModel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 26..
//
//

#ifndef PixImageModel_h
#define PixImageModel_h

#include "cocos2d.h"
#include "../Util/PixImageLoader.h"

USING_NS_CC;

#define SPRITE_TAG 1000

class LocalImageInfo;
class CategoryInfo;

class PixImageModel : public Ref
{
public :
    
    PixImageModel();
    virtual ~PixImageModel();
    PixImageModel* clone();
    
    void addThumbnailImageAsync(Ref *target, SEL_ImageLoader selector, void *pUserData);
    void addThumbnailImageComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate);
    
    virtual void addThumbnailImageAsyncCall(Data *pThumbnaildata, std::string strPath);
    virtual void unbindThumbnailImageAsync();
    
    void addImageCache();
    void removeThumbnailImage();
    void removeTexture();
    
    void setSelected(bool isSelected);
    void setSelectImage(bool isVisible);
    void selectedEffect(bool effect);
    void updateSelected();
    void updateSelected(bool isSelected);
    
    bool isSelected();
    bool isSelectedEffect();
    void createImageLoader();
    void deleteImageLoader();
    
    int getCategoryCount();
    CategoryInfo *getCategoryInfo(int nIndex);
    
    CC_SYNTHESIZE(LocalImageInfo*, _localImageInfo, LocalImageInfo)    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Option, Option)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_MaskFile, MaskFile)
    CC_SYNTHESIZE(int, mn_index, Index)
    CC_SYNTHESIZE(int, mn_smartIndex, SmartIndex)
    CC_SYNTHESIZE(Node*, _cellItem, CellItem)
    CC_SYNTHESIZE(Node*, _cell, Cell)
    CC_SYNTHESIZE(Texture2D*, _texture, Texture)
    CC_SYNTHESIZE(bool, mbln_loading, Loading)
    CC_SYNTHESIZE(Node*, _dateCell, DateCell)

private:
    
    PixImageLoader *imageLoader;
    
    Ref *target;
    SEL_ImageLoader selector;
   
};

#endif /* PixImageModel_h */
