//
//  FaceAgingWrapper.h
//  CppWrapper_FaceAging
//
//  Created by yangboz on 14-7-10.
//  Copyright (c) 2014年 Daniel Nestor Corbatta Barreto. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FaceAgingWrapper : NSObject
//
@property (nonatomic, strong) NSString * _imageName;
@property (nonatomic, strong) NSString * _trackModelName;
@property (nonatomic, strong) NSString * _trainedModeleName;
//
-(id)initWithImageName:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andTrainedModeleName:(NSString *) trainedModeleName;
//
-(void)faceAging;
-(void)findFace;
-(void)vFace;
//
@end