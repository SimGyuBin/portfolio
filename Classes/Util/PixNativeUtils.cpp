//
//  PixNativeUtils.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 28..
//
//

#include "PixNativeUtils.h"
#include "platform/android/jni/JniHelper.h"
#include "../Scene/PixApp.h"
#include "../Common.h"
#include "../UIControl/PixToast.h"
;
static std::function<void()> _callback = nullptr;

// C++ -> Java

void PixNativeUtils::nativeCallPathOfAllImages(pathOfAllImages_CallBack callback)
{
    _callback = callback;

    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
                                       __FUNCTION__, "()V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

void PixNativeUtils::nativeCallPathOfAllImagesComplete()
{
	PIXAPP->hideLoading();

	    if(_callback)
	        (_callback)();

   _callback = nullptr;
}

void PixNativeUtils::nativeCallGetThumbnailImage(const std::string &strUrl, int width, int height)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo,
										native_root.c_str(),
									   __FUNCTION__, "(Ljava/lang/String;II)V"))
	{
		jstring jstrUrl = methodInfo.env->NewStringUTF(strUrl.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrUrl, width, height);

		methodInfo.env->DeleteLocalRef(jstrUrl);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallGetFullScreenImage(const std::string &strUrl, int width, int height)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo,
										native_root.c_str(),
									   __FUNCTION__, "(Ljava/lang/String;II)V"))
	{
		jstring jstrUrl = methodInfo.env->NewStringUTF(strUrl.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrUrl, width, height);

		methodInfo.env->DeleteLocalRef(jstrUrl);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

}

void PixNativeUtils::nativeCallGetUploadImage(const std::string &strUrl)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
									   __FUNCTION__, "(Ljava/lang/String;)V"))
	{
		jstring jstrUrl = methodInfo.env->NewStringUTF(strUrl.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrUrl);

		methodInfo.env->DeleteLocalRef(jstrUrl);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

}

void PixNativeUtils::nativeCallChangeOrientation(int flag)
{
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
                                       __FUNCTION__, "(I)V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, flag);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

Sprite* PixNativeUtils::nativeCallTransformImage(const unsigned char * data, ssize_t dataLen)
{
	return nullptr;
}

std::string PixNativeUtils::nativeCallAES256Encrypt(const std::string &plainText, const std::string &key)
{
	JniMethodInfo methodInfo;
	std::string str = "";

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring plainTextStr = methodInfo.env->NewStringUTF(plainText.c_str());
		jstring keyStr = methodInfo.env->NewStringUTF(key.c_str());
		jstring returnStr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, plainTextStr, keyStr);

		const char *conChr = methodInfo.env->GetStringUTFChars(returnStr, NULL);
		str = conChr;

		methodInfo.env->DeleteLocalRef(plainTextStr);
		methodInfo.env->DeleteLocalRef(keyStr);
		methodInfo.env->ReleaseStringUTFChars(returnStr, conChr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return str;
}

std::string PixNativeUtils::nativeCallAES256Decrypt(const std::string &base64Text, const std::string &key)
{
	JniMethodInfo methodInfo;
	std::string str = "";

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring base64TextStr = methodInfo.env->NewStringUTF(base64Text.c_str());
		jstring keyStr = methodInfo.env->NewStringUTF(key.c_str());
		jstring returnStr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, base64TextStr, keyStr);

		const char *conChr = methodInfo.env->GetStringUTFChars(returnStr, NULL);
		str = conChr;

		methodInfo.env->DeleteLocalRef(base64TextStr);
		methodInfo.env->DeleteLocalRef(keyStr);
		methodInfo.env->ReleaseStringUTFChars(returnStr, conChr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return str;
}

std::string PixNativeUtils::nativeCallAES256Encrypt(const std::string &plainText)
{
    return PixNativeUtils::nativeCallAES256Encrypt(plainText,ENCRYPT_KEY);
}

std::string PixNativeUtils::nativeCallAES256Decrypt(const std::string &base64Text)
{
    return PixNativeUtils::nativeCallAES256Decrypt(base64Text,ENCRYPT_KEY);
}

void PixNativeUtils::nativeCallClipboard(const std::string &paste, const std::string &msg)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
									   __FUNCTION__, "(Ljava/lang/String;)V"))
	{
		jstring jStr = methodInfo.env->NewStringUTF(paste.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jStr);

		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	PixToast::showToast(msg.c_str());
}

