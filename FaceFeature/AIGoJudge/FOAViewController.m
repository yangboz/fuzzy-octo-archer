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
    NSLog(@"segmentedButtonChanged to %ld",(long)self.segementedCtr.selectedSegmentIndex);
    //NULL check!
    if (self.photo==NULL) {
        return;
    }
    // Create paths to output images
    NSString  *pngPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/Test.png"];
    NSString  *jpgPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/Test.jpg"];
    
    // Write a UIImage to JPEG with minimum compression (best quality)
    // The value 'image' must be a UIImage object
    // The value '1.0' represents image compression quality as value from 0.0 to 1.0
    [UIImageJPEGRepresentation(self.photo, 1.0) writeToFile:jpgPath atomically:YES];
    
    // Write image to PNG
    [UIImagePNGRepresentation(self.photo) writeToFile:pngPath atomically:YES];
    
    // Let's check to see if files were successfully written...
    
    // Create file manager
    NSError *error;
    NSFileManager *fileMgr = [NSFileManager defaultManager];
    
    // Point to Document directory
    NSString *documentsDirectory = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
    
    // Write out the contents of home directory to console
    NSLog(@"Documents directory: %@", [fileMgr contentsOfDirectoryAtPath:documentsDirectory error:&error]);
    //
    //FaceFeatures testing code:

    //
//    NSString *imageFilePath = [[NSBundle mainBundle] pathForResource:@"1" ofType:@"jpg"];
//     NSString *imageFilePath = jpgPath;
    //
//    NSString *outputFilePath = [[NSBundle mainBundle] pathForResource:@"agingFace" ofType:@"png"];
//    NSString *outputFilePath = [NSString stringWithFormat:@"%u/Documents/tmpAgingFace%@", NSUserDirectory, @".jpg"];
    NSString *outputFilePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/tmpFaceAging.jpg"];
    /*
     //Load a test image:
     NSString* filePath = [[NSBundle mainBundle] pathForResource:@"chessboard_page01" ofType:@"jpg"];
     // Create file handle
     NSFileHandle* handle =
     [NSFileHandle fileHandleForReadingAtPath:filePath];
     // Read content of the file
     data = [handle readDataToEndOfFile];
     */
    //FaceFeatures operation as following
    CGPoint leftEyePosition;
    CGPoint rightEyePosition;
    CGPoint mouthPosition;
    UIImage *faceImage;
    CGRect faceRect;
    //
    switch (self.segementedCtr.selectedSegmentIndex) {
        case 0:
            //Empty photo
            outputFilePath = @"";
            break;
        case 1:
        {
            outputFilePath = jpgPath;
            //
            CIImage *ciimg = [[CIImage alloc] initWithImage:self.photo];
            NSDictionary *detectorOptions = [[NSDictionary alloc] initWithObjectsAndKeys:CIDetectorAccuracyHigh, CIDetectorAccuracy, nil];
            CIDetector *faceDetector;
            faceDetector = [CIDetector detectorOfType:CIDetectorTypeFace context:nil options:detectorOptions];
            NSArray *features = [faceDetector featuresInImage:ciimg];
            for ( CIFaceFeature *faceFeature in features ) {
                faceRect = [faceFeature bounds];
                // value offeset calculate:
                CGFloat wRatio = self.photo.size.width / 300;
                CGFloat hRatio = self.photo.size.height /400;
                //
                faceRect.origin.y = self.photo.size.height - faceRect.origin.y - faceRect.size.height;
                faceRect.origin.y /= wRatio;
                faceRect.origin.x = self.photo.size.width - faceRect.origin.x - faceRect.size.width;
                faceRect.origin.x /= hRatio;
                //
                CGImageRef cgimg = CGImageCreateWithImageInRect(self.photo.CGImage, faceRect);
                faceImage = [UIImage imageWithCGImage:cgimg];
                 //
//                 CGFloat faceWidth = faceFeature.bounds.size.width;
                //Features set and draw
                if(faceFeature.hasLeftEyePosition)
                {
                    leftEyePosition = faceFeature.leftEyePosition;
                    //
                    NSLog(@"Left eye %f %f", leftEyePosition.x, leftEyePosition.y);
                }
                if(faceFeature.hasRightEyePosition)
                {
                    rightEyePosition = faceFeature.rightEyePosition;
                }
                if(faceFeature.hasMouthPosition)
                {
                    mouthPosition = faceFeature.mouthPosition;
                }
                // Write a UIImage to JPEG with minimum compression (best quality)
                // The value 'image' must be a UIImage object
                // The value '1.0' represents image compression quality as value from 0.0 to 1.0
                [UIImageJPEGRepresentation(self.photo, 1.0) writeToFile:outputFilePath atomically:YES];
                
                // Write image to PNG
//                [UIImagePNGRepresentation(image) writeToFile:pngPath atomically:YES];
                
                // Let's check to see if files were successfully written...
                
                // Create file manager
                NSError *error;
                NSFileManager *fileMgr = [NSFileManager defaultManager];
                
                // Point to Document directory
                NSString *documentsDirectory = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
                
                // Write out the contents of home directory to console
                NSLog(@"Documents directory: %@", [fileMgr contentsOfDirectoryAtPath:documentsDirectory error:&error]);
            }
        }
        break;//OpenCV
        case 2:
        {
            self.photo = [self openCvOprWithImage:self.photo];
        }
        break;
        case 3:
        {

        }
    }
