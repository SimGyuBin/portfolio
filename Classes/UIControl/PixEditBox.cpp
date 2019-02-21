//
//  PixEditBox.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 9. 27..
//
//

#include "PixEditBox.h"
#include "../Util/PixStringUtils.h"
#include "../Manager/PixMessageManager.h"
#include "ui/UIEditBox/UIEditBoxImpl.h"
#include "../Common.h"

static const int PIX_EDIT_BOX_PADDING = 5;
static const cocos2d::Size convertContentSize = cocos2d::Size(10, 10);

PixEditBox * PixEditBox::convertToEditBox(ui::TextField *pTextField, ui::EditBoxDelegate* pDelegate)
{
    PixEditBox *editBox = PixEditBox::create(pTextField->getContentSize() + convertContentSize, ui::Scale9Sprite::create(EDITBOX_BACKGROUND));
    
    if(editBox != nullptr)
    {
        std::vector<std::string> tokens;
        PixStringUtils::getTokens(tokens, pTextField->getString(), ",");
        
        Color4B placeholderFontColor = Color4B::BLACK;
        Color4B fontColor = Color4B::BLACK;
        int nMaxLength = 0;
        
        if(!tokens.empty() && tokens.size() == 9)
        {
            placeholderFontColor.r = atof(tokens[0].c_str());
            placeholderFontColor.b = atof(tokens[1].c_str());
            placeholderFontColor.g = atof(tokens[2].c_str());
            placeholderFontColor.a = atof(tokens[3].c_str());
            
            fontColor.r = atof(tokens[4].c_str());
            fontColor.b = atof(tokens[5].c_str());
            fontColor.g = atof(tokens[6].c_str());
            fontColor.a = atof(tokens[7].c_str());
            
            nMaxLength = atoi(tokens[8].c_str());
        }
        
        editBox->setPosition(pTextField->getPosition());
        editBox->setAnchorPoint(pTextField->getAnchorPoint());
        editBox->setFontName(pTextField->getFontName().c_str());
        editBox->setFontSize(pTextField->getFontSize());
        editBox->setFontColor(fontColor);
//        editBox->setPlaceHolder(CODE_NAME(pTextField->getPlaceHolder()));
//        editBox->setPlaceholderFontName(pTextField->getFontName().c_str());
//        editBox->setPlaceholderFontSize(pTextField->getFontSize());
//        editBox->setPlaceholderFontColor(placeholderFontColor);
        editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        
        if(nMaxLength > 0)
        {
            editBox->setMaxLength(nMaxLength);
        }
        
        editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
        editBox->setCustomDelegate(pDelegate);
        
        if(pTextField->getParent())
        {
            pTextField->getParent()->addChild(editBox);
            editBox->createPlaceholderText(CODE_NAME(pTextField->getPlaceHolder()), pTextField->getPosition(), pTextField->getAnchorPoint(), pTextField->getFontName().c_str(), pTextField->getFontSize(), placeholderFontColor);
        }
        
        pTextField->removeFromParentAndCleanup(true);
        
        return editBox;

    }
    
    return nullptr;
}

