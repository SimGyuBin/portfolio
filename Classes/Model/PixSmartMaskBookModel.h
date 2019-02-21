//
//  PixSmartMaskBookModel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 18..
//
//

#ifndef PixSmartMaskBookModel_h
#define PixSmartMaskBookModel_h

#include "cocos2d.h"
#include "PixSmartMaskPageModel.h"
#include "PixSmartMaskCoverPageModel.h"
#include "editor-support/cocostudio/DictionaryHelper.h"

using namespace cocostudio;

USING_NS_CC;


class PixDictionary;
class PixSmartMaskImageModel;

class PixSmartMaskBookModel : public Ref
{

public :
    
    PixSmartMaskBookModel();
    virtual ~PixSmartMaskBookModel();
    virtual PixSmartMaskBookModel* clone();
    
    void pushSmartMaskPageModel(PixSmartMaskPageModel *pModel);
    PixSmartMaskPageModel* getSmartMaskPageModel(int nIndex);
    bool removeSmartMaskPageModel(int nIndex);
    bool removeSmartMaskPageModel(PixSmartMaskPageModel *pModel);
    int getSmartMaskPageModeCount();
    void createSmartAlbum(PixDictionary *root);
    void createRenderTexture(std::function<void (PixSmartMaskPageModel*)> callBack);
    void cancelRenderTexture();
    bool isRenderTextureComplete();
    void update(float dt);
    void renderTextureCallBack(PixSmartMaskPageModel *pModel);
    void addPage(int nIndex, PixSmartMaskPageModel *pModel, bool blnEnd = false);
    
    PixSmartMaskImageModel* getSmartMaskImageModelBySmartIndex(int nSmartIndex);
    PixSmartMaskImageModel* getSmartMaskImageModelCoverModelByPath(const std::string &strPath);
    PixSmartMaskImageModel* getSmartMaskImageModelPageModelByPath(const std::string &strPath);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _product_code, Product_code)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _product_size, Product_size)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _product_option, Product_option)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _productid, Productid)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _smartalbum, Smartalbum)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _album_id, Album_id)
    CC_SYNTHESIZE(int, _skin_group_id, Skin_group_id)
   
private:
    
    int mn_RequestCount;
    std::function<void (PixSmartMaskPageModel*)> _renderCallBack;
    
    std::vector<PixSmartMaskPageModel*> smartMaskPageModelVec;
    std::deque<PixSmartMaskPageModel*> requestRenderTextureQueue;
    std::deque<PixSmartMaskPageModel*> responseRenderTextureQueue;
};

#endif /* PixSmartMaskBookModel_h */
