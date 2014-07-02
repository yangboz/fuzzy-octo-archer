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
    //
    switch (self.segementedCtr.selectedSegmentIndex) {
        case 0:
            //Empty hanlder
            break;
        case 1:
            // Decode image from the data buffer
            cvImage = cv::imdecode(cv::Mat(1, [data length], CV_8UC1,
                                               (void*)data.bytes),
                                       CV_LOAD_IMAGE_UNCHANGED);
            
            if (!cvImage.empty())
            {
                cv::Mat gray;
                // Convert the image to grayscale
                cv::cvtColor(cvImage, gray, CV_RGBA2GRAY);
                // Apply Gaussian filter to remove small edges
                cv::GaussianBlur(gray, gray,
                                 cv::Size(5, 5), 1.2, 1.2);
                // Calculate edges with Canny
                cv::Mat edges;
                cv::Canny(gray, edges, 0, 50);
                // Fill image with white color
                cvImage.setTo(cv::Scalar::all(255));
                // Change color on edges
                cvImage.setTo(cv::Scalar(0, 128, 255, 255), edges);
            }
                        break;
//@see http://docs.opencv.org/doc/tutorials/calib3d/camera_calibration_square_chess/camera_calibration_square_chess.html
        case 2:
            // Decode image from the data buffer
            cvImage = cv::imdecode(cv::Mat(1, [data length], CV_8UC1,(void*)data.bytes),CV_LOAD_IMAGE_UNCHANGED);
            if (!cvImage.empty())
            {
                cv::Mat gray;
                // Convert the image to grayscale
                cv::cvtColor(cvImage, gray, CV_RGBA2GRAY);
                //Detect a chessboard in this image using findChessboard function.
                cv::vector<cv::Point2f> imageCorners;
                BOOL found = cv::findChessboardCorners(cvImage, cv::Size(cvImage.cols, cvImage.rows), imageCorners,CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
                
                NSLog(@"findChessboardCorners result:%d",(int)found);
            }
            break;
    }
    // Convert cv::Mat to UIImage* and show the resulting image
    self.photo = MatToUIImage(cvImage);
    [self.photoButton setBackgroundImage:self.photo forState:UIControlStateNormal];
}

- (IBAction)choosePhoto:(id)sender
{
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
}


- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
	[self dismissModalViewControllerAnimated:YES];
}

@end
