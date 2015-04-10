//
//  FOAViewController.m
//  fuzzy-octo-archer
//
//  Created by yangboz on 14-6-26.
//  Copyright (c) 2014年 GODPAPER. All rights reserved.
//

#import "FOAViewController.h"
#import "opencv2/highgui/ios.h"

@interface FOAViewController ()

@end

@implementation FOAViewController

@synthesize foaNavigationBar;
@synthesize photoButton;

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    // Do any additional setup after loading the view, typically from a nib.
//    self.navigationController.navigationBar.barStyle = UIBarStyleBlack;
//    self.navigationController.navigationBar.translucent = NO;
    foaNavigationBar.barStyle = UIBarStyleBlack;
    foaNavigationBar.translucent = NO;
    
    //Update the UIButton with border.
    self.photoButton.layer.borderWidth = 1.0f;
    self.photoButton.layer.borderColor = [[UIColor grayColor] CGColor];
    self.photoButton.layer.cornerRadius = 4.0f;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - IBActions
- (IBAction)uploadPhoto:(id)sender
{

}

- (IBAction)segmentedButtonChanged:(id)sender
{
    NSLog(@"segmentedButtonChanged to %d",self.segementedCtr.selectedSegmentIndex);
    //NULL check!
    if (self.photo==NULL) {
        return;
    }
    /*
     //Load a test image:
     NSString* filePath = [[NSBundle mainBundle] pathForResource:@"chessboard_page01" ofType:@"jpg"];
     // Create file handle
     NSFileHandle* handle =
     [NSFileHandle fileHandleForReadingAtPath:filePath];
     // Read content of the file
     data = [handle readDataToEndOfFile];
     */
    //OpenCV2 operation as following.
    // Convert UIImage* to cv::Mat
    UIImageToMat(self.photo, cvImage);
    NSData* data = UIImagePNGRepresentation(self.photo);
    unsigned long oriLen = (unsigned long)[data length];
    NSLog(@"NSData length(before compression):%lu",oriLen);
#warning in
    // Decode image from the data buffer
    //@see: http://ninghang.blogspot.sg/2012/11/list-of-mat-type-in-opencv.html
    cvImage = cv::imdecode(cv::Mat(1, [data length], CV_LOAD_IMAGE_COLOR,
                                   (void*)data.bytes),
                           CV_LOAD_IMAGE_UNCHANGED);
    cv::cvtColor(cvImage, cvImage, CV_BGR2RGB);
    if (cvImage.empty()) return;
    //
    cv::Mat cvImageGray;
    // Convert the image to grayscale
//    cv::cvtColor(cvImage, cvImageGray, CV_RGBA2GRAY);
    // Apply Gaussian filter to remove small edges
//    cv::GaussianBlur(cvImageGray, cvImageGray,cv::Size(5, 5), 1.2, 1.2);
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    //
    switch (self.segementedCtr.selectedSegmentIndex) {
        case 0:
            [self.photoButton setTitle:@"Photo" forState:UIControlStateNormal];
            break;
        case 1://JPG
        {
            [self.photoButton setTitle:[NSString stringWithFormat:@"%ld",oriLen] forState:UIControlStateNormal];
            //
            std::vector<int> params;
            params.push_back(CV_IMWRITE_JPEG_QUALITY);
            params.push_back(10);   // that's percent, so 100 == no compression, 1 == full
//            params.push_back(0);
            //
            NSString *imgFilePath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"savedImage.jpg"]];
            const char* cPath = [imgFilePath cStringUsingEncoding:NSMacOSRomanStringEncoding];
            //length
            unsigned long length = [[NSData dataWithContentsOfFile:imgFilePath] length];
            char buffer[1000];
            int bufferlength = sprintf(buffer, "JPG:%lu",length);
            //
            const cv::string newPaths = (const cv::string)cPath;
            cv::putText(cvImage, buffer, cvPoint(50, 50), cv::FONT_ITALIC, 2, cv::Scalar::all(255),3,8);
            cv::imwrite(newPaths, cvImage);
            buffer[bufferlength]=0;//clear
//            [self saveImage:self.photo imageType:@"jpg"];
            NSLog(@"NSData length(JPG compression):%lu",length);
        }
        break;
        case 2://PNG
        {
            [self.photoButton setTitle:[NSString stringWithFormat:@"%ld",oriLen] forState:UIControlStateNormal];
            //
            std::vector<int> params;
            params.push_back(CV_IMWRITE_PNG_COMPRESSION);
            params.push_back(9);   // that's compression level, 9 == full , 0 == none
//            params.push_back(0);
            //@see: http://stackoverflow.com/questions/13704667/opencv-imwrite-increases-the-size-of-png-image
            params.push_back(CV_IMWRITE_PNG_STRATEGY);
            params.push_back(cv::IMWRITE_PNG_STRATEGY_RLE);
            //
            NSString *imgFilePath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"savedImage.png"]];
            const char* cPath = [imgFilePath cStringUsingEncoding:NSMacOSRomanStringEncoding];
            unsigned long length = [[NSData dataWithContentsOfFile:imgFilePath] length];
            char buffer[1000];
            int bufferlength = sprintf(buffer, "PNG:%lu",length);
            const cv::string newPaths = (const cv::string)cPath;
            cv::putText(cvImage, buffer, cvPoint(50, 50), cv::FONT_ITALIC, 2, cv::Scalar::all(255),3,8);
            cv::imwrite(newPaths, cvImage);
            buffer[bufferlength]=0;//clear
            NSLog(@"NSData length(PNG compression):%lu",(unsigned long)[[NSData dataWithContentsOfFile:imgFilePath] length]);
