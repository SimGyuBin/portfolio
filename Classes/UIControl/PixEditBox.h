//
//  PixEditBox.h
//  Pzle
//
//  Created by 심규빈 on 2016. 9. 27..
//
//

#ifndef PixEditBox_h
#define PixEditBox_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "ui/UIScale9Sprite.h"


USING_NS_CC;

class PixEditBox : public ui::EditBox, public ui::EditBoxDelegate
{
public:
    
    static PixEditBox* convertToEditBox(ui::TextField *pTextField, ui::EditBoxDelegate* pDelegate);
    
    static PixEditBox* create(const Size& size,
                              ui::Scale9Sprite* normalSprite,
                              ui::Scale9Sprite* pressedSprite = nullptr,
                              ui::Scale9Sprite* disabledSprite = nullptr);
    
    static PixEditBox* create(const Size& size,
                              const std::string& normal9SpriteBg,
                              TextureResType texType = TextureResType::LOCAL);

    PixEditBox();
    virtual ~PixEditBox();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    
    void createPlaceholderText(const std::string& textContent, const Vec2 &pos, const Vec2& point, const char* pFontName, int fontSize ,const Color4B& color);
    
    const Color4B& getTextColor();
    void setFontColor(const Color4B& color);
    
    const Color4B& getPlaceholderFontColor();
    void setPlaceholderFontColor(const Color4B& color);
    void setAlignmentCenter();
    void setAlignmentLeft();
    void setAlignmentRight();
    void setNumberOfLines(int nLines);
    int getPlaceholderFontSize();
    void setPlaceholderFontSize(int fontSize);

private:
    
    ui::Text *_placeholderText;
    
    Color4B _textColor;
    Color4B _placeholderFontColor;

    CC_SYNTHESIZE(ui::EditBoxDelegate*, _customDelegate, CustomDelegate)
    CC_SYNTHESIZE(bool, _editing, Editing)
    
#pragma mark - EditBoxDelegate
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(ui::EditBox* editBox) override;
    
};

#endif /* PixEditBox_h */
