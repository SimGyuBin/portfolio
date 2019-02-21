//
//  PixMenuCustomerLayer_FAQ.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#include "PixMenuCustomerLayer_FAQ.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Scene/PixApp.h"
#include "../Widget/PixMenuCustomerLayer_FAQ_Cell.h"
#include "../Util/PixStringUtils.h"

PixMenuCustomerLayer_FAQ *PixMenuCustomerLayer_FAQ::create()
{
    PixMenuCustomerLayer_FAQ *MenuCustomerLayer_FAQ = new (std::nothrow) PixMenuCustomerLayer_FAQ();
    if (MenuCustomerLayer_FAQ && MenuCustomerLayer_FAQ->init())
    {
        MenuCustomerLayer_FAQ->autorelease();
        MenuCustomerLayer_FAQ->initLayer();
        return MenuCustomerLayer_FAQ;
    }
    else
    {
        CC_SAFE_DELETE(MenuCustomerLayer_FAQ);
        return nullptr;
    }
}

PixMenuCustomerLayer_FAQ::PixMenuCustomerLayer_FAQ()
: pixHttpClient(nullptr)
, rootNode(nullptr)
, FAQlistView(nullptr)
, mDelegate(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    faqInfoVec.clear();
}

PixMenuCustomerLayer_FAQ::~PixMenuCustomerLayer_FAQ()
{
    pixHttpClient->cancelRequest(this);
    
    for(auto iter = faqInfoVec.begin(); iter != faqInfoVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    faqInfoVec.clear();
}

void PixMenuCustomerLayer_FAQ::initLayer()
{
    rootNode = CSLoader::createNode("11_Menu/MenuCustomerLayer_FAQ.csb");;
    
    if(rootNode)
    {
        ui::ListView *listView = dynamic_cast<ui::ListView*>(rootNode->getChildByName("ListView"));
        
        if(listView)
        {
            FAQlistView = PixGalleryListView::create();
            FAQlistView->copyClonedListView(listView);
        }
 
        addChild(rootNode);
        boardFaqCall();
    }
}

void PixMenuCustomerLayer_FAQ::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void PixMenuCustomerLayer_FAQ::createFAQlistView()
{
    for (int i = 0; i < faqInfoVec.size(); ++i)
    {
        PixMenuCustomerLayer_FAQ_Cell *MenuCustomerLayer_FAQ_Cell = PixMenuCustomerLayer_FAQ_Cell::create(faqInfoVec.at(i));
        FAQlistView->pushBackCustomItem(MenuCustomerLayer_FAQ_Cell);
    }
}

#pragma mark - Network

void PixMenuCustomerLayer_FAQ::boardFaqCall()
{
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("FAQ");
    pixHttpClient->sendGetRequest(url, "board_faq", this, httpresponse_selector(PixMenuCustomerLayer_FAQ::onHttpManagerRequestCompleted), nullptr);
}

void PixMenuCustomerLayer_FAQ::onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (strcmp(response->getHttpRequest()->getTag(), "board_faq") == 0)
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
                        FaqInfo *faqInfo = new FaqInfo();
                        
                        faqInfo->setID(JMap->stringForKey("id"));
                        faqInfo->setTitle(JMap->stringForKey("strTitle"));
                        faqInfo->setKind(JMap->intForKey("nKind"));
                        
                        std::string setContent =  JMap->stringForKey("strContent");
                        setContent = PixStringUtils::replaceAll(setContent,"\\n","\n");
                        
                        faqInfo->setContent(setContent);
                        faqInfo->setFile(JMap->stringForKey("strFile"));
                        faqInfo->setHit(JMap->intForKey("nHit"));
                        faqInfo->setDate(JMap->stringForKey("regDate"));
                        
                        faqInfoVec.push_back(faqInfo);
                    }
                }
            }
        
            createFAQlistView();
            
            GOOGLEANALYTICS_MANAGER->logEvent("board", "faq", PIXAPP->userInfo->getUserID(), 1);
        }
    }
}
