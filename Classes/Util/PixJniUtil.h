//
//  PixJniUtil.hpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 28..
//
//

#ifndef PixJniUtil_hpp
#define PixJniUtil_hpp

#include "platform/android/jni/JniHelper.h"

// Java -> C++

#ifdef __cplusplus
extern "C" {
#endif
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackPathOfAllImages(JNIEnv *, jclass, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackPathOfAllImagesComplete(JNIEnv *, jclass);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackGetThumbnailImage(JNIEnv *, jclass, jbyteArray, jint, jstring, jstring, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackGetFullScreenImage(JNIEnv *, jclass, jbyteArray, jint, jstring, jstring, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackGetUploadImage(JNIEnv *, jclass, jbyteArray, jint, jstring, jstring, jstring, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackKeyCodeBack(JNIEnv *, jclass);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookLogin(JNIEnv *, jclass, jboolean, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookLoginCheck(JNIEnv *, jclass, jboolean);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookMyInfo(JNIEnv *, jclass, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookUserAlbums(JNIEnv *, jclass, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackFacebookUserImages(JNIEnv *, jclass, jstring, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackPurchaseLayerloadURL(JNIEnv *, jclass, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackGoogleSignInSuccess(JNIEnv *, jclass, jstring, jstring, jstring, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackKakaoLoginSuccess(JNIEnv *, jclass, jstring, jstring, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackIdsAvailable(JNIEnv *, jclass, jstring, jstring);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Natives_nativeCallBackOneSignalHandleNotification(JNIEnv *, jclass, jstring, jstring, jstring, jboolean);
    
#ifdef __cplusplus
}
#endif

#endif /* PixJniUtil_hpp */
