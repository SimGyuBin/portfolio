//
//  PixMenuCustomerLayer_Answer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#include "PixMenuCustomerLayer_Answer.h"
#include "../Model/MyAlbumModel.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuCustomerLayer_Answer* PixMenuCustomerLayer_Answer::create(FaqInfo *pInfo)
{
    PixMenuCustomerLayer_Answer *MenuCustomerLayer_Answer = new (std::nothrow) PixMenuCustomerLayer_Answer(pInfo);
    if (MenuCustomerLayer_Answer && MenuCustomerLayer_Answer->init())
    {
        MenuCustomerLayer_Answer->autorelease();
        return MenuCustomerLayer_Answer;
    }
    else
    {
        CC_SAFE_DELETE(MenuCustomerLayer_Answer);
        return nullptr;
    }
}

PixMenuCustomerLayer_Answer::PixMenuCustomerLayer_Answer(FaqInfo *pInfo)
: rootNode(nullptr)
, MenuCustomerAnswer_Top(nullptr)
, Button_back(nullptr)
, scrollView(nullptr)
, Text_question(nullptr)
, Text_Answer(nullptr)
, mInfo(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
{
    this->mInfo = pInfo;
    
    hideLayerFunction = std::bind(&PixMenuCustomerLayer_Answer::hideLayerAction,this);
}

PixMenuCustomerLayer_Answer::~PixMenuCustomerLayer_Answer()
{
    
}

void PixMenuCustomerLayer_Answer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuCustomerLayer_Answer.csb");
    
    if(rootNode)
    {
        MenuCustomerAnswer_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuCustomerAnswer_Top"));
        
        if(MenuCustomerAnswer_Top)
        {
            Button_back = dynamic_cast<ui::Button*>(MenuCustomerAnswer_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuCustomerLayer_Answer::ButtonBackPressed, this));
            }
            
            ui::Text *title = dynamic_cast<ui::Text*>(MenuCustomerAnswer_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
        }
        
        scrollView = dynamic_cast<ui::ScrollView*>(rootNode->getChildByName("ScrollView_1"));
        
        if(scrollView)
        {
            Text_question = dynamic_cast<ui::Text*>(scrollView->getChildByName("Text_question"));
            
            if(Text_question)
            {
                Text_question->setString(mInfo->getTitle());
            }
            
            Text_Answer = dynamic_cast<ui::Text*>(scrollView->getChildByName("Text_Answer"));
            
            if(Text_Answer)
            {
                Text_Answer->setString(mInfo->getContent());
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        
        addChild(rootNode);
    }
}

void PixMenuCustomerLayer_Answer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

#pragma mark - Button

void PixMenuCustomerLayer_Answer::ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

#pragma mark - Action

void PixMenuCustomerLayer_Answer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCustomerLayer_Answer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuCustomerLayer_Answer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCustomerLayer_Answer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuCustomerLayer_Answer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuCustomerLayer_Answer::hideLayerActionFinish(Node* node)
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

void PixMenuCustomerLayer_Answer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}
