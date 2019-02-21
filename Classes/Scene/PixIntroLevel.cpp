#include "PixIntroLevel.h"
#include "PixApp.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixUtils.h"
#include "../Util/PixUserDefault.h"
#include "../Util/PixStringUtils.h"
#include "../UIControl/PixToast.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Manager/PixOneSignalManager.h"
#include "../Manager/PixKakaoManager.h"
#include "../Layer/PixLoginJoinLayer.h"
#include "../Layer/PixLoginLayer.h"
#include "../Layer/PixSocialLoginLayer.h"
#include "../Layer/PixServerNoticeWebViewLayer.h"
#include "../Common.h"

Scene* PixIntroLevel::scene()
{
	Scene *scene = Scene::create();
	PixIntroLevel *layer = new PixIntroLevel();

	scene->addChild(layer, 0, LAYER_LEVEL);
	layer->release();

	return scene;
}

Scene* PixIntroLevel::logout()
{
    Scene *scene = Scene::create();
    PixIntroLevel *layer = new PixIntroLevel();
    layer->setLoguot(true);
    scene->addChild(layer, 0, LAYER_LEVEL);
    layer->release();
    
    return scene;
}

PixIntroLevel::PixIntroLevel()
: introLayer(nullptr)
, introProgressLayer(nullptr)
, introProgressLabel(nullptr)
, introProgressPercent(nullptr)
, introProgressBar(nullptr)
, pixHttpClient(nullptr)
, pageview(nullptr)
, layout_platform(nullptr)
, Button_join(nullptr)
, Button_login_kakao(nullptr)
, Button_login_face(nullptr)
, Button_join_Google(nullptr)
, Button_login(nullptr)
, curPage(0)
, mbln_ServerInfo(false)
, mbln_ThemeInfo(false)
, mbln_Multilingual(false)
, layout(nullptr)
, mn_CurAni(INTRO_BACK_00)
, mn_MemberCheckCode(0)
, mbln_MemberCheck(false)
, mbln_Loguot(false)
, mstr_ProgressText("")
, mbln_MemberCheckError(false)
{
    FACEBOOK_MANAGER->setListener(this);
    pixHttpClient = PixHttpClient::sharedObject();
    pageItemVec.clear();
    
    for (int i = 0; i < INTRO_BACK_COUNT; ++i)
    {
        backAni[i] = nullptr;
    }
    
    facebookUserInfo.clear();
    
    ONESIGNAL_MANAGER->setIntroFinish(false);
}

PixIntroLevel::~PixIntroLevel()
{
    pixHttpClient->cancelRequest(this);
    pageItemVec.clear();
    facebookUserInfo.clear();
}

void PixIntroLevel::initLevel()
{
    introLayer = CSLoader::createNode("01_Intro/IntroLayer.csb");
    
    if(mbln_Loguot)
    {
        if(introLayer)
        {
            introLayer->setVisible(false);
        }
        
        mbln_Multilingual = true;
        mbln_ServerInfo = true;
        mbln_ThemeInfo = true;
        mbln_MemberCheck = true;
        
        initPageViewLayout();
        
        if(introLayer)
        {
            rootNode->removeChild(introLayer);
        }
        
        layout->setVisible(true);
    }
    else
    {
        if(introLayer)
        {
            Node *Intro_Logo_ani = dynamic_cast<Node*>(introLayer->getChildByName("Intro_Logo_ani"));
            
            if(Intro_Logo_ani)
            {
                ActionTimeline *timeLine = CSLoader::createTimeline("01_Intro/Intro_Logo.csb");
                
                if(timeLine)
                {
                    timeLine->gotoFrameAndPlay(0,true);
                    Intro_Logo_ani->runAction(timeLine);
                }
            }
            
            introProgressLayer = dynamic_cast<Node*>(introLayer->getChildByName("IntroProgressLayer"));
            
            if(introProgressLayer)
            {
                introProgressLayer->setVisible(false);
                
                Node *loading = dynamic_cast<Node*>(introProgressLayer->getChildByName("loading"));
                
                if(loading)
                {
                    ActionTimeline *timeLine = CSLoader::createTimeline("01_Intro/IntroProgress.csb");
                    
                    if(timeLine)
                    {
                        timeLine->gotoFrameAndPlay(0,true);
                        loading->runAction(timeLine);
                    }
                }
                
                introProgressLabel = dynamic_cast<ui::Text*>(introProgressLayer->getChildByName("progress_lv2_label"));
                
                if(introProgressLabel)
                {
                    introProgressLabel->setString("");
                }
                
                introProgressPercent = dynamic_cast<ui::Text*>(introProgressLayer->getChildByName("label"));
                
                if(introProgressPercent)
                {
                    introProgressPercent->setString("0%");
                }
                
                introProgressBar = dynamic_cast<ui::LoadingBar*>(introProgressLayer->getChildByName("progress_lv1"));
                
                if(introProgressBar)
                {
                    introProgressBar->setPercent(0);
                }
            }
            
            ui::Button *buttonDisplayStats = dynamic_cast<ui::Button*>(introLayer->getChildByName("buttonDisplayStats"));
            
            if(buttonDisplayStats)
            {
                buttonDisplayStats->addTouchEventListener(CC_CALLBACK_2(PixIntroLevel::buttonDisplayStatsPressed, this));
            }
            
            rootNode->addChild(introLayer);
            setModalLayer(introLayer);
        }
        
        downloadServerInfo();
        
        PIXAPP->userInfo->setMember_id(PixUserDefault::GetMemberID());
        PIXAPP->userInfo->setUserID(PixUserDefault::GetUserID());
        PIXAPP->userInfo->setName(PixUserDefault::GetName());
        PIXAPP->userInfo->setGrade(PixUserDefault::GetGrade());
    }
}

