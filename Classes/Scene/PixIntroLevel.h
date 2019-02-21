// 인트로 씬 
#ifndef __PIXINTRO_LEVEL_H__
#define __PIXINTRO_LEVEL_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Core/GbLevel.h"
#include "../Network/PixHttpClient.h"
#include "../Network/PixDownloader.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixServerUrlManager.h"
#include "../Manager/PixFacebookManager.h"
#include "../Manager/PixAccountsGoogleManager.h"

#define START_BUTTON_ANI    621
#define CHECK_LAYER_TAG     40916

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

typedef enum
{
    INTRO_BACK_00 = 0,
    INTRO_BACK_01 = 1,
    INTRO_BACK_02 = 2,
    INTRO_BACK_03 = 3,
    INTRO_BACK_COUNT = 4,
} INTRO_BACK_ANI;

class PixIntroLevel : public GbLevel, public PixDownloaderDelegate, public PixFacebookListener
{
public:
	
	static Scene* scene();
    static Scene *logout();
    
	PixIntroLevel();
	virtual ~PixIntroLevel();

private:

	Node *introLayer;
    Node *introProgressLayer;
    ui::Text *introProgressLabel;
    ui::Text *introProgressPercent;
    ui::LoadingBar *introProgressBar;
    PixHttpClient *pixHttpClient;
    ui::PageView *pageview;
    ui::Layout *layout_platform;
    ui::Button *Button_join;
    ui::Button *Button_login_kakao;
    ui::Button *Button_login_face;
    ui::Button *Button_join_Google;
    ui::Button *Button_login;
    ui::Layout *layout;
    Sprite *backAni[INTRO_BACK_COUNT];
    EventListenerTouchOneByOne *mListener;
    
    std::map<std::string, std::string> facebookUserInfo;
    
    int mn_MemberCheckCode;
    int mn_CurAni;
    bool mbln_MemberCheck;
    bool mbln_MemberCheckError;
    
    CC_SYNTHESIZE(bool, mbln_Loguot, Loguot)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_ProgressText, ProgressText)
    
    std::vector<Node*> pageItemVec;
    
    int curPage;
    bool mbln_ThemeInfo;
    bool mbln_ServerInfo;
    bool mbln_Multilingual;
    
	virtual void initLevel();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    virtual void onExit();

    void updateIntroProgressLayer();
    void introLayerAniStart();
    void introLayerAniEnd();
    void introLayerFadeOut(float dt);
    void checkDownloadResources(float dt);
    void introLayerFadeOut();
    void initPageViewLayout();
    void pageViewEventCallBack(Ref *target, ui::PageView::EventType type);
    void setPageMarker(int nPage);
    void goNextLevel(float dt);
    void checkMemberAndResource(float dt);
    void initServcerURL();
    void initThemeInfo();
    void initMultilingual();
    void retryDownloadInfo();
    void updateApplication();
    
#pragma mark - Action
    
    void runBackAniAction(Sprite *back);
    void backAniFinish(Node *node);
    void pageViewAniAction(Node *node);

#pragma mark - Button
    
    void buttonJoinPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonLoginKakao(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonLoginFacePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonLoginGooglePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonLoginPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonDisplayStatsPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - Network
    
    void memberCheckCall(bool isAuth = false);
    void facebookMemberAuth(const std::map<std::string, std::string>& userInfo);
    void googleMemberAuth();
    void kakaoMemberAuth();
    void memberCheckCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void memberCheckError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void memberAuthCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Downloade
    
    void downloadServerInfo();
    void downloadThemeInfo();
    void downloadMultilingual();
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode);
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent);
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data);
    
#pragma mark - FacebookListener
    
    virtual void onLogin(bool isLogin, const std::string& msg);
    virtual void onSharedSuccess(const std::string& message);
    virtual void onSharedFailed(const std::string& message);
    virtual void onSharedCancel();
    virtual void onAPI(const std::string& key, const std::string& jsonData);
    virtual void onPermission(bool isLogin, const std::string& msg);
    virtual void onFetchFriends(bool ok, const std::string& msg);
    virtual void onRequestInvitableFriends( const std::map<std::string, std::string>& friends );
    virtual void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg );
    virtual void onInviteFriendsResult( bool result, const std::string& msg );
    virtual void onGetUserInfo( const std::map<std::string, std::string>& userInfo );
};

#endif // __PIXINTRO_LEVEL_H__
