//
//  PixOrderCancelLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 18..
//
//

#include "PixOrderCancelLayer.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/OrderCheckInfo.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUtils.h"
#include "../Network/PixHttpClient.h"
#include "../Common.h"

PixOrderCancelLayer* PixOrderCancelLayer::create(OrderCheckInfo *pOrderCheckInfo)
{
    PixOrderCancelLayer *orderCancelLayer = new (std::nothrow) PixOrderCancelLayer(pOrderCheckInfo);
    if (orderCancelLayer && orderCancelLayer->init())
    {
        orderCancelLayer->autorelease();
        return orderCancelLayer;
    }
    else
    {
        CC_SAFE_DELETE(orderCancelLayer);
        return nullptr;
    }
}

PixOrderCancelLayer::PixOrderCancelLayer(OrderCheckInfo *pOrderCheckInfo)
: rootNode(nullptr)
, Name(nullptr)
, Text_Bank(nullptr)
, Number(nullptr)
, Pay(nullptr)
, Button_Ok(nullptr)
, Panel_Bank(nullptr)
, listView(nullptr)
, mbln_IsAction(false)
, mOrderCheckInfo(pOrderCheckInfo)
, mDelegate(nullptr)
, mOrderCancelLayerDelegate(nullptr)
, mstr_Bank_code("")
{
    hideLayerFunction = std::bind(&PixOrderCancelLayer::hideOrderCancelLayerAction,this);
}

PixOrderCancelLayer::~PixOrderCancelLayer()
{
    
}

void PixOrderCancelLayer::initLayer()
{
    if(mOrderCheckInfo == nullptr)
        return;
    
    rootNode = CSLoader::createNode("12_Oder/MenuOrderCancel_PopLayer.csb");
    
    if(rootNode)
    {
        Node *MenuOrderCancel_Pop_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuOrderCancel_Pop_Top"));
      
        if(MenuOrderCancel_Pop_Top)
        {
            ui::Layout *Top_01 = dynamic_cast<ui::Layout*>(MenuOrderCancel_Pop_Top->getChildByName("Top_01"));
            
            if(Top_01)
            {
                ui::Text *Text_Title = dynamic_cast<ui::Text*>(Top_01->getChildByName("Text_Title"));
                
                if(Text_Title)
                {
                    Text_Title->setString(CODE_NAME(Text_Title->getString()));
                }
                
                ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Top_01->getChildByName("Button_cancel"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixOrderCancelLayer::buttonCancelPressed, this));
                }
            }
        }
        
        Button_Ok = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Ok"));
        
        if(Button_Ok)
        {
            Button_Ok->addTouchEventListener(CC_CALLBACK_2(PixOrderCancelLayer::buttonOkPressed, this));
            
            ui::Text *Text_Ok = dynamic_cast<ui::Text*>(Button_Ok->getChildByName("Text_Ok"));
            
            if(Text_Ok)
            {
                Text_Ok->setString(CODE_NAME(Text_Ok->getString()));
            }
        }
        
        Sprite *sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("Name"));
        
        if(sprite)
        {
            ui::Text *Text_Title = dynamic_cast<ui::Text*>(sprite->getChildByName("Text_Title"));
            
            if(Text_Title)
            {
                Text_Title->setString(CODE_NAME(Text_Title->getString()));
            }
            
            ui::TextField *TextField = dynamic_cast<ui::TextField*>(sprite->getChildByName("TextField"));
            
            if(TextField)
            {
                Name = PixEditBox::convertToEditBox(TextField, this);
            }
        }
        
        sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("Bank"));
        
        if(sprite)
        {
            ui::Text *Text_Title = dynamic_cast<ui::Text*>(sprite->getChildByName("Text_Title"));
            
            if(Text_Title)
            {
                Text_Title->setString(CODE_NAME(Text_Title->getString()));
            }
            
            ui::Button *Button_Bank = dynamic_cast<ui::Button*>(sprite->getChildByName("Button_Bank"));
            
            if(Button_Bank)
            {
                Button_Bank->addTouchEventListener(CC_CALLBACK_2(PixOrderCancelLayer::buttonBankPressed, this));
                
                Text_Bank = dynamic_cast<ui::Text*>(Button_Bank->getChildByName("Text_Bank"));
                
                if(Text_Bank)
                {
                    Text_Bank->setString(CODE_NAME(Text_Bank->getString()));
                }
            }
        }
        
        sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("Number"));
        
        if(sprite)
        {
            ui::Text *Text_Title = dynamic_cast<ui::Text*>(sprite->getChildByName("Text_Title"));
            
            if(Text_Title)
            {
                Text_Title->setString(CODE_NAME(Text_Title->getString()));
            }
            
            ui::TextField *TextField = dynamic_cast<ui::TextField*>(sprite->getChildByName("TextField"));
            
            if(TextField)
            {
                Number = PixEditBox::convertToEditBox(TextField, this);
                Number->setInputMode(ui::EditBox::InputMode::NUMERIC);
            }
        }
        
        sprite = dynamic_cast<Sprite*>(rootNode->getChildByName("Pay"));
        
        if(sprite)
        {
            ui::Text *Text_Title = dynamic_cast<ui::Text*>(sprite->getChildByName("Text_Title"));
            
            if(Text_Title)
            {
                Text_Title->setString(CODE_NAME(Text_Title->getString()));
            }
            
            ui::Text *Text_Pay = dynamic_cast<ui::Text*>(sprite->getChildByName("Text_Pay"));
            
            if(Text_Pay)
            {
                std::string strMoney = PixStringUtils::commaMoney(mOrderCheckInfo->getPay_amount());
                std::string strText = StringUtils::format(CODE_NAME(Text_Pay->getString()), strMoney.c_str());
                
                Text_Pay->setString(strText);
            }
        }
        
        Panel_Bank = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Panel_Bank"));
        
        if(Panel_Bank)
        {
            ui::ListView *ListView = dynamic_cast<ui::ListView*>(Panel_Bank->getChildByName("ListView"));
            
            if(ListView)
            {
                listView = PixGalleryListView::create();
                listView->copyClonedListView(ListView);
                listView->setSelectListItemCallBack(CC_CALLBACK_3(PixOrderCancelLayer::listViewTouchCallBack, this));
                
                createListView();
            }
            
            Panel_Bank->setVisible(false);
        }

        Size size = rootNode->getContentSize();
        setPosition(Vec2(0,-size.height));
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixOrderCancelLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showOrderCancelLayerAction();
}

void PixOrderCancelLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(Button_Ok)
    {
        Button_Ok->setEnabled(checkOrderCancellation());
    }
}

void PixOrderCancelLayer::createListView()
{
    if(listView == nullptr)
        return;
    
    PixArray *bank = SREVERURL_MANAGER->getBank();
    
    if(bank)
    {
        for (int i = 0; i < bank->count(); ++i)
        {
            PixDictionary *dic = bank->getDictionaryAtIndex(i);
            
            if(dic)
            {
                std::string strBank_name = dic->stringForKey("bank_name");
                std::string strBank_code = dic->stringForKey("bank_code");
                
                PixBankItemCell *bankItemCell = PixBankItemCell::create(strBank_name, strBank_code);
                bankItemCell->setCheckVisible(false);
                
                listView->pushBackCustomItem(bankItemCell);
            }
        }
    }
}

bool PixOrderCancelLayer::checkOrderCancellation()
{
    if(Name)
    {
        if(Name->getText() == nullptr || strlen(Name->getText()) <= 0)
        {
            return false;
        }
    }
    
    if(Number)
    {
        if(Number->getText() == nullptr || strlen(Number->getText()) <= 0)
        {
            return false;
        }
    }
    
    if(mstr_Bank_code.empty())
    {
        return false;
    }
    
    return true;
}

const char* PixOrderCancelLayer::getName()
{
    if(Name)
    {
        return Name->getText();
    }
    
    return "";
}

const char* PixOrderCancelLayer::getNumber()
{
    if(Number)
    {
        return Number->getText();
    }
    
    return "";
}

void PixOrderCancelLayer::setPanelBankVisible(bool bVisible)
{
    if(Panel_Bank)
    {
        Panel_Bank->setVisible(bVisible);
        
        for (auto item : Panel_Bank->getChildren())
        {
            item->setVisible(bVisible);
        }
    }
}

