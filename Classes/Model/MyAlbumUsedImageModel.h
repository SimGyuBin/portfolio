//
//  MyAlbumUsedImagesModel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 14..
//
//

#ifndef MyAlbumUsedImagesModel_h
#define MyAlbumUsedImagesModel_h

#include "cocos2d.h"
#include "../Util/PixImageLoader.h"

USING_NS_CC;

class MyAlbumUsedImageModel : public Ref
{
public:
    
    MyAlbumUsedImageModel();
    virtual ~MyAlbumUsedImageModel();
    MyAlbumUsedImageModel* clone();
    
    void clear();
    void destroy_S_Data();
    void destroy_M_Data();

    void create_S_ImageLoader();
    void create_M_ImageLoader();
    void delete_S_ImageLoader();
    void delete_M_ImageLoader();
    void add_S_ImageAsync(Ref *target, SEL_ImageLoader selector, void* pUserData);
    void add_M_ImageAsync(Ref *target, SEL_ImageLoader selector, void* pUserData);
    void deleteThreadSafe_S_ImageLoader();
    void deleteThreadSafe_M_ImageLoader();
    
private:
    
    PixImageLoader *_S_imageLoader;
    PixImageLoader *_M_imageLoader;
    
    CC_SYNTHESIZE(int, mn_Index, Index)
    CC_SYNTHESIZE(Data*, _S_Data, _S_Data)
    CC_SYNTHESIZE(Data*, _M_Data, _M_Data)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_s_image, _s_image)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_m_image, _m_image)
    CC_SYNTHESIZE(int, mn_Width, Width)
    CC_SYNTHESIZE(int, mn_Height, Height)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_File_create_date, File_create_date)
    CC_SYNTHESIZE(Node*, mItem, Item)
    
    void check_S_ImageLoaderResponseQueue(float dt);
    void check_M_ImageLoaderResponseQueue(float dt);

};

#endif /* MyAlbumUsedImagesModel_h */
