//
//  PixMenuNoticeLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#include "PixMenuNoticeLayer.h"
#include "../Scene/PixApp.h"
#include "../Util/PixUserDefault.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Widget/PixMenuNoticeCell.h"
#include "../Widget/PixMenuNoticeTopCell.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMenuNoticeLayer* PixMenuNoticeLayer::create()
{
    PixMenuNoticeLayer *menuNoticeLayer = new (std::nothrow) PixMenuNoticeLayer();
    if (menuNoticeLayer && menuNoticeLayer->init())
    {
        menuNoticeLayer->autorelease();
        return menuNoticeLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuNoticeLayer);
        return nullptr;
    }
}

PixMenuNoticeLayer::PixMenuNoticeLayer()
: rootNode(nullptr)
, pixHttpClient(nullptr)
, MeneNotice_Top(nullptr)
, Button_cancel(nullptr)
, noticeListView(nullptr)
, mDelegate(nullptr)

, mbln_IsAction(false)
{
    pixHttpClient = PixHttpClient::sharedObject();
    noticeInfoVec.clear();
    
    hideLayerFunction = std::bind(&PixMenuNoticeLayer::hideMenuNoticeLayerAction,this);
}

PixMenuNoticeLayer::~PixMenuNoticeLayer()
{
    pixHttpClient->cancelRequest(this);
    
    for(auto iter = noticeInfoVec.begin(); iter != noticeInfoVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    noticeInfoVec.clear();
}

void PixMenuNoticeLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuNoticeLayer.csb");
    
    if(rootNode)
    {
        MeneNotice_Top = dynamic_cast<Node*>(rootNode->getChildByName("MeneNotice_Top"));
       
        if(MeneNotice_Top)
        {
            Button_cancel = dynamic_cast<ui::Button*>(MeneNotice_Top->getChildByName("Button_back"));
            
            if(Button_cancel)
            {
                Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixMenuNoticeLayer::buttonCancelPressed, this));
            }
            
            ui::Text *title = dynamic_cast<ui::Text*>(MeneNotice_Top->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
        }
        
        ui::ListView* listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView"));
        
        if(listView)
        {
            noticeListView = PixGalleryListView::create();
            noticeListView->copyClonedListView(listView);
        }
        
        setAnchorPoint(Vec2::ZERO);
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        addChild(rootNode);
        
        boardNoticeCall();
    }
}

void PixMenuNoticeLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuNoticeLayerAction();
}

void PixMenuNoticeLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

void PixMenuNoticeLayer::createNoticeListView()
{
    PixMenuNoticeTopCell *menuNoticeTopCell = PixMenuNoticeTopCell::create();
    noticeListView->pushBackCustomItem(menuNoticeTopCell);
    
    for (int i = 0; i < noticeInfoVec.size(); ++i)
    {
        PixMenuNoticeCell *menuNoticeCell = PixMenuNoticeCell::create(noticeInfoVec.at(i));
        menuNoticeCell->setDelegate(this);
        noticeListView->pushBackCustomItem(menuNoticeCell);
    }
}

#pragma mark - Button

void PixMenuNoticeLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideMenuNoticeLayerAction();
    }
}

#pragma mark - Action

void PixMenuNoticeLayer::showMenuNoticeLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuNoticeLayer::showMenuNoticeLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMenuNoticeLayer::hideMenuNoticeLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuNoticeLayer::hideMenuNoticeLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuNoticeLayer::showMenuNoticeLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuNoticeLayer::hideMenuNoticeLayerActionFinish(Node* node)
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

void PixMenuNoticeLayer::showLayerActionStart(Ref *pTarget)
{

}

void PixMenuNoticeLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuNoticeLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuNoticeLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

#pragma mark - Network

void PixMenuNoticeLayer::boardNoticeCall()
{
    PIXAPP->showLoading();
    std::string url = SREVERURL_MANAGER->getUrl("NOTICE");
    pixHttpClient->sendGetRequest(url, "board_notice", this, httpresponse_selector(PixMenuNoticeLayer::onHttpManagerRequestCompleted), nullptr);
}

void PixMenuNoticeLayer::onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (strcmp(response->getHttpRequest()->getTag(), "board_notice") == 0)
    {
        PIXAPP->hideLoading();
        
        PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
        
        if(root)
        {
            PixArray *jArray = root->arrayForKey("results");
            
            if(jArray)
            {
                for (int i = 0; i < jArray->count(); ++i)
                {
                    PixDictionary *JMap = jArray->getDictionaryAtIndex(i);
                    
                    if(JMap)
                    {
                        NoticeInfo *noticeInfo = new NoticeInfo();
                        
                        noticeInfo->setID(JMap->intForKey("id"));
                        noticeInfo->setTitle(JMap->stringForKey("strTitle"));
                        noticeInfo->setKind(JMap->intForKey("nKind"));
                        noticeInfo->setContent(JMap->stringForKey("strContent"));
                        noticeInfo->setFile(JMap->stringForKey("strFile"));
                        noticeInfo->setHit(JMap->intForKey("nHit"));
                        noticeInfo->setDate(JMap->stringForKey("regDate"));
                        
                        noticeInfoVec.push_back(noticeInfo);
                    }
                }
            }
            
            if(!noticeInfoVec.empty())
            {
                NoticeInfo *info = noticeInfoVec.at(0);

                PixUserDefault::setNoticeID(info->getID());
                PIXAPP->userInfo->setNoticeNew(false);
            }
            
            createNoticeListView();
            
            GOOGLEANALYTICS_MANAGER->logEvent("board", "notice", PIXAPP->userInfo->getUserID(), 1);
        }
    }
}

#pragma mark - Android BackPressed

void PixMenuNoticeLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideMenuNoticeLayerAction();
        }
    }
}
