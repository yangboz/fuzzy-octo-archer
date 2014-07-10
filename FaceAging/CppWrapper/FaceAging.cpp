//
//  FaceAging.cpp
//  CppWrapper_FaceAging
//
//  Created by yangboz on 14-7-10.
//  Copyright (c) 2014年 GODPAPER. All rights reserved.
//

#include "FaceAging.h"

#include "Tracker.h"
#include "imgwarp_mls.h"
#include "imgwarp_piecewiseaffine.h"

using namespace cv;
using namespace FACETRACKER;

#define db at<double>
#define SQR(x) x*x

void FaceAging::Draw(cv::Mat &image,cv::Mat &shape, cv::Mat &visi)
{
    int i,n = shape.rows/2; cv::Point p1,p2; cv::Scalar c;
    
    c = CV_RGB(0,255,0);
    //draw points
    char sss[256];
    std::string text;
    for(i = 0; i < n; i++)
    {
        sprintf(sss,"%d", i);
        text = sss;
        if(visi.at<int>(i,0) == 0)continue;
        p1 = cv::Point(shape.at<double>(i,0),shape.at<double>(i+n,0));
        c = CV_RGB(255,0,0);
        cv::circle(image,p1,2,c);
        //cv::putText(image,text,p1,CV_FONT_HERSHEY_SIMPLEX,0.5,CV_RGB(255,255,255));
    }
    return;
}

void FaceAging::Draw1(cv::Mat &image,cv::Mat &shape,cv::Mat &visi)
{
	int i,n = shape.rows/2; cv::Point p1; cv::Scalar c;
	char sss[256];
	std::string text;
	//draw points
	for(i = 0; i < n; i++)
	{
		sprintf(sss,"%d", i);
		text = sss;
		if(visi.at<int>(i,0) == 0)continue;
		p1 = cv::Point(shape.at<double>(i,0),shape.at<double>(i+n,0));
		c = CV_RGB(0,255,0);
		
		cv::circle(image,p1,2,c,-1);
		//cv::putText(image,text,p1,CV_FONT_HERSHEY_SIMPLEX,0.5,CV_RGB(255,255,255));
	}
    
    return;
}
void FaceAging::faceShape(cv::Mat &shape, std::vector <Point2f>& faceOutline)
{
    int i,n = shape.rows/2;
    
    for(i = 0; i < 18; i++)
    {
        cv::Point2f p1;
        p1 = cv::Point2f(shape.at<double>(i,0),shape.at<double>(i+n,0));
        faceOutline.push_back(p1);
    }
	cv::Point2f p2;
	p2 = cv::Point2f(shape.at<double>(26,0),shape.at<double>(26+n,0));
	faceOutline.push_back(p2);
    return;
}

void FaceAging::getTexture2(cv::Mat &shape, std::vector <Point2f>& texture2)
{
	int n = shape.rows/2;
    
	cv::Point2f p1;
	p1 = cv::Point2f(shape.at<double>(21,0),shape.at<double>(21+n,0));
	texture2.push_back(p1);
	cv::Point2f p2;
	p2 = cv::Point2f(shape.at<double>(27,0),shape.at<double>(27+n,0));
	texture2.push_back(p2);
    
    return;
}

void FaceAging::getTexture3(cv::Mat &shape, std::vector <Point2f>& texture3)
{
	int n = shape.rows/2;
    
	cv::Point2f p1;
	p1 = cv::Point2f(shape.at<double>(0,0),shape.at<double>(0+n,0));
	texture3.push_back(p1);
	cv::Point2f p2;
	p2 = cv::Point2f(shape.at<double>(36,0),shape.at<double>(36+n,0));
	texture3.push_back(p2);
    
    return;
}


void FaceAging::getTexture4(cv::Mat &shape, std::vector <Point2f>& texture4)
{
	int n = shape.rows/2;
	cv::Point2f p1;
	p1 = cv::Point2f(shape.at<double>(47,0),shape.at<double>(47+n,0));
	cv::Point2f p2;
	p2 = cv::Point2f(shape.at<double>(54,0),shape.at<double>(54+n,0));
	cv::Point2f p3((p1.x + p2.x)/2, (p1.y + p2.y)/2);
	texture4.push_back(p3);
	texture4.push_back(p2);
    
    
    return;
}