#warning out
//            [self saveImage:self.photo imageType:@"png"];
        }
        break;
    }
    // Convert cv::Mat to UIImage* and show the resulting image
    UIImage *anewImage = MatToUIImage(cvImage);
//    self.photo = MatToUIImage(cvImage);
    [self.photoButton setBackgroundImage:anewImage forState:UIControlStateNormal];
}

- (IBAction)choosePhoto:(id)sender
{
    if (self.segementedCtr.selectedSegmentIndex) {
        return;//Avoid false selection.
    }
    //
    UIActionSheet *choosePhotoActionSheet;
    
    if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
        choosePhotoActionSheet = [[UIActionSheet alloc] initWithTitle:NSLocalizedString(@"choose_photo", @"")
                                                             delegate:self
                                                    cancelButtonTitle:NSLocalizedString(@"cancel", @"")
                                               destructiveButtonTitle:nil
                                                    otherButtonTitles:NSLocalizedString(@"take_photo_from_camera", @""), NSLocalizedString(@"take_photo_from_library", @""), nil];
    } else {
        choosePhotoActionSheet = [[UIActionSheet alloc] initWithTitle:NSLocalizedString(@"choose_photo", @"")
                                                             delegate:self
                                                    cancelButtonTitle:NSLocalizedString(@"cancel", @"")
                                               destructiveButtonTitle:nil
                                                    otherButtonTitles:NSLocalizedString(@"take_photo_from_library", @""), nil];
    }
    
    [choosePhotoActionSheet showInView:self.view];
//    [choosePhotoActionSheet release];
}

#pragma mark - UIActionSheetDelegate

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
	UIImagePickerControllerSourceType sourceType =UIImagePickerControllerSourceTypePhotoLibrary;
    if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
        switch (buttonIndex) {
            case 0:
                sourceType = UIImagePickerControllerSourceTypeCamera;
                break;
            case 1:
                sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
                break;
            case 2:
                return;
        }
    } else {
        if (buttonIndex == 1) {
            return;
        } else {
            sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
        }
    }
    
	UIImagePickerController *imagePickerController = [[UIImagePickerController alloc] init];
	imagePickerController.delegate = self;
	imagePickerController.allowsEditing = YES;
    imagePickerController.sourceType = sourceType;
	[self presentModalViewController:imagePickerController animated:YES];
}


#pragma mark - UIImagePickerControllerDelegate

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
	[picker dismissModalViewControllerAnimated:YES];
	self.photo = [info objectForKey:UIImagePickerControllerEditedImage];
    //
    [self.photoButton setBackgroundImage:self.photo forState:UIControlStateNormal];
    //
//    [self saveImage:self.photo imageType:@"png"];
}


- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
	[self dismissModalViewControllerAnimated:YES];
}

#pragma mark - SaveAndLoad UIImage
- (void)saveImage: (UIImage*)image imageType:(NSString*)imageTypeStr
{
    if (image != nil)
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                             NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString* path = [[documentsDirectory stringByAppendingPathComponent:
                          @"test." ] stringByAppendingString:imageTypeStr];
        NSData* data = UIImagePNGRepresentation(image);
        [data writeToFile:path atomically:YES];
    }
}

- (UIImage*)loadImage: imageType:(NSString*)imageTypeStr
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                         NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString* path = [[documentsDirectory stringByAppendingPathComponent:
                      @"test."] stringByAppendingString:imageTypeStr];
    UIImage* image = [UIImage imageWithContentsOfFile:path];
    return image;
}
@end