PixEditBox* PixEditBox::create(const Size &size, ui::Scale9Sprite *normalSprite, ui::Scale9Sprite *pressedSprite, ui::Scale9Sprite* disabledSprite)
{
    PixEditBox* pRet = new (std::nothrow) PixEditBox();
    
    if (pRet != nullptr && pRet->initWithSizeAndBackgroundSprite(size, normalSprite))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

PixEditBox* PixEditBox::create(const Size& size,
                               const std::string& normalSprite,
                               TextureResType texType /*= TextureResType::LOCAL*/)
{
    PixEditBox* pRet = new (std::nothrow) PixEditBox();
    
    if (pRet != nullptr && pRet->initWithSizeAndBackgroundSprite(size, normalSprite, texType))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

PixEditBox::PixEditBox()
: _placeholderText(nullptr)
, _textColor(Color4B::BLACK)
, _placeholderFontColor(Color4B::BLACK)
, _customDelegate(nullptr)
, _editing(false)
{
    setDelegate(this);
}

PixEditBox::~PixEditBox()
{
    
}

void PixEditBox::onEnter()
{
    ui::EditBox::onEnter();
    scheduleUpdate();
}

void PixEditBox::onExit()
{
    unscheduleUpdate();
    ui::EditBox::onExit();
}

void PixEditBox::update(float dt)
{
    ui::EditBox::update(dt);
    
    if(_placeholderText)
    {
        if(getEditing())
        {
            _placeholderText->setVisible(false);
        }
        else
        {
            if(getText() == nullptr || strlen(getText()) <= 0)
            {
                _placeholderText->setVisible(true);
            }
            else
            {
                _placeholderText->setVisible(false);
            }
        }
        
    }
}

void PixEditBox::createPlaceholderText(const std::string& textContent, const Vec2 &pos, const Vec2& point, const char* pFontName, int fontSize ,const Color4B& color)
{
    if(this->getParent() == nullptr)
        return;
    
    if(_placeholderText)
    {
        this->getParent()->removeChild(_placeholderText);
        _placeholderText = nullptr;
    }
    
    _placeholderText = ui::Text::create();
    _placeholderText->setString(textContent);
    _placeholderText->setPosition(Vec2(pos.x + PIX_EDIT_BOX_PADDING,pos.y));
    _placeholderText->setAnchorPoint(point);
    _placeholderText->setFontName(pFontName);
    _placeholderText->setFontSize(fontSize);
    _placeholderText->setTextColor(color);
    _placeholderText->setContentSize(getContentSize());
    
    this->getParent()->addChild(_placeholderText);
}

const Color4B& PixEditBox::getTextColor()
{
    return _textColor;
}

void PixEditBox::setFontColor(const Color4B& color)
{
    ui::EditBox::setFontColor(color);
    
    _textColor = color;
}

const Color4B& PixEditBox::getPlaceholderFontColor()
{
    return _placeholderFontColor;
}

void PixEditBox::setPlaceholderFontColor(const Color4B& color)
{
    ui::EditBox::setPlaceholderFontColor(color);
    
    _placeholderFontColor = color;
}

void PixEditBox::setAlignmentCenter()
{
    if(_editBoxImpl != nullptr)
    {
        _editBoxImpl->setAlignmentCenter();
    }
    
    if(_placeholderText)
    {
        Vec2 pos = _placeholderText->getPosition();
//        pos.x += getContentSize().width * 0.5f;
//        pos.y += getContentSize().height * 0.5f;
//        _placeholderText->setPosition(pos);
        _placeholderText->setAnchorPoint(Vec2(0.5f,0.5f));
        _placeholderText->setTextVerticalAlignment(TextVAlignment::CENTER);
        _placeholderText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    }
}

void PixEditBox::setAlignmentLeft()
{
    if(_editBoxImpl != nullptr)
    {
        _editBoxImpl->setAlignmentLeft();
    }
    
    if(_placeholderText)
    {
        Vec2 pos = _placeholderText->getPosition();
        //        pos.x += getContentSize().width * 0.5f;
        //        pos.y += getContentSize().height * 0.5f;
        //        _placeholderText->setPosition(pos);
        _placeholderText->setAnchorPoint(Vec2(0.5f,0.5f));
        _placeholderText->setTextVerticalAlignment(TextVAlignment::CENTER);
        _placeholderText->setTextHorizontalAlignment(TextHAlignment::LEFT);
    }
}

void PixEditBox::setAlignmentRight()
{
    if(_editBoxImpl != nullptr)
    {
        _editBoxImpl->setAlignmentRight();
    }
    
    if(_placeholderText)
    {
        Vec2 pos = _placeholderText->getPosition();
        //        pos.x += getContentSize().width * 0.5f;
        //        pos.y += getContentSize().height * 0.5f;
        //        _placeholderText->setPosition(pos);
        _placeholderText->setAnchorPoint(Vec2(0.5f,0.5f));
        _placeholderText->setTextVerticalAlignment(TextVAlignment::CENTER);
        _placeholderText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    }
}

void PixEditBox::setNumberOfLines(int nLines)
{
    if(_editBoxImpl != nullptr)
    {
        _editBoxImpl->setNumberOfLines(nLines);
    }
}

int PixEditBox::getPlaceholderFontSize()
{
    if(_placeholderText)
    {
        return _placeholderText->getFontSize();
    }
    
    return 0;
}

void PixEditBox::setPlaceholderFontSize(int fontSize)
{
    if(_placeholderText)
    {
        _placeholderText->setFontSize(fontSize);
    }
}

#pragma mark - EditBoxDelegate

void PixEditBox::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    setEditing(true);
    
    if(getCustomDelegate())
    {
        getCustomDelegate()->editBoxEditingDidBegin(editBox);
    }
}

void PixEditBox::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    setEditing(false);
    
    if(getCustomDelegate())
    {
        getCustomDelegate()->editBoxEditingDidEnd(editBox);
    }
}

void PixEditBox::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    if(getCustomDelegate())
    {
        getCustomDelegate()->editBoxTextChanged(editBox, text);
    }
}

void PixEditBox::editBoxReturn(ui::EditBox* editBox)
{
    if(getCustomDelegate())
    {
        getCustomDelegate()->editBoxReturn(editBox);
    }
}
