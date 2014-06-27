//
//  FOAViewController.h
//  fuzzy-octo-archer
//
//  Created by yangboz on 14-6-26.
//  Copyright (c) 2014年 GODPAPER. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "FOANavigationBar.h"

@interface FOAViewController : UIViewController <UIImagePickerControllerDelegate>
{
}
//IBOutlets
@property(nonatomic, retain) UIImage *photo;
@property (retain, nonatomic) IBOutlet UIButton *photoButton;
@property(nonatomic,retain) IBOutlet FOANavigationBar *foaNavigationBar;
@property(nonatomic,retain) IBOutlet UISegmentedControl *segementedCtr;
//IBActions
- (IBAction)uploadPhoto:(id)sender;
- (IBAction)choosePhoto:(id)sender;
- (IBAction)segmentedButtonChanged:(id)sender;
@end
