//
//  PixMenuCustomerLayer_FAQ.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#include "PixMenuCustomerLayer.h"
#include "../Scene/PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Widget/PixMenuCustomerLayer_FAQ_Cell.h"
#include "../Widget/PixMenuCustomerLayer_FAQ_BottomCell.h"
#include "../Util/PixStringUtils.h"
#include "PixMenuCustomerLayer_Answer.h"
#include "../Common.h"

PixMenuCustomerLayer* PixMenuCustomerLayer::create()
{
    PixMenuCustomerLayer *MenuCustomerLayer = new (std::nothrow) PixMenuCustomerLayer();
    if (MenuCustomerLayer && MenuCustomerLayer->init())
    {
        MenuCustomerLayer->autorelease();
        return MenuCustomerLayer;
    }
    else
    {
        CC_SAFE_DELETE(MenuCustomerLayer);
        return nullptr;
    }
}

PixMenuCustomerLayer::PixMenuCustomerLayer()
: pixHttpClient(nullptr)
, rootNode(nullptr)
, MenuCustomer_Top(nullptr)
, Button_cancel(nullptr)
, FAQlistView(nullptr)
, Text_Category(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
, mn_Kind(0)
{
    pixHttpClient = PixHttpClient::sharedObject();
    faqInfoVec.clear();
    faqCategoryInfoVec.clear();
    
    hideLayerFunction = std::bind(&PixMenuCustomerLayer::hideMenuCustomerLayerAction,this);
}

PixMenuCustomerLayer::~PixMenuCustomerLayer()
{
    pixHttpClient->cancelRequest(this);
    
    for(auto iter = faqInfoVec.begin(); iter != faqInfoVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    faqInfoVec.clear();
    
    for(auto iter = faqCategoryInfoVec.begin(); iter != faqCategoryInfoVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    faqCategoryInfoVec.clear();
}

void PixMenuCustomerLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuCustomerLayer.csb");
    
    if(rootNode)
    {
        MenuCustomer_Top = dynamic_cast<Node*>(rootNode->getChildByName("MenuCustomer_Top"));
        
        if(MenuCustomer_Top)
        {
            Button_cancel = dynamic_cast<ui::Button*>(MenuCustomer_Top->getChildByName("Button_back"));
            
            if(Button_cancel)
            {
                Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixMenuCustomerLayer::buttonCancelPressed, this));
            }
            
            ui::Text *title = dynamic_cast<ui::Text*>(MenuCustomer_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString().c_str()));
            }
        }
        
        Node *Node_MenuCustomerItem_1 = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuCustomerItem_1"));
        
        if(Node_MenuCustomerItem_1)
        {
            ui::Button *Button_All = dynamic_cast<ui::Button*>(Node_MenuCustomerItem_1->getChildByName("Button_All"));
            
            if(Button_All)
            {
                Button_All->addTouchEventListener(CC_CALLBACK_2(PixMenuCustomerLayer::buttonCategoryPressed, this));

                Text_Category = dynamic_cast<ui::Text*>(Button_All->getChildByName("Text_All"));
                
                if(Text_Category)
                {
                    Text_Category->setString(CODE_NAME(Text_Category->getString()));
                }
            }
        }
        
        ui::ListView *listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView"));
        
        if(listView)
        {
            FAQlistView = PixGalleryListView::create();
            FAQlistView->copyClonedListView(listView);
            FAQlistView->setSelectListItemCallBack(CC_CALLBACK_3(PixMenuCustomerLayer::listViewTouchCallBack, this));
        }
        
        setAnchorPoint(Vec2::ZERO);
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        addChild(rootNode);
        
        boardFaqCall();
    }
}

void PixMenuCustomerLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuCustomerLayerAction();
}

void PixMenuCustomerLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

void PixMenuCustomerLayer::createFAQlistView()
{
    FAQlistView->removeAllItems();
    
    // 전체보기
    if(getKind() == 0)
    {
        for (auto iter = faqInfoVec.begin(); iter != faqInfoVec.end(); ++iter)
        {
            PixMenuCustomerLayer_FAQ_Cell *MenuCustomerLayer_FAQ_Cell = PixMenuCustomerLayer_FAQ_Cell::create((*iter));
            MenuCustomerLayer_FAQ_Cell->setMenuCustomerLayer(this);
            FAQlistView->pushBackCustomItem(MenuCustomerLayer_FAQ_Cell);
        }
    }
    else
    {
        for (auto iter = faqInfoVec.begin(); iter != faqInfoVec.end(); ++iter)
        {
            if((*iter)->getKind() == getKind())
            {
                PixMenuCustomerLayer_FAQ_Cell *MenuCustomerLayer_FAQ_Cell = PixMenuCustomerLayer_FAQ_Cell::create((*iter));
                MenuCustomerLayer_FAQ_Cell->setMenuCustomerLayer(this);
                FAQlistView->pushBackCustomItem(MenuCustomerLayer_FAQ_Cell);
            }
        }
    }
    
    PixMenuCustomerLayer_FAQ_BottomCell *bottomCell = PixMenuCustomerLayer_FAQ_BottomCell::create();
    FAQlistView->pushBackCustomItem(bottomCell);
}