void PixIntroLevel::onEnterTransitionDidFinish()
{
    GbLevel::onEnterTransitionDidFinish();
    
    if(mbln_Loguot == false)
    {
        introLayerAniStart();
    }
}

void PixIntroLevel::update(float delta)
{
    GbLevel::update(delta);
    
    if(pageview)
    {
        int nPageIndex = (int)pageview->getCurrentPageIndex();
        
        if(curPage != nPageIndex)
        {
            curPage = nPageIndex;
            setPageMarker(curPage);
        }
    }
    
    if(SMARTALBUM_MANAGER->getDownloadResourcesCount() > 0)
    {
        updateIntroProgressLayer();
    }
}

void PixIntroLevel::onExit()
{
    GbLevel::onExit();
    
    if(PIXAPP->isLocalImageLoading == false)
    {
        PIXAPP->nativeCallPathOfAllImages();
    }
}

void PixIntroLevel::updateIntroProgressLayer()
{
    if(introProgressLayer)
    {
        introProgressLayer->setVisible(true);
        
        int nMaxCount = SMARTALBUM_MANAGER->getDownloadResourcesCount();
        int nCurCount = nMaxCount - SMARTALBUM_MANAGER->getDownloadCount();
        
        if(introProgressBar && introProgressPercent)
        {
            float fPercent = (nCurCount / (float)nMaxCount) * 100.0f;
            introProgressBar->setPercent(fPercent);
            
            std::string strPercent = StringUtils::format("%d%%",(int)fPercent);
            introProgressPercent->setString(strPercent);
        }
        
        if(introProgressLabel)
        {
            if(!mstr_ProgressText.empty())
            {
                introProgressLabel->setString(StringUtils::format(mstr_ProgressText.c_str(),
                                                                  nCurCount,
                                                                  nMaxCount));
            }
        }
    }
}

void PixIntroLevel::introLayerAniStart()
{
    if(PIXAPP->userInfo->getGrade().compare("0") == 0)
    {
        this->schedule(schedule_selector(PixIntroLevel::introLayerFadeOut), INTRO_DELAY);
    }
    else
    {
        this->schedule(schedule_selector(PixIntroLevel::goNextLevel), INTRO_DELAY);
    }
}

void PixIntroLevel::introLayerAniEnd()
{
    if(introLayer)
    {
        rootNode->removeChild(introLayer);
        introLayer = nullptr;
        introProgressLayer = nullptr;
    }
    
    layout->setVisible(true);
    
    int nCount = (int)layout->getChildrenCount();
    FadeIn *fadeIn = FadeIn::create(0.3f);
    
    for(int i = 0; i < nCount; ++i)
    {
        Node *child = dynamic_cast<Node*>(layout->getChildren().at(i));
        
        if(child)
        {
            child->setOpacity(0);
            child->runAction(fadeIn->clone());
        }
    }
    
    nCount = (int)pageItemVec.size();
    
    for(int i = 0; i < nCount; ++i)
    {
        pageItemVec.at(i)->setOpacity(0);
        pageItemVec.at(i)->runAction(fadeIn->clone());
    }
}


void PixIntroLevel::introLayerFadeOut(float dt)
{
    this->unschedule(schedule_selector(PixIntroLevel::introLayerFadeOut));
    this->schedule(schedule_selector(PixIntroLevel::checkDownloadResources));
}

void PixIntroLevel::checkDownloadResources(float dt)
{
    if(introLayer)
    {
        if(SMARTALBUM_MANAGER->isDownloadResources())
        {
            // 다운로드중
        }
        else
        {
            if(mbln_ThemeInfo && mbln_ServerInfo && mbln_Multilingual)
            {
                // 업데이트 알림이나 공지가 있으면 다음 페이지로 이동 시키지 않는다
                if(Director::getInstance()->getRunningScene()->getChildByTag(CHECK_LAYER_TAG) == nullptr)
                {
                    this->unschedule(schedule_selector(PixIntroLevel::checkDownloadResources));
                    introLayerFadeOut();
                    initPageViewLayout();
                }
            }
        }
    }
}

void PixIntroLevel::introLayerFadeOut()
{
    FadeOut *fadeOut = FadeOut::create(0.3f);
    
    CallFunc *callFunc = CallFunc::create(CC_CALLBACK_0(PixIntroLevel::introLayerAniEnd, this));
    Sequence *sequence = Sequence::create(fadeOut, callFunc, NULL);
    introLayer->runAction(sequence);
    
    if(introLayer)
    {
        Node *Intro_Logo_ani = dynamic_cast<Node*>(introLayer->getChildByName("Intro_Logo_ani"));
        
        if(Intro_Logo_ani)
        {
            introLayer->removeChild(Intro_Logo_ani);
        }
    }
}

