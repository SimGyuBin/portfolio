//
//  PixPushToast.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 7. 7..
//
//

#include "PixPushToast.h"
#include "../Scene/PixApp.h"

PixPushToast* PixPushToast::showToast(const std::string &message)
{
    Scene *scene = PIXAPP->isScene;
    Node *beforeToast = dynamic_cast<Node*>(scene->getChildByTag(PUSH_TOAST_TAG));
    if (beforeToast) beforeToast->removeFromParentAndCleanup(true);
    
    PixPushToast *toast = new PixPushToast();
    toast->initPushToast(message);
    scene->addChild(toast, 301, PUSH_TOAST_TAG);
    toast->release();
    
    return toast;
}

PixPushToast::PixPushToast()
: rootNode(nullptr)
, Image_background_push_toast(nullptr)
, startPos(Vec2::ZERO)
, endPos(Vec2::ZERO)
, mstr_Message("")
{
    
}

PixPushToast::~PixPushToast()
{
    
}

void PixPushToast::initPushToast(const std::string &message)
{
    setMessage(message);
    
    rootNode = CSLoader::createNode("00_Common/Push_Toast.csb");
    
    if(rootNode)
    {
        Image_background_push_toast = dynamic_cast<ui::ImageView*>(rootNode->getChildByName("Image_background_push_toast"));
        
        if(Image_background_push_toast)
        {
            endPos = Image_background_push_toast->getPosition();
            
            startPos = Vec2(endPos.x,
                            (rootNode->getContentSize().height + (Image_background_push_toast->getContentSize().height * 0.5f)) + 300);
            
            Image_background_push_toast->setPosition(startPos);
            
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Image_background_push_toast->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(getMessage());
            }
        }
        
        addChild(rootNode);
    }
}

void PixPushToast::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    showPushToastAction();
}

void PixPushToast::showPushToastAction()
{
    if(rootNode == nullptr)
        return;
    
    if(Image_background_push_toast == nullptr)
        return;
    
    EaseBackOut *easeElasticOut = EaseBackOut::create(MoveTo::create(0.3f,endPos));
    DelayTime *delayTime = DelayTime::create(5.0f);
    EaseSineOut *easeSineOut = EaseSineOut::create(MoveTo::create(0.3f,startPos));
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixPushToast::showPushToastActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeElasticOut, delayTime, easeSineOut, callFuncN, NULL);
    
    Image_background_push_toast->runAction(sequence);
}

void PixPushToast::showPushToastActionFinish(Node* node)
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}
