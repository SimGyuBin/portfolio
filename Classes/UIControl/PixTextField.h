//
//  PixTextField.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 17..
//
//

#ifndef PixTextField_h
#define PixTextField_h

#define PIXTEXTFILD_FONT_NAME "Common/KoPubDotumMedium.ttf"
#define PIXTEXTFILD_TAG 16617

#include "ui/CocosGUI.h"

USING_NS_CC;

class PixTextField : public cocos2d::ui::TextField
{
public:
    PixTextField();
    virtual ~PixTextField();
    
    static PixTextField* create();
    static PixTextField* create(const std::string& fontName, int fontSize);
    
    // TODO: possibly need touch handlers (override or new)
    virtual void onEnter();
    virtual void onExit();
    
    void updateCursor();
    
    void copyClonedTextField(ui::TextField *pTextField);
    void addTextFieldEventListener(const std::function<void(Ref*, ui::TextField::EventType)> callback);
    void textFieldIMEEvent(Ref *pSender, ui::TextField::EventType type);
    
protected:
    Label* _cursorLabel;
    std::function<void(Ref*, ui::TextField::EventType)> _textFieldEventCallback;
};



#endif /* PixTextField_h */
