//
//  PixImageDataInfo.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 12..
//
//

#ifndef PixImageDataInfo_h
#define PixImageDataInfo_h

#include "cocos2d.h"
#include "../Util/PixImageLoader.h"
#include "PixNetworkImageModelDelegate.h"

USING_NS_CC;

typedef std::function<void(Texture2D*, void*)> onCompleteTextureCallback;

class LocalImageInfo;

class PixNetworkImageModel : public Ref
{
    
public:
    
    static PixNetworkImageModel* create();
    
    PixNetworkImageModel();
    virtual ~PixNetworkImageModel();
    
    void createImageLoader();
    void deleteImageLoader();
    void addImageAsync(onCompleteTextureCallback callback);
    void addImageAsync(PixNetworkImageModelDelegate *pDelegate);
    void addLocalImageAsync(onCompleteTextureCallback callback);
    void addLocalImageAsync(PixNetworkImageModelDelegate *pDelegate);
    void localImage();
    void addImageAsyncCallBack(Texture2D *pTexture, void* pUserData);
    void addLocalImageComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate);
    
public:
    
    int nType;
    Vec2 pos;
    Vec2 cocos2dPos;
    Size size;
    Size parentSize;
    float rotate;
    float cocos2dScale;
    int zOrder;
    int nTag;
    int nOrientation;
    int nSmartIndex;
    std::string path;
    std::string date;
    
private:
    
    PixImageLoader *imageLoader;
    onCompleteTextureCallback mCallback;
    LocalImageInfo *mlocalImageInfo;
    Texture2D *mTexture;
    
    CC_SYNTHESIZE(PixNetworkImageModelDelegate*, mDelegate, Delegate)
    
};


#endif /* PixImageDataInfo_h */
