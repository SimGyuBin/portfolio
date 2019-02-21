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

USING_NS_CC;

class MyAlbumUsedImagesModel : public Ref
{
public:
    
    MyAlbumUsedImagesModel();
    virtual ~MyAlbumUsedImagesModel();

    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_m_image, _m_image)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_s_image, _s_image)
    CC_SYNTHESIZE(int, mn_Width, Width)
    CC_SYNTHESIZE(int, mn_Height, Height)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_File_create_date, File_create_date)
};

#endif /* MyAlbumUsedImagesModel_h */
