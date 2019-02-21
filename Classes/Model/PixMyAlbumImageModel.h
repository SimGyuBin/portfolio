//
//  PixMyAlbumImageModel.hpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 23..
//
//

#ifndef PixMyAlbumImageModel_h
#define PixMyAlbumImageModel_h

#include "cocos2d.h"
#include "../Util/PixImageLoader.h"

USING_NS_CC;

class PixAlbumCoverCell;

class PixMyAlbumImageModel : public Ref
{
public:
    
    PixMyAlbumImageModel();
    virtual ~PixMyAlbumImageModel();
    PixMyAlbumImageModel* clone();
    
    void clear();
    void destroyAlbumImageData();
    
    void createImageLoader();
    void deleteImageLoader();
    void addImageAsync(Ref *target, SEL_ImageLoader selector, void *pUserData);
    void deleteThreadSafeImageLoader();
    
private:

    PixImageLoader *imageLoader;
    
    CC_SYNTHESIZE(Data*, _albumImageData, AlbumImageData)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _id, Id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _url, Url)
    CC_SYNTHESIZE(int, _width, Width)
    CC_SYNTHESIZE(int, _height, Height)
    CC_SYNTHESIZE(PixAlbumCoverCell*, _albumCoverCell, AlbumCoverCell)
    
    void checkImageLoaderResponseQueue(float dt);

};


#endif /* PixMyAlbumImageModel_h */
