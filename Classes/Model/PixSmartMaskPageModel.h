//
//  PixSmartMaskItemModel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 18..
//
//

#ifndef PixSmartMaskPageModel_h
#define PixSmartMaskPageModel_h

#include "cocos2d.h"
#include "PixSmartMaskImageModel.h"
#include "../Network/PixDownloader.h"
#include "PixNetworkImageModelDelegate.h"

USING_NS_CC;

class PixSmartMaskPageModel;

typedef enum
{
    ITEM_NONE = 0,
    ITEM_COVER,
    ITEM_PAGE,
    
}BOOK_ITEM_TYPE;

typedef enum
{
    CLIPPING_NONE = 0,
    CLIPPING_SKIN,
    CLIPPING_MASK,
    CLIPPING_IMAGE,
    CLIPPING_MASKLAYOUT,
    
} CLIPPING_TYPE;

class autolayout
{
    
public:
    
    autolayout()
    {
        _layout_type.clear();
        _page_size.clear();
        _skin_width.clear();
        _skin_height.clear();
        _skin_crop_top_bottom.clear();
        _skin_crop_left_right.clear();
        _page_size_real_width.clear();
        _page_size_real_height.clear();
        _dpi.clear();
    };
   
    ~autolayout()
    {
    
    };
    
    autolayout* clone()
    {
        autolayout *clone = new autolayout();
        
        clone->setLayout_type(getLayout_type());
        clone->setPage_size(getPage_size());
        clone->setSkin_width(getSkin_width());
        clone->setSkin_height(getSkin_height());
        clone->setSkin_crop_top_bottom(getSkin_crop_top_bottom());
        clone->setSkin_crop_left_right(getSkin_crop_left_right());
        clone->setPage_size_real_width(getPage_size_real_width());
        clone->setPage_size_real_height(getPage_size_real_height());
        clone->setdpi(getdpi());
        
        return clone;
    }
    
    void getAutoLayoutPageModelAddPos( double& nAddPosX, double& nAddPosY );
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _layout_type, Layout_type)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _page_size, Page_size)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _skin_width, Skin_width)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _skin_height, Skin_height)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _skin_crop_top_bottom, Skin_crop_top_bottom)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _skin_crop_left_right, Skin_crop_left_right)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _page_size_real_width, Page_size_real_width)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _page_size_real_height, Page_size_real_height)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _dpi, dpi)
};

class PixDictionary;
class PixNetworkImageModel;

class PixSmartMaskPageModel : public Ref, public PixDownloaderDelegate, public PixNetworkImageModelDelegate
{

public :

    static PixSmartMaskPageModel *create();
    
    PixSmartMaskPageModel();
    virtual ~PixSmartMaskPageModel();
    void clear();
    void smartMaskImageModelVecClear();
    virtual void initRenderTexture();
    
    
    virtual PixSmartMaskPageModel* clone();
    void copy(PixSmartMaskPageModel *pModel);
    
    void pushSmartMaskImageModel(PixSmartMaskImageModel *pModel);
    PixSmartMaskImageModel* getSmartMaskImageModel(int nIndex);
    PixSmartMaskImageModel* getSmartMaskImageModelBySmartIndex(int nSmartIndex);
    PixSmartMaskImageModel* getSmartMaskImageModelByPath(const std::string &strPath);
    void destroyFullScreenImageData();
    bool removeSmartMaskImageModelBySmartIndex(int nSmartIndex);
    int getSmartMaskImageSeqByPath(const std::string &strPath);
    int getSmartMaskImageModelCount();
    void changelayout(PixDictionary *root);
    void createF1200(std::function<void (PixSmartMaskPageModel*)> callBack);
    void update(float dt);
    bool isF1200Data();
    
    const Data& getF1200Data()
    {
        return F1200Data;
    }
    
    void clearF1200Data();
    
    void removelayout();
    void removeAllRenderTexture();
    
    CC_SYNTHESIZE(BOOK_ITEM_TYPE, _type, Type)
    CC_SYNTHESIZE(autolayout*, _autolayout, Autolayout)
    CC_SYNTHESIZE(int, _index, Index)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _skin_res_id, Skin_res_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _skin_url, Skin_url)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _layout_res_id, Layout_res_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _layout_res_mask_layout, Layout_res_mask_layout)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _layout_res_code_sub, layout_res_code_sub)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _F1200filename, F1200filename)
    CC_SYNTHESIZE(bool, mbln_F1200DataRender, F1200DataRender)
    
            
protected:
    
    std::vector<PixSmartMaskImageModel*> smartMaskImageModelVec;
    std::deque<PixDownloader*> downloaderQueue;
    std::function<void (PixSmartMaskPageModel*)> _renderCallBack;
    
    ClippingNode *AlbumRenderLayer;
    ui::Widget *skin;
    ui::Widget *item;
    ui::Widget *mask;
    ui::Widget *effect;
    Data F1200Data;
    
    int mn_RequestImageCount;
    bool mbln_IsRequestImage;
    
    void loadSmartLayout();
    void loadLocalImage();
    void completeTexture(Texture2D *pTexture,  void *pUserData);
    
    void renderTextureF1200();
    void renderTextureF1200Callback(RenderTexture* pRenderTexture, const std::string& filename);
    void schedulerRenderTextureH600(float dt);
    virtual void renderTextureH600(std::function<void (PixSmartMaskPageModel*)> callBack);
    virtual void renderTextureH600Callback(RenderTexture* pRenderTexture, const std::string& filename);
    
#pragma mark - Network
    
    void sendGetImageUrl_Call(const std::string &url, void *pUserData);
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode);
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent);
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data);
    
    virtual void networkImageModelcompleteTexture(Texture2D *pTexture, void *pUserData);
    
};



#endif /* PixSmartMaskPageModel_h */