void PixIntroLevel::initPageViewLayout()
{
    layout = dynamic_cast<ui::Layout*>(rootNode->getChildByName("layout"));
    
    if(layout)
    {
        pageview = dynamic_cast<ui::PageView*>(layout->getChildByName("pageview"));
        
        if(pageview)
        {
            pageview->addEventListener(CC_CALLBACK_2(PixIntroLevel::pageViewEventCallBack, this));
            
            for (int i = 1; i < 5; ++i)
            {
                std::string strLayout = "01_Intro/IntroSkip_0";
                strLayout.append(StringUtils::format("%d.csb",i));

                Node *item = CSLoader::createNode(strLayout);
                
                if(item)
                {
                    ui::Layout *layout = ui::Layout::create();
                    layout->setContentSize(item->getContentSize());
                    layout->addChild(item);
                    pageview->addPage(layout);
                    pageItemVec.push_back(item);
                }
            }
            
            pageview->setCurrentPageIndex(0);
        }
        
        ui::Layout* ani = dynamic_cast<ui::Layout*>(layout->getChildByName("ani"));
        
        if(ani)
        {
            for (int i = 0; i < INTRO_BACK_COUNT; ++i)
            {
                std::string strName = "back_0";
                strName.append(StringUtils::format("%d",i));
                backAni[i] = dynamic_cast<Sprite*>(ani->getChildByName(strName));
                
                if(backAni[i])
                {
                    backAni[i]->setTag(i);
                }
            }
            
            if(backAni[mn_CurAni])
            {
                runBackAniAction(backAni[mn_CurAni]);
            }
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        layout_platform = dynamic_cast<ui::Layout*>(layout->getChildByName("Panel_IOS"));
        
        if(layout_platform)
        {
            layout_platform->setVisible(false);
        }
        
        layout_platform = dynamic_cast<ui::Layout*>(layout->getChildByName("Panel_AOS"));
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        layout_platform = dynamic_cast<ui::Layout*>(layout->getChildByName("Panel_AOS"));
        
        if(layout_platform)
        {
            layout_platform->setVisible(false);
        }
        
        layout_platform = dynamic_cast<ui::Layout*>(layout->getChildByName("Panel_IOS"));
        
#endif
        
        if(layout_platform)
        {
            layout_platform->setVisible(true);

            
            Button_join = dynamic_cast<ui::Button*>(layout_platform->getChildByName("Button_join"));
            
            if (Button_join)
            {
                Button_join->addTouchEventListener(CC_CALLBACK_2(PixIntroLevel::buttonJoinPressed, this));
                
                ui::Text *text = dynamic_cast<ui::Text*>(Button_join->getChildByName("Text_login_join"));
                
                if(text)
                {
                    text->setString(CODE_NAME(text->getString()));
                }
            }
            
            Button_login_kakao = dynamic_cast<ui::Button*>(layout_platform->getChildByName("Button_login_kakao"));
            
            if (Button_login_kakao)
            {
                Button_login_kakao->addTouchEventListener(CC_CALLBACK_2(PixIntroLevel::buttonLoginKakao, this));
                
                ui::Text *text = dynamic_cast<ui::Text*>(Button_login_kakao->getChildByName("Text_login_kakao"));
                
                if(text)
                {
                    text->setString(CODE_NAME(text->getString()));
                }
            }
            
            Button_login_face = dynamic_cast<ui::Button*>(layout_platform->getChildByName("Button_login_face"));
            
            if (Button_login_face)
            {
                Button_login_face->addTouchEventListener(CC_CALLBACK_2(PixIntroLevel::buttonLoginFacePressed, this));
                
                ui::Text *text = dynamic_cast<ui::Text*>(Button_login_face->getChildByName("Text_login_face"));
                
                if(text)
                {
                    text->setString(CODE_NAME(text->getString()));
                }
            }
            
            Button_join_Google = dynamic_cast<ui::Button*>(layout_platform->getChildByName("Button_join_Google"));
            
            if (Button_join_Google)
            {
                Button_join_Google->addTouchEventListener(CC_CALLBACK_2(PixIntroLevel::buttonLoginGooglePressed, this));
                
                ui::Text *text = dynamic_cast<ui::Text*>(Button_join_Google->getChildByName("Text_login_join"));
                
                if(text)
                {
                    text->setString(CODE_NAME(text->getString()));
                }
            }
            
            Button_login = dynamic_cast<ui::Button*>(layout_platform->getChildByName("Button_login"));
            
            if (Button_login)
            {
                Button_login->addTouchEventListener(CC_CALLBACK_2(PixIntroLevel::buttonLoginPressed, this));
                
                ui::Text *text = dynamic_cast<ui::Text*>(Button_login->getChildByName("Text_login"));
                
                if(text)
                {
                    text->setString(CODE_NAME(text->getString()));
                }
            }
        }
        
       
        
        layout->setVisible(false);
    }
    
    setPageMarker(curPage);
}

void PixIntroLevel::pageViewEventCallBack(Ref *target, ui::PageView::EventType type)
{
    if(type == ui::PageView::EventType::TURNING)
    {
//        if(pageview)
//        {
//            int curPage = (int)pageview->getCurrentPageIndex();
//            
//            setPageMarker(curPage);
//        }
    }
}

void PixIntroLevel::setPageMarker(int nPage)
{
    if(nPage == 0)
    {
        layout->getChildByName("intro_next_on_3")->setVisible(true);
        layout->getChildByName("intro_next_on_3_0")->setVisible(false);
        layout->getChildByName("intro_next_on_3_0_0")->setVisible(false);
        layout->getChildByName("intro_next_on_3_0_0_0")->setVisible(false);
    }
    else if(nPage == 1)
    {
        layout->getChildByName("intro_next_on_3")->setVisible(false);
        layout->getChildByName("intro_next_on_3_0")->setVisible(true);
        layout->getChildByName("intro_next_on_3_0_0")->setVisible(false);
        layout->getChildByName("intro_next_on_3_0_0_0")->setVisible(false);

    }
    else if(nPage == 2)
    {
        layout->getChildByName("intro_next_on_3")->setVisible(false);
        layout->getChildByName("intro_next_on_3_0")->setVisible(false);
        layout->getChildByName("intro_next_on_3_0_0")->setVisible(true);
        layout->getChildByName("intro_next_on_3_0_0_0")->setVisible(false);
    }
    else if(nPage >= 3)
    {
        layout->getChildByName("intro_next_on_3")->setVisible(false);
        layout->getChildByName("intro_next_on_3_0")->setVisible(false);
        layout->getChildByName("intro_next_on_3_0_0")->setVisible(false);
        layout->getChildByName("intro_next_on_3_0_0_0")->setVisible(true);
    }
}

void PixIntroLevel::goNextLevel(float dt)
{
    this->unschedule(schedule_selector(PixIntroLevel::goNextLevel));
    this->schedule(schedule_selector(PixIntroLevel::checkMemberAndResource));
}

void PixIntroLevel::checkMemberAndResource(float dt)
{
    if(mbln_ThemeInfo && mbln_ServerInfo && mbln_Multilingual)
    {
        // 업데이트 알림이나 공지가 있으면 다음 페이지로 이동 시키지 않는다
        if(Director::getInstance()->getRunningScene()->getChildByTag(CHECK_LAYER_TAG) == nullptr)
        {
            if(!SMARTALBUM_MANAGER->isDownloadResources() && pixHttpClient->isRequest("MEMBER_CHECK_SYNC") == false)
            {
                if(mbln_MemberCheck)
                {
                    this->unschedule(schedule_selector(PixIntroLevel::checkMemberAndResource));
                    PIXAPP->hideLoading();
                    
                    PIXAPP->userInfo->setMember_id(PixUserDefault::GetMemberID());
                    PIXAPP->userInfo->setUserID(PixUserDefault::GetUserID());
                    PIXAPP->userInfo->setName(PixUserDefault::GetName());
                    PIXAPP->userInfo->setGrade(PixUserDefault::GetGrade());
                    PIXAPP->userInfo->setEmail(PixUserDefault::GetEmail());
                    
                    PIXAPP->goLevel(0, PT_MYALBUM, TR_FADE);
                }
                else
                {
                    if(mbln_MemberCheckError)
                    {
                        
                    }
                    else
                    {
                        this->unschedule(schedule_selector(PixIntroLevel::checkMemberAndResource));
                        
                        if(mn_MemberCheckCode == 1303)
                        {
                            PixUserDefault::SetMemberID("");
                            PixUserDefault::SetUserID("0");
                            PixUserDefault::SetName("");
                            PixUserDefault::SetGrade("0");
                            PixUserDefault::SetPhone("0");
                            PixUserDefault::SetEmail("");
                            PixUserDefault::setNoticeID(0);
                            PixUserDefault::SetUserType(0);
                        }
                        
                        introLayerFadeOut();
                        initPageViewLayout();
                    }
                }
            }
        }
    }
}

void PixIntroLevel::initServcerURL()
{
    mbln_ServerInfo = true;
    SREVERURL_MANAGER->init();
    downloadThemeInfo();
    downloadMultilingual();
    
    if(PIXAPP->userInfo->getGrade().compare("0") == 0)
    {
        mbln_MemberCheck = true;
    }
    else
    {
        memberCheckCall();
    }
}

void PixIntroLevel::initThemeInfo()
{
    mbln_ThemeInfo = true;
    SMARTALBUM_MANAGER->init();
}

void PixIntroLevel::initMultilingual()
{
    mbln_Multilingual = true;
    MASSAGE_MANAGER->init();
    
    // 전체 공지 및 업데이트 처리
    // 우선 순위는 업데이트 먼저
    // 알럿 창에 다국어 코드가 들어가므로 다국어 다운로드 이후 버전 체크
    
    PixDictionary *updateInfo =  SREVERURL_MANAGER->getUpdateInfo();
    
    if(updateInfo)
    {
        //                    type	1:일반 , 2: 강제
        //                    msg	업데이트 공지 메시지
        //                    version	앱 버전
        //                    store_url	스토어 url
        int nType = updateInfo->intForKey("type");
        std::string msg = updateInfo->stringForKey("msg");
        std::string version = updateInfo->stringForKey("version");
        std::string store_url = updateInfo->stringForKey("store_url");
        std::string serAppVersion = PixNativeUtils::getAppVersionString();
        
        int nVer = atoi(PixStringUtils::replaceAll(version, ".", "").c_str());
        int nCurVer =  atoi(PixStringUtils::replaceAll(serAppVersion, ".", "").c_str());
        
        if(nVer > nCurVer)
        {
            if(nType == 1)
            {
                PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, CHECK_LAYER_TAG, this, callfunc_selector(PixIntroLevel::updateApplication));
                return;
            }
            else if(nType == 2)
            {
                PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK, CHECK_LAYER_TAG, this, callfunc_selector(PixIntroLevel::updateApplication));
                return;
            }
            else
            {
                
            }
        }
    }
    
    PixDictionary *serverNoticeInfo =  SREVERURL_MANAGER->getServerNoticeInfo();
    
    if(serverNoticeInfo)
    {
        //                    type	1:메시지타입, 2: 웹뷰타입
        //                    msg	메시지
        //                    url	웹뷰 URL
        
        int nType = serverNoticeInfo->intForKey("type");
        std::string msg = serverNoticeInfo->stringForKey("msg");
        std::string url = serverNoticeInfo->stringForKey("url");
        
        if(nType == 1)
        {
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK, CHECK_LAYER_TAG);
            return;
        }
        else if(nType == 2)
        {
            PixServerNoticeWebViewLayer *serverNoticeWebViewLayer = PixServerNoticeWebViewLayer::create(url);
            Director::getInstance()->getRunningScene()->addChild(serverNoticeWebViewLayer, 100, CHECK_LAYER_TAG);
            return;
        }
        else
        {
            
        }
    }
    
    PIXAPP->setLoadingLabelString();
    
    if(introProgressLabel)
    {
        mstr_ProgressText = CODE_NAME("Intro05");
        
        if(!mstr_ProgressText.empty())
        {
            introProgressLabel->setString(StringUtils::format(mstr_ProgressText.c_str(), 0,0));
        }
    }
}

