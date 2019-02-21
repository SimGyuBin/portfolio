//
//  UIImagePickerBridge.h
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 8..
//
//

#ifndef UIImagePickerBridge_h
#define UIImagePickerBridge_h

#import <UIKit/UIKit.h>
#import "PixImagePicker.h"

@interface UIImagePickerBridge : NSObject <UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{
    UIImagePickerController *imagePicker;
    NSString *imageName;
    
    PixImagePicker *pixImagePicker;
}

- (void)showImagePicker:(NSString *)saveImageName;
- (UIImage *)resizeImage:(UIImage *)image;
- (void)saveImage:(UIImage *)image;

- (BOOL)isShow;
- (void)showAnimation;
- (void)hideAnimation;
- (void)successSaveImage;

- (void)setPixImagePicker:(PixImagePicker *)_pixImagePicker;

@end

#endif /* UIImagePickerBridge_h */
