//
//  PixSmartalbumSaveLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 23..
//
//

#include "PixSmartalbumSaveLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Model/PixSmartMaskBookModel.h"
#include "../Manager/PixUploadImageManager.h"
#include "../Manager/PixImageCacheManager.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Manager/PixMyAlbumManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUtils.h"
#include "../UIControl/PixAlert.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

static const bool SEND_IMMEDIATE = false;

PixSmartalbumSaveLayer* PixSmartalbumSaveLayer::create(PixSmartMaskBookModel *pModel)
{
    PixSmartalbumSaveLayer *smartalbumSaveLayer = new (std::nothrow) PixSmartalbumSaveLayer(pModel);
    if (smartalbumSaveLayer && smartalbumSaveLayer->init())
    {
        smartalbumSaveLayer->autorelease();
        return smartalbumSaveLayer;
    }
    else
    {
        CC_SAFE_DELETE(smartalbumSaveLayer);
        return nullptr;
    }
}

PixSmartalbumSaveLayer::PixSmartalbumSaveLayer(PixSmartMaskBookModel *pModel)
: rootNode(nullptr)
, btn_back(nullptr)
, Button_Save(nullptr)
, covertitle(nullptr)
, renderingProgress(nullptr)
, smartMaskBookModel(nullptr)
, nUploadImagesCount(0)
, nUploadImagesTotalCount(0)
, nUploadPreviewCount(0)
, nUploadPreviewTotalCount(0)
, mbln_IsAction(false)
, mn_Album_id(0)
, mstr_AlbumPhotoSaveURL("")
, mstr_FileRenderXML("")
, mn_SaveDoneRetry(3)
{
    this->smartMaskBookModel = pModel;
    albumImagesH600Queue.clear();
    albumImagesF1200Queue.clear();
    uploadImageInfotVec.clear();
        
    pixHttpClient = PixHttpClient::sharedObject();
    pixHttpClient->setTimeoutForConnect(60 * 3);
    pixHttpClient->setMaxRequestImmediateCount(SREVERURL_MANAGER->getImageMaxUploadCount());
    
    hideLayerFunction = std::bind(&PixSmartalbumSaveLayer::hideLayerAction,this);
}

PixSmartalbumSaveLayer::~PixSmartalbumSaveLayer()
{
    pixHttpClient->cancelRequest(this);
    pixHttpClient->setTimeoutForConnect(30);
    
    CC_SAFE_RELEASE_NULL(renderingProgress);
    albumImagesH600Queue.clear();
    albumImagesF1200Queue.clear();
    uploadImageInfoVecClear();
    
    UPLOADIMAGE_MANAGER->clear();
}

void PixSmartalbumSaveLayer::initLayer()
{
    rootNode = CSLoader::createNode("05_AlbumView/smartalbumSaveLayer.csb");
    
    if(rootNode)
    {
        Node *smartalbumSaveTopLayer = dynamic_cast<Node*>(rootNode->getChildByName("smartalbumSaveTopLayer"));
        
        if(smartalbumSaveTopLayer)
        {
            ui::Text *tltle = dynamic_cast<ui::Text*>(smartalbumSaveTopLayer->getChildByName("label"));
            
            if(tltle)
            {
                tltle->setString(CODE_NAME(tltle->getString()));
            }
            
            btn_back = dynamic_cast<ui::Button*>(smartalbumSaveTopLayer->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumSaveLayer::buttonBackPressed, this));
            }
        }
        
        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("covertitle"));
        
        if(textField)
        {
            covertitle = PixEditBox::convertToEditBox(textField, this);
            covertitle->setInputMode(ui::EditBox::InputMode::ANY);
            covertitle->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
            covertitle->setAlignmentCenter();
            covertitle->setNumberOfLines(2);
        }
        
        ui::Text *text = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_1"));
        
        if(text)
        {
            text->setString(CODE_NAME(text->getString()));
        }
        
        text = dynamic_cast<ui::Text*>(rootNode->getChildByName("Text_2"));
        
        if(text)
        {
            text->setString(CODE_NAME(text->getString()));
        }
        
        Button_Save = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Save"));
        
        if(Button_Save)
        {
            Button_Save->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumSaveLayer::buttonSavePressed, this));
            Button_Save->setEnabled(false);
            
            ui::Text *text = dynamic_cast<ui::Text*>(Button_Save->getChildByName("Text_Save"));
            
            if(text)
            {
                text->setString(CODE_NAME(text->getString()));
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        
        float fheight = Director::getInstance()->getVisibleSize().height;
        setPosition(Vec2(0,-fheight));
        
        addChild(rootNode);
    }
    
    setAlbumPhotoSaveURL(SREVERURL_MANAGER->getUrl("ALBUM_PHOTO_SAVE"));
    createRenderingProgress();
}

void PixSmartalbumSaveLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixSmartalbumSaveLayer::update(float delta)
{
    PixModalLayer::update(delta);
    
    if(covertitle && Button_Save)
    {
        if(covertitle->getText() != nullptr && strlen(covertitle->getText()) > 0)
        {
            if(Button_Save)
            {
                Button_Save->setEnabled(true);
            }
            else
            {
                Button_Save->setEnabled(false);
            }
        }
    }
}

void PixSmartalbumSaveLayer::onExit()
{
    PixModalLayer::onExit();
}

void PixSmartalbumSaveLayer::uploadImageInfoVecClear()
{
    for (auto iter = uploadImageInfotVec.begin(); iter != uploadImageInfotVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    uploadImageInfotVec.clear();
}

PixUploadImageInfo* PixSmartalbumSaveLayer::getUploadImageInfoByPath(const std::string &path)
{
    for (auto iter = uploadImageInfotVec.begin(); iter != uploadImageInfotVec.end(); ++iter)
    {
        if((*iter)->getPath().compare(path) == 0)
        {
            return (*iter);
        }
    }
    
    return nullptr;
}

void PixSmartalbumSaveLayer::createPreview()
{
    smartMaskBookModel->createRenderTexture(CC_CALLBACK_1(PixSmartalbumSaveLayer::previewRenderTextureCallBack, this));
    
    int nCount = smartMaskBookModel->getSmartMaskPageModeCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixSmartMaskPageModel *pModel = smartMaskBookModel->getSmartMaskPageModel(i);
        albumImagesF1200Queue.push_back(pModel);
        
        PixSmartMaskCoverPageModel *pCover = dynamic_cast<PixSmartMaskCoverPageModel*>(pModel);
        
        if(pCover)
        {
            albumImagesH600Queue.push_back(pCover);
        }
    }
    
    nUploadPreviewCount = (int)albumImagesF1200Queue.size() + (int)albumImagesH600Queue.size();
    nUploadPreviewTotalCount = nUploadPreviewCount;
}

void PixSmartalbumSaveLayer::previewRenderTextureCallBack(PixSmartMaskPageModel *pModel)
{
    albumImageSave(pModel);
}

void PixSmartalbumSaveLayer::createUploadImages()
{
    uploadImageInfoVecClear();
    
    int nPageCount = smartMaskBookModel->getSmartMaskPageModeCount();

    int nSeq = 0;
    
    for(int i = 0; i < nPageCount; ++i)
    {
        PixSmartMaskPageModel *smartMaskPageModel = smartMaskBookModel->getSmartMaskPageModel(i);
        
        if(smartMaskPageModel)
        {
            int nImageCount = smartMaskPageModel->getSmartMaskImageModelCount();
            
            for(int j = 0; j < nImageCount; ++j)
            {
                PixSmartMaskImageModel *smartMaskImageModel =smartMaskPageModel->getSmartMaskImageModel(j);
                
                if(smartMaskImageModel)
                {
                    PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                    
                    if(smartImageModel)
                    {
                        bool isEqual = false;
                        
                        for (auto iter = uploadImageInfotVec.begin(); iter != uploadImageInfotVec.end(); ++iter)
                        {
                            if((*iter)->getPath().compare(smartImageModel->getPath()) == 0)
                            {
                                isEqual = true;
                                break;
                            }
                        }
                        
                        if(isEqual == false)
                        {
                            PixUploadImageInfo *uploadImageInfo = new PixUploadImageInfo();
                            uploadImageInfo->setPath(smartImageModel->getPath());
                            
                            if(smartMaskPageModel->getType() == ITEM_COVER)
                            {
                                uploadImageInfo->setCover(true);
                            }
                            else
                            {
                                uploadImageInfo->setCover(false);
                            }
                            
                            uploadImageInfo->setSeq(nSeq);
                            uploadImageInfo->setAlbumSeq(smartMaskPageModel->getIndex());
                            uploadImageInfotVec.push_back(uploadImageInfo);
                        }
                    }
                    
                    ++nSeq;
                }
            }
        }
    }
    
    nUploadImagesCount = (int)uploadImageInfotVec.size();
    nUploadImagesTotalCount = nUploadImagesCount;
    
    for(int i = 0; i < nUploadImagesCount; ++i)
    {
        std::string strPath = uploadImageInfotVec.at(i)->getPath();
        
        UPLOADIMAGE_MANAGER->imageCreate(strPath, 0, 0, IMAGECREATE_CALLBACK(PixSmartalbumSaveLayer::createUploadImageComplete, this));
    }
}