#pragma mark - Button

void PixOrderCancelLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        if(Panel_Bank)
        {
            if(Panel_Bank->isVisible())
            {
                setPanelBankVisible(false);
                return;
            }
        }
        
        hideOrderCancelLayerAction();
    }
}

void PixOrderCancelLayer::buttonBankPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        if(Panel_Bank)
        {
            setPanelBankVisible(!Panel_Bank->isVisible());
        }
    }
}

void PixOrderCancelLayer::buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        if(Panel_Bank)
        {
            if(Panel_Bank->isVisible())
            {
                setPanelBankVisible(false);
                return;
            }
        }
        
        if(checkOrderCancellation() == false)
        {
            return;
        }
        
        if(mOrderCancelLayerDelegate)
        {
            mOrderCancelLayerDelegate->orderCancellation(getNumber(), mstr_Bank_code, getName());
        }
    }
}

#pragma mark - Touch

void PixOrderCancelLayer::listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
    if(touchType == LISTVIEW_LONG_TAP)
    {
        
    }
    else
    {
        PixBankItemCell *bankItemCell = dynamic_cast<PixBankItemCell*>(pSender);
        
        if(bankItemCell)
        {
            if(!bankItemCell->isCheckVisible())
            {
                bankItemCell->setCheckVisible(true);
                
                if(Text_Bank)
                {
                    Text_Bank->setString(bankItemCell->getBank_name());
                }
                
                setBank_code(bankItemCell->getBank_code());
                
                if(Panel_Bank->isVisible())
                {
                    setPanelBankVisible(false);
                }
                
                for (auto item : listView->getItems())
                {
                    PixBankItemCell *cell = dynamic_cast<PixBankItemCell*>(item);
                    
                    if(cell)
                    {
                        if(cell != bankItemCell)
                        {
                            cell->setCheckVisible(false);
                        }
                    }
                }
            }
            else
            {
                if(Panel_Bank->isVisible())
                {
                    setPanelBankVisible(false);
                }
            }
        }
    }
}

#pragma mark - Touch & Touches

void PixOrderCancelLayer::onTouchEnded(Touch* touch, Event* event)
{
    if(Panel_Bank && mbln_IsAction == false)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            if(Panel_Bank->isVisible())
            {
                if(!NodeContainsPoint(Panel_Bank, touch->getLocation()))
                {
                    setPanelBankVisible(false);
                }
            }
        }
    }
}

#pragma mark - EditBox

void PixOrderCancelLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    if(Panel_Bank->isVisible())
    {
        setPanelBankVisible(false);
    }
}

void PixOrderCancelLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    if(listView)
    {
        listView->resetGestureRecognizer();
    }
}

void PixOrderCancelLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    if(editBox == nullptr)
        return;
    
    if(editBox == Number)
    {
        // 숫자만 입력 되도록 막음
        if(text.empty())
            return;
        
        const char *strNumber = text.c_str();
        
        ssize_t size = strlen(strNumber);
        
        if(size == 0)
            return;
        
        std::vector<char> numberVec;
        std::vector<char> textVec;
        
        for (int i = 0; i < size; i++)
        {
            if (isdigit(strNumber[i]) == 0)
            {
                textVec.push_back(strNumber[i]);
            }
            else
            {
                numberVec.push_back(strNumber[i]);
            }
        }
        
        if(!textVec.empty())
        {
            if(!numberVec.empty())
            {
                std::string strText(numberVec.begin(), numberVec.end());
                editBox->setText(strText.c_str());
            }
            else
            {
                editBox->setText("");
            }
        }
    }
}

void PixOrderCancelLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Action

void PixOrderCancelLayer::showOrderCancelLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixOrderCancelLayer::showOrderCancelLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixOrderCancelLayer::hideOrderCancelLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixOrderCancelLayer::hideOrderCancelLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixOrderCancelLayer::showOrderCancelLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixOrderCancelLayer::hideOrderCancelLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionFinish(this);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixOrderCancelLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(Panel_Bank)
        {
            if(Panel_Bank->isVisible())
            {
                setPanelBankVisible(false);
                return;
            }
        }
        
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideOrderCancelLayerAction();
        }
    }
}
