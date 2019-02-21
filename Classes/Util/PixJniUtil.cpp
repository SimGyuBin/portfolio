//
//  PixJniUtil.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 28..
//
//

#include "PixJniUtil.h"
#include "PixNativeUtils.h"
#include "../Manager/PixImageCreateManager.h"
#include "../Manager/PixUploadImageManager.h"
#include "../Manager/PixFacebookManager.h"
#include "../Manager/PixAccountsGoogleManager.h"
#include "../Manager/PixKakaoManager.h"
#include "../Manager/PixOneSignalManager.h"
#include "../Info/UserInfo.h"
#include "../Scene/PixApp.h"
#include "../Util/PixNativeUtils.h"
#include "../Network/PixHttpClient.h"

// Java -> C++

#ifdef __cplusplus
extern "C" {
#endif
    
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackPathOfAllImages(JNIEnv *env, jclass cls, jstring path, jstring date_modified, jstring date_added, jstring fulldate, jstring size, jstring width, jstring height, jstring category_path, jstring category_name)
{
    std::string strPath = "";
    std::string strDate_modified = "";
    std::string strDate_added = "";
    std::string strFullDate = "";
    std::string strSize = "";
    std::string strWidth = "";
    std::string strHeight = "";
    std::string strCategoryPath = "";
    std::string strCategoryName = "";

    if(path != NULL)
    	strPath = env->GetStringUTFChars(path, NULL);

    if(date_modified != NULL)
    	strDate_modified = env->GetStringUTFChars(date_modified, NULL);

    if(date_added != NULL)
    	strDate_added = env->GetStringUTFChars(date_added, NULL);

    if(fulldate != NULL)
    	strFullDate = env->GetStringUTFChars(fulldate, NULL);

    if(size != NULL)
    	strSize = env->GetStringUTFChars(size, NULL);

    if(width != NULL)
    	strWidth = env->GetStringUTFChars(width, NULL);

    if(height != NULL)
    	strHeight = env->GetStringUTFChars(height, NULL);

    if(category_path != NULL)
    	strCategoryPath = env->GetStringUTFChars(category_path, NULL);

    if(category_name != NULL)
        strCategoryName = env->GetStringUTFChars(category_name, NULL);

    LocalImageInfo info;
    info.setPath(strPath.c_str());
    info.setDateModified(strDate_modified.c_str());
    info.setDateAdded(strDate_added.c_str());
    info.setFullDate(strFullDate.c_str());
    info.setFileSize(strSize.c_str());
    info.setWidth(strWidth);
    info.setHeight(strHeight);
    info.setOrientation(0);
    info.setThumbnailData(nullptr);
    info.setFullScreenData(nullptr);

    info.setCategoryInfo(strCategoryPath, strCategoryName);

    IMAGECREATE_MANAGER->setLocalImages(info);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackPathOfAllImagesComplete(JNIEnv *env, jclass cls)
{
	PixNativeUtils::nativeCallPathOfAllImagesComplete();
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackGetThumbnailImage(JNIEnv *env, jclass cls, jbyteArray javaBytes, jint length, jstring path, jstring width, jstring height)
{
	int len = env->GetArrayLength (javaBytes);

	unsigned char *buf = (unsigned char *)malloc(sizeof(unsigned char)*len);
	env->GetByteArrayRegion (javaBytes, 0, len, reinterpret_cast<jbyte*>(buf));

//	size_t len = env->GetArrayLength(javaBytes);
//	jbyte *nativeBytes = env->GetByteArrayElements(javaBytes, 0);
//	unsigned char *buff = (unsigned char *)malloc(length+1);
//	memcpy(buff, nativeBytes, len);
//	env->ReleaseByteArrayElements(javaBytes, nativeBytes, JNI_ABORT);

	 Data *thumbnaildata = new Data;
     thumbnaildata->fastSet(buf, len);

     std::string strPath = "";
     std::string strWidth = "";
     std::string strHeight = "";

     if(path != NULL)
    	 strPath = env->GetStringUTFChars(path, NULL);

     if(width != NULL)
    	 strWidth = env->GetStringUTFChars(width, NULL);

     if(height != NULL)
    	 strHeight = env->GetStringUTFChars(height, NULL);

     IMAGECREATE_MANAGER->addThumbnailData(strPath, thumbnaildata, strWidth, strHeight);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackGetFullScreenImage(JNIEnv *env, jclass cls, jbyteArray javaBytes, jint length, jstring path, jstring width, jstring height)
{
	int len = env->GetArrayLength (javaBytes);
	unsigned char *buf = (unsigned char *)malloc(sizeof(unsigned char)*len);
	env->GetByteArrayRegion (javaBytes, 0, len, reinterpret_cast<jbyte*>(buf));

//	size_t len = env->GetArrayLength(javaBytes);
//	jbyte *nativeBytes = env->GetByteArrayElements(javaBytes, 0);
//	unsigned char *buff = (unsigned char *)malloc(length+1);
//	memcpy(buff, nativeBytes, len);
//	env->ReleaseByteArrayElements(javaBytes, nativeBytes, JNI_ABORT);

	 Data *originaldata = new Data;
	 originaldata->fastSet(buf, len);

     std::string strPath = "";
     std::string strWidth = "";
     std::string strHeight = "";

     if(path != NULL)
    	 strPath = env->GetStringUTFChars(path, NULL);

     if(width != NULL)
    	 strWidth = env->GetStringUTFChars(width, NULL);

     if(height != NULL)
    	 strHeight = env->GetStringUTFChars(height, NULL);

     IMAGECREATE_MANAGER->addFullScreenData(strPath, originaldata, strWidth, strHeight);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackGetUploadImage(JNIEnv *env, jclass cls, jbyteArray javaBytes, jint length, jstring path, jstring width, jstring height, jstring date)
{
	int len = env->GetArrayLength (javaBytes);
	unsigned char *buf = (unsigned char *)malloc(sizeof(unsigned char)*len);
	env->GetByteArrayRegion (javaBytes, 0, len, reinterpret_cast<jbyte*>(buf));

//	size_t len = env->GetArrayLength(javaBytes);
//	jbyte *nativeBytes = env->GetByteArrayElements(javaBytes, 0);
//	unsigned char *buff = (unsigned char *)malloc(length+1);
//	memcpy(buff, nativeBytes, len);
//	env->ReleaseByteArrayElements(javaBytes, nativeBytes, JNI_ABORT);

	 Data *originaldata = new Data;
	 originaldata->fastSet(buf, len);

     std::string strPath = "";
     std::string strWidth = "";
     std::string strHeight = "";
     std::string strDate = "";

     if(path != NULL)
    	 strPath = env->GetStringUTFChars(path, NULL);

     if(width != NULL)
    	 strWidth = env->GetStringUTFChars(width, NULL);

     if(height != NULL)
    	 strHeight = env->GetStringUTFChars(height, NULL);

     if(date != NULL)
    	 strDate = env->GetStringUTFChars(date, NULL);

     UPLOADIMAGE_MANAGER->uploadImageComplete(originaldata, strPath, strWidth, strHeight, strDate);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackKeyCodeBack(JNIEnv *env, jclass cls)
{

}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookLogin(JNIEnv *env, jclass cls, jboolean isLogin, jstring msg)
{    
    std::string strMsg = "";
    
    if(msg != NULL)
        strMsg = env->GetStringUTFChars(msg, NULL);

    FACEBOOK_MANAGER->onLogin(isLogin, strMsg);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookLoginCheck(JNIEnv *env, jclass cls, jboolean isLogin)
{
        
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookMyInfo(JNIEnv *env, jclass cls, jstring data)
{
	std::string strData = "";

	if(data != NULL)
		strData = env->GetStringUTFChars(data, NULL);

	if(!strData.empty())
	{
		 rapidjson::Document document;
		 document.Parse<0>(strData.c_str());

		 CCLOG("getDictionaryFromElement %s", strData.c_str());

		 if (document.HasParseError() == rapidjson::kParseErrorNone)
		 {
			 PixDictionary *root =  PixHttpClient::sharedObject()->getDictionary(document);

			 if(root)
			 {
				 std::map<std::string, std::string> userInfo;

				 std::string strId = root->stringForKey("id");
				 std::string strEmail = root->stringForKey("email");
				 std::string strBirthday = root->stringForKey("birthday");
				 std::string strGender = root->stringForKey("gender");
				 std::string strLink = root->stringForKey("link");
				 std::string strLast_name = root->stringForKey("last_name");
				 std::string strFirst_name = root->stringForKey("first_name");
                 std::string strName = root->stringForKey("name");

				 userInfo["id"] = strId;
				 userInfo["email"] = strEmail;
				 userInfo["birthday"] = strBirthday;
				 userInfo["gender"] = strGender;
				 userInfo["link"] = strLink;
				 userInfo["name"] = strName;

				 CCLOG("getDictionaryFromElement id : %s", strId.c_str());
				 CCLOG("getDictionaryFromElement email :%s", strEmail.c_str());
				 CCLOG("getDictionaryFromElement birthday : %s", strBirthday.c_str());
				 CCLOG("getDictionaryFromElement gender : %s", strGender.c_str());
				 CCLOG("getDictionaryFromElement link :%s", strLink.c_str());
				 CCLOG("getDictionaryFromElement last_name :%s", strLast_name.c_str());
				 CCLOG("getDictionaryFromElement first_name :%s", strFirst_name.c_str());

				 FACEBOOK_MANAGER->onGetUserInfo(userInfo);
			 }
		 }
	}
}
    
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookUserAlbums(JNIEnv *env, jclass cls, jstring data)
{
    std::string strData = "";
    
    if(data != NULL)
        strData = env->GetStringUTFChars(data, NULL);
    
    if(!strData.empty())
    {
        FACEBOOK_MANAGER->onGetUserAlbums(true, strData);
    }
    else
    {
        FACEBOOK_MANAGER->onGetUserAlbums(false, "");
    }
}
    
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookUserImages(JNIEnv *env, jclass cls, jstring albumID, jstring data)
{
    std::string strAlbumID = "";
    std::string strData = "";
    
    if(albumID != NULL)
        strAlbumID = env->GetStringUTFChars(albumID, NULL);
    
    if(data != NULL)
        strData = env->GetStringUTFChars(data, NULL);
    
    if(!strAlbumID.empty() &&!strData.empty())
    {
        FACEBOOK_MANAGER->onGetUserImages(true, strAlbumID, strData);
    }
    else
    {
        FACEBOOK_MANAGER->onGetUserImages(false, "", "");
    }
}
    
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackPurchaseLayerloadURL(JNIEnv *env, jclass cls, jstring url)
{
    std::string strUrl = "";
    
    if(url != NULL)
        strUrl = env->GetStringUTFChars(url, NULL);
    
    
    if(!strUrl.empty())
    {
        PIXAPP->purchaseLayerloadURL(strUrl);
    }
    else
    {
        
    }
}
    
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackGoogleSignInSuccess(JNIEnv *env, jclass cls, jstring displayName, jstring Id, jstring Email, jstring token)
{
    std::string strDisplayName = "";
    std::string strId = "";
    std::string strEmail = "";
    std::string strToken = "";
    
    if(displayName != NULL)
        strDisplayName = env->GetStringUTFChars(displayName, NULL);
    
    if(Id != NULL)
        strId = env->GetStringUTFChars(Id, NULL);
    
    if(Email != NULL)
        strEmail = env->GetStringUTFChars(Email, NULL);
    
    if(token != NULL)
        strToken = env->GetStringUTFChars(token, NULL);
    
    ACCOUNTS_GOOGLE_MANAGER->androidGoogleSignInSuccess(strDisplayName, strId, strEmail, strToken);
  
}
   
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackKakaoLoginSuccess(JNIEnv *env, jclass cls,  jstring Id, jstring Nickname, jstring Email)
{
    std::string strId = "";
    std::string strNickname = "";
    std::string strEmail = "";
    
    if(Id != NULL)
        strId = env->GetStringUTFChars(Id, NULL);
    
    if(Nickname != NULL)
        strNickname = env->GetStringUTFChars(Nickname, NULL);

    if(Email != NULL)
        strEmail = env->GetStringUTFChars(Email, NULL);
    
    strId = strEmail;
    
    KAKAO_MANAGER->setUserID(strId);
    KAKAO_MANAGER->setUserName(strNickname);
    KAKAO_MANAGER->setUserEmail(strEmail);
    
    KAKAO_MANAGER->kakaoLoginSuccess();
}
    
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackIdsAvailable(JNIEnv *env, jclass cls,  jstring userId, jstring registrationId)
{
    std::string strUserId = "";
    std::string strRegistrationId = "";
    
    if(userId != NULL)
        strUserId = env->GetStringUTFChars(userId, NULL);
    
    if(registrationId != NULL)
        strRegistrationId = env->GetStringUTFChars(registrationId, NULL);
        
    ONESIGNAL_MANAGER->init(strUserId, strRegistrationId);
}
    
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackOneSignalHandleNotification(JNIEnv *env, jclass cls, jstring message, jstring title, jstring push_event, jboolean active)
{
    std::string strMessage = "";
    std::string strTitle = "";
    std::string strPush_event = "";
    bool blnActive = (active == JNI_TRUE) ? true : false;
    
    if(message != NULL)
        strMessage = env->GetStringUTFChars(message, NULL);
    
    if(title != NULL)
        strTitle = env->GetStringUTFChars(title, NULL);
    
    if(push_event != NULL)
        strPush_event = env->GetStringUTFChars(push_event, NULL);
    
    if(blnActive)
    {
        if(ONESIGNAL_MANAGER->getIntroFinish())
        {
            ONESIGNAL_MANAGER->pushEventExecution(strMessage, strTitle, strPush_event, blnActive);
        }
        else
        {
            ONESIGNAL_MANAGER->setOneSignalHandleNotification(strMessage, strTitle, strPush_event, blnActive);
        }
    }
    else
    {
        ONESIGNAL_MANAGER->setOneSignalHandleNotification(strMessage, strTitle, strPush_event, blnActive);
    }
}
    
#ifdef __cplusplus
}
#endif
