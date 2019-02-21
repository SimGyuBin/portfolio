/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "PixApp.h"
#import "PixNativeUtils.h"
#import "Common.h"
#import "PixStringUtils.h"

#import "AppController.h"
#import <UIKit/UIKit.h>
#import <Photos/Photos.h>
#import <SafariServices/SafariServices.h>

#define degreesToRadian(x) (M_PI * (x) / 180.0)

#define SAFE_RELEASE(X)  [X release]; X = nil;
#define SEMAPHORE_COUNT 1

@interface RootViewController ()<SFSafariViewControllerDelegate, UIAlertViewDelegate>
@end

@implementation RootViewController
/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
    
    localImageInfoVec.clear();
    _safariViewController = nil;
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    
    // Initialize the GLView attributes
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Initialize the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [UIScreen mainScreen].bounds
                                         pixelFormat: (__bridge NSString *)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:YES];
    
    // Set EAGLView as view of RootViewController
    self.view = eaglView;
    
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)self.view);
    
    //set the GLView as OpenGLView of the Director
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    //run the cocos2d-x game scene
    app->run();
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}


// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
#ifdef __IPHONE_6_0
- (NSUInteger) supportedInterfaceOrientations{
    if(PIXAPP->frameType == FRAME_HORIZONTAL)
    {
        return UIInterfaceOrientationMaskLandscapeRight;
    }
    else if(PIXAPP->frameType == FRAME_VERTICALSIZE)
    {
        return UIInterfaceOrientationMaskPortrait;
    }
    
    return UIInterfaceOrientationMaskPortrait;
}
#endif

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    auto glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (__bridge CCEAGLView *)glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void) pathOfAllImages
{
    dispatch_semaphore_t sema = dispatch_semaphore_create(SEMAPHORE_COUNT);
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    
    dispatch_async(queue, ^{
        
        localImageInfoVec.clear();
        
        // 이미지 정보 가져 오기
        PHFetchResult *smartAlbums = [PHAssetCollection fetchAssetCollectionsWithType:PHAssetCollectionTypeSmartAlbum
                                                                              subtype:PHAssetCollectionSubtypeSmartAlbumUserLibrary options:nil];

        [smartAlbums enumerateObjectsUsingBlock:^(PHAssetCollection* collection, NSUInteger idx, BOOL *stop) {
            
            PHFetchOptions *options = [[PHFetchOptions alloc] init];
            options.predicate = [NSPredicate predicateWithFormat:@"mediaType = %d",PHAssetMediaTypeImage];
            
            PHFetchResult* assets = [PHAsset fetchAssetsInAssetCollection:collection options:options];
            
            [assets enumerateObjectsUsingBlock:^(PHAsset *asset, NSUInteger idx, BOOL *stop) {
                
                NSString *identifier = [asset valueForKey:@"uniformTypeIdentifier"];
                
                if(![identifier isEqualToString:@"com.compuserve.gif"])
                {
                    std::string strPath = [asset.localIdentifier UTF8String];
                    
                    NSDate *creationDate = asset.creationDate;
                    NSDate *modificationDate = asset.modificationDate;
                    
                    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
                    [dateFormatter setDateFormat:@"yyyyMMddHHmmss"];
                    
                    std::string strFullDate = [[dateFormatter stringFromDate:creationDate] UTF8String];
                    std::string strDate_modified = [[dateFormatter stringFromDate:modificationDate] UTF8String];
                    std::string strDate_added = [[dateFormatter stringFromDate:creationDate] UTF8String];
                    
                    [dateFormatter release];
                    
                    float fWidth = asset.pixelWidth;
                    float fHeight = asset.pixelHeight;
                    
                    std::string strWidth = StringUtils::format("%lf",fWidth);
                    std::string strHeight = StringUtils::format("%lf",fHeight);
                    
                    LocalImageInfo *info = new LocalImageInfo();
                    info->setPath(strPath.c_str());
                    info->setDateModified(strDate_modified.c_str());
                    info->setDateAdded(strDate_added.c_str());
                    info->setFullDate(strFullDate.c_str());
                    //                info.setFileSize(strSize.c_str());
                    //                info.setOrientation((int)orientation);
                    info->setWidth(strWidth);
                    info->setHeight(strHeight);
                    
                    localImageInfoVec.push_back(info);
                    
                }
            }];
        }];
        
        // 아이튠즈 이미지 가져 오기
        PHFetchResult *syncedAlbums = [PHAssetCollection fetchAssetCollectionsWithType:PHAssetCollectionTypeAlbum
                                                                               subtype:PHAssetCollectionSubtypeAlbumSyncedAlbum options:nil];
        
        
        [syncedAlbums enumerateObjectsUsingBlock:^(PHAssetCollection* collection, NSUInteger idx, BOOL *stop) {
            
            PHFetchOptions *options = [[PHFetchOptions alloc] init];
            options.predicate = [NSPredicate predicateWithFormat:@"mediaType = %d",PHAssetMediaTypeImage];
            
            PHFetchResult* assets = [PHAsset fetchAssetsInAssetCollection:collection options:options];
            
            [assets enumerateObjectsUsingBlock:^(PHAsset *asset, NSUInteger idx, BOOL *stop) {
                
                NSString *identifier = [asset valueForKey:@"uniformTypeIdentifier"];
                
                if(![identifier isEqualToString:@"com.compuserve.gif"])
                {
                    std::string strPath = [asset.localIdentifier UTF8String];
                    
                    NSDate *creationDate = asset.creationDate;
                    NSDate *modificationDate = asset.modificationDate;
                    
                    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
                    [dateFormatter setDateFormat:@"yyyyMMddHHmmss"];
                    
                    std::string strFullDate = [[dateFormatter stringFromDate:creationDate] UTF8String];
                    std::string strDate_modified = [[dateFormatter stringFromDate:modificationDate] UTF8String];
                    std::string strDate_added = [[dateFormatter stringFromDate:creationDate] UTF8String];
                    
                    [dateFormatter release];
                    
                    float fWidth = asset.pixelWidth;
                    float fHeight = asset.pixelHeight;
                    
                    std::string strWidth = StringUtils::format("%lf",fWidth);
                    std::string strHeight = StringUtils::format("%lf",fHeight);
                    
                    LocalImageInfo *info = new LocalImageInfo();
                    info->setPath(strPath.c_str());
                    info->setDateModified(strDate_modified.c_str());
                    info->setDateAdded(strDate_added.c_str());
                    info->setFullDate(strFullDate.c_str());
                    //                info.setFileSize(strSize.c_str());
                    //                info.setOrientation((int)orientation);
                    info->setWidth(strWidth);
                    info->setHeight(strHeight);
                    
                    // 아이튠즈는 카테고리 셋팅
                    std::string strCategory = [collection.localizedTitle UTF8String];
                    info->setCategoryInfo(strCategory, strCategory);
                    
                    localImageInfoVec.push_back(info);
                    
                }
            }];
        }];
        
        // 이미지 카테고리 정보 가져오기
        PHFetchResult *topLevelUserCollections = [PHCollectionList fetchTopLevelUserCollectionsWithOptions:nil];
        [topLevelUserCollections enumerateObjectsUsingBlock:^(PHAssetCollection* collection, NSUInteger idx, BOOL *stop) {
            
            PHFetchOptions *options = [[PHFetchOptions alloc] init];
            options.predicate = [NSPredicate predicateWithFormat:@"mediaType = %d",PHAssetMediaTypeImage];
            
            PHFetchResult* assets = [PHAsset fetchAssetsInAssetCollection:collection options:options];
            
            [assets enumerateObjectsUsingBlock:^(PHAsset *asset, NSUInteger idx, BOOL *stop) {
                
                NSString *identifier = [asset valueForKey:@"uniformTypeIdentifier"];
                
                if(![identifier isEqualToString:@"com.compuserve.gif"])
                {
                    std::string strPath = [asset.localIdentifier UTF8String];
                    
                    size_t nCount = localImageInfoVec.size();
                    
                    for(size_t i = 0; i < nCount; ++i)
                    {
                        LocalImageInfo *info = localImageInfoVec.at(i);
                        
                        if(info->getPath().compare(strPath) == 0)
                        {
                            std::string strCategory = [collection.localizedTitle UTF8String];
                            info->setCategoryInfo(strCategory, strCategory);
                        }
                    }
                }
            }];
        }];

        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([=](void) -> void {
            
            size_t nCount = localImageInfoVec.size();
            
            for(size_t i = 0; i < nCount; ++i)
            {
                LocalImageInfo *info = localImageInfoVec.at(i);
                IMAGECREATE_MANAGER->setLocalImages((*info));
                delete info;
            }
            
            localImageInfoVec.clear();
            PixNativeUtils::nativeCallPathOfAllImagesComplete();
            
        });
    
        dispatch_semaphore_signal(sema);
        
    });
}

