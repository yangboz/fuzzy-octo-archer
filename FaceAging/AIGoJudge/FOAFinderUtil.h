//
//  FOAFinderUtil.h
//  AIGoJudge
//
//  Created by yangboz on 14-7-9.
//  Copyright (c) 2014年 Alexander Shishkov & Kirill Kornyakov. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FOAFinderUtil : NSObject
{}
-(CvSeq*)findRectangle:(IplImage *)img withStorage:(CvMemStorage *)storage;
-(void)drawRectangle: (IplImage*)img withSquares:(CvSeq*)squares;
@end
