//
//  PixMessageManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 18..
//
//

#ifndef PixMessageManager_h
#define PixMessageManager_h

#include "cocos2d.h"
#include "../Network/PixHttpClient.h"

#define MASSAGE_MANAGER     (PixMessageManager::sharedObject())

#define TEXT_001_ko             "네트워크 연결에 실패하였습니다.\n다시 시도하시겠습니까?"
#define TEXT_001_en             "The network connection failed.\nWould you like to try again?"
#define TEXT_001_ja             "ネットワーク接続に失敗しました。\n再試行しますか？"
#define TEXT_001_zh             "未能连接到网络。\n再试一次吗？"
#define TEXT_002_ko             "네트워크 연결에 실패하였습니다.\n확인 후 다시 시도해주세요."
#define TEXT_002_en             "The network connection failed.\nPlease check and try again."
#define TEXT_002_ja             "ネットワーク接続に失敗しました。\n確認後、再試行してください。"
#define TEXT_002_zh             "它无法连接到网络。\n请重试。"

USING_NS_CC;

class PixMessageManager
{
public :
    
    static PixMessageManager* sharedObject();
    
    PixMessageManager();
    virtual ~PixMessageManager();
    
    void clear();
    void init();
    std::string getCodeName(const std::string& cd);
    std::string getNetworkErrorMessage(bool blnRetry = true);
    inline bool managerMapEmpty()
    {
        return (m_ManagerMap == nullptr) ? true : false;
    }
    
    
private:
    
    PixDictionary *m_ManagerMap;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_LanguageCode, LanguageCode)
    
    std::string replaceAllMsg(const std::string &str);
    
};

#endif /* PixMessageManager_h */


