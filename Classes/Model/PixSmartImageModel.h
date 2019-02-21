//
//  PixSmartImageModel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 1..
//
//

#ifndef PixSmartImageModel_h
#define PixSmartImageModel_h

#include "cocos2d.h"

USING_NS_CC;

class PixSmartImageModel : public Ref
{
public :
    
    PixSmartImageModel();
    virtual ~PixSmartImageModel();
    
    PixSmartImageModel* clone();
    
    CC_SYNTHESIZE(int, mn_SmartIndex, SmartIndex)
    CC_SYNTHESIZE(int, mn_Orientation, Orientation)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_path, Path)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_date, Date)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_width, Width)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_height, Height)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Option, Option)
};

#endif /* PixSmartImageModel_h */
