#include "AppDelegate.h"
#include "../Classes/Scene/PixApp.h"
#include "Util/PixUserDefault.h"
#include "Manager/PixFacebookManager.h"
#include "Manager/PixOneSignalManager.h"
#include "Manager/PixGoogleAnalyticsManager.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
//static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
//static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
//static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    
    // 화면 꺼짐 방지
    Device::setKeepScreenOn(true);
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Pzle", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
        glview->getFrameZoomFactor();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        glview = GLViewImpl::createWithRect("Pzle", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
        glview->getFrameZoomFactor();
#else
        glview = GLViewImpl::createWithRect("Pzle", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
        //glview = GLViewImpl::createWithRect("cocos2dx_pixholic", Rect(0, 0, 1280, 720));
        //glview = GLViewImpl::create("cocos2dx_pixholic");
#endif
        director->setOpenGLView(glview);
    }
    
    // 최초 glview의 상태 확인
    Size frameSize = glview->getFrameSize();
    
    if(frameSize.width > frameSize.height)
    {
        PixApp::sharedObject()->sethorizontalframeSize(Size(frameSize.width, frameSize.height));
        PixApp::sharedObject()->setVerticalframeSize(Size(frameSize.height, frameSize.width));
    }
    else
    {
        PixApp::sharedObject()->sethorizontalframeSize(Size(frameSize.height, frameSize.width));
        PixApp::sharedObject()->setVerticalframeSize(Size(frameSize.width, frameSize.height));
    }
    
    //	PixApp::sharedObject()->sethorizontalframeSize(frameSize);
    //	PixApp::sharedObject()->setVerticalframeSize(Size(frameSize.height, frameSize.width));
    
    director->setAnimationInterval(1.0 / 60);
    
    register_all_packages();
    
    if(strcmp(SERVER_INFO_TYPE, "dev") == 0)
    {
        bool blnEnabel = PixUserDefault::GetDisplayStats();
        director->setDisplayStats(blnEnabel);
    }
    else
    {
        director->setDisplayStats(false);
    }
    
    // 국가별 리소스 경로 설정
    std::vector<std::string> searchPaths;
    searchPaths.push_back("csb");
    searchPaths.push_back("img");

    const char *code = Application::getInstance()->getCurrentLanguageCode();
    searchPaths.push_back(code);
    
    if(strcmp(code, DEFAULT_LANGUAGE) != 0)
    {
        searchPaths.push_back(DEFAULT_LANGUAGE);
    }
    
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
    // 앱의 시작 점
    PixApp::sharedObject()->startApp();
        
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    CCLOG("AppDelegate::applicationDidEnterBackground()");

    PIXAPP->applicationDidEnterBackground();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    CCLOG("AppDelegate::applicationWillEnterForeground()");

    PIXAPP->applicationWillEnterForeground();
}
