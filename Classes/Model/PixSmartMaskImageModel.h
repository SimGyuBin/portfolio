//
//  PixSmartMaskImageModel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 18..
//
//

#ifndef PixSmartMaskImageModel_h
#define PixSmartMaskImageModel_h

#include "cocos2d.h"
#include "PixSmartImageModel.h"

USING_NS_CC;

class PixSmartMaskImageModel : public Ref
{
public :
    
    PixSmartMaskImageModel();
    virtual ~PixSmartMaskImageModel();
    
    PixSmartMaskImageModel* clone();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _layout_res_mask_mask, Layout_res_mask_mask)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _item_x, Item_x)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _item_y, Item_y)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _item_width, Item_width)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _item_height, Item_height)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _item_rotate, Item_rotate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _smart_image_id, Smart_image_id)
    CC_SYNTHESIZE(PixSmartImageModel*, _smartImageModel, SmartImageModel)

};

#endif /* PixSmartMaskImageModel_h */