- (void) getThumbnailImage:(std::string)strUrl width:(int)width height:(int)height
{
//    dispatch_semaphore_t sema = dispatch_semaphore_create(SEMAPHORE_COUNT);
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    
    dispatch_async(queue, ^{

        NSString *url = [NSString stringWithCString:strUrl.c_str()
                                           encoding:[NSString defaultCStringEncoding]];
        
        PHFetchResult* assetResult = [PHAsset fetchAssetsWithLocalIdentifiers:@[url] options:nil];
        PHAsset *asset = [assetResult firstObject];
        
        float fWidth = asset.pixelWidth;
        float fHeight = asset.pixelHeight;
        float fScale = 1.0f;
        
        // 썸네일 생성
        if(fWidth > THUMBNAIL_SIZE || fHeight > THUMBNAIL_SIZE)
        {
            float tw = ((float) THUMBNAIL_SIZE / ((float) fWidth));
            float th = ((float) THUMBNAIL_SIZE / ((float) fHeight));
            
            if (tw > th)
            {
                fScale = tw;
            }
            else
            {
                fScale = th;
            }
        }
        
        PHCachingImageManager *imageManager = [[PHCachingImageManager alloc] init];
        
        PHImageRequestOptions *options = [[PHImageRequestOptions alloc] init];
        options.synchronous = YES;
        options.resizeMode = PHImageRequestOptionsResizeModeExact;
        options.deliveryMode = PHImageRequestOptionsDeliveryModeOpportunistic;
        
        CGSize targetSize = CGSizeMake(fWidth * fScale, fHeight * fScale);
        
        [imageManager requestImageForAsset:asset targetSize:targetSize contentMode:PHImageContentModeAspectFill options:options resultHandler:^(UIImage *result, NSDictionary *info) {
            
            if(result)
            {
                NSData *imageData = UIImageJPEGRepresentation(result, 1.0f);
                NSUInteger size = [imageData length];
                uint8_t *buff = (uint8_t *)malloc(sizeof(uint8_t)*size);
                memcpy(buff, [imageData bytes], size);
                
                Data *thumbnaildata = new Data;
                thumbnaildata->fastSet(buff, size);
                
                std::string strWidth = [[NSString stringWithFormat:@"%.0f", fWidth] UTF8String];
                std::string strHeight = [[NSString stringWithFormat:@"%.0f", fHeight] UTF8String];
                
                auto scheduler = cocos2d::Director::getInstance()->getScheduler();
                scheduler->performFunctionInCocosThread([=](void) -> void {
                    IMAGECREATE_MANAGER->addThumbnailData(strUrl, thumbnaildata, strWidth, strHeight);
                });
            }
        }];
        
        [imageManager release];
        [options release];
    
//        dispatch_semaphore_signal(sema);
        
    });
    
//    dispatch_release(sema);
    dispatch_release(queue);
}