void PixIntroLevel::retryDownloadInfo()
{
    mbln_ThemeInfo = false;
    mbln_ServerInfo = false;
    mbln_Multilingual = false;

    downloadServerInfo();

}

void PixIntroLevel::updateApplication()
{
    PixDictionary *updateInfo =  SREVERURL_MANAGER->getUpdateInfo();
    
    if(updateInfo)
    {
        PixNativeUtils::goURL(updateInfo->stringForKey("store_url").c_str());
        
        //업데이트 마켓이동  다음 씬 진행되는데 필요한 값을 모두 false 처리
        mbln_ThemeInfo = false;
        mbln_ServerInfo = false;
        mbln_Multilingual = false;
    }
}

#pragma mark - Action

void PixIntroLevel::runBackAniAction(Sprite *back)
{
    if(back)
    {
        back->setScale(1.0f);
        back->setOpacity(255);
        back->setVisible(true);
        back->setLocalZOrder(2);
       
        int nNextAni = mn_CurAni + 1;
        
        if(nNextAni == INTRO_BACK_COUNT)
        {
            nNextAni = INTRO_BACK_00;
        }

        if(backAni[nNextAni])
        {
            backAni[nNextAni]->setScale(1.0f);
            backAni[nNextAni]->setOpacity(255);
            backAni[nNextAni]->setVisible(true);
            backAni[nNextAni]->setLocalZOrder(1);
        }
        
        ScaleTo *scaleTo = ScaleTo::create(2.5f, 1.1f);
        FadeTo *fadeTo = FadeTo::create(1.2f, 0);
        
        CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixIntroLevel::pageViewAniAction, this));
        
        Spawn *spawn = Spawn::create(scaleTo,
                                     Sequence::create(DelayTime::create(1.2f), callFuncN, NULL),
                                     Sequence::create(DelayTime::create(0.8f), fadeTo, NULL),
                                     NULL);
        
        callFuncN = CallFuncN::create(CC_CALLBACK_1(PixIntroLevel::backAniFinish, this));
        Sequence *sequence = Sequence::create(DelayTime::create(0.5f), spawn, callFuncN, NULL);
        
        back->runAction(sequence);
    }
}

