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
    
}

@end
@implementation FaceAgingWrapper
///
bool vFace(char* imageName, char* modelName, char* trainModelName, char* vFaceOutName);
bool faceAging(char* imageName, char* modelName, char* agingFaceOutName);
bool findFace(char* imageName, char* modelName, char* findFaceOutName);
///
@synthesize _imageName,_trackModelName,_trainedModeleName;
//
-(id)init;
{
    if (self = [super init]) {
        //FaceAging initilization here:
        
    }
    return self;
}
//
-(void)faceAging:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andAgingFaceOutputName:(NSString *) agingFaceOutputName
{
    NSLog(@"faceAging params,imageName:%@,trackModelName:%@,agingFaceOutputName:%@",imageName, trackModelName, agingFaceOutputName);
    //
    faceAging((char *)[imageName UTF8String], (char *)[trackModelName UTF8String], (char *)[agingFaceOutputName UTF8String]);
}
-(void)findFace:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andFindFaceOutputName:(NSString *) findFaceOutputName
{
    NSLog(@"findFace params,imageName:%@,trackModelName:%@,findFaceOutputName:%@",imageName, trackModelName, findFaceOutputName);
    ///imageNameChar
    const char * const_imageNameChar = [imageName UTF8String];
    char *imageNameChar = (char *)const_imageNameChar;
    ///trackModelNameChar
    const char * const_trackModelNameChar = [trackModelName UTF8String];
    char * trackModelNameChar = (char *)const_trackModelNameChar;
    //findFaceOutputNameChar
    const char * const_findFaceOutputNameStr = [findFaceOutputName UTF8String];
    char * findFaceOutputNameChar = (char *)const_findFaceOutputNameStr;
    //
    findFace(imageNameChar, trackModelNameChar, findFaceOutputNameChar);
}
-(void)vFace:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andTrainedModeleName:(NSString *) trainedModeleName andvFaceOutputName:(NSString *) vFaceOutputName
{
    NSLog(@"vFace params,imageName:%@,trackModelName:%@,trainedModeleName:%@,vFaceOutputName:%@",imageName, trackModelName, trainedModeleName, vFaceOutputName);
    ///imageNameChar
    const char * const_imageNameChar = [imageName UTF8String];
    char *imageNameChar = (char *)const_imageNameChar;
    ///trackModelNameChar
    const char * const_trackModelNameChar = [trackModelName UTF8String];
    char * trackModelNameChar = (char *)const_trackModelNameChar;
    ///trainedModeleNameChar
    const char * const_trainedModeleNameChar = [trainedModeleName UTF8String];
    char * trainedModeleNameChar = (char *)const_trainedModeleNameChar;
    ///vFaceOutputNameChar
    const char * const_vFaceOutputNameChar  = [vFaceOutputName UTF8String];
    char * vFaceOutputNameChar = (char *)const_vFaceOutputNameChar;
    //
    vFace(imageNameChar, trackModelNameChar, trainedModeleNameChar, vFaceOutputNameChar);
}
//
@end