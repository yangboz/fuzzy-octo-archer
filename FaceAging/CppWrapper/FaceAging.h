//
//  FaceAging.h
//  CppWrapper_FaceAging
//
//  Created by yangboz on 14-7-10.
//  Copyright (c) 2014年 GODPAPER. All rights reserved.
//

#ifndef CppWrapper_FaceAging_FaceAging_h
#define CppWrapper_FaceAging_FaceAging_h

#include "FaceAging.h"

#include "Tracker.h"
#include "imgwarp_mls.h"
#include "imgwarp_piecewiseaffine.h"

using namespace cv;
using namespace FACETRACKER;

#include <iostream>
class FaceAging
{
public:								//constructor always have same name as class
	//FaceAging();							//constructor with 0 input
	FaceAging();
    //
    bool faceAging(char* imageName, char* modelName, char* agingFaceOutName);
    bool findFace(char* imageName, char* modelName, char* findFaceOutName);
    bool vFace(char* imageName, char* modelName, char* trainModelName, char* vFaceOutName);
private:
    //
    void Draw(cv::Mat &image,cv::Mat &shape, cv::Mat &visi);
    void Draw1(cv::Mat &image,cv::Mat &shape,cv::Mat &visi);
    void faceShape(cv::Mat &shape, std::vector <Point2f>& faceOutline);
    void getTexture2(cv::Mat &shape, std::vector <Point2f>& texture2);
    void getTexture3(cv::Mat &shape, std::vector <Point2f>& texture3);
    void getTexture4(cv::Mat &shape, std::vector <Point2f>& texture4);
    void getTexture5(cv::Mat &shape, std::vector <Point2f>& texture5);
    void getTexture6(cv::Mat &shape, std::vector <Point2f>& texture6);
    void _CalcSimT(cv::Mat &src,cv::Mat &dst,
                   double &a,double &b,double &tx,double &ty);
    void _SimT(cv::Mat &s,double a,double b,double tx,double ty);
    void Mat2WarpPoint(const cv::Mat& res, vector< cv::Point2i > &WarpPointV);
    void _knn(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, int K, cv::Mat& newparam);
    void getResult(Mat img, Mat newImg, bool bDeformPic, int imgWarpAlg, vector< cv::Point2i >& fittedPointV, vector< cv::Point2i >& newPV, char* vFaceOutName);
    
};


#endif