void PixIntroLevel::backAniFinish(Node *node)
{
    Sprite *back = dynamic_cast<Sprite*>(node);
    
    if(back)
    {
        back->setVisible(false);
        back->setLocalZOrder(0);
    }
    
    ++mn_CurAni;
    
    if(mn_CurAni == INTRO_BACK_COUNT)
    {
        mn_CurAni = INTRO_BACK_00;
    }
    
    runBackAniAction(backAni[mn_CurAni]);
}

void PixIntroLevel::pageViewAniAction(Node *node)
{
    if(pageview)
    {
        int nIndex = (int)pageview->getChildrenCount();
        
        ++curPage;
        
        if(curPage > (nIndex - 1))
        {
            curPage = 0;
        }
        
        pageview->scrollToPage(curPage);
    }
}

#pragma mark - Button

void PixIntroLevel::buttonJoinPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        PixLoginJoinLayer *loginJoinLayer = PixLoginJoinLayer::create();
        Director::getInstance()->getRunningScene()->addChild(loginJoinLayer);
    }
}

void PixIntroLevel::buttonLoginKakao(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        KAKAO_MANAGER->kakaoLogin(CC_CALLBACK_0(PixIntroLevel::kakaoMemberAuth, this));
    }
}

void PixIntroLevel::buttonLoginFacePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        FACEBOOK_MANAGER->onLogin();
    }
}

void PixIntroLevel::buttonLoginGooglePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        ACCOUNTS_GOOGLE_MANAGER->accountsGoogle(CC_CALLBACK_0(PixIntroLevel::googleMemberAuth, this));
    }
}

void PixIntroLevel::buttonLoginPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        PixLoginLayer *loginLayer = PixLoginLayer::create();
        Director::getInstance()->getRunningScene()->addChild(loginLayer);
    }
}

void PixIntroLevel::buttonDisplayStatsPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(strcmp(SERVER_INFO_TYPE, "dev") == 0)
        {
            auto director = Director::getInstance();
            bool blnEnabel = PixUserDefault::GetDisplayStats();
            PixUserDefault::SetDisplayStats(!blnEnabel);
            director->setDisplayStats(!blnEnabel);
        }
    }
}

