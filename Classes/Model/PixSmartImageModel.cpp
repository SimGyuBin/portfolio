//
//  PixSmartImageModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 1..
//
//

#include "PixSmartImageModel.h"

PixSmartImageModel::PixSmartImageModel()
: mstr_path("")
, mstr_date("")
, mstr_width("")
, mstr_height("")
, mstr_Option("")
, mn_SmartIndex(-1)
, mn_Orientation(0)
{
    
}

PixSmartImageModel::~PixSmartImageModel()
{
    
}

PixSmartImageModel* PixSmartImageModel::clone()
{
    PixSmartImageModel *clone = new PixSmartImageModel();
    
    clone->setPath(getPath());
    clone->setDate(getDate());
    clone->setWidth(getWidth());
    clone->setHeight(getHeight());
    clone->setOption(getOption());
    clone->setSmartIndex(getSmartIndex());
    clone->setOrientation(getOrientation());
    
    return clone;
}
