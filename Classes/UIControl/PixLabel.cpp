//
//  PixLabel.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 23..
//
//

#include "PixLabel.h"

PixLabel* PixLabel::labelWithString(const char *label, const Size& dimensions,
                                    TextHAlignment alignment, const char *fontName, int fontSize)
{
    PixLabel *layout = new PixLabel();
    layout->init(label, dimensions, alignment, fontName, fontSize);
    layout->autorelease();
    
    return layout;
}

#pragma mark - Public

void PixLabel::setString(const char *label, float interval)
{
    unschedule(schedule_selector(PixLabel::textAnimation));
    m_string = label;
    
    if (interval > 0 && m_string.size() > 0)
    {
        m_aniString = "";
        schedule(schedule_selector(PixLabel::textAnimation), interval);
    }
    else
    {
        createLabel(m_string.c_str());
    }
}


void PixLabel::textAnimation(float dt)
{
    if (m_aniString.size() == m_string.size() - 1)
    {
        unschedule(schedule_selector(PixLabel::textAnimation));
    }
    
    size_t size = 1;
    char ch = *m_string.substr(m_aniString.size(), 1).c_str();
    
    // 한글이면
    if (0x80 & ch)
    {
        size = 3;
    }
    
    m_aniString.append(m_string.substr(m_aniString.size(), size));
    createLabel(m_aniString.c_str());
}


const char* PixLabel::getString()
{
    return m_string.c_str();
}



void PixLabel::setSize(const Size& dimensioins)
{
    m_size = dimensioins;
    createLabel(m_string.c_str());
}

Size PixLabel::getSize()
{
    return m_size;
}



void PixLabel::setAlignment(TextHAlignment alignment)
{
    m_alignment = alignment;
    createLabel(m_string.c_str());
}

TextHAlignment PixLabel::getAlignment()
{
    return m_alignment;
}


void PixLabel::setFontName(const char *fontName)
{
    m_fontName = fontName;
    createLabel(m_string.c_str());
}

const char* PixLabel::getFontName()
{
    return m_fontName.c_str();
}

void PixLabel::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
    createLabel(m_string.c_str());
}

int PixLabel::getFontSize()
{
    return m_fontSize;
}

void PixLabel::setSpaceX(float space)
{
    m_spaceX = PIXLABEL_DEFAULT_SPACE(m_fontSize) - space;
    createLabel(m_string.c_str());
}

float PixLabel::getSpaceX()
{
    return m_spaceX;
}

void PixLabel::setSpaceY(float space)
{
    m_spaceY = space;
    createLabel(m_string.c_str());
}

float PixLabel::getSpaceY()
{
    return m_spaceY;
}

void PixLabel::setColor(Color3B color)
{
    m_color = color;
    createLabel(m_string.c_str());
}

void PixLabel::setAdjustsFontSizeToFitWidth(bool flag)
{
    m_isFit = flag;
    createLabel(m_string.c_str());
}

#pragma mark - Private

void PixLabel::init(const char *label, const Size& dimensions,
                   TextHAlignment alignment, const char *fontName, int fontSize)
{
    ui::Layout::init();
    
    m_string = label;
    m_size = dimensions;
    m_alignment = alignment;
    m_fontName = fontName;
    m_fontSize = fontSize;
    m_spaceX = PIXLABEL_DEFAULT_SPACE(m_fontSize);
    m_spaceY = 0.0f;
    m_isFit = false;
    
    m_color = Color3B(255, 255, 255);
    
    createLabel(label);
}

void PixLabel::createLabel(const char *label)
{
    // 기존 라벨 제거
    removeAllChildrenWithCleanup(true);
    
    if (m_isFit)
    {
        createLabelFit(label);
    }
    else
    {
        createLabelLineOver(label);
    }
}

