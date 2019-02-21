//
//  PixMyAlbumCorrectLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 21..
//
//

#include "PixMyAlbumCorrectLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../UIControl/PixAlert.h"
#include "../Util/PixStringUtils.h"
#include "../Model/MyAlbumModel.h"
#include "../Scene/PixApp.h"
#include "../Util/PixUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Manager/PixMyAlbumManager.h"
#include "../Common.h"

PixMyAlbumCorrectLayer* PixMyAlbumCorrectLayer::create(MyAlbumModel *pModel)
{
    PixMyAlbumCorrectLayer *myAlbumCorrectLayer = new (std::nothrow) PixMyAlbumCorrectLayer(pModel);
    if (myAlbumCorrectLayer && myAlbumCorrectLayer->init())
    {
        myAlbumCorrectLayer->autorelease();
        return myAlbumCorrectLayer;
    }
    else
    {
        CC_SAFE_DELETE(myAlbumCorrectLayer);
        return nullptr;
    }
}

PixMyAlbumCorrectLayer::PixMyAlbumCorrectLayer(MyAlbumModel *pModel)
: pixHttpClient(nullptr)
, rootNode(nullptr)
, title(nullptr)
, MyAlbumCorrectTopLayer(nullptr)
, btn_back(nullptr)
, Button_Save(nullptr)
, mstr_Title("")
, Background_title(nullptr)
, Background_title_on(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
, mMyAlbumModel(nullptr)
{
    this->mMyAlbumModel = pModel;
    
    if(mMyAlbumModel)
    {
        mstr_Title = mMyAlbumModel->getTitle();
    }
    
    pixHttpClient = PixHttpClient::sharedObject();
    
    hideLayerFunction = std::bind(&PixMyAlbumCorrectLayer::hideLayerAction,this);
}

PixMyAlbumCorrectLayer::~PixMyAlbumCorrectLayer()
{
    pixHttpClient->cancelRequest(this);
}

void PixMyAlbumCorrectLayer::initLayer()
{
    rootNode = CSLoader::createNode("07_My_AlbumList/MyAlbumCorrectLayer.csb");
    
    if(rootNode)
    {
        ui::TextField *textField = dynamic_cast<ui::TextField*>(rootNode->getChildByName("covertitle"));
        
        if(textField)
        {
            title = PixEditBox::convertToEditBox(textField, this);
            title->setInputMode(ui::EditBox::InputMode::ANY);
            title->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
            title->setAlignmentCenter();
            title->setNumberOfLines(2);
            title->setText(getTitle().c_str());
        }
        
        Background_title = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_title"));
        
        if(Background_title)
        {
            Background_title->setVisible(true);
        }
        
        Background_title_on = dynamic_cast<Sprite*>(rootNode->getChildByName("Background_title_on"));
        
        if(Background_title_on)
        {
            Background_title_on->setVisible(false);
        }

        MyAlbumCorrectTopLayer = dynamic_cast<Node*>(rootNode->getChildByName("MyAlbumCorrectTopLayer"));
        
        if(MyAlbumCorrectTopLayer)
        {
            ui::Text *title = dynamic_cast<ui::Text*>(MyAlbumCorrectTopLayer->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            btn_back = dynamic_cast<ui::Button*>(MyAlbumCorrectTopLayer->getChildByName("btn_back"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumCorrectLayer::buttonBackPressed, this));
            }
        }
        
        Button_Save = dynamic_cast<ui::Button*>(rootNode->getChildByName("Button_Save"));
        
        if(Button_Save)
        {
            Button_Save->addTouchEventListener(CC_CALLBACK_2(PixMyAlbumCorrectLayer::buttonSavePressed, this));
            Button_Save->setEnabled(false);
            
            ui::Text *text = dynamic_cast<ui::Text*>(Button_Save->getChildByName("Text_Save"));
            
            if(text)
            {
                text->setString(CODE_NAME(text->getString()));
            }
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2(0,-size.height));
        addChild(rootNode);
    }
}

void PixMyAlbumCorrectLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showLayerAction();
}

void PixMyAlbumCorrectLayer::update(float delta)
{
    PixModalLayer::update(delta);
    
    if(title && Button_Save)
    {
        if(title->getText() != nullptr && strlen(title->getText()) > 0)
        {
            Button_Save->setEnabled(true);
        }
        else
        {
            Button_Save->setEnabled(false);
        }
    }
}

#pragma mark - Action

void PixMyAlbumCorrectLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMyAlbumCorrectLayer::showLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixMyAlbumCorrectLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixMyAlbumCorrectLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixMyAlbumCorrectLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixMyAlbumCorrectLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionFinish(this);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixMyAlbumCorrectLayer::buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
    }
}

void PixMyAlbumCorrectLayer::buttonSavePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(title->getText() == nullptr || strlen(title->getText()) <= 0)
        {
            std::string msg = CODE_NAME("P35");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
            return;
        }
        
        if(PixStringUtils::strlen_kr(title->getText()) > title->getMaxLength())
        {
            std::string msg = CODE_NAME("P44");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_OK);
            return;
        }
        
        bool blnValidate = true;
        
        std::u16string utf16;
        StringUtils::UTF8ToUTF16(title->getText(), utf16);
        
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
            return;
        }
        
        if(strcmp(getTitle().c_str(), title->getText()) == 0)
        {
            hideLayerAction();
        }
        else
        {
            albumModify(title->getText());
        }
    }
}

#pragma mark - EditBox

void PixMyAlbumCorrectLayer::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    if(title == editBox)
    {
        if(Background_title)
        {
            Background_title->setVisible(false);
        }
        
        if(Background_title_on)
        {
            Background_title_on->setVisible(true);
        }
    }
}

void PixMyAlbumCorrectLayer::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    if(title == editBox)
    {
        if(Background_title)
        {
            Background_title->setVisible(true);
        }
        
        if(Background_title_on)
        {
            Background_title_on->setVisible(false);
        }
    }
}

void PixMyAlbumCorrectLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
//    int count = 0;
//    char newLine = '\n';
//    
//    int len = static_cast<int>(text.size());
//    
//    int i = len - 1;
//    for (; i >= 0; --i)
//    {
//        if (text[i] == newLine)
//        {
//            ++count;
//        }
//    }
//    
//    int maxNumberOfLines = 1;
//    
//    if(count > maxNumberOfLines)
//    {
//        editBox->setText(text.substr(0, text.size() - 1).c_str());
//    }
}

void PixMyAlbumCorrectLayer::editBoxReturn(ui::EditBox* editBox)
{
    
}

#pragma mark - Network

void PixMyAlbumCorrectLayer::albumModify(const std::string &strRename)
{
    if(mMyAlbumModel == nullptr)
        return;
    
    if(pixHttpClient == nullptr)
        return;
    
    PIXAPP->showLoading();
    
    std::string url = SREVERURL_MANAGER->getUrl("ALBUM_MODIFY_NEW");
    std::map<std::string, std::string> postParams;
    
//    member_id	사용자 유니크 ID (로그인시 리턴 받는 결과)	필수
//    vendor	벤더명	필수
//    album_id	페이지 수	필수
//    title	앨범 제목	필수
    
    postParams["member_id"] = PIXAPP->userInfo->getMember_id();
    postParams["vendor"] = PIXAPP->userInfo->getVendor();
    postParams["album_id"] = StringUtils::format("%d",mMyAlbumModel->getAlbum_id());
    postParams["title"] = strRename;
    
    pixHttpClient->getAES256EncryptPostParams(postParams);
    
    pixHttpClient->getFormData(url, postParams, "ALBUM_MODIFY",this, httpresponse_selector(PixMyAlbumCorrectLayer::albumModifyCallback), nullptr);

}

void PixMyAlbumCorrectLayer::albumModifyCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(mMyAlbumModel)
        {
            int album_id = root->intForKey("album_id");
            
            if(mMyAlbumModel->getAlbum_id() == album_id)
            {
                /*
                 mMyAlbumModel 은 clone 데이타
                 MYALBUM_MANAGER->getMyAlbumModelByID 통해 업데이트
                 */
                
                mMyAlbumModel->setTitle(title->getText());
                
                MyAlbumModel *pModel = MYALBUM_MANAGER->getMyAlbumModelByID(mMyAlbumModel->getAlbum_id());
                
                if(pModel)
                {
                    pModel->setTitle(title->getText());
                }
                
                hideLayerAction();
                
                GOOGLEANALYTICS_MANAGER->logEvent("smart", "albums_action_rename", PIXAPP->userInfo->getUserID(), 1);
            }
        }
    }
}

#pragma mark - Android BackPressed

void PixMyAlbumCorrectLayer::androidBackPressed()
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