void FaceAging::getTexture5(cv::Mat &shape, std::vector <Point2f>& texture5)
{
	int n = shape.rows/2;
    
	cv::Point2f p1;
	p1 = cv::Point2f(shape.at<double>(31,0),shape.at<double>(31+n,0));
	texture5.push_back(p1);
	cv::Point2f p2;
	p2 = cv::Point2f(shape.at<double>(48,0),shape.at<double>(48+n,0));
	texture5.push_back(p2);
    
    return;
}

void FaceAging::getTexture6(cv::Mat &shape, std::vector <Point2f>& texture6)
{
	int n = shape.rows/2;
    
	cv::Point2f p1;
	p1 = cv::Point2f(shape.at<double>(53,0),shape.at<double>(53+n,0));
	texture6.push_back(p1);
	cv::Point2f p2;
	p2 = cv::Point2f(shape.at<double>(54,0),shape.at<double>(54+n,0));
	cv::Point2f p;
	p.x = 2*p2.x - p1.x;
	p.y = 2*p2.y - p1.y;
    
	texture6.push_back(p);
    
    return;
}

void FaceAging::_CalcSimT(cv::Mat &src,cv::Mat &dst,
               double &a,double &b,double &tx,double &ty)
{
    assert((src.type() == CV_64F) && (dst.type() == CV_64F) &&
           (src.rows == dst.rows) && (src.cols == dst.cols) && (src.cols == 1));
    int i,n = src.rows/2;
    cv::Mat H(4,4,CV_64F,cv::Scalar(0));
    cv::Mat g(4,1,CV_64F,cv::Scalar(0));
    cv::Mat p(4,1,CV_64F);
    cv::MatIterator_<double> ptr1x = src.begin<double>();
    cv::MatIterator_<double> ptr1y = src.begin<double>()+n;
    cv::MatIterator_<double> ptr2x = dst.begin<double>();
    cv::MatIterator_<double> ptr2y = dst.begin<double>()+n;
    for(i = 0; i < n; i++,++ptr1x,++ptr1y,++ptr2x,++ptr2y){
        H.db(0,0) += SQR(*ptr1x) + SQR(*ptr1y);
        H.db(0,2) += *ptr1x; H.db(0,3) += *ptr1y;
        g.db(0,0) += (*ptr1x)*(*ptr2x) + (*ptr1y)*(*ptr2y);
        g.db(1,0) += (*ptr1x)*(*ptr2y) - (*ptr1y)*(*ptr2x);
        g.db(2,0) += *ptr2x; g.db(3,0) += *ptr2y;
    }
    H.db(1,1) = H.db(0,0); H.db(1,2) = H.db(2,1) = -1.0*(H.db(3,0) = H.db(0,3));
    H.db(1,3) = H.db(3,1) = H.db(2,0) = H.db(0,2); H.db(2,2) = H.db(3,3) = n;
    cv::solve(H,g,p,CV_CHOLESKY);
    a = p.db(0,0); b = p.db(1,0); tx = p.db(2,0); ty = p.db(3,0); return;
}

//=============================================================================
void FaceAging::_SimT(cv::Mat &s,double a,double b,double tx,double ty)
{
    assert((s.type() == CV_64F) && (s.cols == 1));
    int i,n = s.rows/2; double x,y;
    cv::MatIterator_<double> xp = s.begin<double>(),yp = s.begin<double>()+n;
    
    for(i = 0; i < n; i++,++xp,++yp)
    {
        x = *xp;
        y = *yp;
        *xp = a*x - b*y + tx;
        *yp = b*x + a*y + ty;
    }
    
    return;
}

void FaceAging::Mat2WarpPoint(const cv::Mat& res, vector< cv::Point2i > &WarpPointV)
{
	int nPoints = res.rows / 2;
	if(WarpPointV.size() != nPoints)
		WarpPointV.resize(nPoints);
	for (int i = 0; i < nPoints ; i++)
    {
        WarpPointV[i].x = res.db(i, 0);
        WarpPointV[i].y = res.db(i + nPoints, 0);
	}
}