void PixLabel::createLabelFit(const char* label)
{
    // 먼저 적절한 폰트사이즈를 알아낸다 (개행문자는 무시한다)
    std::string totalMessage = label;
    std::string useMessge = "";
    float width = 0;
    
    for (int i = 0; i < totalMessage.size(); i++)
    {
        std::string sub;
        
        if (totalMessage[i] & 0x80)     // 한글일 경우
        {
            sub = totalMessage.substr(i, 3);
            useMessge.append(sub);
            i += 2;
        }
        else if (totalMessage[i] == '\n')
        {
            continue;
        }
        else
        {
            sub = totalMessage.substr(i, 1);
            useMessge.append(sub);
        }
        
        width += Label::createWithTTF(sub.c_str(), m_fontName.c_str(), m_fontSize)->getContentSize().width - m_spaceX;
    }
    
    if (width <= 0)
        return;
    
    float ratio = m_size.width / width;
    int fontSize = MIN(m_fontSize, int(m_fontSize * ratio));
    
    
    // 적절한 폰트사이즈를 알아냈으니 라벨을 만든다
    Label *textLabel = Label::createWithTTF(useMessge.c_str(), m_fontName.c_str(), fontSize);
    textLabel->setColor(m_color);
    
    switch (m_alignment)
    {
        case TextHAlignment::LEFT:
            textLabel->setAnchorPoint( Vec2(0.0f, 0.5f) );
            textLabel->setPosition( Vec2(0, m_size.height * 0.5f) );
            break;
            
        case TextHAlignment::CENTER:
            textLabel->setAnchorPoint( Vec2(0.5f, 0.5f) );
            textLabel->setPosition( Vec2(m_size.width * 0.5f, m_size.height * 0.5f) );
            break;
            
        case TextHAlignment::RIGHT:
            textLabel->setAnchorPoint( Vec2(1.0f, 0.5f) );
            textLabel->setPosition( Vec2(m_size.width, m_size.height * 0.5f) );
            break;
    }
    
    addChild(textLabel);
    
    setContentSize(m_size);
}

void PixLabel::createLabelLineOver(const char *label)
{
    // 라벨 순차적으로 생성
    std::string totalMessage = label;
    float height = 0;
    float width = 0;
    
    Sprite *rowSprite = new Sprite();   // 각 행을 담당하는 Sprite
    rowSprite->init();
    addChild(rowSprite);
    rowSprite->release();
    
    for (int i = 0; i < totalMessage.size(); i++)
    {
        std::string sub;
        
        if (totalMessage[i] & 0x80)     // 한글일 경우
        {
            sub = totalMessage.substr(i, 3);
            i += 2;
        }
        else if (totalMessage[i] == '\n')
        {
            sub = "0";
        }
        else
        {
            sub = totalMessage.substr(i, 1);
        }
        
        Label *textLabel = Label::createWithTTF(sub.c_str(), m_fontName.c_str(), m_fontSize);
        textLabel->setColor(m_color);
        textLabel->setAnchorPoint( Vec2::ZERO );
        textLabel->setPosition( Vec2(width, 0) );
        
        
        if (totalMessage[i] == '\n')     // 개행문자일 경우
        {
            rowSprite->setContentSize(Size(width, textLabel->getContentSize().height));
            rowSprite->setPosition( Vec2(0, -height) );
            
            width = 0;
            height += textLabel->getContentSize().height + m_spaceY;
            
            rowSprite = new Sprite();
            rowSprite->init();
            rowSprite->setContentSize(Size(width, textLabel->getContentSize().height));
            rowSprite->setPosition( Vec2(0, -height) );
            addChild(rowSprite);
            rowSprite->release();
        }
        
        else
        {
            rowSprite->setContentSize(Size(width, textLabel->getContentSize().height));
            rowSprite->setPosition( Vec2(0, -height) );
            
            if (width + textLabel->getContentSize().width > m_size.width)
            {
                textLabel->setPosition( Vec2::ZERO );
                width = textLabel->getContentSize().width - m_spaceX;
                height += textLabel->getContentSize().height + m_spaceY;
                
                rowSprite = new Sprite();
                rowSprite->init();
                rowSprite->setContentSize(Size(width, textLabel->getContentSize().height));
                rowSprite->setPosition( Vec2(0, -height) );
                addChild(rowSprite);
                rowSprite->release();
            }
            
            else
            {
                width += textLabel->getContentSize().width - m_spaceX;
                
                rowSprite->setContentSize(Size(width, textLabel->getContentSize().height));
                rowSprite->setPosition( Vec2(0, -height) );
            }
            
            rowSprite->addChild(textLabel);
        }
        
        if (i == totalMessage.size() - 1)
        {
            height += textLabel->getContentSize().height;
        }
    }
    
    
    // 위로 올림 + 정렬
    for (int i = 0; i < getChildrenCount(); i++)
    {
        Sprite *row = dynamic_cast<Sprite *>(getChildren().at(i));
        if (row)
        {
            switch (m_alignment)
            {
                case TextHAlignment::LEFT:
                    row->setAnchorPoint(Vec2(0.0f, 1.0f));
                    row->setPosition(row->getPosition() + Vec2(0, height));
                    break;
                    
                case TextHAlignment::RIGHT:
                    row->setAnchorPoint(Vec2(1.0f, 1.0f));
                    row->setPosition(row->getPosition() + Vec2(m_size.width, height));
                    break;
                    
                case TextHAlignment::CENTER:
                    row->setAnchorPoint(Vec2(0.5f, 1.0f));
                    row->setPosition(row->getPosition() + Vec2(m_size.width * 0.5f, height));
                    break;
            }
        }
    }
    
    setContentSize(Size(m_size.width, MAX(height, m_size.height)));
}





