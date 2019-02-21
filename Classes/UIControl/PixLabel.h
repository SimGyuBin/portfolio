//
//  PixLabel.h
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 23..
//
//

#ifndef PixLabel_h
#define PixLabel_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;


// 기본 자간. (원래 CCLabelTTF보다 자간이 넓은 문제로 인해 이 값만큼 보정해줘야함 (px) )
#define PIXLABEL_DEFAULT_SPACE(__fontSize__)      ( MAX((0.6f - 0.01f * __fontSize__), 0) )

class PixLabel : public ui::Layout {
    
public:
    static PixLabel* labelWithString(const char *label, const Size& dimensions,
                                     TextHAlignment alignment, const char *fontName, int fontSize);
    
    void setString(const char *label, float interval = 0.0f);
    void textAnimation(float dt);
    
    const char* getString();
    
    
    void setSize(const Size& dimensioins);
    Size getSize();
    
    
    void setAlignment(TextHAlignment alignment);
    TextHAlignment getAlignment();
    
    
    void setFontName(const char *fontName);
    const char* getFontName();
    
    
    void setFontSize(int fontSize);
    int getFontSize();
    
    
    void setSpaceX(float space);
    float getSpaceX();
    
    void setSpaceY(float space);
    float getSpaceY();
    
    void setColor(Color3B color);
    
    
    // 글자수가 많아지면 자동적으로 폰트사이즈를 줄인다 (개행 없음)   : default = false;
    void setAdjustsFontSizeToFitWidth(bool flag);
    
private:
    
    void init(const char *label, const Size& dimensions,
              TextHAlignment alignment, const char *fontName, int fontSize);
    
    void createLabel(const char* label);
    void createLabelFit(const char* label);
    void createLabelLineOver(const char* label);
    
    
public:
    std::string m_string, m_aniString;
    Size m_size;
    TextHAlignment m_alignment;
    std::string m_fontName;
    int m_fontSize;
    float m_spaceX, m_spaceY;
    bool m_isFit;
    
    Color3B m_color;
    
    float m_height;
};


#endif /* PixLabel_h */
