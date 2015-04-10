//
//  LandMark.h
//  CppWrapper_LandMark
//
//  Created by yangboz on 15-1-29.
//  Copyright (c) 2015年 GODPAPER. All rights reserved.
//
extern "C" void Initialize(
                int width,
                int height,
                CvSize windowSize=cvSize(8, 8),
                int pyramidLevel=3
                );
extern "C" int TrackFeatures(
                  IplImage* prevGrayImage,
                  IplImage* currGrayImage,
                  std::vector<MatchedPoints>* prevPoints,
                  std::vector<MatchedPoints>* currentPoints,
                  int id
                  );
extern "C" void normCrossCorrelation(IplImage *imgI, IplImage *imgJ,
                          CvPoint2D32f *points0,  CvPoint2D32f *points1, int nPts, char *status,
                          float *match, int winsize, int method);
