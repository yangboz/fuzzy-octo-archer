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
-(id)init;
//
-(void)faceAging:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andAgingFaceOutputName:(NSString *) agingFaceOutputName;
-(void)findFace:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andFindFaceOutputName:(NSString *) findFaceOutputName;
-(void)vFace:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andTrainedModeleName:(NSString *) trainedModeleName andvFaceOutputName:(NSString *) vFaceOutputName;
//
@end