void FaceAging::_knn(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, int K, cv::Mat& newparam)
{
	float** nn = new float *[testData.rows * K];
	for(int i = 0; i < K; i++)
	{
		nn[i] = new float[testData.rows * testData.cols];
	}
	std::vector< cv::Mat> nnbors;
	cv::Mat dist(1, K, CV_64F);
	cv::Mat pt(1, testData.cols, CV_64F);
	cv::Mat sumParam(1, testData.cols, CV_64F);
	cv::Mat trainData;
	trainingData.convertTo(trainData, CV_32FC1);
	cv::Mat trainClasses;
	trainingClasses.convertTo(trainClasses, CV_32FC1);
	cv::Mat sample;
	testData.convertTo(sample, CV_32FC1);
	
	cv::KNearest knn(trainData, trainClasses, cv::Mat(), true, K);
    knn.find_nearest(sample, K, 0, (const float**)nn, 0, &dist);
	
	
	
	for(int ii = 0; ii < K; ii++)
	{
		cv::Mat tmp(1, testData.cols, CV_32FC1, nn[ii]);
		nnbors.push_back(tmp);
	}
	
	double sumW = 0;
	for(int ii = 0; ii < K; ii++)
	{
		sumW += 1 /(dist.at<float>(0, ii) + 0.000000000000001);
	}
	//cv::Mat sp;
	//nnbors[1].convertTo(sp, CV_64F);
	
	sumParam.setTo(0);
	for(int ii = 0; ii < K; ii++)
	{
		double w = 1 / (dist.at<float>(0, ii) + 0.000000000000001);
		for(int jj = 0; jj < nnbors[ii].cols; jj++)
		{
			pt.db(0, jj) = nnbors[ii].at<float>(0, jj) * w;
			sumParam.db(0, jj) += pt.db(0, jj);
		}
        
	}
	double ratio = 1 / sumW;
	newparam.setTo(0);
	for(int ii = 0; ii < newparam.cols; ii++)
	{
		newparam.db(0, ii) = sumParam.db(0, ii) * ratio;
	}
	
	
	for(int i = 0; i < K; i++)
	{
		nn[i] = NULL;
		delete[] nn[i];
	}
	delete[] nn;
}


void FaceAging::getResult(Mat img, Mat newImg, bool bDeformPic, int imgWarpAlg, vector< cv::Point2i >& fittedPointV, vector< cv::Point2i >& newPV, char* vFaceOutName)
{
	if (bDeformPic)
	{
        // Actually warp image!
        ImgWarp_MLS *warp;
        if (imgWarpAlg==0)
		{
            warp = new ImgWarp_PieceWiseAffine;
            ((ImgWarp_PieceWiseAffine *) warp)->backGroundFillAlg = ImgWarp_PieceWiseAffine::BGMLS;
        }
        
        warp->setTargetSize(img.cols, img.rows);
        warp->setSize(img.cols, img.rows);
        warp->setSrcPoints(fittedPointV);
        warp->setDstPoints(newPV);
        warp->alpha = 1;
        warp->gridSize = 10;
		warp->calcDelta();
        
		newImg = warp->genNewImg(img, 1);
		//imwrite(vFaceOutName, newImg);
		//imshow("result", newImg);
		//imshow("src", img);
		//waitKey();
		imwrite(vFaceOutName, newImg);
        delete warp;
        
    }
    else
	{
        newImg = img;
    }
}