void PixNativeUtils::nativeCallBrowser(const std::string &strUrl)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
									   __FUNCTION__, "(Ljava/lang/String;)V"))
	{
		jstring jStr = methodInfo.env->NewStringUTF(strUrl.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jStr);

		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallSaveImage(cocos2d::Data *pData, const std::string &strFileName, const std::string &msg)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										   __FUNCTION__, "(Ljava/lang/String;[BI)V"))
	{
		jbyteArray javaBytes;
		int len = (int)pData->getSize();
		javaBytes = methodInfo.env->NewByteArray(len);
		methodInfo.env->SetByteArrayRegion(javaBytes, 0, len, (jbyte*)pData->getBytes());

		jstring jStr = methodInfo.env->NewStringUTF(strFileName.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jStr, javaBytes, len);

		methodInfo.env->DeleteLocalRef(javaBytes);
		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

    if(!msg.empty())
    {
        PixToast::showToast(msg.c_str());
    }
}

std::string PixNativeUtils::getDeviceId()
{
	return getJavaString(__FUNCTION__);
}

std::string PixNativeUtils::getAppVersionString()
{
	return getJavaString(__FUNCTION__);
}

std::string PixNativeUtils::getOsVersionString()
{
	return getJavaString(__FUNCTION__);
}

int PixNativeUtils::getDeviceWidth()
{
	JniMethodInfo methodInfo;
	int nWidth = 0;

	if (JniHelper::getStaticMethodInfo(methodInfo,  native_root.c_str(),
									   __FUNCTION__, "()I"))
	{
		nWidth = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return nWidth;
}

int PixNativeUtils::getDeviceHeight()
{
	JniMethodInfo methodInfo;
	int nHeight = 0;

	if (JniHelper::getStaticMethodInfo(methodInfo,  native_root.c_str(),
									   __FUNCTION__, "()I"))
	{
		nHeight = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return nHeight;
}

std::string PixNativeUtils::getManuFacturer()
{
	return getJavaString(__FUNCTION__);
}

std::string PixNativeUtils::getModelName()
{
	return getJavaString(__FUNCTION__);
}

bool PixNativeUtils::isEmailPatternMatching(const std::string &str)
{
	JniMethodInfo methodInfo;
	bool isPatternMatching = false;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "(Ljava/lang/String;)Z"))
	{

		jstring jStr = methodInfo.env->NewStringUTF(str.c_str());

		isPatternMatching = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jStr);
		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

	}

	return isPatternMatching;
}


bool PixNativeUtils::isPhoneNumberPatternMatching(const std::string &str)
{
	JniMethodInfo methodInfo;
	bool isPatternMatching = false;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "(Ljava/lang/String;)Z"))
	{
		jstring jStr = methodInfo.env->NewStringUTF(str.c_str());

		isPatternMatching = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jStr);
		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return isPatternMatching;
}

bool PixNativeUtils::isPasswordPatternMatching(const std::string &str)
{
	JniMethodInfo methodInfo;
	bool isPatternMatching = false;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "(Ljava/lang/String;)Z"))
	{
		jstring jStr = methodInfo.env->NewStringUTF(str.c_str());

		isPatternMatching = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jStr);
		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return isPatternMatching;
}

bool PixNativeUtils::checkValidateString(const std::string &str)
{
	JniMethodInfo methodInfo;
	bool isPatternMatching = false;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "(Ljava/lang/String;)Z"))
	{
		jstring jStr = methodInfo.env->NewStringUTF(str.c_str());

		isPatternMatching = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jStr);
		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return isPatternMatching;
}

#pragma mark - Facebook

void PixNativeUtils::nativeCallFacebookLogin()
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallFacebookLogOut()
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

bool PixNativeUtils::nativeCallFacebookLoginCheck()
{
	JniMethodInfo methodInfo;

	bool isLoging = false;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
											__FUNCTION__, "()Z"))
	{
		isLoging = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return isLoging;
}

