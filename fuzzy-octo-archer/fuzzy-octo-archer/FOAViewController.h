//
//  FOAViewController.h
//  fuzzy-octo-archer
//
//  Created by yangboz on 14-6-26.
//  Copyright (c) 2014年 GODPAPER. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "FOANavigationBar.h"
#import "SegmentButtonView.h"

@interface FOAViewController : UIViewController <SegmentButtonViewDelegate,UIImagePickerControllerDelegate>
{
}
//IBOutlets
@property(nonatomic, retain) UIImage *photo;
@property (retain, nonatomic) IBOutlet UIButton *photoButton;
@property(nonatomic,retain) IBOutlet FOANavigationBar *foaNavigationBar;
//IBActions
- (IBAction)uploadPhoto:(id)sender;
- (IBAction)choosePhoto:(id)sender;
@end