void PixSmartalbumSaveLayer::createUploadImageComplete(Data *data, const std::string &strUrl, const std::string &strWidth, const std::string &strHeight, const std::string &strDate)
{
    if(data == nullptr)
        return;
    
    std::string fileRender(data->getBytes(), data->getBytes() + data->getSize());
    
    CC_SAFE_DELETE(data);
    
    PixUploadImageInfo *pUserData = nullptr;
    
    pUserData = getUploadImageInfoByPath(strUrl);
    
    albumPhotoSave(fileRender, strWidth, strHeight, strDate, pUserData);
}

void PixSmartalbumSaveLayer::createFileRenderXML()
{
    mstr_FileRenderXML.clear();
    
    pugi::xml_document doc;
    
    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version") = "1.0";
  
    auto IpSmartMaskBook = doc.append_child("IpSmartMaskBook");
 
    std::string product_size = "";
    std::string product_code = "";
    std::string product_option = "";
    std::string product_id = "";
    
    PixDictionary *themeInfo = PIXAPP->userInfo->getSelectThemeInfo();
    
    if(themeInfo)
    {
        PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(themeInfo);
        
        if(leaf)
        {
            product_code = leaf->stringForKey("product_code");
            
            PixDictionary *item = SMARTALBUM_MANAGER->getItemDictionary(leaf);
            
            if(item)
            {
                product_size = item->stringForKey("title");
                product_option = item->stringForKey("cover");
                product_id = item->stringForKey("product_id");
            }
        }
    }
    
    IpSmartMaskBook.append_attribute("product_code") = product_code.c_str();
    IpSmartMaskBook.append_attribute("product_size") = product_size.c_str();
    IpSmartMaskBook.append_attribute("product_option") = product_option.c_str();
    IpSmartMaskBook.append_attribute("productid") = product_id.c_str();
    
    int nCount = smartMaskBookModel->getSmartMaskPageModeCount();
    
    for (int i = 0; i < nCount; ++i)
    {
        PixSmartMaskPageModel *model = smartMaskBookModel->getSmartMaskPageModel(i);
        
        if(model)
        {
            if(model->getType() == ITEM_COVER)
            {
                PixSmartMaskCoverPageModel *cover = (PixSmartMaskCoverPageModel*)model;
                
                auto IpSmartMaskCoverPage = IpSmartMaskBook.append_child("IpSmartMaskCoverPage");
                IpSmartMaskCoverPage.append_attribute("seneka_color") = cover->getSeneka_color().c_str();
                IpSmartMaskCoverPage.append_attribute("seneka_width") = cover->getSeneka_width().c_str();
                IpSmartMaskCoverPage.append_attribute("seneka_text") = cover->getSeneka_text().c_str();
                IpSmartMaskCoverPage.append_attribute("skin_res_id") = cover->getSkin_res_id().c_str();
                IpSmartMaskCoverPage.append_attribute("layout_res_id") = cover->getLayout_res_id().c_str();
                
                int nImageCount = cover->getSmartMaskImageModelCount();
                
                for (int j = 0; j < nImageCount; ++j)
                {
                    PixSmartMaskImageModel *smartMaskImageModel = cover->getSmartMaskImageModel(j);
                    
                    if(smartMaskImageModel)
                    {
                        auto IpSmartImage = IpSmartMaskCoverPage.append_child("IpSmartImage");
                        IpSmartImage.append_attribute("dimension_rotate") = smartMaskImageModel->getItem_rotate().c_str();
                        IpSmartImage.append_attribute("dimension_height") = smartMaskImageModel->getItem_height().c_str();
                        IpSmartImage.append_attribute("dimension_width") = smartMaskImageModel->getItem_width().c_str();
                        IpSmartImage.append_attribute("dimension_y") = smartMaskImageModel->getItem_y().c_str();
                        IpSmartImage.append_attribute("dimension_x") = smartMaskImageModel->getItem_x().c_str();
                        IpSmartImage.append_attribute("is_dimension_modify") = "0";
                        
                        PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                        
                        if(smartImageModel)
                        {
                            PixUploadImageInfo *pInfo = getUploadImageInfoByPath(smartImageModel->getPath());

                            if(pInfo)
                            {
                                std::string smart_image_id = "smartimg:" + pInfo->getSmart_image_id();
                                IpSmartImage.append_attribute("smart_image_id") = smart_image_id.c_str();
                                
                                smartMaskImageModel->setSmart_image_id(pInfo->getSmart_image_id());
                            }
                        }
                    }
                }
            }
            
            if(model->getType() == ITEM_PAGE)
            {
                PixSmartMaskPageModel *page = model;
            
                auto IpSmartMaskPage = IpSmartMaskBook.append_child("IpSmartMaskPage");
                IpSmartMaskPage.append_attribute("skin_res_id") = page->getSkin_res_id().c_str();
                IpSmartMaskPage.append_attribute("layout_res_id") = page->getLayout_res_id().c_str();
                
                int nImageCount = page->getSmartMaskImageModelCount();
                
                for (int j = 0; j < nImageCount; ++j)
                {
                    PixSmartMaskImageModel *smartMaskImageModel = page->getSmartMaskImageModel(j);
                    
                    if(smartMaskImageModel)
                    {
                        auto IpSmartImage = IpSmartMaskPage.append_child("IpSmartImage");
                        IpSmartImage.append_attribute("dimension_rotate") = smartMaskImageModel->getItem_rotate().c_str();
                        IpSmartImage.append_attribute("dimension_height") = smartMaskImageModel->getItem_height().c_str();
                        IpSmartImage.append_attribute("dimension_width") = smartMaskImageModel->getItem_width().c_str();
                        IpSmartImage.append_attribute("dimension_y") = smartMaskImageModel->getItem_y().c_str();
                        IpSmartImage.append_attribute("dimension_x") = smartMaskImageModel->getItem_x().c_str();
                        IpSmartImage.append_attribute("is_dimension_modify") = "0";
                        
                        PixSmartImageModel *smartImageModel = smartMaskImageModel->getSmartImageModel();
                        
                        if(smartImageModel)
                        {
                            PixUploadImageInfo *pInfo = getUploadImageInfoByPath(smartImageModel->getPath());
                            
                            if(pInfo)
                            {
                                std::string smart_image_id = "smartimg:" + pInfo->getSmart_image_id();
                                IpSmartImage.append_attribute("smart_image_id") = smart_image_id.c_str();
                                
                                smartMaskImageModel->setSmart_image_id(pInfo->getSmart_image_id());
                            }
                        }
                    }
                }
            }
        }
    }
    
    std::ostringstream stream;
    doc.save(stream);

    mstr_FileRenderXML = stream.str();
}

