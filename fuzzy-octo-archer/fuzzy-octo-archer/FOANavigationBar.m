//
//  FOANavigationBar.m
//  fuzzy-octo-archer
//
//  Created by yangboz on 14-6-26.
//  Copyright (c) 2014年 GODPAPER. All rights reserved.
//

#import "FOANavigationBar.h"

@implementation FOANavigationBar

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}
- (void)awakeFromNib {
    [super awakeFromNib];
    
    // draw shadow underneath navigationBar
    self.layer.masksToBounds = NO;
    self.layer.shadowOffset = CGSizeMake(0, 3);
    self.layer.shadowOpacity = 0.8;
    self.layer.shadowPath = [UIBezierPath bezierPathWithRect:self.bounds].CGPath;
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
