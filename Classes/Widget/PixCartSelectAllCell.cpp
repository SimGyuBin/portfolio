//
//  PixCartSelectAllCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 9..
//
//

#include "PixCartSelectAllCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixCartSelectAllCell* PixCartSelectAllCell::create()
{
    PixCartSelectAllCell* widget = new (std::nothrow) PixCartSelectAllCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixCartSelectAllCell::PixCartSelectAllCell()
: _customItem(nullptr)
, check(nullptr)
, SelectAll(nullptr)
, mDelegate(nullptr)
, mCallback(nullptr)
{
    
    
}

PixCartSelectAllCell::~PixCartSelectAllCell()
{
    
}

bool PixCartSelectAllCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixCartSelectAllCell::onEnter()
{
    Widget::onEnter();
}

void PixCartSelectAllCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("12_Oder/MenuCartItem_01.csb");
    
    if(_customItem)
    {
        check = dynamic_cast<ui::CheckBox*>(_customItem->getChildByName("check"));
        
        if(check)
        {
            check->addEventListener(CC_CALLBACK_2(PixCartSelectAllCell::checkBoxPressed, this));
        }
        
        SelectAll = dynamic_cast<ui::Text*>(_customItem->getChildByName("SelectAll"));
        
        if(SelectAll)
        {
            SelectAll->setString(CODE_NAME(SelectAll->getString()));
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixCartSelectAllCell::setCheckSelected(bool var)
{
    if(check)
    {
        check->setSelected(var);
    }
}

void PixCartSelectAllCell::setCheckEnabled(bool var)
{
    if(check)
    {
        check->setEnabled(var);
    }
}

void PixCartSelectAllCell::runDeleteAction(const std::function<void(ui::Widget*)>& callback)
{
    mCallback = callback;
    
    Vec2 movePos = Vec2(getPositionX() + getContentSize().width,getPositionY());
    MoveTo *moveTo = MoveTo::create(0.28f,movePos);
    EaseSineIn *easeSineIn = EaseSineIn::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixCartSelectAllCell::deleteActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineIn, callFuncN, NULL);
    runAction(sequence);
}

void PixCartSelectAllCell::deleteActionFinish()
{
    if(mCallback)
    {
        (mCallback)(this);
        mCallback = nullptr;
    }
}

#pragma mark - Button

void PixCartSelectAllCell::checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    ui::CheckBox *checkBox = dynamic_cast<ui::CheckBox*>(pSender);
    
    if(checkBox)
    {
        if(mDelegate)
        {
            mDelegate->cartSelectAllCellCheckBoxPressed(checkBox->isSelected());
        }
    }
}