//    self.photo = [[UIImage  alloc] initWithContentsOfFile:outputFilePath];
//    self.photo = [self drawWithCGRectOnImage:faceRect onImage:self.photo];
//    self.photo = [self drawWithCGPointOnImage:leftEyePosition onImage:self.photo];
//    self.photo = [self drawWithCGPointOnImage:leftEyePosition onImage:faceImage];
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


#pragma mark -Utility funcs
- (UIImage *)drawWithCGRectOnImage:(CGRect)rect onImage:(UIImage *)image{
    // Convert UIImage* to cv::Mat
    UIImageToMat(self.photo, cvImage);
    //
    NSData* data = UIImagePNGRepresentation(image);
    // Decode image from the data buffer
    cvImage = cv::imdecode(cv::Mat(1, [data length], CV_8UC3,(void*)data.bytes),CV_LOAD_IMAGE_UNCHANGED);
//    assert(cvImage.empty());
    // draw the circle outline
//    cv::circle( cvImage, center, rect.size.width, cv::Scalar(0,255,0), 1, 20, 0 );
    cv::rectangle(cvImage, cvRect(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height), cv::Scalar(0,255,255));
    // Convert cv::Mat to UIImage* and show the resulting image
    UIImage *newImage = MatToUIImage(cvImage);
    //
    return newImage;
}
//
- (UIImage *)drawWithCGPointOnImage:(CGPoint)point onImage:(UIImage *)image{
    // Convert UIImage* to cv::Mat
    UIImageToMat(self.photo, cvImage);
    //
    NSData* data = UIImagePNGRepresentation(image);
    // Decode image from the data buffer
    cvImage = cv::imdecode(cv::Mat(1, [data length], CV_8UC3,(void*)data.bytes),CV_LOAD_IMAGE_UNCHANGED);
    //    assert(cvImage.empty());
    // draw the circle center
    // value offeset calculate:
    //    CGFloat wRatio = self.photo.size.width / 300;
    //    CGFloat hRatio = self.photo.size.height /400;
    //    CGFloat xPos = point.x/wRatio;
    //    CGFloat yPos = point.y/hRatio;
    cv::Point center(point.x,point.y);
    // draw the circle outline
    cv::circle( cvImage, center, 20, cv::Scalar(0,255,0), 1, 20, 0 );
    // Convert cv::Mat to UIImage* and show the resulting image
    UIImage *newImage = MatToUIImage(cvImage);
    //
    return newImage;
}
//
- (UIImage *)openCvOprWithImage:(UIImage *)image{
    //OpenCV related
    std::vector<cv::Rect> faces;
    cv::CascadeClassifier face_cascade;
    // Convert UIImage* to cv::Mat
    UIImageToMat(self.photo, cvImage);
    //
    NSData* data = UIImagePNGRepresentation(self.photo);
    // Decode image from the data buffer
    cvImage = cv::imdecode(cv::Mat(1, [data length], CV_8UC3,(void*)data.bytes),CV_LOAD_IMAGE_UNCHANGED);
    //@see http://docs.opencv.org/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html
    /** Global variables */
    NSString *face_cascade_name = [[NSBundle mainBundle] pathForResource:@"haarcascade_frontalface_alt_tree" ofType:@"xml"];
    NSString *eyes_cascade_name = [[NSBundle mainBundle] pathForResource:@"haarcascade_eye" ofType:@"xml"];
    NSString *smile_cascade_name = [[NSBundle mainBundle] pathForResource:@"haarcascade_smile" ofType:@"xml"];
    //
    
    cv::CascadeClassifier eyes_cascade;
    cv::CascadeClassifier smile_cascade;
    //-- 1. Load the cascades
    if (!face_cascade.load((std::string)[face_cascade_name UTF8String])){ NSLog(@"face_cascade(!)Error loading\n"); };
    if (!eyes_cascade.load((std::string)[eyes_cascade_name UTF8String])){ NSLog(@"eyes_cascade(!)Error loading\n");  };
    if (!smile_cascade.load((std::string)[smile_cascade_name UTF8String])){ NSLog(@"smile_cascade(!)Error loading\n");};
//
cv::cvtColor(cvImage, cvImageGray, CV_BGR2GRAY);
cv::equalizeHist(cvImageGray, cvImageGray);
//-- Detect faces
face_cascade.detectMultiScale(cvImageGray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

for (size_t i = 0; i < faces.size(); i++)
{
    cv::Point center(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
    //ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
    cv::rectangle(cvImage, cv::Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height), cv::Scalar(255, 0, 255));
    
    //
    cv::Mat faceROI = cvImageGray(faces[i]);
     std::vector<cv::Rect> eyes;
     
     //-- In each face, detect eyes
     eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
     
     for (size_t j = 0; j < eyes.size(); j++)
     {
     cv::Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
     int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
         cv::circle(cvImage, center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
     }
     
     std::vector<cv::Rect> smiles;
     
     //-- In each face, detect smiles
     smile_cascade.detectMultiScale(faceROI, smiles, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
     
     for (size_t k = 0; k < smiles.size(); k++)
     {
     cv::Point center(faces[i].x + smiles[k].x + smiles[k].width*0.5, faces[i].y + smiles[k].y + smiles[k].height*0.5);
     //int radius = cvRound((smiles[k].width + smiles[k].height)*0.125);
     //circle(frame, center, radius, cv::Scalar(255, 100, 0), 4, 8, 0);
     cv::ellipse(cvImage, center, cv::Size(smiles[k].width*0.5, smiles[k].height*0.5), 0, 0, 360, cv::Scalar(255, 200, 0), 4, 8, 0);
     }
     
    }
    // Convert cv::Mat to UIImage* and show the resulting image
    UIImage *newImage = MatToUIImage(cvImage);
    //
    return newImage;
}
@end