void PixSmartalbumSaveLayer::createRenderingProgress()
{
    renderingProgress = PixSaveProgressLayer::create(CC_CALLBACK_2(PixSmartalbumSaveLayer::buttonSaveCancelPressed, this));
    
    if(renderingProgress)
    {
        renderingProgress->retain();
    }
}

float PixSmartalbumSaveLayer::getRenderingProgressPercent()
{
    if(nUploadImagesTotalCount != 0 && nUploadPreviewTotalCount != 0)
    {
        int nPreviewCount = (int)albumImagesF1200Queue.size() + (int)albumImagesH600Queue.size();
        int nCount = (nUploadImagesTotalCount + nUploadPreviewTotalCount) - (nPreviewCount + nUploadImagesCount);
        float fPercent = (float)nCount / (float)(nUploadImagesTotalCount + nUploadPreviewTotalCount);
        
        return fPercent;  
    }
    
    return 0.0f;
}

void PixSmartalbumSaveLayer::updateImageUpload(const std::string &strMessage)
{
    if(renderingProgress)
    {
        renderingProgress->updateImageUpload(strMessage, getImageUploadPercent());
    }
}

float PixSmartalbumSaveLayer::getImageUploadPercent()
{
    if(nUploadImagesTotalCount != 0)
    {
        int nCount = nUploadImagesTotalCount - nUploadImagesCount;
        float fPercent = (float)nCount / (float)nUploadImagesTotalCount;
        
        return fPercent;
    }
    
    return 0.0f;
}

void PixSmartalbumSaveLayer::updatePreviewUpload(const std::string &strMessage)
{
    if(renderingProgress)
    {
        renderingProgress->updatePreviewUpload(strMessage, getPreviewUploadPercent());
    }
}

float PixSmartalbumSaveLayer::getPreviewUploadPercent()
{
    nUploadPreviewCount = (int)albumImagesF1200Queue.size() + (int)albumImagesH600Queue.size();
    int nCount = nUploadPreviewTotalCount - nUploadPreviewCount;

    float fPercent = (float)nCount / (float)nUploadPreviewTotalCount;
    
    return fPercent;
}

void PixSmartalbumSaveLayer::showRenderingProgress()
{
    PIXAPP->showRenderingProgress(renderingProgress);
}

void PixSmartalbumSaveLayer::resetRenderingProgress()
{
    if(renderingProgress)
    {
        renderingProgress->resetUpload();
    }
}