#pragma mark - Network

void PixIntroLevel::memberCheckCall(bool isAuth)
{
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_CHECK_SYNC");
    std::map<std::string, std::string> postParams;
    
    //    암호화(member_id=xxx&vendor=xxx&notice_id=xxx)
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["notice_id"] = StringUtils::format("%d",PixUserDefault::GetNoticeID());
    postParams["player_id"] = ONESIGNAL_MANAGER->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    std::string requestTag = (isAuth == true) ? "MEMBER_CHECK_SYNC_AUTH" : "MEMBER_CHECK_SYNC";
    
    pixHttpClient->getFormData(url, postParams, requestTag, this, httpresponse_selector(PixIntroLevel::memberCheckCallback), httpresponse_selector(PixIntroLevel::memberCheckError), nullptr);
}

void PixIntroLevel::facebookMemberAuth(const std::map<std::string, std::string>& userInfo)
{
    PIXAPP->showLoading();
    
    facebookUserInfo = userInfo;
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_LOGIN");
    std::map<std::string, std::string> postParams;
    
    std::string osname = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    osname = "Android";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    osname = "IOS";
#endif
    
    std::string strUUID = PixUserDefault::GetUUID();
    
    if(strUUID.compare("0") == 0)
    {
        strUUID = PixNativeUtils::getDeviceId();
        PixUserDefault::SetUUID(strUUID);
    }
    
    std::string strUserID;
    std::string strUserName;
    std::string f_uid;
    std::string f_url;
    
    std::map<std::string, std::string>::const_iterator iter;
    
    for (iter = userInfo.begin(); iter != userInfo.end(); ++iter)
    {
        if(iter->first.compare("email") == 0)
        {
            strUserID = iter->second;
        }
        else if(iter->first.compare("name") == 0)
        {
            strUserName = iter->second;
        }
        else if(iter->first.compare("id") == 0)
        {
            f_uid = iter->second;
        }
        else if(iter->first.compare("link") == 0)
        {
            f_url = iter->second;
        }
    }
    
    postParams["strUserID"] = strUserID;
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["password"] = "0";
    //    postParams["hp"] = PixNativeUtils::getPhoneNumber();
    postParams["uuid"] = strUUID;
    postParams["width"] = StringUtils::format("%d",PixNativeUtils::getDeviceWidth());
    postParams["height"] = StringUtils::format("%d",PixNativeUtils::getDeviceHeight());
    postParams["manufacturer"] = PixNativeUtils::getManuFacturer();
    postParams["model"] = PixNativeUtils::getDeviceBrand();
    postParams["architecture"] = PixNativeUtils::getOsVersionString();
    postParams["name"] = osname;
    postParams["osname"] = osname.append(PixNativeUtils::getOsVersionString());
    postParams["strUserName"] = strUserName;
    postParams["token"] = ONESIGNAL_MANAGER->getPushToken();
    postParams["userType"] = "2";
    postParams["f_uid"] = f_uid;
    postParams["f_url"] = f_url;
    postParams["referer"] = "2";
    postParams["isReceiveMail"] = "0";
    postParams["player_id"] = ONESIGNAL_MANAGER->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    pixHttpClient->getFormData(url, postParams, "member_auth_facebook", this, httpresponse_selector(PixIntroLevel::memberAuthCallback), nullptr);
}

void PixIntroLevel::googleMemberAuth()
{
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_LOGIN");
    std::map<std::string, std::string> postParams;
    
    std::string osname = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    osname = "Android";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    osname = "IOS";
#endif
    
    std::string strUUID = PixUserDefault::GetUUID();
    
    if(strUUID.compare("0") == 0)
    {
        strUUID = PixNativeUtils::getDeviceId();
        PixUserDefault::SetUUID(strUUID);
    }
    
    postParams["strUserID"] = ACCOUNTS_GOOGLE_MANAGER->getUserEmail();
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["password"] = "0";
    //    postParams["hp"] = PixNativeUtils::getPhoneNumber();
    postParams["uuid"] = strUUID;
    postParams["width"] = StringUtils::format("%d",PixNativeUtils::getDeviceWidth());
    postParams["height"] = StringUtils::format("%d",PixNativeUtils::getDeviceHeight());
    postParams["manufacturer"] = PixNativeUtils::getManuFacturer();
    postParams["model"] = PixNativeUtils::getDeviceBrand();
    postParams["architecture"] = PixNativeUtils::getOsVersionString();
    postParams["name"] = osname;
    postParams["osname"] = osname.append(PixNativeUtils::getOsVersionString());
    postParams["strUserName"] = ACCOUNTS_GOOGLE_MANAGER->getUserName();
    postParams["token"] = ONESIGNAL_MANAGER->getPushToken();
    postParams["userType"] = "4";
    postParams["f_uid"] = ACCOUNTS_GOOGLE_MANAGER->getUserID();
    postParams["f_url"] = ACCOUNTS_GOOGLE_MANAGER->getUserProfile();
    postParams["referer"] = "2";
    postParams["isReceiveMail"] = "0";
    postParams["player_id"] = ONESIGNAL_MANAGER->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    pixHttpClient->getFormData(url, postParams, "member_auth_google", this, httpresponse_selector(PixIntroLevel::memberAuthCallback), nullptr);
}

