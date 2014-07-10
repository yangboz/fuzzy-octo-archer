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
    //
    std::string imageNameStr;
    std::string trackModelNameStr;
    std::string trainedModeleNameStr;
    char * imageNameChar;
    char * trackModelNameChar;
    char * trainedModeleNameChar;
    //
    char* findFaceOutputName;
    char* agingFaceOutputName;
    char* vFaceOutputName;
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
        imageNameStr = std::string([imageName UTF8String]);
        trackModelNameStr = std::string([trackModelName UTF8String]);
        trainedModeleNameStr = std::string([trainedModeleName UTF8String]);
        faceAging = new FaceAging(imageNameStr,trackModelNameStr,trainedModeleNameStr);
        //
        strcpy(imageNameChar,imageNameStr.c_str());
        strcpy(trackModelNameChar,trackModelNameStr.c_str());
        strcpy(trainedModeleNameChar,trainedModeleNameStr.c_str());
        //
        findFaceOutputName = "foundFace.png";
        agingFaceOutputName = "agingFace.png";
        vFaceOutputName = "vFace.png";
    }
    return self;
}
//
-(void)faceAging
{
    //
    faceAging->faceAging(imageNameChar, trackModelNameChar, agingFaceOutputName);
}
-(void)findFace
{
    //
    faceAging->findFace(imageNameChar, trackModelNameChar, findFaceOutputName);
}
-(void)vFace
{
    //
    faceAging->vFace(imageNameChar, trackModelNameChar, trainedModeleNameChar, vFaceOutputName);
}
//
@end