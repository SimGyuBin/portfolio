//
//  PixImagePicker.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 8..
//
//

#include "PixImagePicker.h"
#include "PixNativeUtils.h"
#include "platform/android/jni/JniHelper.h"

PixImagePicker::PixImagePicker(PixImagePickerDelegate *delegate)
{
    mDelegate = delegate;
}

PixImagePicker::~PixImagePicker()
{
    CCLOG("%s", __FUNCTION__);
}

void PixImagePicker::showImagePicker(const char *savePhotoName)
{
    JniMethodInfo methodInfo;
    
    if (JniHelper::getStaticMethodInfo(methodInfo,
                                       native_root.c_str(),
                                       "showImagePicker", "(Ljava/lang/String;)V"))
    {
        jstring str = methodInfo.env->NewStringUTF(savePhotoName);
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, str);
        
        methodInfo.env->DeleteLocalRef(str);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

void PixImagePicker::successImagePicker()
{
    if (mDelegate)
    {
        mDelegate->successImagePicker();
    }
}

void PixImagePicker::canceledImagePicker()
{
    if (mDelegate)
    {
        mDelegate->canceledImagePicker();
    }
}