void PixIntroLevel::kakaoMemberAuth()
{
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("MEMBER_LOGIN");
    std::map<std::string, std::string> postParams;
    
    std::string osname = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    osname = "Android";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    osname = "IOS";
#endif
    
    std::string strUUID = PixUserDefault::GetUUID();
    
    if(strUUID.compare("0") == 0)
    {
        strUUID = PixNativeUtils::getDeviceId();
        PixUserDefault::SetUUID(strUUID);
    }
    
    postParams["strUserID"] = KAKAO_MANAGER->getUserEmail();
    postParams["strVendor"] = PIXAPP->userInfo->getVendor();
    postParams["password"] = "0";
    //    postParams["hp"] = PixNativeUtils::getPhoneNumber();
    postParams["uuid"] = strUUID;
    postParams["width"] = StringUtils::format("%d",PixNativeUtils::getDeviceWidth());
    postParams["height"] = StringUtils::format("%d",PixNativeUtils::getDeviceHeight());
    postParams["manufacturer"] = PixNativeUtils::getManuFacturer();
    postParams["model"] = PixNativeUtils::getDeviceBrand();
    postParams["architecture"] = PixNativeUtils::getOsVersionString();
    postParams["name"] = osname;
    postParams["osname"] = osname.append(PixNativeUtils::getOsVersionString());
    postParams["strUserName"] = KAKAO_MANAGER->getUserName();
    postParams["token"] = ONESIGNAL_MANAGER->getPushToken();
    postParams["userType"] = "3";
    postParams["f_uid"] = KAKAO_MANAGER->getUserID();
    postParams["f_url"] = KAKAO_MANAGER->getUserProfile();
    postParams["referer"] = "2";
    postParams["isReceiveMail"] = "0";
    postParams["player_id"] = ONESIGNAL_MANAGER->getUserID();
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    pixHttpClient->getFormData(url, postParams, "member_auth_kakao", this, httpresponse_selector(PixIntroLevel::memberAuthCallback), nullptr);
}

void PixIntroLevel::memberCheckCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        mn_MemberCheckCode = root->intForKey("code");
       
        if(mn_MemberCheckCode == 1303)
        {
            return;
        }
        
        std::string strUserID = root->stringForKey("strUserID");
                    
        if(PIXAPP->userInfo->getUserID().compare(strUserID) == 0)
        {
            int nCartCount = root->intForKey("cartCount");
            int nNewNotice = root->intForKey("newNotice");
            int nMax_event_id = root->intForKey("max_event_id");
            
            PIXAPP->userInfo->setCartCount(nCartCount);
            PIXAPP->userInfo->setNoticeNew((nNewNotice > 0) ? true : false);
            PIXAPP->userInfo->setEventNew((nMax_event_id > PixUserDefault::GetEventID()) ? true : false);
            
            GOOGLEANALYTICS_MANAGER->logEvent("member", "member_check", strUserID, 1);
            mbln_MemberCheck = true;
            
            if(strcmp(response->getHttpRequest()->getTag(), "MEMBER_CHECK_SYNC_AUTH") == 0)
            {
                PIXAPP->goLevel(0, PT_MYALBUM, TR_FADE);
            }
            
            return;
        }
        else
        {
            CCLOG("MEMBER_CHECK_SYNC ERROR PIXAPP->userInfo->getUserID() : %s", PIXAPP->userInfo->getUserID().c_str());
            
            CCLOG("MEMBER_CHECK_SYNC ERROR strUserID : %s", strUserID.c_str());
        }
    }
    else
    {
        PixAlert::show("Error", "Error", nullptr, ALERT_TYPE_OK);
        return;
    }
}

void PixIntroLevel::memberCheckError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    mbln_MemberCheckError = true;
}

void PixIntroLevel::memberAuthCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        int nCode = root->intForKey("code");
        
        if(nCode == 1113)
        {
            PixSocialLoginLayer *socialLoginLayer = PixSocialLoginLayer::facebookLogin(facebookUserInfo);
            Director::getInstance()->getRunningScene()->addChild(socialLoginLayer);
            return;
        }
        
        PIX_USER_TYPE eUserType = USER_NONE;
        
        if (strcmp(response->getHttpRequest()->getTag(), "member_auth") == 0)
        {
            eUserType = USER_GENERAL;
        }
        else if(strcmp(response->getHttpRequest()->getTag(), "member_auth_facebook") == 0)
        {
            eUserType = USER_FACEBOOK;
        }
        else if(strcmp(response->getHttpRequest()->getTag(), "member_auth_google") == 0)
        {
            eUserType = USER_GOOGLE;
        }
        else if(strcmp(response->getHttpRequest()->getTag(), "member_auth_kakao") == 0)
        {
            eUserType = USER_KAKAO;
        }
        
        std::string member_id = root->stringForKey("member_id");
        std::string strUserID = root->stringForKey("strUserID");
        std::string name = root->stringForKey("name");
        std::string nGrade = root->stringForKey("nGrade");
        std::string strPhone = root->stringForKey("strPhone");
        std::string strEmail = root->stringForKey("strEmail");
    
        PIXAPP->userInfo->setMember_id(member_id);
        PIXAPP->userInfo->setUserID(strUserID);
        PIXAPP->userInfo->setName(name);
        PIXAPP->userInfo->setGrade(nGrade);
        PIXAPP->userInfo->setPhoneNumber(strPhone);
        PIXAPP->userInfo->setEmail(strEmail);
        PIXAPP->userInfo->setUserType(eUserType);
        
        PixUserDefault::SetMemberID(member_id);
        PixUserDefault::SetUserID(strUserID);
        PixUserDefault::SetName(name);
        PixUserDefault::SetGrade(nGrade);
        PixUserDefault::SetPhone(strPhone);
        PixUserDefault::SetEmail(strEmail);
        PixUserDefault::SetUserType((int)eUserType);
        
        GOOGLEANALYTICS_MANAGER->logEvent("member", "auth", PIXAPP->userInfo->getUserID(), 1);
        
        // 서버에서 내려주는 값으로 계산 해야된다
        // 회원가입 이벤트 팝업 활성화
        
        if(root->intForKey("isNew") == 1)
        {
            PIXAPP->userInfo->setShowMembershipEventLayer(true);
        }
        
        memberCheckCall(true);
    }
    else
    {
        PixAlert::show("Error", "Error", nullptr, ALERT_TYPE_OK);
        return;
    }
}

