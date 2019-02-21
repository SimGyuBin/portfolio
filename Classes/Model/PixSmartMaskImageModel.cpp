//
//  PixSmartMaskImageModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 18..
//
//

#include "PixSmartMaskImageModel.h"

PixSmartMaskImageModel::PixSmartMaskImageModel()
: _layout_res_mask_mask("")
, _item_x("")
, _item_y("")
, _item_width("")
, _item_height("")
, _item_rotate("")
, _smart_image_id("")
, _smartImageModel(nullptr)
{
    
}

PixSmartMaskImageModel::~PixSmartMaskImageModel()
{
    CC_SAFE_DELETE(_smartImageModel);
}

PixSmartMaskImageModel* PixSmartMaskImageModel::clone()
{
    PixSmartMaskImageModel *clone = new PixSmartMaskImageModel();
    
    clone->setLayout_res_mask_mask(getLayout_res_mask_mask());
    clone->setItem_x(getItem_x());
    clone->setItem_y(getItem_y());
    clone->setItem_width(getItem_width());
    clone->setItem_height(getItem_height());
    clone->setItem_rotate(getItem_rotate());
    clone->setSmart_image_id(getSmart_image_id());
    
    if(getSmartImageModel())
    {
        clone->setSmartImageModel(getSmartImageModel()->clone());
    }
    
    return clone;
}

