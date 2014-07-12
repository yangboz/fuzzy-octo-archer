//
//  FaceAgingWrapper.m
//  CppWrapper_FaceAging
//
//  Created by yangboz on 14-7-10.
//  Copyright (c) 2014年 Daniel Nestor Corbatta Barreto. All rights reserved.
//

#import "FaceAgingWrapper.h"
#import "FaceAging.h"

@interface FaceAgingWrapper(){
    FaceAging * faceAging;
}

@end
@implementation FaceAgingWrapper
@synthesize _imageName,_trackModelName,_trainedModeleName;
//
-(id)init;
{
    if (self = [super init]) {
        //FaceAging
        faceAging = new FaceAging();
    }
    return self;
}
//
-(void)faceAging:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andAgingFaceOutputName:(NSString *) agingFaceOutputName
{
    NSLog(@"faceAging params,imageName:%@,trackModelName:%@,agingFaceOutputName:%@",imageName, trackModelName, agingFaceOutputName);
    //    char	*strncpy(char *, const char *, size_t);
    ///imageNameChar
    const char *const_imageNameChar = [imageName UTF8String];
    char *imageNameChar = new char[strlen(const_imageNameChar)+1];
    strcpy(imageNameChar, const_imageNameChar);
    ///trackModelNameChar
    const char *const_trackModelNameChar = [trackModelName UTF8String];
    char *trackModelNameChar = new char[strlen(const_trackModelNameChar)];
    strcpy(trackModelNameChar, const_trackModelNameChar);
    ///agingFaceOutputNameChar
    const char *const_agingFaceOutputNameChar = [agingFaceOutputName UTF8String];
    char *agingFaceOutputNameChar = new char[strlen(const_agingFaceOutputNameChar)];
    strcpy(agingFaceOutputNameChar, const_agingFaceOutputNameChar);
    //
    faceAging->faceAging(imageNameChar, trackModelNameChar, agingFaceOutputNameChar);
}
-(void)findFace:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andFindFaceOutputName:(NSString *) findFaceOutputName
{
    NSLog(@"findFace params,imageName:%@,trackModelName:%@,findFaceOutputName:%@",imageName, trackModelName, findFaceOutputName);
    ///imageNameChar
    const char * const_imageNameChar = [imageName UTF8String];
    char *imageNameChar = new char[strlen(const_imageNameChar)+1];
    strcpy(imageNameChar, const_imageNameChar);
    ///trackModelNameChar
    const char * const_trackModelNameChar = [trackModelName UTF8String];
    char * trackModelNameChar = new char[strlen(const_trackModelNameChar)+1];
    strcpy(trackModelNameChar, const_trackModelNameChar);
    //findFaceOutputNameChar
    const char * const_findFaceOutputNameStr = [findFaceOutputName UTF8String];
    char * findFaceOutputNameChar = new char[strlen(const_findFaceOutputNameStr)+1];
    strcpy(findFaceOutputNameChar, const_findFaceOutputNameStr);
    //
    faceAging->findFace(imageNameChar, trackModelNameChar, findFaceOutputNameChar);
}
-(void)vFace:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andTrainedModeleName:(NSString *) trainedModeleName andvFaceOutputName:(NSString *) vFaceOutputName
{
    NSLog(@"vFace params,imageName:%@,trackModelName:%@,trainedModeleName:%@,vFaceOutputName:%@",imageName, trackModelName, trainedModeleName, vFaceOutputName);
    ///imageNameChar
    const char * const_imageNameChar = [imageName UTF8String];
    char *imageNameChar = new char[strlen(const_imageNameChar)+1];
    strcpy(imageNameChar, const_imageNameChar);
    ///trackModelNameChar
    const char * const_trackModelNameChar = [trackModelName UTF8String];
    char * trackModelNameChar = new char[strlen(const_trackModelNameChar)+1];
    strcpy(trackModelNameChar, const_trackModelNameChar);
    ///trainedModeleNameChar
    const char * const_trainedModeleNameChar = [trainedModeleName UTF8String];
    char * trainedModeleNameChar = new char[strlen(const_trainedModeleNameChar)+1];
    strcpy(trainedModeleNameChar, const_trainedModeleNameChar);
    ///vFaceOutputNameChar
    const char * const_vFaceOutputNameChar  = [vFaceOutputName UTF8String];
    char * vFaceOutputNameChar = new char[strlen(const_vFaceOutputNameChar)+1];
    strcpy(vFaceOutputNameChar, const_vFaceOutputNameChar);
    //
    faceAging->vFace(imageNameChar, trackModelNameChar, trainedModeleNameChar, vFaceOutputNameChar);
}
//
@end