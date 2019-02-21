//
//  PixSmartMaskCoverPageModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 18..
//
//

#include "PixSmartMaskCoverPageModel.h"
#include "../Common.h"

PixSmartMaskCoverPageModel *PixSmartMaskCoverPageModel::create()
{
    PixSmartMaskCoverPageModel *ret = new (std::nothrow) PixSmartMaskCoverPageModel();
    
    return ret;
}

PixSmartMaskCoverPageModel::PixSmartMaskCoverPageModel()
: _seneka_width("")
, _seneka_text("")
, _seneka_color("")
, _H600filename("")
, _coverRenderCallBack(nullptr)
, mbln_H600DataRender(false)
{
    setType(ITEM_COVER);
}

PixSmartMaskCoverPageModel::~PixSmartMaskCoverPageModel()
{
    
}

PixSmartMaskCoverPageModel* PixSmartMaskCoverPageModel::clone()
{
    PixSmartMaskCoverPageModel *clone = PixSmartMaskCoverPageModel::create();
    
    clone->copy(this);
    clone->setSeneka_width(getSeneka_width());
    clone->setSeneka_text(getSeneka_text());
    clone->setSeneka_color(getSeneka_color());
    
    return clone;
}

void PixSmartMaskCoverPageModel::initRenderTexture()
{
    PixSmartMaskPageModel::initRenderTexture();
}

void PixSmartMaskCoverPageModel::renderTextureH600(std::function<void (PixSmartMaskPageModel*)> callback)
{
    PixSmartMaskPageModel::renderTextureH600(callback);
    
    _coverRenderCallBack = callback;
    
    if(isH600Data())
    {
        if(_coverRenderCallBack)
        {
            _coverRenderCallBack(this);
        }
        
        _coverRenderCallBack = nullptr;
    }
    else
    {
        setH600DataRender(true);
        
        Vec2 pos = Vec2( (-(AlbumRenderLayer->getContentSize().width * AlbumRenderLayer->getScaleX()) * 0.5f),0);
        
        effect->setVisible(false);
        
        Size size = Size((AlbumRenderLayer->getContentSize().width * AlbumRenderLayer->getScaleX()) * 0.5f,
                         AlbumRenderLayer->getContentSize().height * AlbumRenderLayer->getScaleY());
        
        RenderTexture *coverRender = RenderTexture::create(size.width, size.height);
        
        coverRender->begin();
        AlbumRenderLayer->setPosition(pos);
        AlbumRenderLayer->Node::visit();
        coverRender->end();
        
        effect->setVisible(true);
        
        std::string filename = std::string(RENDER_DIRECTORY);
        filename.append(StringUtils::format("SmartMaskCoverPage_H600_%d.jpg",getIndex()));
        
        coverRender->saveToFile(filename, Image::Format::JPG, true, CC_CALLBACK_2(PixSmartMaskCoverPageModel::renderTextureH600Callback, this));
    }
}

void PixSmartMaskCoverPageModel::renderTextureH600Callback(RenderTexture* pRenderTexture, const std::string& filename)
{
    PixSmartMaskPageModel::renderTextureH600Callback(pRenderTexture, filename);
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
    
    CC_ASSERT(FileUtils::getInstance()->isFileExist(fullPath));
    
    H600Data.clear();
    H600Data = FileUtils::getInstance()->getDataFromFile(fullPath);
    
    setH600filename(filename);
    
    if(_coverRenderCallBack)
    {
        _coverRenderCallBack(this);
    }

    _coverRenderCallBack = nullptr;
}

bool PixSmartMaskCoverPageModel::isH600Data()
{
    if(H600Data.isNull())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void PixSmartMaskCoverPageModel::clearH600Data()
{
    H600Data.clear();
}