- (void) getFullScreenImage:(std::string)strUrl width:(int)width height:(int)height
{
//    dispatch_semaphore_t sema = dispatch_semaphore_create(SEMAPHORE_COUNT);
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    
    dispatch_async(queue, ^{
    
        NSString *url = [NSString stringWithCString:strUrl.c_str()
                                           encoding:[NSString defaultCStringEncoding]];
        
        PHFetchResult* assetResult = [PHAsset fetchAssetsWithLocalIdentifiers:@[url] options:nil];
        PHAsset *asset = [assetResult firstObject];
        
        float fWidth = asset.pixelWidth;
        float fHeight = asset.pixelHeight;
        float fScale = 1.0f;
        
        // 썸네일 생성
        if(fWidth > FULLSCREEN_SIZE || fHeight > FULLSCREEN_SIZE)
        {
            float tw = ((float) FULLSCREEN_SIZE / ((float) fWidth));
            float th = ((float) FULLSCREEN_SIZE / ((float) fHeight));
            
            if (tw > th)
            {
                fScale = tw;
            }
            else
            {
                fScale = th;
            }
        }
        
        PHCachingImageManager *imageManager = [[PHCachingImageManager alloc] init];
        
        PHImageRequestOptions *options = [[PHImageRequestOptions alloc] init];
        options.synchronous = YES;
        options.resizeMode = PHImageRequestOptionsResizeModeExact;
        options.deliveryMode = PHImageRequestOptionsDeliveryModeOpportunistic;
        
        CGSize targetSize = CGSizeMake(fWidth * fScale, fHeight * fScale);
        
        [imageManager requestImageForAsset:asset targetSize:targetSize contentMode:PHImageContentModeAspectFill options:options resultHandler:^(UIImage *result, NSDictionary *info) {
            
            if(result)
            {
                NSData *imageData = UIImageJPEGRepresentation(result, 1.0f);
                NSUInteger size = [imageData length];
                uint8_t *buff = (uint8_t *)malloc(sizeof(uint8_t)*size);
                memcpy(buff, [imageData bytes], size);
                
                Data *fullScreenData = new Data;
                fullScreenData->fastSet(buff, size);
                
                std::string strWidth = [[NSString stringWithFormat:@"%.0f", fWidth] UTF8String];
                std::string strHeight = [[NSString stringWithFormat:@"%.0f", fHeight] UTF8String];
                
                auto scheduler = cocos2d::Director::getInstance()->getScheduler();
                scheduler->performFunctionInCocosThread([=](void) -> void {
                    IMAGECREATE_MANAGER->addFullScreenData(strUrl, fullScreenData, strWidth, strHeight);
                });
            }
        }];
        
        [imageManager release];
        [options release];
        
//        dispatch_semaphore_signal(sema);
        
    });
    
//    dispatch_release(sema);
    dispatch_release(queue);
    
}