bool FaceAging::vFace(char* imageName, char* modelName, char* trainModelName, char* vFaceOutName)
{
	
	vector< cv::Point2i > fittedPointV;
	vector< cv::Point2i > newPV;
	bool fcheck = false;
	int fpd = -1;
    
	std::vector<int> wSize2(3);
	wSize2[0] = 11;
	wSize2[1] = 9;
	wSize2[2] = 7;
	int nIter = 5;
	double clamp=3;
	double fTol=0.01;
	FACETRACKER::Tracker model(modelName);
	
	/****************************************************************/
	cv::Mat gray,im;
	im = cv::imread(imageName);
	//cv::flip(im,im,1);
	cv::cvtColor(im,gray,CV_BGR2GRAY);
	cv::Mat shapeO;
	
	cv::Rect R;
	if(model.Track(gray,wSize2,fpd,nIter,clamp,fTol,fcheck, R) == 0)
	{
        /*int idx = model._clm.GetViewIdx();
         Draw(im,model._shape,model._clm._visi[idx]); */
        
		model._shape.copyTo(shapeO);
		Mat2WarpPoint(shapeO, fittedPointV);
		double a1,b1,tx1,ty1,a2,b2,tx2,ty2;
		_CalcSimT(model._shape, model._clm._refs, a1,b1,tx1,ty1);
		_SimT(model._shape, a1,b1,tx1,ty1);
		model._clm._pdm.CalcParams(model._shape, model._clm._plocal, model._clm._pglobl);
        
        
		cv::Mat m(1, model._clm._pdm.nModes(), CV_64F);
		for(int j = 0; j < model._clm._pdm.nModes(); j++)
		{
			m.db(0, j) = model._clm._plocal.db(j, 0);
		}
		/****************************************************************/
		std::vector< cv::Mat> __allParam;
		const char* fname = trainModelName;
		ifstream s(fname);
		assert(s.is_open());
		int size;
		s >> size;
		cv::Mat trainData(size, model._clm._pdm.nModes(), CV_64F);
		cv::Mat trainClasses(size, 1, CV_64F);
		trainClasses.setTo(0);
		
		for(int i = 0; i < size; i++)
		{
			cv::Mat t(1, model._clm._pdm.nModes() + 6, CV_64F);
			IO::ReadMat(s, t);
			__allParam.push_back(t);
		}
		for(int r = 0; r < trainData.rows; r++)
		{
			for(int c = 0; c < trainData.cols; c++)
			{
				trainData.db(r, c) = __allParam[r].db(0, c + 6);
			}
		}
        
        /****************************************************************/
        
		int K = 5;
		cv::Mat newparam(1, trainData.cols, CV_64F);
		_knn(trainData, trainClasses, m, K, newparam);
		//_PrintMatrix(newparam);
        
        /****************************************************************/
		for(int j = 0; j < model._clm._pdm.nModes(); j++)
		{
			model._clm._plocal.db(j, 0) = newparam.db(0, j);
		}
		model._clm._pdm.CalcShape2D(model._shape, model._clm._plocal, model._clm._pglobl);
		_CalcSimT( model._shape, shapeO, a1,b1,tx1,ty1);
		_SimT(model._shape, a1,b1,tx1,ty1);
        
		Mat2WarpPoint(model._shape, newPV);
        
		Mat newImg;
		getResult(im, newImg, 1, 0, fittedPointV, newPV, vFaceOutName);
		//int idx = model._clm.GetViewIdx();
		//Draw(im, shapeO,model._clm._visi[idx]);
		//Draw1(im, model._shape,model._clm._visi[idx]);
		//imshow("Face", newImg);
		//cvWaitKey(0);
		//imwrite(vFaceOutName, newImg);
		return true;
        
	}
	else
	{
		printf("cannot find face \n");
		return false;
	}
	
}

bool FaceAging::findFace(char* imageName, char* modelName, char* findFaceOutName)
{
	std::vector<int> wSize2(3); wSize2[0] = 11; wSize2[1] = 9; wSize2[2] = 7;
	bool fcheck = false;
	int fpd = -1;
	int nIter = 5; double clamp=3,fTol=0.01;
	
	FACETRACKER::Tracker model(modelName);
	Mat gray;
	Mat img = cv::imread(imageName, 1);
	if(img.empty())
	{
		printf("cannot read image \n");
		return false;
	}
	cv::cvtColor(img, gray, CV_BGR2GRAY);
	cv::Rect R;
	if(model.Track(gray,wSize2,fpd,nIter,clamp,fTol,fcheck, R) == 0)
	{
		//int idx = model._clm.GetViewIdx();
		//cv::rectangle(img, R, Scalar(255, 0, 0));
		//Draw(img, model._shape, model._clm._visi[idx]);
        
		RotatedRect rotRect;
		std::vector <Point2f>faceOutline;
		faceShape(model._shape, faceOutline);
		rotRect = fitEllipse(faceOutline);
		ellipse(img,rotRect, Scalar(0, 255, 0), 2, 8 );
		printf("%f, %f %f, %f %f\n", rotRect.center.x, rotRect.center.y, rotRect.size.width, rotRect.size.height, rotRect.angle);
		//imshow("img", img);
		imwrite(findFaceOutName, img);
		//waitKey();
		return true;
	}
	else
	{
		printf("cannot find face \n");
		return false;
	}
	
	
}