void PixNativeUtils::nativeCallFacebookMyInfo()
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallFacebookonUserAlbums()
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallFacebookonUserImages(const std::string &strAlbumID)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
									   __FUNCTION__, "(Ljava/lang/String;)V"))
	{
		jstring jStr = methodInfo.env->NewStringUTF(strAlbumID.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jStr);

		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallFacebookShare(const std::string &strUrl)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
									   __FUNCTION__, "(Ljava/lang/String;)V"))
	{
		jstring jStr = methodInfo.env->NewStringUTF(strUrl.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jStr);

		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallFacebookSharePhotoData(cocos2d::Data *pData)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "([BI)V"))
	{
		jbyteArray javaBytes;
		int len = (int)pData->getSize();
		javaBytes = methodInfo.env->NewByteArray(len);
		methodInfo.env->SetByteArrayRegion(javaBytes, 0, len, (jbyte*)pData->getBytes());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, javaBytes, len);

		methodInfo.env->DeleteLocalRef(javaBytes);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallFacebookSharePhotoPath(const std::string &strPath)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "(Ljava/lang/String;)V"))
	{
		jstring jStr = methodInfo.env->NewStringUTF(strPath.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jStr);

		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

#pragma mark - Google

void PixNativeUtils::nativeCallAccountsGoogle(const std::string &strUrl) {};

#pragma mark - for Android

void PixNativeUtils::nativeCallAndroidDestroy()
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
												   __FUNCTION__, "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

#pragma mark - kakao

void PixNativeUtils::nativeCallKakaoLogin()
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
													__FUNCTION__, "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallKakaoLogout()
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
													__FUNCTION__, "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

#pragma mark - OneSignal

void PixNativeUtils::nativeCallOneSignalsetSubscription(bool enable)
{

	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
	                                       __FUNCTION__, "(Z)V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, enable);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallAndroidToast(const std::string &msg)
{
	Scene *scene = Director::getInstance()->getRunningScene();
	Node *beforeToast = dynamic_cast<Node*>(scene->getChildByTag(TOAST_TAG));
	if (beforeToast) beforeToast->removeFromParentAndCleanup(true);

	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										__FUNCTION__, "(Ljava/lang/String;)V"))
	{
		jstring jStr = methodInfo.env->NewStringUTF(msg.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jStr);

		methodInfo.env->DeleteLocalRef(jStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

std::string PixNativeUtils::getPhoneNumber()
{
	return getJavaString(__FUNCTION__);
}

std::string PixNativeUtils::getDeviceBrand()
{
	return getJavaString(__FUNCTION__);
}

// 안드로이드 외장메모리 남은용량 체크(MB)
int PixNativeUtils::getFreeDisk()
{
    JniMethodInfo methodInfo;
    int freeDisk = 0;
    
    if (JniHelper::getStaticMethodInfo(methodInfo,  native_root.c_str(),
                                       __FUNCTION__, "()I"))
    {
        freeDisk = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    return freeDisk;
}

bool PixNativeUtils::isWiFi()
{
	JniMethodInfo methodInfo;
	bool returnValue = false;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
										   "isWiFi", "()Z"))
	{
		returnValue = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return returnValue;
}

void PixNativeUtils::goURL(const char* url)
{
    JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo,
										native_root.c_str(),
									   "goURL", "(Ljava/lang/String;)V"))
	{
		jstring urlStr = methodInfo.env->NewStringUTF(url);

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, urlStr);

		methodInfo.env->DeleteLocalRef(urlStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallWriteMail(const std::string &strUrl)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo,
										native_root.c_str(),
										"writeMail", "(Ljava/lang/String;)V"))
	{
		jstring urlStr = methodInfo.env->NewStringUTF(strUrl.c_str());

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, urlStr);

		methodInfo.env->DeleteLocalRef(urlStr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

std::string PixNativeUtils::getJavaString(const char *methodName)
{
	JniMethodInfo methodInfo;
    std::string str = "";

    if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(), methodName, "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);

		const char *conChr = methodInfo.env->GetStringUTFChars(jstr, NULL);
		str = conChr;

		methodInfo.env->ReleaseStringUTFChars(jstr, conChr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	return str;
}

void PixNativeUtils::nativeCallIntentURIstartActivity(const std::string &url)
{
    JniMethodInfo methodInfo;
    
    if (JniHelper::getStaticMethodInfo(methodInfo,
                                       native_root.c_str(),
                                       "intentURIstartActivity", "(Ljava/lang/String;)V"))
    {
        jstring urlStr = methodInfo.env->NewStringUTF(url.c_str());
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, urlStr);
        
        methodInfo.env->DeleteLocalRef(urlStr);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

bool PixNativeUtils::nativeCallstartsWith(const std::string &strUrl, const std::string &strKey)
{
    JniMethodInfo methodInfo;
    
    bool isSstartsWith = false;
    
    if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
                                       __FUNCTION__, "(Ljava/lang/String;Ljava/lang/String;)Z"))
    {
    	isSstartsWith = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    return isSstartsWith;
}

bool PixNativeUtils::isVirtualButton()
{
    JniMethodInfo methodInfo;
    bool returnValue = false;
    
    if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
                                       __FUNCTION__, "()Z"))
    {
        returnValue = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    return returnValue;
    
}

void PixNativeUtils::nativeCallGoogleSignIn()
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
												   __FUNCTION__, "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallGoogleSignOut()
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
												   __FUNCTION__, "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void PixNativeUtils::nativeCallIntroFinish(bool var)
{
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, native_root.c_str(),
												   __FUNCTION__, "(Z)V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}