#pragma mark - Button

void PixMenuCustomerLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        hideMenuCustomerLayerAction();
    }
}

void PixMenuCustomerLayer::buttonCategoryPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_IsAction == false)
    {
        PixMenuCustomerCategoryLayer *menuCustomerCategoryLayer = PixMenuCustomerCategoryLayer::create(&faqCategoryInfoVec);
        menuCustomerCategoryLayer->setDelegate(this);
        Director::getInstance()->getRunningScene()->addChild(menuCustomerCategoryLayer);
    }
 }

#pragma mark - ListView Event

void PixMenuCustomerLayer::listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
    if(touchType == LISTVIEW_LONG_TAP)
    {
        
    }
    else
    {
        PixMenuCustomerLayer_FAQ_Cell *cell = dynamic_cast<PixMenuCustomerLayer_FAQ_Cell*>(pSender);
        
        if(cell)
        {
            FaqInfo *faqInfo = cell->getFaqInfo();
            
            if(faqInfo)
            {
                PixMenuCustomerLayer_Answer *MenuCustomerLayer_Answer = PixMenuCustomerLayer_Answer::create(faqInfo);
                MenuCustomerLayer_Answer->setDelegate(mDelegate);
                Director::getInstance()->getRunningScene()->addChild(MenuCustomerLayer_Answer);
            }
        }
    }
}

#pragma mark - Action

void PixMenuCustomerLayer::showMenuCustomerLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCustomerLayer::showMenuCustomerLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuCustomerLayer::hideMenuCustomerLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuCustomerLayer::hideMenuCustomerLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuCustomerLayer::showMenuCustomerLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuCustomerLayer::hideMenuCustomerLayerActionFinish(Node* node)
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

#pragma mark - Delegate

void PixMenuCustomerLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMenuCustomerLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuCustomerLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuCustomerLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

void PixMenuCustomerLayer::customerCategoryPressed(FaqCategoryInfo *pFaqCategoryInfo)
{
    if(pFaqCategoryInfo == nullptr)
        return;
    
    if(mn_Kind == pFaqCategoryInfo->getID())
        return;
    
    mn_Kind = pFaqCategoryInfo->getID();
    
    if(Text_Category)
    {
        Text_Category->setString(pFaqCategoryInfo->getCategory_name());
    }
    
    createFAQlistView();
}

#pragma mark - Network

void PixMenuCustomerLayer::boardFaqCall()
{
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("FAQ");
    pixHttpClient->sendGetRequest(url, "board_faq", this, httpresponse_selector(PixMenuCustomerLayer::boardFaqCallback), nullptr);
}

void PixMenuCustomerLayer::boardFaqCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (strcmp(response->getHttpRequest()->getTag(), "board_faq") == 0)
    {
        PIXAPP->hideLoading();
        
        PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
        
        if(root)
        {
            PixArray *results = root->arrayForKey("results");
            
            if(results)
            {
                for (int i = 0; i < results->count(); ++i)
                {
                    PixDictionary *faq = results->getDictionaryAtIndex(i);
                    
                    if(faq)
                    {
                        FaqInfo *faqInfo = new FaqInfo();
                        
                        faqInfo->setID(faq->stringForKey("id"));
                        faqInfo->setKind(faq->intForKey("nKind"));
                        faqInfo->setTitle(faq->stringForKey("strTitle"));
                        faqInfo->setHit(faq->intForKey("nHit"));
                        faqInfo->setDate(faq->stringForKey("regDate"));
                        
                        std::string setContent = faq->stringForKey("strContent");
                        setContent = PixStringUtils::replaceAll(setContent,"\\n","\n");
                        faqInfo->setContent(setContent);
                        
                        faqInfo->setFile(faq->stringForKey("strFile"));
                        
                        
                        faqInfoVec.push_back(faqInfo);
                    }
                }
            }
            
            // 전체 보기
            FaqCategoryInfo *faqCategoryInfo = new FaqCategoryInfo();
            
            faqCategoryInfo->setID(0);
            faqCategoryInfo->setCategory_name(CODE_NAME("Menu43"));
            
            faqCategoryInfoVec.push_back(faqCategoryInfo);
            
            PixArray *category = root->arrayForKey("category");
            
            if(category)
            {
                for (int i = 0; i < category->count(); ++i)
                {
                    PixDictionary *dic = category->getDictionaryAtIndex(i);
                    
                    if(dic)
                    {
                        FaqCategoryInfo *faqCategoryInfo = new FaqCategoryInfo();
                        
                        faqCategoryInfo->setID(dic->intForKey("id"));
                        faqCategoryInfo->setCategory_name(dic->stringForKey("category_name"));
                        
                        faqCategoryInfoVec.push_back(faqCategoryInfo);
                    }
                }
            }
            
            createFAQlistView();
            
            GOOGLEANALYTICS_MANAGER->logEvent("board", "faq", PIXAPP->userInfo->getUserID(), 1);
        }
    }
}

#pragma mark - Android BackPressed

void PixMenuCustomerLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideMenuCustomerLayerAction();
        }
    }
}