void PixSmartalbumSaveLayer::hideRenderingProgress()
{
    PIXAPP->hideRenderingProgress();
}

bool PixSmartalbumSaveLayer::checkCovertitle()
{
    if(covertitle->getText() == nullptr || strlen(covertitle->getText()) <= 0)
    {
        std::string msg = CODE_NAME("P43");
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        return false;
    }
    
    if(PixStringUtils::strlen_kr(covertitle->getText()) > covertitle->getMaxLength())
    {
        std::string msg = CODE_NAME("P44");;
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        return false;
    }
    
    bool blnValidate = true;
    
    std::u16string utf16;
    StringUtils::UTF8ToUTF16(covertitle->getText(), utf16);
   
    for (int i = 0; i < utf16.length(); i++)
    {
        unsigned ch = utf16[i];
        
        // 이모티콘 유니코드
        if((ch >= 0xD83C && ch <= 0xDBFF) || (ch >= 0xDC00 && ch <= 0xDFFF))
        {
            blnValidate = false;
            break;
        }
        
        // 숫자, 영문 이 아니먄
        if (!((ch > 47 && ch < 58) || (ch > 64 && ch < 91) || (ch > 96 && ch < 123)))
        {
            // 한,중,일, 스페이스,개행 문자 가 아니면
            if ( !((StringUtils::isCJKUnicode(ch)) || (StringUtils::isUnicodeSpace(ch)) || (ch == (char16_t)TextFormatter::NewLine)))
            {
                char16_t wcharString[2] = { (char16_t) ch, 0 };
                std::string utf8String;
                StringUtils::UTF16ToUTF8(wcharString, utf8String);
                
                // 입력가능한 기호가 아닐 경우
                if(PixNativeUtils::checkValidateString(utf8String) == false)
                {
                    blnValidate = false;
                    break;
                }
            }
        }
    }
    
    if(blnValidate == false)
    {
        std::string msg = CODE_NAME("P61");;
        PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
        return false;
    }
    
    return true;
}

bool PixSmartalbumSaveLayer::checkAlbumSaveDone()
{
    if(nUploadImagesCount == 0)
    {
        if(albumImagesH600Queue.empty() && albumImagesF1200Queue.empty())
        {
            return true;
        }
    }
    
    return false;
}

#pragma mark - Buttonr

void PixSmartalbumSaveLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixSmartalbumSaveLayer::buttonSavePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(checkCovertitle())
        {
            if(smartMaskBookModel)
            {
                showRenderingProgress();
             
                updateImageUpload(CODE_NAME("Save04"));
                updatePreviewUpload(CODE_NAME("Save06"));
                
                Director::getInstance()->getTextureCache()->removeAllTextures();
                IMAGECACHE_MANAGER->removeAllImageModel();
                
                albumBasicSave();
            }
        }
    }
}

void PixSmartalbumSaveLayer::buttonSaveCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        std::string msg = CODE_NAME("P54");
        PixAlert *alert = PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixSmartalbumSaveLayer::albumSaveDoneError), false);
        
        alert->setTag(SVAE_CENCEL_ALERT_TAG);
    }
}

#pragma mark - EditBox

void PixSmartalbumSaveLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
   
}

void PixSmartalbumSaveLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
   
}

void PixSmartalbumSaveLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    
}

void PixSmartalbumSaveLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Action

void PixSmartalbumSaveLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSmartalbumSaveLayer::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
}

void PixSmartalbumSaveLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    float fheight = Director::getInstance()->getVisibleSize().height;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-fheight));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSmartalbumSaveLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
}

void PixSmartalbumSaveLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
}

void PixSmartalbumSaveLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Network

