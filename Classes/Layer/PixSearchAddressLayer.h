//
//  PixSearchAddressLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 22..
//
//

#ifndef PixSearchAddressLayer_h
#define PixSearchAddressLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixSearchAddressLayerDelegate;

class PixSearchAddressLayer : public PixModalLayer
{
    
public :
    
    static PixSearchAddressLayer* create();
    
    PixSearchAddressLayer();
    virtual ~PixSearchAddressLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
    void searchAddressCompile(const std::string& str);
    void hideLayerAction();
    
private :
    
    Node *rootNode;
    
    CC_SYNTHESIZE(PixSearchAddressLayerDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
    
};

class PixSearchAddressLayerDelegate
{
public :
    
    virtual void searchAddressCompile(const std::string& str) {};
};

#endif /* PixSearchAddressLayer_h */
