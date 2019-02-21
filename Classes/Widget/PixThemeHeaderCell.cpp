//
//  PixThemeHeaderCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 26..
//
//

#include "PixThemeHeaderCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Network/PixHttpClient.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

PixThemeHeaderCell* PixThemeHeaderCell::create(const Size &contentSize)
{
    PixThemeHeaderCell* widget = new (std::nothrow) PixThemeHeaderCell(contentSize);
    if (widget && widget->init())
    {
        widget->setContentSize(contentSize);
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixThemeHeaderCell::PixThemeHeaderCell(const Size &contentSize)
: mDictionary(nullptr)
{
    
}

PixThemeHeaderCell::~PixThemeHeaderCell()
{
    
}

bool PixThemeHeaderCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixThemeHeaderCell::onEnter()
{
    Widget::onEnter();
}

void PixThemeHeaderCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
}

void PixThemeHeaderCell::createCustomItem()
{
    
//    _customItem = CSLoader::createNode("03_Theme/ThemeHeader.csb");
//    this->addChild(_customItem);
    
    if(mDictionary)
    {
        PixDictionary *imageDic = mDictionary->dictionaryForKey("image_path");
        
        if(imageDic)
        {
            std::string image_path = imageDic->stringForKey(SMARTALBUM_MANAGER->getLanguageCode());
            
            if(image_path.empty())
            {
                image_path = imageDic->stringForKey(DEFAULT_LANGUAGE);
            }
            
            if(!image_path.empty())
            {
                std::vector<std::string> tokens;
                PixStringUtils::getTokens(tokens, image_path, "/");
                
                std::string filename;
                filename.clear();
                
                if(!tokens.empty())
                {
                    filename = tokens[tokens.size() - 1];
                }
                
                std::string fullPath = std::string(THEME_PATH).append(filename);
                
                Sprite *headerSprite = Sprite::create(fullPath);
                
                if(headerSprite)
                {
                    headerSprite->setPosition(Vec2(getContentSize().width * 0.5f,
                                                   getContentSize().height * 0.5f));
                    headerSprite->setAnchorPoint(Vec2(0.5f,0.5f));
                    addChild(headerSprite);
                }
            }
        }
    }
}