- (void) getUploadImage:(std::string)strUrl
{
    dispatch_semaphore_t sema = dispatch_semaphore_create(SEMAPHORE_COUNT);
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    
    dispatch_async(queue, ^{
        
        NSString *url = [NSString stringWithCString:strUrl.c_str()
                                           encoding:[NSString defaultCStringEncoding]];
        
        PHFetchResult* assetResult = [PHAsset fetchAssetsWithLocalIdentifiers:@[url] options:nil];
        PHAsset *asset = [assetResult firstObject];
        
        PHCachingImageManager *imageManager = [[PHCachingImageManager alloc] init];
        
        PHImageRequestOptions *options = [[PHImageRequestOptions alloc] init];
        options.synchronous = YES;
        options.resizeMode = PHImageRequestOptionsResizeModeExact;
        options.deliveryMode = PHImageRequestOptionsDeliveryModeOpportunistic;
        
        [imageManager requestImageDataForAsset:asset options:options resultHandler:^(NSData *result, NSString * dataUTI, UIImageOrientation orientation, NSDictionary *info) {
            
            NSUInteger size = [result length];
            uint8_t *buff = (uint8_t *)malloc(sizeof(uint8_t)*size);
            memcpy(buff, [result bytes], size);
            
            Data *originaldata = new Data();
            originaldata->fastSet(buff, size);
            
            float fWidth = asset.pixelWidth;
            float fHeight = asset.pixelHeight;
            
            std::string strWidth = StringUtils::format("%lf",fWidth);
            std::string strHeight = StringUtils::format("%lf",fHeight);
            
            NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
            [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
            NSString *strDate = [dateFormatter stringFromDate:asset.creationDate];
            [dateFormatter release];
            
            std::string strDateTime = [strDate UTF8String];
            
            auto scheduler = cocos2d::Director::getInstance()->getScheduler();
            scheduler->performFunctionInCocosThread([=](void) -> void {
                UPLOADIMAGE_MANAGER->uploadImageComplete(originaldata, strUrl, strWidth, strHeight, strDateTime);
            });
        }];
        
        [imageManager release];
        [options release];
        
        dispatch_semaphore_signal(sema);
        
    });
    
    dispatch_release(sema);
    dispatch_release(queue);
}

- (void) accountsGoogle:(std::string)strUrl
{
    NSURL *url = [NSURL URLWithString:@(strUrl.c_str())];
    
    _safariViewController = [[SFSafariViewController alloc] initWithURL:url entersReaderIfAvailable:NO];
    _safariViewController.delegate = self;
    
    [self presentViewController:_safariViewController animated:YES completion:nil];
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
//    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
//    
//    // Code here will execute before the rotation begins.
//    // Equivalent to placing it in the deprecated method -[willRotateToInterfaceOrientation:duration:]
//    
//    [coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext> context) {
//        
//        // Place code here to perform animations during the rotation.
//        // You can pass nil or leave this block empty if not necessary.
//        
//    } completion:^(id<UIViewControllerTransitionCoordinatorContext> context) {
//        
//        // Code here will execute after the rotation has finished.
//        // Equivalent to placing it in the deprecated method -[didRotateFromInterfaceOrientation:]
//        
//    }];
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)dealloc {
    
    [super dealloc];
}

- (void) dismissAuthorizationAnimated
{
    SFSafariViewController *safariVC = _safariViewController;

    [self cleanUp];
   
    if (safariVC)
    {
        [safariVC dismissViewControllerAnimated:YES completion:nil];
    }
}

- (void) showAlertViewWithEvent:(NSString*)msg tagNum:(NSInteger)tag
{
    UIAlertView *v = [[UIAlertView alloc]initWithTitle:@"알림"
                                               message:msg
                                              delegate:self cancelButtonTitle:@"확인"
                                     otherButtonTitles:nil];
    v.tag = tag;
    [v show];
}

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(alertView.tag == 99) {
        // ISP 앱 스토어로 이동
        NSString* URLString = @"https://itunes.apple.com/app/mobail-gyeolje-isp/id369125087?mt=8";
        NSURL* storeURL = [NSURL URLWithString:URLString];
        [[UIApplication sharedApplication] openURL:storeURL];
    }
}

- (void) cleanUp {
    // The weak references to |_safariVC| and |_session| are set to nil to avoid accidentally using
    // them while not in an authorization flow.
    _safariViewController = nil;  
}


#pragma mark - SFSafariViewControllerDelegate

- (void)safariViewControllerDidFinish:(SFSafariViewController *)controller
{
    
}

@end
