//
//  PixMyAlbumSharePopup.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 20..
//
//

#include "PixMyAlbumSharePopup.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixFacebookManager.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"
#include "../Util/PixNativeUtils.h"
#include "../Model/MyAlbumModel.h"
#include "../Util/StackBlur_Host.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixMyAlbumSharePopup* PixMyAlbumSharePopup::create(MyAlbumModel *pModel)
{
    PixMyAlbumSharePopup *myAlbumSharePopup = new (std::nothrow) PixMyAlbumSharePopup(pModel);
    if (myAlbumSharePopup && myAlbumSharePopup->init())
    {
        myAlbumSharePopup->autorelease();
        return myAlbumSharePopup;
    }
    else
    {
        CC_SAFE_DELETE(myAlbumSharePopup);
        return nullptr;
    }
}

PixMyAlbumSharePopup::PixMyAlbumSharePopup(MyAlbumModel *pModel)
: rootNode(nullptr)
, background(nullptr)
, MyAlbumSharepopup(nullptr)
, blur(nullptr)
, blurTexture(nullptr)
, back_popup_share(nullptr)
, Button_facebook(nullptr)
, Button_link(nullptr)
, cancel(nullptr)
, mbln_isAction(false)
, mModel(nullptr)
, pixHttpClient(nullptr)
, mstr_ShareUrl("")
, captureImage(nullptr)
{
    this->mModel = pModel;
    
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMyAlbumSharePopup::hideLayerAction,this);
}

PixMyAlbumSharePopup::~PixMyAlbumSharePopup()
{
    pixHttpClient->cancelRequest(this);
    
    if(blurTexture)
    {
        Director::getInstance()->getTextureCache()->removeTexture(blurTexture);
    }
}

void PixMyAlbumSharePopup::initLayer()
{
    rootNode = CSLoader::createNode("07_My_AlbumList/MyAlbumSharepopupLayer.csb");
    
    if(rootNode)
    {
        background = dynamic_cast<Sprite*>(rootNode->getChildByName("background"));
        
        if(background)
        {
            background->setOpacity(0);
        }
        
        MyAlbumSharepopup = dynamic_cast<Node*>(rootNode->getChildByName("MyAlbumSharepopup"));
        
        if(MyAlbumSharepopup)
        {
            back_popup_share = dynamic_cast<Sprite*>(MyAlbumSharepopup->getChildByName("back_popup_share"));
            
            if(back_popup_share)
            {
                back_popup_share->setOpacity(0);
                
                ui::Text *title = dynamic_cast<ui::Text*>(back_popup_share->getChildByName("label"));
                
                if(title)
                {
                    title->setString(CODE_NAME(title->getString()));
                }
                
                Button_facebook = dynamic_cast<ui::Button*>(back_popup_share->getChildByName("Button_facebook"));
                
                if(Button_facebook)
                {
                    Button_facebook->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumSharePopup::facebookPressed, this));
                    
                    ui::Text *label = dynamic_cast<ui::Text*>(Button_facebook->getChildByName("label"));
                    
                    if(label)
                    {
                        label->setString(CODE_NAME(label->getString()));
                    }
                }
                
                Button_link = dynamic_cast<ui::Button*>(back_popup_share->getChildByName("Button_link"));
                
                if(Button_link)
                {
                    Button_link->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumSharePopup::linkPressed, this));
                    
                    ui::Text *label = dynamic_cast<ui::Text*>(Button_link->getChildByName("label"));
                    
                    if(label)
                    {
                        label->setString(CODE_NAME(label->getString()));
                    }
                }
                
                cancel = dynamic_cast<ui::Button*>(back_popup_share->getChildByName("cancel"));
                
                if(cancel)
                {
                    cancel->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumSharePopup::cancelPressed, this));
                    
                    ui::Text *label = dynamic_cast<ui::Text*>(cancel->getChildByName("Text_cencel"));
                    
                    if(label)
                    {
                        label->setString(CODE_NAME(label->getString()));
                    }
                }
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }

    captureImage = utils::captureNode(Director::getInstance()->getRunningScene());
    
    auto t = std::thread(&PixMyAlbumSharePopup::threadStack_blur_rgba32, this, captureImage);
    t.detach();
}

void PixMyAlbumSharePopup::threadStack_blur_rgba32(Image *pImage)
{
    int nW = pImage->getWidth();
    int nH = pImage->getHeight();
    
    stack_blur_rgba32((unsigned int*)pImage->getData(), nW, nH, pImage->getHeight() / 1280.0f * 15);
    
    if (nullptr != _scheduler)
    {
        _scheduler->performFunctionInCocosThread([this] {
           
            createblur();
        });
    }
}