void PixSmartalbumSaveLayer::albumBasicSave()
{
    if(smartMaskBookModel == nullptr)
        return;
    
//    theme_id	테마 ID	필수
//    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
//    item_id	아이템 ID	필수
//    vendor	벤더명	필수
//    title	앨범 제목	필수
//    total	앨범 페이지 수	필수
//    contents	앨범 상세 설명
//    tag	태그 (쉼표[,] 로 구분)
//    skin_group_id 스킨 그룹 ID 필수
    
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_BASIC_SAVE");
    std::map<std::string, std::string> postParams;
    
    std::string theme_id = "";
    std::string member_id = PIXAPP->userInfo->getMember_id();
    std::string item_id = "";
    std::string vendor = PIXAPP->userInfo->getVendor();
    std::string title = covertitle->getText();
    std::string total = StringUtils::format("%d",smartMaskBookModel->getSmartMaskPageModeCount());
    std::string contents = "";
    std::string tag = "";
    std::string skin_group_id = StringUtils::format("%d",smartMaskBookModel->getSkin_group_id());
    
    PixDictionary *themeInfo = PIXAPP->userInfo->getSelectThemeInfo();
    
    if(themeInfo)
    {
        PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(themeInfo);
        
        if(leaf)
        {
            theme_id = leaf->stringForKey("theme_id");
            
            PixDictionary *item = SMARTALBUM_MANAGER->getItemDictionary(leaf);
            
            if(item)
            {
                item_id = item->stringForKey("item_id");
            }
        }
    }
    
    postParams["theme_id"] = theme_id;
    postParams["member_id"] = member_id;
    postParams["item_id"] = item_id;
    postParams["vendor"] = vendor;
    postParams["title"] = title;
    postParams["total"] = total;
    postParams["contents"] = contents;
    postParams["tag"] = tag;
    postParams["skin_group_id"] = skin_group_id;
    
    pixHttpClient->getAES256EncryptPostParams(postParams);

    pixHttpClient->getFormData(url, postParams, "ALBUM_BASIC_SAVE", this, httpresponse_selector(PixSmartalbumSaveLayer::albumBasicSaveCallback), httpresponse_selector(PixSmartalbumSaveLayer::albumBasicSaveError), nullptr, SEND_IMMEDIATE);
}

void PixSmartalbumSaveLayer::albumPhotoSave(const std::string &fileRender, const std::string &strWidth, const std::string &strHeight, const std::string &strDate,  PixUploadImageInfo* pInfo)
{
    std::string url = getAlbumPhotoSaveURL();

    std::map<std::string, std::string> postParams;
    
    //    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
    //    album_id	앨범 ID	필수
    //    seq	이미지(앨범) 순서 	필수
    //    vendor	벤더명	필수
    //    width	원본 이미지 가로 사이즈 (px)	필수
    //    height	원본 이미지 세로 사이즈 (px)	필수
    //    file_create_date	파일생성일시 (yyyy-mm-dd hh:mi:ss)	필수
    //    is_cover	커버 사용 여부 (사용=1 미사용=0)	필수
    //    file	앨범 이미지	필수

    std::string file = fileRender;
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["album_id"] = StringUtils::format("%d",getAlbum_id());
    postParams["seq"] = StringUtils::format("%d",pInfo->getSeq());
    postParams["album_seq"] = StringUtils::format("%d",pInfo->getAlbumSeq());
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["width"] = strWidth;
    postParams["height"] = strHeight;
    postParams["file_create_date"] = strDate;
    postParams["is_cover"] = (pInfo->getCover() == true) ?  "1" : "0";
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    file = pixHttpClient->getBase64String(file);
    file = PixStringUtils::replaceAll(file,"+",".");
    file = PixStringUtils::replaceAll(file,"=","-");
    file = PixStringUtils::replaceAll(file,"/","_");
    postParams["file"] = file;
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_PHOTO_SAVE", this, httpresponse_selector(PixSmartalbumSaveLayer::albumPhotoSaveCallback), httpresponse_selector(PixSmartalbumSaveLayer::albumPhotoSaveError), pInfo, SEND_IMMEDIATE);
}