bool FaceAging::faceAging(char* imageName, char* modelName, char* agingFaceOutName)
{
	std::vector<int> wSize2(3); wSize2[0] = 11; wSize2[1] = 9; wSize2[2] = 7;
	bool fcheck = false;
	int fpd = -1;
	int nIter = 5; double clamp=3,fTol=0.01;
	
	FACETRACKER::Tracker model(modelName);
	Mat gray;
	Mat img = cv::imread(imageName, 1);
	if(img.empty())
	{
		printf("cannot read image \n");
		return false;
	}
	cv::cvtColor(img, gray, CV_BGR2GRAY);
	cv::Rect R;
	if(model.Track(gray,wSize2,fpd,nIter,clamp,fTol,fcheck, R) == 0)
	{
		//int idx = model._clm.GetViewIdx();
		//cv::rectangle(img, R, Scalar(255, 0, 0));
		//Draw(img, model._shape, model._clm._visi[idx]);
        
		RotatedRect rotRect;
		std::vector <Point2f>faceOutline;
		faceShape(model._shape, faceOutline);
		rotRect = fitEllipse(faceOutline);
		ellipse(img,rotRect, Scalar(0, 255, 0), 2, 8 );
        
        
		Mat rot_mat;
		if(rotRect.angle > 90)
			rot_mat = getRotationMatrix2D(rotRect.center,-rotRect.angle, 1.0);
		else
			rot_mat = getRotationMatrix2D(rotRect.center,180 - rotRect.angle, 1.0);
        
        
		std::vector <Point2f> srcPoint(1);
		srcPoint[0].x = rotRect.center.x;
		srcPoint[0].y = rotRect.center.y + 3.*rotRect.size.height / 8;
		std::vector <Point2f> dstPoint(1);
		cv::transform(srcPoint, dstPoint, rot_mat);
		circle(img, dstPoint[0], 10, Scalar(255, 255, 255), 1);
		circle(img, dstPoint[0], 8, Scalar(255, 255, 255), -1);
        
		std::vector <Point2f>texture2;
		getTexture2(model._shape, texture2);
		Point2f t2((texture2[0].x + texture2[1].x)/2, (texture2[0].y + texture2[1].y)/2);
		circle(img, t2, 10, Scalar(255, 255, 255), 1);
		circle(img, t2, 8, Scalar(255, 255, 255), -1);
        
		std::vector <Point2f>texture3;
		getTexture3(model._shape, texture3);
		Point2f t3((texture3[0].x + texture3[1].x)/2, (texture3[0].y + texture3[1].y)/2);
		circle(img, t3, 10, Scalar(255, 255, 255), 1);
		circle(img, t3, 8, Scalar(255, 255, 255), -1);
        
		std::vector <Point2f>texture4;
		getTexture4(model._shape, texture4);
		Point2f t4((texture4[0].x + texture4[1].x)/2, (texture4[0].y + texture4[1].y)/2);
		circle(img, t4, 10, Scalar(255, 255, 255),1);
		circle(img, t4, 8, Scalar(255, 255, 255), -1);
        
		std::vector <Point2f>texture5;
		getTexture5(model._shape, texture5);
		Point2f t5((texture5[0].x + texture5[1].x)/2, (texture5[0].y + texture5[1].y)/2);
		circle(img, t5, 10, Scalar(255, 255, 255),1);
		circle(img, t5, 8, Scalar(255, 255, 255), -1);
        
		std::vector <Point2f>texture6;
		getTexture6(model._shape, texture6);
		Point2f t6((texture6[1].x), (texture6[1].y));
		circle(img, t6, 10, Scalar(255, 255, 255),1);
		circle(img, t6, 8, Scalar(255, 255, 255), -1);
		
		//imshow("img", img);
		imwrite(agingFaceOutName, img);
		//waitKey();
		return true;
	}
	else
	{
		printf("cannot find face \n");
		return false;
	}
	
	
}

FaceAging::FaceAging(
                     std::string _imageName,
                     std::string _trackModelName,
                     std::string _trainedModeleName
                     )
{
    imageName = _imageName;
    trackModelName = _trackModelName;
    trainedModeleName = _trainedModeleName;
    //
    /*
    char* __imageName = "1.jpg";
    char* __trackModelName = "face2.tracker";
    char* __trainedModeleName = "train.dat";
    //
    char* findFaceOutputName = "foundFace.png";
    char* agingFaceOutputName = "agingFace.png";
    char* vFaceOutputName = "vFace.png";
    
    faceAging(__imageName, __trackModelName, agingFaceOutputName);
    findFace(__imageName, __trackModelName, findFaceOutputName);
    vFace(__imageName, __trackModelName, __trainedModeleName, vFaceOutputName);
     */
}

