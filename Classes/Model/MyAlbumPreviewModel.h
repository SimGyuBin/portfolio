//
//  MyAlbumPreviewModel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 14..
//
//

#ifndef MyAlbumPreviewModel_h
#define MyAlbumPreviewModel_h

#include "cocos2d.h"
#include "../Util/PixImageLoader.h"

USING_NS_CC;

class MyAlbumPreviewModel : public Ref
{
public:

    MyAlbumPreviewModel();
    virtual ~MyAlbumPreviewModel();
    MyAlbumPreviewModel* clone();
    
    void clear();
    void destroyPreviewData();
    
    void createImageLoader();
    void deleteImageLoader();
    void addImageAsync(Ref *target, SEL_ImageLoader selector, void *pUserData);
    void addImageAsync(Ref *target, SEL_ImageLoader selector, const std::string &filepath, void *pUserData);
                        
    void unbindAllImageAsync();
    void deleteThreadSafeImageLoader();

private:
    
    PixImageLoader *imageLoader;
    
    CC_SYNTHESIZE(Data*, _previewData, PreviewData)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Id, Id)
    CC_SYNTHESIZE(int, mn_Page_no, Page_no)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Thumb_url, Thumb_url)
    
    void checkImageLoaderResponseQueue(float dt);
    
};

#endif /* MyAlbumPreviewModel_hpp */
