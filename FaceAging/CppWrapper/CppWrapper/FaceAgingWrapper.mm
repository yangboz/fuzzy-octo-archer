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
- (id) initWithImageName:(NSString *) imageName andTrackModelName:(NSString *) trackModelName andTrainedModeleName:(NSString *) trainedModeleName
{
    if (self = [super init]) {
        self._imageName = imageName;
        self._trackModelName = trackModelName;
        self._trainedModeleName = trainedModeleName;
        //
        std::string imageNameStr = std::string([imageName UTF8String]);
        std::string trackModelNameStr = std::string([trackModelName UTF8String]);
        std::string trainedModeleNameStr = std::string([trainedModeleName UTF8String]);
        faceAging = new FaceAging(imageNameStr,trackModelNameStr,trainedModeleNameStr);
    }
    return self;
}
//
-(void)faceAging
{
    std::string imageNameStr = std::string([self._imageName UTF8String]);
    std::string trackModelNameStr = std::string([self._trackModelName UTF8String]);
    std::string trainedModeleNameStr = std::string([self._trainedModeleName UTF8String]);
    //
    char* __imageName = "1.jpg";
    char* __trackModelName = "face2.tracker";
    //
    char* agingFaceOutputName = "agingFace.png";
    //
    faceAging->faceAging(__imageName, __trackModelName, agingFaceOutputName);
}
-(void)findFace
{
    //
    char* __imageName = "1.jpg";
    char* __trackModelName = "face2.tracker";
    //
    char* findFaceOutputName = "foundFace.png";
    //
    faceAging->findFace(__imageName, __trackModelName, findFaceOutputName);
}
-(void)vFace
{
    //
    char* __imageName = "1.jpg";
    char* __trackModelName = "face2.tracker";
    char* __trainedModeleName = "train.dat";
    //
    char* vFaceOutputName = "vFace.png";
    //
    faceAging->vFace(__imageName, __trackModelName, __trainedModeleName, vFaceOutputName);
}
//
@end