//
//  PixSaveProgressLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 1. 19..
//
//

#include "PixSaveProgressLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixSaveProgressLayer* PixSaveProgressLayer::create(std::function<void(Ref*,ui::Widget::TouchEventType)> saveCancelCallback)
{
    PixSaveProgressLayer *saveProgressLayer = new (std::nothrow) PixSaveProgressLayer(saveCancelCallback);
    if (saveProgressLayer && saveProgressLayer->init())
    {
        saveProgressLayer->autorelease();
        return saveProgressLayer;
    }
    else
    {
        CC_SAFE_DELETE(saveProgressLayer);
        return nullptr;
    }
}

PixSaveProgressLayer::PixSaveProgressLayer(std::function<void(Ref*,ui::Widget::TouchEventType)> saveCancelCallback)
: rootNode(nullptr)
, upload_text(nullptr)
, loading(nullptr)
, imageUploadLoadingBar(nullptr)
, imageUploadPercentLabel(nullptr)
, imageUploadLabel(nullptr)
, previewUploadLoadBar(nullptr)
, previewUploadPercentLabel(nullptr)
, previewUploadLabel(nullptr)
, _saveCancelCallback(saveCancelCallback)
{
     hideLayerFunction = std::bind(&PixSaveProgressLayer::hideLayerAction,this);
}

PixSaveProgressLayer::~PixSaveProgressLayer()
{
    
}

void PixSaveProgressLayer::initLayer()
{
    rootNode = CSLoader::createNode("05_AlbumView/SaveProgress.csb");
    
    if(rootNode)
    {
        upload_text = dynamic_cast<ui::Text*>(rootNode->getChildByName("upload_text"));
        
        if(upload_text)
        {
            upload_text->setString(CODE_NAME(upload_text->getString()));
        }
        
        loading = dynamic_cast<Node*>(rootNode->getChildByName("loading"));
        
        if(loading)
        {
            cocostudio::timeline::ActionTimeline *timeLine = CSLoader::createTimeline("05_AlbumView/SaveProgress_ani.csb");
            
            if(timeLine)
            {
                timeLine->gotoFrameAndPlay(0,true);
                loading->runAction(timeLine);
            }
        }
        
        std::string strPercent = StringUtils::format("%d%%", 0);
        
        imageUploadLoadingBar = dynamic_cast<ui::LoadingBar*>(rootNode->getChildByName("imageUploadLoadingBar"));
        
        if(imageUploadLoadingBar)
        {
            imageUploadLoadingBar->setPercent(0);
        }

        imageUploadPercentLabel = dynamic_cast<ui::Text*>(rootNode->getChildByName("imageUploadPercentLabel"));
        
        if(imageUploadPercentLabel)
        {
            imageUploadPercentLabel->setString(strPercent);
        }
        
        imageUploadLabel = dynamic_cast<ui::Text*>(rootNode->getChildByName("imageUploadLabel"));
        
        if(imageUploadLabel)
        {
            imageUploadLabel->setString(CODE_NAME("Save04"));
        }
        
        previewUploadLoadBar = dynamic_cast<ui::LoadingBar*>(rootNode->getChildByName("previewUploadLoadBar"));
        
        if(previewUploadLoadBar)
        {
            previewUploadLoadBar->setPercent(0);
        }
        
        previewUploadPercentLabel = dynamic_cast<ui::Text*>(rootNode->getChildByName("previewUploadPercentLabel"));
        
        if(previewUploadPercentLabel)
        {
            previewUploadPercentLabel->setString(strPercent);
        }
        
        previewUploadLabel = dynamic_cast<ui::Text*>(rootNode->getChildByName("previewUploadLabel"));
        
        if(previewUploadLabel)
        {
            previewUploadLabel->setString(CODE_NAME("Save06"));
        }
        
        ui::Button *btn_back = dynamic_cast<ui::Button*>(rootNode->getChildByName("btn_back"));
        
        if(btn_back)
        {
            btn_back->addTouchEventListener(_saveCancelCallback);
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixSaveProgressLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixSaveProgressLayer::update(float delta)
{
    PixModalLayer::update(delta);
}

void PixSaveProgressLayer::updateImageUpload(const std::string &strMessage, float percent)
{
    float fPercent = percent * 100;
    
    if(imageUploadLoadingBar)
    {
        imageUploadLoadingBar->setPercent(fPercent);
    }
    
    if(imageUploadPercentLabel)
    {
        std::string strPercent = StringUtils::format("%d%%", (int)fPercent);
        imageUploadPercentLabel->setString(strPercent);
    }
    
    if(imageUploadLabel)
    {
        imageUploadLabel->setString(strMessage);
    }
}

void PixSaveProgressLayer::updatePreviewUpload(const std::string &strMessage, float percent)
{
    float fPercent = percent * 100;
    
    if(previewUploadLoadBar)
    {
        previewUploadLoadBar->setPercent(fPercent);
    }
    
    if(previewUploadPercentLabel)
    {
        std::string strPercent = StringUtils::format("%d%%", (int)fPercent);
        previewUploadPercentLabel->setString(strPercent);
    }
    
    if(previewUploadLabel)
    {
        previewUploadLabel->setString(strMessage);
    }
}

void PixSaveProgressLayer::resetUpload()
{
    std::string strPercent = StringUtils::format("%d%%", 0);
    
    if(imageUploadLoadingBar)
    {
        imageUploadLoadingBar->setPercent(0);
    }
    
    if(imageUploadPercentLabel)
    {
        imageUploadPercentLabel->setString(strPercent);
    }
    
    if(imageUploadLabel)
    {
        imageUploadLabel->setString(CODE_NAME("Save04"));
    }
    
    if(previewUploadLoadBar)
    {
        previewUploadLoadBar->setPercent(0);
    }
    
    if(previewUploadPercentLabel)
    {
        previewUploadPercentLabel->setString(strPercent);
    }
    
    if(previewUploadLabel)
    {
        previewUploadLabel->setString(CODE_NAME("Save06"));
    }
}

void PixSaveProgressLayer::hideLayerAction()
{
    
}

#pragma mark - Android BackPressed

void PixSaveProgressLayer::androidBackPressed()
{
    
}
