//
//  FaceAging.h
//  CppWrapper_FaceAging
//
//  Created by yangboz on 14-7-10.
//  Copyright (c) 2014年 GODPAPER. All rights reserved.
//
extern "C" bool vFace(char* imageName, char* modelName, char* trainModelName, char* vFaceOutName);
extern "C" bool faceAging(char* imageName, char* modelName, char* agingFaceOutName);
extern "C" bool findFace(char* imageName, char* modelName, char* findFaceOutName);