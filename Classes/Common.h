#include "cocos2d.h"

#define PZLE_SERVER_HOST    "http://14.63.193.112:3003"

#define ENCRYPT_KEY                 "14ade6891732ca74f130322673c6964a"
#define SEARCHADDRES_ENCRYPT_KEY    "397fdY678925X6zykma79wzqd3f45fej"
#define ONESIGNAL_EMAIL             "support@pixholic.com"

#define DEFAULT_LANGUAGE    "en"

// 테스트 코드 
#define ALBUM_MAKE_ONE_PIC      false   // 강제로 1시간 단위로 시간 셋팅

// --------------------------------------------------------------------------------------|
// 개발서버
//#define SERVER_INFO_URL         "https://da7q89edzfrkt.cloudfront.net/system/common/dev/server.json"
//#define SERVER_INFO_TYPE        "dev"
// --------------------------------------------------------------------------------------|

// --------------------------------------------------------------------------------------|
// 실제 서비스
#define SERVER_INFO_URL         "https://da7q89edzfrkt.cloudfront.net/system/common/prod/server.json"
#define SERVER_INFO_TYPE        "prod"
// --------------------------------------------------------------------------------------|

#define THEME_LANGUAGE_CODE     PixSmartAlbumManager::sharedObject()->getLanguageCode()

#define SERVER_INFO_PATH        FileUtils::getInstance()->getWritablePath().append(".serverinfo/").c_str()
#define SERVER_INFO_FILENAME    "server.json"

#define THEME_PATH              FileUtils::getInstance()->getWritablePath().append("Theme/").append(THEME_LANGUAGE_CODE).append("/").c_str()
#define THEME_INFO_FILENAME     std::string("theme.json").append(".").append(THEME_INFO_VERSION).c_str()
#define THEME_INFO_VERSION      "v5"

#define CARDSKIN_PATH           FileUtils::getInstance()->getWritablePath().append("Cardskin/").c_str()
#define THEME_PREVIEW_PATH      FileUtils::getInstance()->getWritablePath().append("ThemePreview/").c_str()

#define MASSAGE_INFO_PATH        FileUtils::getInstance()->getWritablePath().append(".massage/").c_str()
#define MASSAGE_INFO_FILENAME    "language.json"

#define SMARTLAYOUT_PATH        FileUtils::getInstance()->getWritablePath().append("Smartlayout/").c_str()

#define RENDER_DIRECTORY        ".render/"
#define RENDER_PATH             FileUtils::getInstance()->getWritablePath().append(RENDER_DIRECTORY).c_str()

#define CODE_NAME(_Cd_)         (PixMessageManager::sharedObject())->getCodeName(_Cd_).c_str()

#define INTRO_DELAY 3.0f

#define MAX_CACHE_COUNT 250
#define MAX_ALBUM_CACHE_COUNT 5
#define MAX_ALBUM_LOADIMAGE_COUNT 6

#define THREAD_POOL_SIZE 12
#define NETWORK_POOL_SIZE 10
#define ALBUM_SAVE_POOL_SIZE 5
#define ALBUM_RENDER_COUNT 3

#define SELECT_ITEM_COUNT 4
#define ALBUM_SKIN_COUNT 3
#define LOAD_SELECT_ITEM 50

#define SELECT_THEME_COUNT 2
#define SELECT_ADDPHOTO_ITEM_COUNT 7

#define ALBUMSELECT_THUMB_COUNT 4
#define THUMBNAIL_SIZE 180
#define FULLSCREEN_SIZE 640
#define kMaxTextureSize	2048
#define EDITBOX_BACKGROUND  "Common/Text_Mask.png"

#define SCREEN_SIZE             Director::getInstance()->getWinSize()
#define SCREEN_CENTER           Vec2 (SCREEN_SIZE.width * 0.5f, SCREEN_SIZE.height * 0.5f)
#define SCREEN_WIDTH            SCREEN_SIZE.width
#define SCREEN_HEIGHT           SCREEN_SIZE.height

#define SAFE_REMOVE(__child__)  if (__rchild__ && __child__->getParent()) \
{ __child__->removeFromParentAndCleanup(true);  __child__ = NULL;}

#define IMAGECREATE_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, ##__VA_ARGS__)


