//
//  FOAViewController.m
//  fuzzy-octo-archer
//
//  Created by yangboz on 14-6-26.
//  Copyright (c) 2014年 GODPAPER. All rights reserved.
//

#import "FOAViewController.h"
#import "opencv2/highgui/ios.h"
//OCR flags
#define WHITE 1
#define BLACK 0

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
    //OCR
    ///Draw hLine
    std::vector<int> lineRowVector;
    bool preRowColor = WHITE;
    bool curRowColor = WHITE;
    // 画横线
    bool preColColor = WHITE;
    bool curColColor = WHITE;
    int preCol = 0;
    //
    switch (self.segementedCtr.selectedSegmentIndex) {
        case 0:
            //Empty hanlder
            break;
        case 1://@see http://ju.outofmemory.cn/entry/18954
        {
            for (int row = 0; row < cvImage.rows; row++)
            {
                curRowColor = WHITE;
                for (int col = 0; col < cvImage.cols; col++)
                {
                    float color = cvImage.at<uchar>(cv::Point(col, row));
                    // 由于文字和背景色不是纯黑和纯白，小于 100 算有文字的地方，也就是 BLACK LINE
                    if (color < 100) {
                        curRowColor = BLACK;
                        break;
                    }
                }
                // 如果当前行的颜色和上一行不一样的话，就画线
                if (curRowColor != preRowColor) {
                    lineRowVector.push_back(row);
                    for (int col = 0; col < cvImage.cols; col++)
                    {
                        cvImage.at<uchar>(cv::Point(col, row)) = 0;
                    }
                }
                preRowColor = curRowColor;
            }
            //
            for (int i = 0; i < lineRowVector.size(); i += 2)
            {
                if (i + 1 >= lineRowVector.size()) {
                    break;
                }
                for (int col = 0; col < cvImage.cols; col++) {
                    curColColor = WHITE;
                    for (int row = lineRowVector[i] + 1; row < lineRowVector[i + 1]-1; row++)
                    {
                        float color = cvImage.at<uchar>(cv::Point(col, row));
                        if (color < 100)
                        {
                            color = 0;
                            curColColor = BLACK;
                            break;
                        }
                    }
                    if (curColColor != preColColor)
                    {
                        for (int row = lineRowVector[i] + 1; row < lineRowVector[i + 1]-1; row++)
                        {
                            cvImage.at<uchar>(cv::Point(col, row)) = 0;
                        }
                        preCol = col;
                    }
                    preColColor = curColColor;
                }
            }
        }
        break;
        case 2:
        {
            
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
