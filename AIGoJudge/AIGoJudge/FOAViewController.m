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
    // Decode image from the data buffer
    cvImage = cv::imdecode(cv::Mat(1, [data length], CV_8UC1,
                                   (void*)data.bytes),
                           CV_LOAD_IMAGE_UNCHANGED);
    if (cvImage.empty()) return;
    //
    cv::Mat cvImageGray;
    // Convert the image to grayscale
    cv::cvtColor(cvImage, cvImageGray, CV_RGBA2GRAY);
    // Apply Gaussian filter to remove small edges
    cv::GaussianBlur(cvImageGray, cvImageGray,cv::Size(5, 5), 1.2, 1.2);
    //
    switch (self.segementedCtr.selectedSegmentIndex) {
        case 0:
            //Empty hanlder
            break;
        case 1:
        {
            // Calculate edges with Canny
            cv::Mat edges;
            cv::Canny(cvImageGray, edges, 0, 50);
            // Fill image with white color
            cvImage.setTo(cv::Scalar::all(255));
            // Change color on edges
            cvImage.setTo(cv::Scalar(0, 128, 255, 255), edges);
        }
        break;
        case 2:
            //@see http://docs.opencv.org/doc/tutorials/calib3d/camera_calibration_square_chess/camera_calibration_square_chess.html
        {
            //Detect a chessboard in this image using findChessboard function.
            int board_w = 8; // Board width in squares
            int board_h = 5; // Board height
            // Dimensions of board
            cv::Size board_sz = cv::Size( board_h, board_w );
            //
            cv::vector<cv::Point2f> imageCorners;
            //
            BOOL found = cv::findChessboardCorners(cvImageGray, board_sz, imageCorners,CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
            //
            NSLog(@"findChessboardCorners result:%d",(int)found);
            //HoughLine detection
            /*
            std::vector<cv::Vec2f> lines;
            cv::HoughLines(cvImageGray, lines, 1, CV_PI/180, 100);
            //
            std::vector<cv::Vec2f>::iterator it = lines.begin();
            for(; it!=lines.end(); ++it) {
                float rho = (*it)[0], theta = (*it)[1];
                cv::Point pt1, pt2;
                double a = cos(theta), b = sin(theta);
                double x0 = a*rho, y0 = b*rho;
                pt1.x = cv::saturate_cast<int>(x0 + 1000*(-b));
                pt1.y = cv::saturate_cast<int>(y0 + 1000*(a));
                pt2.x = cv::saturate_cast<int>(x0 - 1000*(-b));
                pt2.y = cv::saturate_cast<int>(y0 - 1000*(a));
                //cv::line(cvImage, pt1, pt2, cv::Scalar(0,0,255), 0.1, CV_AA);
            }
             */
            //HoughLinesP detection
            /*
            std::vector<cv::Vec4i> lines;
            cv::HoughLinesP(cvImageGray, lines, 1, CV_PI/180, 50, 50, 100);
            
            std::vector<cv::Vec4i>::iterator it = lines.begin();
            for(; it!=lines.end(); ++it) {
                cv::Vec4i l = *it;
                cv::line(cvImage, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 0.01, CV_AA);
            }
             */
            //! finds circles in the grayscale image using 2+1 gradient Hough transform
            //HoughLinesCircle
            std::vector<cv::Vec3f> circles;
            cv::HoughCircles(cvImageGray, circles, CV_HOUGH_GRADIENT,
                         2, cvImageGray.rows/4, 200, 100 );
            for( size_t i = 0; i < circles.size(); i++ )
            {
                cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                // draw the circle center
                cv::circle( cvImage, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
                // draw the circle outline
                cv::circle( cvImage, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
            }
        }
        break;
    }
    // Convert cv::Mat to UIImage* and show the resulting image
    self.photo = MatToUIImage(cvImage);
    [self.photoButton setBackgroundImage:self.photo forState:UIControlStateNormal];
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
}


- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
	[self dismissModalViewControllerAnimated:YES];
}

@end