void PixSmartalbumSaveLayer::albumImageSave(PixSmartMaskPageModel *pModel)
{
    if(pModel)
    {
        if(pModel->getType() == ITEM_COVER)
        {
            PixSmartMaskCoverPageModel *pCover = dynamic_cast<PixSmartMaskCoverPageModel*>(pModel);
            
            if(pCover)
            {
                std::string url = SREVERURL_MANAGER->getUrl("ALBUM_IMAGE_SAVE");
                std::map<std::string, std::string> postParams;
                
                //    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
                //    album_id	앨범 ID	필수
                //    image_type	이미지 형태 (F1200 [1170x585)], H600[585x585])	필수
                //    seq	이미지(앨범) 순서 	필수
                //    vendor	벤더명	필수
                //    file	앨범 이미지	필수
                
                std::string member_id = PIXAPP->userInfo->getMember_id();
                std::string album_id = StringUtils::format("%d",getAlbum_id());
                std::string image_type = "";
                std::string seq = "";
                std::string vendor = PIXAPP->userInfo->getVendor();
                std::string file = "";
                
                PixSmartMaskPageModel *pUserData = pCover;
                
                image_type = "H600";
                seq =StringUtils::format("%d",pCover->getIndex());
                
                if(pCover->isH600Data())
                {
                    Data H600Data = pCover->getH600Data();
                    file = std::string(H600Data.getBytes(), H600Data.getBytes() + H600Data.getSize());
                }
                
                postParams["member_id"] = member_id;
                postParams["album_id"] = album_id;
                postParams["image_type"] = image_type;
                postParams["vendor"] = vendor;
                postParams["seq"] = seq;
                
                pixHttpClient->getAES256EncryptPostParams(postParams);
                
                file = pixHttpClient->getBase64String(file);
                file = PixStringUtils::replaceAll(file,"+",".");
                file = PixStringUtils::replaceAll(file,"=","-");
                file = PixStringUtils::replaceAll(file,"/","_");
                postParams["file"] = file;
                
                pixHttpClient->getFormData(url, postParams, image_type, this, httpresponse_selector(PixSmartalbumSaveLayer::albumImageSaveCallback), httpresponse_selector(PixSmartalbumSaveLayer::albumImageSaveError), pUserData, SEND_IMMEDIATE);
            }
        }
        
        std::string url = SREVERURL_MANAGER->getUrl("ALBUM_IMAGE_SAVE");
        std::map<std::string, std::string> postParams;
        
        //    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
        //    album_id	앨범 ID	필수
        //    image_type	이미지 형태 (F1200 [1170x585)], H600[585x585])	필수
        //    seq	이미지(앨범) 순서 	필수
        //    vendor	벤더명	필수
        //    file	앨범 이미지	필수
        
        std::string member_id = PIXAPP->userInfo->getMember_id();
        std::string album_id = StringUtils::format("%d",getAlbum_id());
        std::string image_type = "";
        std::string seq = "";
        std::string vendor = PIXAPP->userInfo->getVendor();
        std::string file = "";
        
        PixSmartMaskPageModel *pUserData = pModel;
        
        image_type = "F1200";
        seq =StringUtils::format("%d",pModel->getIndex());
        
        if(pModel->isF1200Data())
        {
            Data F1200Data = pModel->getF1200Data();
            file = std::string(F1200Data.getBytes(), F1200Data.getBytes() + F1200Data.getSize());
        }
        
        postParams["member_id"] = member_id;
        postParams["album_id"] = album_id;
        postParams["image_type"] = image_type;
        postParams["vendor"] = vendor;
        postParams["seq"] = seq;
        
        pixHttpClient->getAES256EncryptPostParams(postParams);
        
        file = pixHttpClient->getBase64String(file);
        file = PixStringUtils::replaceAll(file,"+",".");
        file = PixStringUtils::replaceAll(file,"=","-");
        file = PixStringUtils::replaceAll(file,"/","_");
        postParams["file"] = file;
        
        pixHttpClient->getFormData(url, postParams, image_type, this, httpresponse_selector(PixSmartalbumSaveLayer::albumImageSaveCallback), httpresponse_selector(PixSmartalbumSaveLayer::albumImageSaveError), pUserData, SEND_IMMEDIATE);
    }
}

void PixSmartalbumSaveLayer::albumSaveDone()
{
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_SAVE_DONE");
    std::map<std::string, std::string> postParams;
    
//    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
//    album_id	앨범 ID	필수
//    xml	xml	필수
//    vendor	벤더명	필수
    
    // xml 생성
    createFileRenderXML();
    
    std::string strFileRenderXML =  mstr_FileRenderXML;
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["album_id"] =  StringUtils::format("%d",getAlbum_id());
    postParams["xml"] = strFileRenderXML;
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    
    std::string eventAction =StringUtils::format("album_save_done : %s", PIXAPP->userInfo->getUserID().c_str());
    GOOGLEANALYTICS_MANAGER->logEvent("smart", eventAction, strFileRenderXML, 1);
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_SAVE_DONE", this, httpresponse_selector(PixSmartalbumSaveLayer::albumSaveDoneCallback), httpresponse_selector(PixSmartalbumSaveLayer::albumSaveDoneError), nullptr, SEND_IMMEDIATE);
}

void PixSmartalbumSaveLayer::albumBasicSaveCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        setAlbum_id(root->intForKey("album_id"));
        
        createUploadImages();
        createPreview();
    }
}

void PixSmartalbumSaveLayer::albumPhotoSaveCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        std::string image_id = root->stringForKey("image_id");
        
        PixUploadImageInfo *info =static_cast<PixUploadImageInfo*>(response->getHttpRequest()->getUserData());
        
        if(info)
        {
            info->setSmart_image_id(image_id);
        }
        
        --nUploadImagesCount;
        
        int nCount = nUploadImagesTotalCount - nUploadImagesCount;
        
        if(nCount == nUploadImagesTotalCount)
        {
            nCount = nUploadImagesTotalCount;
        }
        
        std::string msg = StringUtils::format(CODE_NAME("Save05"),nCount,nUploadImagesTotalCount);
        
        updateImageUpload(msg.c_str());
        
        if(checkAlbumSaveDone())
        {
            auto scheduler = cocos2d::Director::getInstance()->getScheduler();
            scheduler->performFunctionInCocosThread([=](void) -> void {
                
                albumSaveDone();
                
            });
        }
    }
}