void PixMyAlbumSharePopup::createblur()
{
    if(captureImage)
    {
        blurTexture = Director::getInstance()->getTextureCache()->addImage(captureImage,"MyAlbumSharePopup_blur");
        
        if(blurTexture)
        {
            blur = Sprite::createWithTexture(blurTexture);
            blur->setAnchorPoint(Vec2::ZERO);
            blur->setOpacity(0);
            addChild(blur, -10);
        }
        
        CC_SAFE_RELEASE(captureImage);
    }
    
    if(background && MyAlbumSharepopup)
    {
        showLayerAction();
    }
}

void PixMyAlbumSharePopup::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

#pragma mark - Button

void PixMyAlbumSharePopup::facebookPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        get_share_url(pSender);
    }
}

void PixMyAlbumSharePopup::linkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        get_share_url(pSender);
    }
}

void PixMyAlbumSharePopup::cancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && mbln_isAction == false)
    {
        if(MyAlbumSharepopup && mbln_isAction == false)
        {
            hideLayerAction();
        }
    }
}

#pragma mark - Touch & Touches

void PixMyAlbumSharePopup::onTouchEnded(Touch* touch, Event* event)
{
//    if(MyAlbumSharepopup && mbln_isAction == false)
//    {
//        if(NodeContainsPoint(this, touch->getLocation()))
//        {
//            if(!NodeContainsPoint(MyAlbumSharepopup, touch->getLocation()))
//            {
//                hideLayerAction();
//            }
//        }
//    }
}

#pragma mark - Action

void PixMyAlbumSharePopup::showLayerAction()
{
    mbln_isAction = true;
    
    EaseSineOut *easeSineOut = EaseSineOut::create(FadeIn::create(0.2f));
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMyAlbumSharePopup::showLayerActionFinish, this));
    
    background->runAction(Sequence::create(easeSineOut, callFuncN, NULL));
    
    blur->runAction(easeSineOut->clone());
    
    ScaleTo *scaleTo_1 = ScaleTo::create(0.1f, 1.1f);
    EaseSineOut *easeSineOut_1 = EaseSineOut::create(scaleTo_1);
    
    ScaleTo *scaleTo_2 = ScaleTo::create(0.1f, 1.0f);
    EaseSineOut *easeSineOut_2 = EaseSineOut::create(scaleTo_2);
    
    Sequence *sequence = Sequence::create(easeSineOut_1, easeSineOut_2, NULL);
    EaseSineOut *easeSineOut_3 = EaseSineOut::create(FadeIn::create(0.2f));
    Spawn *spawn = Spawn::create(sequence, easeSineOut_3, NULL);
  
    back_popup_share->stopAllActions();
    back_popup_share->runAction(spawn);
}

void PixMyAlbumSharePopup::hideLayerAction()
{
    mbln_isAction = true;
    hideLayerActionFinish(nullptr);
}

void PixMyAlbumSharePopup::showLayerActionFinish(Node* node)
{
    mbln_isAction = false;
}

void PixMyAlbumSharePopup::hideLayerActionFinish(Node* node)
{
    mbln_isAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Network

void PixMyAlbumSharePopup::get_share_url(void *pUserData)
{
    if(mModel == nullptr)
        return;
    
    if(pixHttpClient == nullptr)
        return;
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("GET_SHARE_URL");
    
    std::map<std::string, std::string> postParams;
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["album_id"] = StringUtils::format("%d",mModel->getAlbum_id());
    postParams["strUserID"] = PIXAPP->userInfo->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "get_share_url", this, httpresponse_selector(PixMyAlbumSharePopup::get_share_urlCallback), nullptr, pUserData);

}

void PixMyAlbumSharePopup::get_share_urlCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        std::string strUrl = root->stringForKey("url");
        
        ui::Button *button = static_cast<ui::Button*>(response->getHttpRequest()->getUserData());
        
        if(button)
        {
            if(button->getName().compare("Button_facebook") == 0)
            {  
                PixNativeUtils::nativeCallFacebookShare(strUrl);
            }
            else if(button->getName().compare("Button_link") == 0)
            {
                PixNativeUtils::nativeCallClipboard(strUrl, CODE_NAME("P41"));
            }
        }
        
        hideLayerAction();
        
        sdkbox::PluginGoogleAnalytics::logEvent("social", "get_share_url", PIXAPP->userInfo->getUserID(), 1);
        sdkbox::PluginGoogleAnalytics::dispatchHits();

    }
    else
    {
        PixAlert::show("Error", "Error", nullptr, ALERT_TYPE_OK);
        return;
    }
}

#pragma mark - Android BackPressed

void PixMyAlbumSharePopup::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_isAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}
