//
//  PixMenuEventLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 25..
//
//

#include "PixMenuEventLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Widget/PixEventCell.h"
#include "PixMenuEventDetailLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Scene/PixApp.h"
#include "../Info/EventInfo.h"
#include "../Util/PixUserDefault.h"
#include "../Common.h"

PixMenuEventLayer* PixMenuEventLayer::create()
{
    PixMenuEventLayer *menuEventLayer = new (std::nothrow) PixMenuEventLayer();
    if (menuEventLayer && menuEventLayer->init())
    {
        menuEventLayer->autorelease();
        return menuEventLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuEventLayer);
        return nullptr;
    }
}

PixMenuEventLayer::PixMenuEventLayer()
: rootNode(nullptr)
, pixHttpClient(PixHttpClient::sharedObject())
, eventListView(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
{
    eventVec.clear();
    
    hideLayerFunction = std::bind(&PixMenuEventLayer::hideMenuEventLayerAction,this);
}

PixMenuEventLayer::~PixMenuEventLayer()
{
    pixHttpClient->cancelRequest(this);
    
    for(auto iter = eventVec.begin(); iter != eventVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    eventVec.clear();
}

void PixMenuEventLayer::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuEventLayer.csb");
    
    if(rootNode)
    {
        Node *Node_MenuEvent_Top = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuEvent_Top"));
        
        if(Node_MenuEvent_Top)
        {
            ui::Text *label = dynamic_cast<ui::Text*>(Node_MenuEvent_Top->getChildByName("label"));
            
            if(label)
            {
                label->setString(CODE_NAME(label->getString()));
            }
            
            ui::Button *Button_back = dynamic_cast<ui::Button*>(Node_MenuEvent_Top->getChildByName("Button_back"));
            
            if(Button_back)
            {
                Button_back->addTouchEventListener(CC_CALLBACK_2(PixMenuEventLayer::buttonBackPressed, this));
            }
        }
        
        ui::ListView* listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView"));
        
        if(listView)
        {
            eventListView = PixGalleryListView::create();
            eventListView->copyClonedListView(listView);
            eventListView->setSelectListItemCallBack(CC_CALLBACK_3(PixMenuEventLayer::listViewTouchCallBack, this));
        }
        
        setAnchorPoint(Vec2::ZERO);
        float fWidth = Director::getInstance()->getVisibleSize().width;
        setPosition(Vec2(fWidth,0));
        addChild(rootNode);
        
        getEventCall();
    }
}

void PixMenuEventLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showMenuEventLayerAction();
}

void PixMenuEventLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

void PixMenuEventLayer::createEventListView()
{
    if(eventListView == nullptr)
        return;
    
    for(auto iter = eventVec.begin(); iter != eventVec.end(); ++iter)
    {
        PixEventCell *eventCell = PixEventCell::create((*iter));
        eventListView->pushBackCustomItem(eventCell);
    }
    
    Size size = eventListView->getContentSize();
    ui::Widget *bottomMargin = ui::Widget::create();
    bottomMargin->setContentSize(Size(size.width,50.0f));
    eventListView->pushBackCustomItem(bottomMargin);
}

#pragma mark - Button

void PixMenuEventLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideMenuEventLayerAction();
    }
}

#pragma mark - Touch

void PixMenuEventLayer::listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
{
    if(touchType == LISTVIEW_LONG_TAP)
    {
        
    }
    else
    {
        
        PixEventCell *eventCell = dynamic_cast<PixEventCell*>(pSender);
        
        if(eventCell)
        {
            EventInfo *eventInfo = eventCell->getEventInfo();
            
            if(eventInfo)
            {
                if(eventInfo->getEnd() == false)
                {
                    Scene *scene = PIXAPP->isScene;
                    
                    if(scene)
                    {
                        std::string strUrl = SREVERURL_MANAGER->getBoardWebViewUrl(BOARD_EVENT,eventInfo->getID());
                        
                        PixMenuEventDetailLayer *menuEventDetailLayer = PixMenuEventDetailLayer::create(strUrl);
                        menuEventDetailLayer->setDelegate(this);
                        scene->addChild(menuEventDetailLayer);
                    }
                }
            }
        }
    }
}

#pragma mark - Action

void PixMenuEventLayer::showMenuEventLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuEventLayer::showMenuEventLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }

}

void PixMenuEventLayer::hideMenuEventLayerAction()
{
    mbln_IsAction = true;
    
    float fWidth = Director::getInstance()->getVisibleSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(fWidth,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMenuEventLayer::hideMenuEventLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMenuEventLayer::showMenuEventLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMenuEventLayer::hideMenuEventLayerActionFinish(Node* node)
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

void PixMenuEventLayer::showLayerActionStart(Ref *pTarget)
{
    
}

void PixMenuEventLayer::showLayerActionFinish(Ref *pTarget)
{
    setVisible(false);
}

void PixMenuEventLayer::hideLayerActionStart(Ref *pTarget)
{
    setVisible(true);
}

void PixMenuEventLayer::hideLayerActionFinish(Ref *pTarget)
{
    
}

#pragma mark - Network

void PixMenuEventLayer::getEventCall()
{
    
    PIXAPP->showLoading();
    
    //    파라미터 : 1=전체 , 2=현재진행중
    std::string url = SREVERURL_MANAGER->getUrl("GET_EVENT");
    url.append("1");
    
    pixHttpClient->sendGetRequest(url, "board_notice", this, httpresponse_selector(PixMenuEventLayer::getEventCallback), nullptr);
}

void PixMenuEventLayer::getEventCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    std::vector<char>* data = response->getResponseData();
    std::string strResponseData(data->begin(), data->end());
    
    rapidjson::Document document;
    document.Parse<0>(strResponseData.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        PixDictionary *root = pixHttpClient->getDictionary(document);
        
        if(root)
        {
            PixArray* results = root->arrayForKey("results");
            
            for (int i = 0; i < results->count(); ++i)
            {
                PixDictionary *event = results->getDictionaryAtIndex(i);
                
                if(event)
                {
                    EventInfo *eventInfo = new EventInfo();
                    
                    eventInfo->setID(event->intForKey("id"));
                    eventInfo->setBanner_path(event->stringForKey("banner_path"));
                    eventInfo->setTitle(event->stringForKey("title"));
                    eventInfo->setSdate(event->stringForKey("sdate"));
                    eventInfo->setEdate(event->stringForKey("edate"));
                    eventInfo->setEnd((event->intForKey("is_end") == 0) ? false : true);
                    
                    eventVec.push_back(eventInfo);
                }
            }
            
            int nMax_event_id = root->intForKey("max_event_id");
            
            PixUserDefault::setEventID(nMax_event_id);
            PIXAPP->userInfo->setEventNew(false);
            
            createEventListView();
        }
    }
}

#pragma mark - Android BackPressed

void PixMenuEventLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideMenuEventLayerAction();
        }
    }
}