void PixSmartalbumSaveLayer::albumImageSaveCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if (strcmp(response->getHttpRequest()->getTag(), "H600") == 0)
        {
            PixSmartMaskCoverPageModel *pCover = static_cast<PixSmartMaskCoverPageModel*>(response->getHttpRequest()->getUserData());
            
            if(pCover)
            {
                for(auto iter = albumImagesH600Queue.begin(); iter != albumImagesH600Queue.end(); ++iter)
                {
                    if((*iter) == pCover)
                    {
                        albumImagesH600Queue.erase(iter);
                        break;
                    }
                }
            }
        }
        else if(strcmp(response->getHttpRequest()->getTag(), "F1200") == 0)
        {
            PixSmartMaskPageModel *pModel = static_cast<PixSmartMaskPageModel*>(response->getHttpRequest()->getUserData());
            
            for(auto iter = albumImagesF1200Queue.begin(); iter != albumImagesF1200Queue.end(); ++iter)
            {
                if((*iter) == pModel)
                {
                    albumImagesF1200Queue.erase(iter);
                    break;
                }
            }
        }
        
        if(checkAlbumSaveDone())
        {
            albumSaveDone();
        }
    }
    
    float fPercent = getPreviewUploadPercent() * 100.0f;
    std::string msg = StringUtils::format(CODE_NAME("Save07"),(int)fPercent);
    updatePreviewUpload(msg.c_str());
}

void PixSmartalbumSaveLayer::albumSaveDoneCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        int nCode = root->intForKey("code");
        
        if(nCode == 2505)
        {
            if(mn_SaveDoneRetry == 0)
            {
                std::string msg = root->stringForKey("error").c_str();
                PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK, 0, this, callfunc_selector(PixSmartalbumSaveLayer::albumSaveDoneError));
            }
            else
            {
                --mn_SaveDoneRetry;
                
                auto scheduler = cocos2d::Director::getInstance()->getScheduler();
                scheduler->performFunctionInCocosThread([=](void) -> void {
                    
                    albumSaveDone();
                    
                });
            }
            
            return;
        }
        
        std::string album_id = root->stringForKey("album_id");
        smartMaskBookModel->setAlbum_id(album_id);
                
        uploadImageInfoVecClear();
        hideRenderingProgress();
        
        MYALBUM_MANAGER->setReferrer(1);
        
        if(Director::getInstance()->getRunningScene()->getChildByTag(SVAE_CENCEL_ALERT_TAG))
        {
            Node *alert =  Director::getInstance()->getRunningScene()->getChildByTag(SVAE_CENCEL_ALERT_TAG);
            alert->removeFromParentAndCleanup(true);
        }
        

        PIXAPP->goLevel(0, PT_MYALBUM, TR_FADE);
    }
}

void PixSmartalbumSaveLayer::albumBasicSaveError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    albumSaveDoneError();
}

void PixSmartalbumSaveLayer::albumPhotoSaveError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    albumSaveDoneError();
}

void PixSmartalbumSaveLayer::albumImageSaveError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    albumSaveDoneError();
}

void PixSmartalbumSaveLayer::albumSaveDoneError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    albumSaveDoneError();
}

void PixSmartalbumSaveLayer::albumSaveDoneError()
{
    for(int i = 0; i < uploadImageInfotVec.size(); ++i)
    {
        std::string strPath = uploadImageInfotVec.at(i)->getPath();
        UPLOADIMAGE_MANAGER->unbindImage(strPath);
    }
    
    UPLOADIMAGE_MANAGER->clear();
    
    uploadImageInfoVecClear();
    hideRenderingProgress();
    
    nUploadImagesCount = 0;
    nUploadImagesTotalCount = 0;
    nUploadPreviewCount = 0;
    nUploadPreviewTotalCount = 0;
    
    albumImagesH600Queue.clear();
    albumImagesF1200Queue.clear();
    
    smartMaskBookModel->cancelRenderTexture();
    
    pixHttpClient->cancelRequest(this);
//    pixHttpClient->destroyInstance();
    pixHttpClient->setTimeoutForConnect(60 * 3);
}

#pragma mark - Android BackPressed

void PixSmartalbumSaveLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}
