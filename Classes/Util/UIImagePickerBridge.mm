//
//  UIImagePickerBridge.mㅡ
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 8..
//
//

#import "UIImagePickerBridge.h"
#import "platform/ios/CCEAGLView-ios.h"


#define IMAGE_MAX_SIZE      160
#define IMAGE_QUALITY       0.8     // compressionQuality (0:최대압축 ~ 1:최소압축)


@implementation UIImagePickerBridge

- (id)init
{
    self = [super init];
    if(self)
    {
        imagePicker = nil;
        pixImagePicker = NULL;
    }
    return self;
    
}

- (void)dealloc
{
    if(imagePicker)
    {
        [imagePicker release]; imagePicker = nil;
    }
    if(imageName)
    {
        [imageName release]; imageName = nil;
    }
    
    [super dealloc];
}



- (void)showImagePicker:(NSString *)saveImageName
{
    // 카메라 앨범 사용여부 체크
    if(![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary])
    {
        NSLog(@"카메라 앨범 사용할 수 없음.");
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                        message:@"Device does not support a Photo Library."
                                                       delegate:self cancelButtonTitle:@"Ok"
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
        
        return;
    }
    
    imageName = saveImageName;
    [imageName retain];
    
    
    // Create image picker controller
    imagePicker = [[UIImagePickerController alloc] init];
    
    // Set source to the PhotoLibrary
    imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    
    // Allow editing of image ?
    imagePicker.allowsEditing = YES;
    
    // Delegate is self
    imagePicker.delegate = self;
    
    // Show image picker
    //    [self presentModalViewController:imagePicker animated:YES];
    //
    //    [imagePicker release];
    
    
    //    imagePicker.view.bounds = CGRectMake(0.0, 0.0, 480.0, 320.0);
    
    imagePicker.view.frame = CGRectMake(0.0, 1280.0, 720.0, 1280.0);
    
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *)view->getEAGLView();
    
    [eaglview addSubview:imagePicker.view];
    
    [self showAnimation];
    
}

- (BOOL)isShow
{
    if(imagePicker){
        return YES;
    }
    else {
        return NO;
    }
}

- (void)showAnimation
{
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:0.3];
    
    imagePicker.view.frame = CGRectMake(0.0, 0.0, 720.0, 1280.0);
    [UIView commitAnimations];
}

- (void)hideAnimation
{
    [UIView animateWithDuration:0.3
                     animations:^{
                         
                         imagePicker.view.frame = CGRectMake(0.0, 1280.0, 720.0, 1280.0);
                     }
                     completion:^(BOOL finished) {
                         
                         [imagePicker.view removeFromSuperview];
                         
                         if(imagePicker)
                         {
                             [imagePicker release]; imagePicker = nil;
                         }
                     }];
    
}



- (UIImage *)resizeImage:(UIImage *)image
{
    CGImageRef imgRef = image.CGImage;
    
    CGFloat width = CGImageGetWidth(imgRef);
    CGFloat height = CGImageGetHeight(imgRef);
    
    // 기본 EditedImage 해상도 640*640;
    int kMaxResolution = IMAGE_MAX_SIZE;
    
    CGRect bounds = CGRectMake(0, 0, width, height);
    if (width > kMaxResolution || height > kMaxResolution)
    {
        CGFloat ratio = width/height;
        // 가로가 더 클 때
        if (ratio > 1) {
            bounds.size.width = kMaxResolution;
            bounds.size.height = bounds.size.width / ratio;
        }
        // 같거나 세로가 클 때
        else {
            bounds.size.height = kMaxResolution;
            bounds.size.width = bounds.size.height * ratio;
        }
    }
    
    
    CGAffineTransform transform = CGAffineTransformIdentity;
    CGFloat scaleRatio = bounds.size.width / width;
    CGSize imageSize = CGSizeMake(CGImageGetWidth(imgRef), CGImageGetHeight(imgRef));
    CGFloat boundHeight;
    
    UIImageOrientation orient = image.imageOrientation;
    switch(orient) {
            
        case UIImageOrientationUp: //EXIF = 1
            transform = CGAffineTransformIdentity;
            break;
            
        case UIImageOrientationUpMirrored: //EXIF = 2
            transform = CGAffineTransformMakeTranslation(imageSize.width, 0.0);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            break;
            
        case UIImageOrientationDown: //EXIF = 3
            transform = CGAffineTransformMakeTranslation(imageSize.width, imageSize.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
            
        case UIImageOrientationDownMirrored: //EXIF = 4
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.height);
            transform = CGAffineTransformScale(transform, 1.0, -1.0);
            break;
            
        case UIImageOrientationLeftMirrored: //EXIF = 5
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, imageSize.width);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationLeft: //EXIF = 6
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.width);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationRightMirrored: //EXIF = 7
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeScale(-1.0, 1.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        case UIImageOrientationRight: //EXIF = 8
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, 0.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        default:
            [NSException raise:NSInternalInconsistencyException format:@"Invalid image orientation"];
            
    }
    
    UIGraphicsBeginImageContext(bounds.size);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    if (orient == UIImageOrientationRight || orient == UIImageOrientationLeft) {
        CGContextScaleCTM(context, -scaleRatio, scaleRatio);
        CGContextTranslateCTM(context, -height, 0);
    }
    else {
        CGContextScaleCTM(context, scaleRatio, -scaleRatio);
        CGContextTranslateCTM(context, 0, -height);
    }
    
    CGContextConcatCTM(context, transform);
    
    CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);
    UIImage *resizeImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return resizeImage;
    
}


- (void)saveImage:(UIImage *)image
{
    
    //    NSData *imageData = UIImagePNGRepresentation(resizeImage);
    NSData *imageData = UIImageJPEGRepresentation(image, IMAGE_QUALITY);
    
    // save Image
    if(imageData != nil)
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
        NSString *imageFullPath = [paths objectAtIndex:0];
        
        imageFullPath = [imageFullPath stringByAppendingPathComponent:imageName];
        
        [imageData writeToFile:imageFullPath atomically:NO];
        
        
        [self performSelector:@selector(successSaveImage) withObject:nil afterDelay:0.5f];
        
        //        [picker dismissModalViewControllerAnimated:YES];
        
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                        message:@"Unable to save image to Photo Album."
                                                       delegate:self cancelButtonTitle:@"Ok"
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
    }
    
}

- (void)successSaveImage
{
    if(pixImagePicker)
        pixImagePicker->successImagePicker();
    
    [self hideAnimation];
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(pixImagePicker)
        pixImagePicker->canceledImagePicker();
    
    // After saving iamge, dismiss camera
    //	[self dismissModalViewControllerAnimated:YES];
    
    if(imagePicker)
    {
        [self hideAnimation];
    }
    
}


# pragma mark - imagePicker delegate
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    if(pixImagePicker)
        pixImagePicker->canceledImagePicker();
    
    //    [picker dismissModalViewControllerAnimated:YES];
    [self hideAnimation];
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    
    // Access the uncropped image from info dictionary
    UIImage *image = [info valueForKey:UIImagePickerControllerEditedImage];
    
    UIImage *resizeImage = [self resizeImage:image];
    [self saveImage:resizeImage];
    
}




- (void)setPixImagePicker:(PixImagePicker *)_pixImagePicker
{
    pixImagePicker = _pixImagePicker;
}


@end

