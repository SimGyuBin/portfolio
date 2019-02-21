//
//  PixTextField.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 17..
//
//

#include "PixTextField.h"

PixTextField::PixTextField()
: _cursorLabel(nullptr)
, _textFieldEventCallback(nullptr)
{

}

PixTextField::~PixTextField()
{

}

PixTextField* PixTextField::create()
{
    return PixTextField::create(PIXTEXTFILD_FONT_NAME, 20);
}

PixTextField* PixTextField::create(const std::string& fontName, int fontSize)
{
    PixTextField* widget = new (std::nothrow) PixTextField();
    if (widget && widget->init())
    {
        widget->setFontName(fontName);
        widget->setFontSize(fontSize);
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

void PixTextField::onEnter()
{
    TextField::onEnter();
    
    // create cursor label
    Label *lbl = Label::createWithTTF("|", getFontName(), this->getFontSize());
    if(! lbl) return;
    lbl->setPosition(Vec2(0,lbl->getContentSize().height / 2));
    Sequence *seq = Sequence::create(FadeOut::create(0.5), FadeIn::create(0.25), nullptr);
    lbl->runAction(RepeatForever::create(seq));
    lbl->setTag(PIXTEXTFILD_TAG);
    this->addChild(lbl,10);
    
    _cursorLabel = lbl;
    _cursorLabel->setVisible(false);
    
    this->addEventListener(CC_CALLBACK_2(PixTextField::textFieldIMEEvent, this));
}

void PixTextField::onExit()
{
    this->addEventListener(nullptr);
    if(_cursorLabel) {
        _cursorLabel->removeFromParent();
        _cursorLabel = nullptr;
    }
    TextField::onExit();
}

void PixTextField::updateCursor()
{
    Label *lbl = dynamic_cast<Label*>(getVirtualRenderer());
    if(! lbl) return;
//
//    // TODO: could just use bbox length
//    auto px = lbl->getBoundingBox().size.width + 2.0f;
//    auto py = this->getBoundingBox().size.height * 0.5f;
//    _cursorLabel->setPosition(px, py);
//    
//    return;
    
    //    // Could also find position based on a specific character (if insert text in middle were possible)
    
    // Here's getting specific LAST character
    // get current length
    int len = lbl->getStringLength();
    if(len <= 0)
    {
        _cursorLabel->setPosition(0, _cursorLabel->getContentSize().height / 2);
    }
    else
    {
        Sprite *letter = lbl->getLetter((len == 0) ? 0 : len - 1);
        Vec2 p = letter->getPosition(); // may need bbox
        float px = p.x + letter->getBoundingBox().size.width * 0.5f;
        float py = this->getBoundingBox().size.height * 0.5f;
        _cursorLabel->setPosition(px, py);
    }
}

void PixTextField::copyClonedTextField(ui::TextField *pTextField)
{
    this->copyProperties(pTextField);
    this->copyClonedWidgetChildren(pTextField);
    
    this->setPlaceHolder(pTextField->getPlaceHolder());
    
    if(pTextField->getParent())
    {
        pTextField->getParent()->addChild(this);
        pTextField->getParent()->removeChild(pTextField);
    }
    
    pTextField->removeFromParentAndCleanup(true);
}

void PixTextField::addTextFieldEventListener(const std::function<void(Ref*, ui::TextField::EventType)> callback)
{
    _textFieldEventCallback = callback;
}

void PixTextField::textFieldIMEEvent(Ref *pSender, ui::TextField::EventType type)
{
    if(type == EventType::ATTACH_WITH_IME)
    {
        // show cursor
        _cursorLabel->setVisible(true);
    }
    else if(type == EventType::DETACH_WITH_IME)
    {
        // hide cursor
        _cursorLabel->setVisible(false);
    }
    else if(type == EventType::INSERT_TEXT)
    {
        // move right (or just update)
        updateCursor();
    }
    else if(type == EventType::DELETE_BACKWARD)
    {
        // move left (or just update)
        updateCursor();
    }
    
    if(_textFieldEventCallback)
    {
        (_textFieldEventCallback)(pSender,type);
    }
}