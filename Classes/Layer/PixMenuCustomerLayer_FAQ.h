//
//  PixMenuCustomerLayer_FAQ.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#ifndef PixMenuCustomerLayer_FAQ_h
#define PixMenuCustomerLayer_FAQ_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Network/PixHttpClient.h"
#include "PixLayerActionDelegate.h"
#include "../Info/FaqInfo.h"

USING_NS_CC;

class PixMenuCustomerLayer_FAQ : public Layer
{
public:
    
    static PixMenuCustomerLayer_FAQ *create();
    
    PixMenuCustomerLayer_FAQ();
    virtual ~PixMenuCustomerLayer_FAQ();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
    PixGalleryListView *FAQlistView;
    
    std::vector<FaqInfo*> faqInfoVec;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
    void createFAQlistView();
    
#pragma mark - Network
    
    void boardFaqCall();
    void onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
};


#endif /* PixMenuCustomerLayer_FAQ_h */