#pragma mark - Downloade 

void PixIntroLevel::downloadServerInfo()
{
    PixDownloader *pixDownloader = PixDownloader::create(SERVER_INFO_URL, SERVER_INFO_PATH, this);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->createDownloadDataTask("server_info");
}

void PixIntroLevel::downloadThemeInfo()
{
    std::string url = SREVERURL_MANAGER->getUrl("THEME_JSON");
    url.append(".").append(THEME_INFO_VERSION);
    
    PixDownloader *pixDownloader = PixDownloader::create(url.c_str(), THEME_PATH, this);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->createDownloadDataTask("theme_info");
}

void PixIntroLevel::downloadMultilingual()
{
    std::string url = SREVERURL_MANAGER->getUrl("MULTILINGUAL");
    
    PixDownloader *pixDownloader = PixDownloader::create(url.c_str(), MASSAGE_INFO_PATH, this);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->createDownloadDataTask("Multilingual");
}

#pragma mark - Downloade Delegate

void PixIntroLevel::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        downloader->release();
        
        if(identifier.compare("server_info") == 0)
        {
            std::string fullPath = std::string(SERVER_INFO_PATH).append(SERVER_INFO_FILENAME);
            
            bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
            
            if(isFileExist)
            {
                initServcerURL();
            }
            else
            {
                PixAlert::show(MASSAGE_MANAGER->getNetworkErrorMessage().c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixIntroLevel::retryDownloadInfo));
            }
            
        }
        else if(identifier.compare("theme_info") == 0)
        {
            std::string fullPath = std::string(THEME_PATH).append(THEME_INFO_FILENAME);
            
            bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
            
            if(isFileExist)
            {
                initThemeInfo();
            }
            else
            {
                PixAlert::show(MASSAGE_MANAGER->getNetworkErrorMessage().c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixIntroLevel::retryDownloadInfo));
            }
        }
        else if(identifier.compare("Multilingual") == 0)
        {
            std::string fullPath = std::string(MASSAGE_INFO_PATH).append(MASSAGE_INFO_FILENAME);
            
            bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
            
            if(isFileExist)
            {
                initMultilingual();
            }
            else
            {
                PixAlert::show(MASSAGE_MANAGER->getNetworkErrorMessage().c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixIntroLevel::retryDownloadInfo));
                
            }
        }
        
    });
}

void PixIntroLevel::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        if(identifier.compare("server_info") == 0)
        {
            
        }
        else if(identifier.compare("theme_info") == 0)
        {
            
        }
        else if(identifier.compare("Multilingual") == 0)
        {
            
        }
        
    });
}

void PixIntroLevel::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    downloader->release();
    
    if(identifier.compare("server_info") == 0)
    {
        initServcerURL();
    }
    else if(identifier.compare("theme_info") == 0)
    {
        initThemeInfo();
    }
    else if(identifier.compare("Multilingual") == 0)
    {
        initMultilingual();
    }
}

#pragma mark - FacebookListener

void PixIntroLevel::onLogin(bool isLogin, const std::string& msg)
{
    if(isLogin)
    {
        /*
         android 이슈로 로그인 전에 미리 uuid 생성
         */
        
        PIXAPP->showLoading();
        
        std::string strUUID = PixUserDefault::GetUUID();
        
        if(strUUID.compare("0") == 0)
        {
            strUUID = PixNativeUtils::getDeviceId();
            PixUserDefault::SetUUID(strUUID);
        }
        
        FACEBOOK_MANAGER->onGetMyInfo();
    }
}

void PixIntroLevel::onSharedSuccess(const std::string& message)
{
    
}

void PixIntroLevel::onSharedFailed(const std::string& message)
{
    
}

void PixIntroLevel::onSharedCancel()
{
    
}

void PixIntroLevel::onAPI(const std::string& key, const std::string& jsonData)
{
    
}

void PixIntroLevel::onPermission(bool isLogin, const std::string& msg)
{
    
}

void PixIntroLevel::onFetchFriends(bool ok, const std::string& msg)
{
    
}

void PixIntroLevel::onRequestInvitableFriends( const std::map<std::string, std::string>& friends )
{
    
}

void PixIntroLevel::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
    
}

void PixIntroLevel::onInviteFriendsResult( bool result, const std::string& msg )
{
    
}

void PixIntroLevel::onGetUserInfo( const std::map<std::string, std::string>& userInfo )
{
    PIXAPP->hideLoading();
    facebookMemberAuth(userInfo);
}
