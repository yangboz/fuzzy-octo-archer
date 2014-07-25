/****************************************************************************
*                                                                           *
*   IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.       *
*                                                                           *
*   By downloading, copying, installing or using the software you agree to  *
*   this license. If you do not agree to this license, do not download,     *
*   install, copy or use the software.                                      *
*                                                                           *
*                           License Agreement                               *
*                   For Vision Open Statistical Models                      *
*                                                                           *
*   Copyright (C):      2006~2012 by JIA Pei, all rights reserved.          *
*                                                                           *
*   VOSM is free software under the terms of the GNU Lesser General Public  *
*   License (GNU LGPL) as published by the Free Software Foundation; either *
*   version 3.0 of the License, or (at your option) any later version.      *
*   You can use it, modify it, redistribute it, etc; and redistribution and *
*   use in source and binary forms, with or without modification, are       *
*   permitted provided that the following conditions are met:               *
*                                                                           *
*   a) Redistribution's of source code must retain this whole paragraph of  *
*   copyright notice, including this list of conditions and all the         *
*   following contents in this  copyright paragraph.                        *
*                                                                           *
*   b) Redistribution's in binary form must reproduce this whole paragraph  *
*   of copyright notice, including this list of conditions and all the      *
*   following contents in this copyright paragraph, and/or other materials  *
*   provided with the distribution.                                         *
*                                                                           *
*   c) The name of the copyright holders may not be used to endorse or      *
*   promote products derived from this software without specific prior      *
*   written permission.                                                     *
*                                                                           *
*   Any publications based on this code must cite the following five papers,*
*   technical reports and on-line materials.                                *
*   1) P. JIA, 2D Statistical Models, Technical Report of Vision Open       *
*   Working Group, 2st Edition, October 21, 2010.                           *
*   http://www.visionopen.com/members/jiapei/publications/pei_sm2dreport2010.pdf*
*   2) P. JIA. Audio-visual based HMI for an Intelligent Wheelchair.        *
*   PhD thesis, University of Essex, February, 2011.                        *
*   http://www.visionopen.com/members/jiapei/publications/pei_phdthesis2010.pdf*
*   3) T. Cootes and C. Taylor. Statistical models of appearance for        *
*   computer vision. Technical report, Imaging Science and Biomedical       *
*   Engineering, University of Manchester, March 8, 2004.                   *
*   http://www.isbe.man.ac.uk/~bim/Models/app_models.pdf                    *
*   4) I. Matthews and S. Baker. Active appearance models revisited.        *
*   International Journal of Computer Vision, 60(2):135--164, November 2004.*
*   http://www.ri.cmu.edu/pub_files/pub4/matthews_iain_2004_2/matthews_iain_2004_2.pdf*
*   5) M. B. Stegmann, Active Appearance Models: Theory, Extensions and     *
*   Cases, 2000.                                                            *
*   http://www2.imm.dtu.dk/~aam/main/                                       *
*                                                                           *
* Version:          0.4                                                     *
* Author:           JIA Pei                                                 *
* Contact:          jp4work@gmail.com                                       *
* URL:              http://www.visionopen.com                               *
* Create Date:      2010-11-04                                              *
* Revise Date:      2012-03-22                                              *
*****************************************************************************/

#include <iostream>
#include <fstream>
#include "float.h"
#include <boost/filesystem.hpp>
#include <boost/regex/v4/fileiter.hpp>

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include "smf.h"
#include "VO_FaceKeyPoint.h"

using namespace std;
using namespace cv;


void usage_build()
{
    cout << "Usage: test_smfitting [options] trained_data type testing_images testing_annotations database staticORdynamic recording" << endl
        << "options: " << endl
        << "   -o    trained data directory (required) " << endl
        << "   -t    fitting method to be used (ASM_PROFILEND, ASM_LTC, AAM_BASIC, AAM_CMUICIA, AAM_IAIA. default ASM_PROFILEND ) " << endl
        << "   -i   testing image directory containing at least one image (required) " << endl
        << "   -a   testing annotation directory (can be ignored) " << endl
        << "   -d   testing database -- if annotation directory is specified, database should also be specified for further evaluation on fitting performance (can be ignored) " << endl
        << "   -s   static image sequence or dynamic image sequence (default value true) " << endl
        << "   -r   recording the fitting results or not (default value false) " << endl
        << endl << endl;

    cout << "Note: current smfitting doesn't defaultly afford 1D Profile ASM. " << endl
        << "If you would like to try 1D Profile ASM for static images in current smfitting, "
        << "you have to manually change the code in function VO_Fitting2DSM::VO_StartFitting "
        << "in file VO_Fitting2DSM.cpp, say, around line 306 of file VO_Fitting2DSM.cpp "
        << "change the 5th parameter from '2' to '1' of function "
        << "dynamic_cast<VO_FittingASMNDProfiles*>(this)->VO_ASMNDProfileFitting. " << endl << endl
        << "If you would like to try 1D Profile ASM for dynamic image sequence in current smfitting, "
        << "you have to manually change the code in function main() "
        << "in file smfitting.cpp, say around line 666 of file smfitting.cpp "
        << "change the 5th parameter from '2' to '1' of function "
        << "dynamic_cast<VO_FittingASMNDProfiles*>(fitting2dsm)->VO_ASMNDProfileFitting "
        << endl << endl;
    
    cout << "Face Detection: current smfitting use Adaboost technology to detect face as well as face components "
        << "for face location initialization. Refer to CFaceDetectionAlgs in main(). "
        << "Default Adaboost detectors installed with OpenCV installation are used in current smfitting "
        << "You may manually change the Adaboost detectors according to your own cascade file paths "
        << endl << endl;
    
    cout << "Face Tracking: current smfitting deals with image sequences. "
        << "If static images are to be tested, there is no point to carry out tracking because "
        << "for every image, Adaboost detection will be carried out. "
        << "If dynamic image sequences are to be tested, current smfitting only affords Camshift tracking strategy. "
        << "Please Refer to CTrackingAlgs() in main(), the default setting of function CTrackingAlgs() is Camshift algorithm "
        << endl << endl;
        
    cout<< "Vision Open doesn't afford the video IO or webcam IO, although the author has done his own IO for all kinds. "
        << "Users are highly encouraged to use their own video file or webcam IO and use VOSM in their own real-time applications. "
        << endl << endl;
    
    exit(0);
}

void parse_option(  int argc,
                    char **argv,
                    string& trainedData,
                    unsigned int& type,
                    vector<string>& imageFNs,
                    vector<string>& annotationFNs,
                    unsigned int& database,
                    bool& staticOrNot,
                    bool& recordOrNot)
{
    char *arg = NULL;
    int optindex, handleoptions=1;
    
    /* parse options */
    optindex = 0;
    while (++optindex < argc)
    {
        if(argv[optindex][0] != '-') break;
        if(++optindex >= argc) usage_build();

        switch(argv[optindex-1][1])
        {
        case 'o':
            trainedData     = argv[optindex];
            break;
        case 't':
        {
            if(strcmp(argv[optindex], "ASM_PROFILEND") == 0)
                type        = VO_AXM::ASM_PROFILEND;
            else if(strcmp(argv[optindex], "ASM_LTC") == 0)
                type        = VO_AXM::ASM_LTC;
            else if(strcmp(argv[optindex], "AAM_BASIC") == 0)
                type        = VO_AXM::AAM_BASIC;
            else if(strcmp(argv[optindex], "AAM_CMUICIA") == 0)
                type        = VO_AXM::AAM_CMUICIA;
            else if(strcmp(argv[optindex], "AAM_IAIA") == 0)
                type        = VO_AXM::AAM_IAIA;
            else
            {
                cerr << "Wrong fitting type parameters!" << endl;
                exit(EXIT_FAILURE);
            }
        }
            break;
        case 'i':
        {
            if ( ! boost::filesystem::is_directory( argv[optindex] ) )
            {
                cerr << "image path does not exist!" << endl;
                exit(EXIT_FAILURE);
            }
            imageFNs        = VO_IO::ScanNSortImagesInDirectory ( argv[optindex] );
        }
            break;
        case 'a':
        {
            if ( ! boost::filesystem::is_directory( argv[optindex] ) )
            {
                cerr << "landmark path does not exist!" << endl;
                exit(EXIT_FAILURE);
            }
            annotationFNs   = VO_IO::ScanNSortAnnotationInDirectory ( argv[optindex] );
        }
            break;
        case 'd':
        {
            if(strcmp(argv[optindex], "PUT") == 0)
                database    = CAnnotationDBIO::PUT;
            else if(strcmp(argv[optindex], "IMM") == 0)
                database    = CAnnotationDBIO::IMM;
            else if(strcmp(argv[optindex], "AGING") == 0)
                database    = CAnnotationDBIO::AGING;
            else if(strcmp(argv[optindex], "BIOID") == 0)
                database    = CAnnotationDBIO::BIOID;
            else if(strcmp(argv[optindex], "XM2VTS") == 0)
                database    = CAnnotationDBIO::XM2VTS;
            else if(strcmp(argv[optindex], "FRANCK") == 0)
                database    = CAnnotationDBIO::FRANCK;
            else if(strcmp(argv[optindex], "EMOUNT") == 0)
                database    = CAnnotationDBIO::EMOUNT;
            else if(strcmp(argv[optindex], "JIAPEI") == 0)
                database    = CAnnotationDBIO::JIAPEI;
            else
            {
                cerr << "Wrong database parameters!" << endl;
                exit(EXIT_FAILURE);
            }
        }
            break;
        case 's':
        {
            if(strcmp(argv[optindex], "false") == 0)
                staticOrNot    = false;
            else if(strcmp(argv[optindex], "true") == 0)
                staticOrNot    = true;
            else
            {
                cerr << "Wrong StaticOrNot parameter!" << endl;
                exit(EXIT_FAILURE);
            }
        }
            break;
        case 'r':
        {
            if(strcmp(argv[optindex], "false") == 0)
                recordOrNot    = false;
            else if(strcmp(argv[optindex], "true") == 0)
                recordOrNot    = true;
            else
            {
                cerr << "Wrong recordOrNot parameter!" << endl;
                exit(EXIT_FAILURE);
            }
        }
            break;
        default:
        {
            cerr << "unknown options" << endl;
            usage_build();
        }
            break;
        }
    }
    
    if (imageFNs.size() == 0)
    {
        cerr << " No image loaded" << endl;
        usage_build();
        exit(EXIT_FAILURE);
    }
    if (annotationFNs.size() != 0 && annotationFNs.size() != imageFNs.size() )
    {
        cerr << " If annotations are loaded, then, the number of landmarks should be equal to the number of images " << endl;
        usage_build();
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char **argv)
{
    string              traineddatadir;
    unsigned int        fittingmtd = VO_AXM::ASM_PROFILEND;
    unsigned int        database = CAnnotationDBIO::EMOUNT;
    vector<string>      AllImgFiles4Testing;
    vector<string>      AllLandmarkFiles4Evaluation;
    bool                staticOrNot = true;
    bool                record = false;
    
    parse_option(   argc,
                    argv,
                    traineddatadir,
                    fittingmtd,
                    AllImgFiles4Testing,
                    AllLandmarkFiles4Evaluation,
                    database,
                    staticOrNot,
                    record);

    VO_Fitting2DSM* fitting2dsm = NULL;
    switch(fittingmtd)
    {
    case VO_AXM::AAM_BASIC:
    case VO_AXM::AAM_DIRECT:
        fitting2dsm = new VO_FittingAAMBasic();
        dynamic_cast<VO_FittingAAMBasic*>(fitting2dsm)->VO_LoadParameters4Fitting(traineddatadir);
        break;
    case VO_AXM::CLM:
    case VO_AXM::AFM:
        fitting2dsm = new VO_FittingAFM();
        dynamic_cast<VO_FittingAFM*>(fitting2dsm)->VO_LoadParameters4Fitting(traineddatadir);
        break;
    case VO_AXM::AAM_IAIA:
    case VO_AXM::AAM_CMUICIA:
        fitting2dsm = new VO_FittingAAMInverseIA();
        dynamic_cast<VO_FittingAAMInverseIA*>(fitting2dsm)->VO_LoadParameters4Fitting(traineddatadir);
        break;
    case VO_AXM::AAM_FAIA:
        fitting2dsm = new VO_FittingAAMForwardIA();
        dynamic_cast<VO_FittingAAMForwardIA*>(fitting2dsm)->VO_LoadParameters4Fitting(traineddatadir);
        break;
    case VO_AXM::ASM_LTC:
        fitting2dsm = new VO_FittingASMLTCs();
        dynamic_cast<VO_FittingASMLTCs*>(fitting2dsm)->VO_LoadParameters4Fitting(traineddatadir);
        break;
    case VO_AXM::ASM_PROFILEND:
        fitting2dsm = new VO_FittingASMNDProfiles();
        dynamic_cast<VO_FittingASMNDProfiles*>(fitting2dsm)->VO_LoadParameters4Fitting(traineddatadir);
        break;
    }


    vector<Mat> oImages;
    vector<VO_Shape> oShapes;
    int nb = 20;
    bool doEvaluation = false;
    unsigned int nbOfTestingSamples = AllImgFiles4Testing.size();
    Mat_<float> nbOfIterations = Mat_<float>::zeros(1, nbOfTestingSamples);
    Mat_<float> deviations;
    Mat_<float> ptsErrorFreq;
    Mat_<float> times = Mat_<float>::zeros(1, nbOfTestingSamples);
    if (AllLandmarkFiles4Evaluation.size() !=0 )
    {
        doEvaluation = true;
        deviations = Mat_<float>::zeros(1, nbOfTestingSamples);
        ptsErrorFreq = Mat_<float>::zeros(nbOfTestingSamples, nb);
    }
    CAnnotationDBIO::VO_LoadShapeTrainingData( AllLandmarkFiles4Evaluation, database, oShapes);

    CFaceDetectionAlgs fd;
    Point2f ptLeftEyeCenter, ptRightEyeCenter, ptMouthCenter;

#ifdef POSIX
    fd.SetConfiguration("/usr/local/share/OpenCV/lbpcascades/lbpcascade_frontalface.xml", 
        "/usr/local/share/OpenCV/haarcascades/haarcascade_profileface.xml",
        "/usr/local/share/OpenCV/haarcascades/haarcascade_mcs_lefteye.xml",
        "/usr/local/share/OpenCV/haarcascades/haarcascade_mcs_righteye.xml",
        "/usr/local/share/OpenCV/haarcascades/haarcascade_mcs_nose.xml",
        "/usr/local/share/OpenCV/haarcascades/haarcascade_mcs_mouth.xml",
        VO_AdditiveStrongerClassifier::BOOSTING,
        CFaceDetectionAlgs::FRONTAL );
#elif WIN32
    fd.SetConfiguration("C:\\opencv\\opencv\\data\\lbpcascades\\lbpcascade_frontalface.xml", 
        "C:\\opencv\\opencv\\data\\haarcascades\\haarcascade_profileface.xml",
        "C:\\opencv\\opencv\\data\\haarcascades\\haarcascade_mcs_lefteye.xml",
        "C:\\opencv\\opencv\\data\\haarcascades\\haarcascade_mcs_righteye.xml",
        "C:\\opencv\\opencv\\data\\haarcascades\\haarcascade_mcs_nose.xml",
        "C:\\opencv\\opencv\\data\\haarcascades\\haarcascade_mcs_mouth.xml",
        VO_AdditiveStrongerClassifier::BOOSTING,
        CFaceDetectionAlgs::FRONTAL );
#endif 

    Mat iImage, resizedImage, drawImage, fittedImage;
    VO_Shape fittingShape;
    unsigned int detectionTimes = 0;
    
    // For static images from standard face databases
    // (Detection only, no tracking) + ASM/AAM
    if(staticOrNot)
    {
        detectionTimes = 0;
        for(unsigned int i = 0; i < AllImgFiles4Testing.size(); i++)
        {
            iImage = imread(AllImgFiles4Testing[i]);
            // Explained by JIA Pei. You can use cv::resize() to ensure before fitting starts,
            // every image to be tested is of a standard size, say (320, 240)
            iImage.copyTo(resizedImage);
            // cv::resize(iImage, resizedImage, Size(320, 240) );
            iImage.copyTo(fittedImage);
            size_t found1 = AllImgFiles4Testing[i].find_last_of("/\\");
            size_t found2 = AllImgFiles4Testing[i].find_last_of(".");
            string prefix = AllImgFiles4Testing[i].substr(found1+1, found2-1-found1);
            
            detectionTimes++;
            fd.FullFaceDetection(   resizedImage,
                                    NULL,
                                    true,
                                    true,
                                    true,
                                    true,
                                    1.0,
                                    Size(80,80),
                                    Size( min(resizedImage.rows,resizedImage.cols), min(resizedImage.rows,resizedImage.cols) ) ); // Size(240,240)

            if( fd.IsFaceDetected() )
            {
                fd.CalcFaceKeyPoints();
                double tmpScaleX = (double)iImage.cols/(double)resizedImage.cols;
                double tmpScaleY = (double)iImage.rows/(double)resizedImage.rows;
                Rect rect = fd.GetDetectedFaceWindow();
                ptLeftEyeCenter = fd.GetDetectedFaceKeyPoint(VO_KeyPoint::LEFTEYECENTER);
                ptRightEyeCenter = fd.GetDetectedFaceKeyPoint(VO_KeyPoint::RIGHTEYECENTER);
                ptMouthCenter = fd.GetDetectedFaceKeyPoint(VO_KeyPoint::MOUTHCENTER);
                ptLeftEyeCenter.x *= tmpScaleX;
                ptLeftEyeCenter.y *= tmpScaleY;
                ptRightEyeCenter.x *= tmpScaleX;
                ptRightEyeCenter.y *= tmpScaleY;
                ptMouthCenter.x *= tmpScaleX;
                ptMouthCenter.y *= tmpScaleY;
                
                // Explained by JIA Pei, you can save to see the detection results.
                iImage.copyTo(drawImage);
                cv::rectangle(drawImage, Point(ptLeftEyeCenter.x-1, ptLeftEyeCenter.y-1),
                    Point(ptLeftEyeCenter.x+1, ptLeftEyeCenter.y+1),
                    colors[5], 2, 8, 0);
                cv::rectangle(drawImage, Point(ptRightEyeCenter.x-1, ptRightEyeCenter.y-1),
                    Point(ptRightEyeCenter.x+1, ptRightEyeCenter.y+1),
                    colors[6], 2, 8, 0);
                cv::rectangle(drawImage, Point(ptMouthCenter.x-1, ptMouthCenter.y-1),
                    Point(ptMouthCenter.x+1, ptMouthCenter.y+1),
                    colors[7], 2, 8, 0);
                imwrite("drawImage.jpg", drawImage);
                imwrite("resizedImage.jpg", resizedImage);
                fitting2dsm->VO_StartFitting(   iImage,
                                                oImages,
                                                fittingmtd,
                                                ptLeftEyeCenter,
                                                ptRightEyeCenter,
                                                ptMouthCenter,
                                                VO_Fitting2DSM::EPOCH, // at most, how many iterations will be carried out
                                                4,
                                                record );
                nbOfIterations(0,i) = (float)(fitting2dsm->GetNbOfIterations());
                fittingShape = fitting2dsm->VO_GetFittedShape();
                times(0,i) = fitting2dsm->GetFittingTime();
//                cout << nbOfIterations(0,i) << endl;
            }
            
            if(record)
            {
                // Explained by JIA Pei. For static images, we can save all intermediate images of the fitting process.
                SaveSequentialImagesInFolder(oImages, prefix);
                string fn = prefix+".jpg";
                if(oImages.size() > 0)
                {
                    fittedImage = oImages.back();
                    imwrite(fn.c_str(), fittedImage);
                    oImages.clear();
                }
            }
            
            // For evaluation
            if(doEvaluation)
            {
                vector<float> ptErrorFreq;
                float deviation = 0.0f;
                vector<unsigned int> unsatisfiedPtList;
                unsatisfiedPtList.clear();
                CRecognitionAlgs::CalcShapeFittingEffect(   oShapes[i],
                                                            fittingShape,
                                                            deviation,
                                                            ptErrorFreq,
                                                            nb);
                deviations(0,i) = deviation;
                for(unsigned int j = 0; j < nb; j++)
                    ptsErrorFreq(i, j) = ptErrorFreq[j];
                CRecognitionAlgs::SaveShapeRecogResults(    "./",
                                                            prefix,
                                                            deviation,
                                                            ptErrorFreq);
            }
        }
        
        cout << "detection times = " << detectionTimes << endl;
        float avgIter = cv::mean(nbOfIterations).val[0];
        cout << "Average Interation Times = " << avgIter << endl;
        float avgTime = cv::mean(times).val[0];
        cout << "Averaget Detection time (in ms) = " << avgTime << endl;
        Scalar avgDev, stdDev;
        if(doEvaluation)
        {
            cv::meanStdDev(deviations, avgDev, stdDev);
            cout << "Average Deviation of Errors = " << avgDev.val[0] << " " 
                << "Standard Deviation of Errors = " << stdDev.val[0] << endl << endl;
            vector<float> avgErrorFreq(nb, 0.0f);
            for(int j = 0; j < nb; j++)
            {
                Mat_<float> col = ptsErrorFreq.col(j);
                avgErrorFreq[j] = cv::mean(col).val[0];
                cout << avgErrorFreq[j] << "percentage of points are in " << j << "pixels" << endl;
            }
        }
    }
    // For dynamic image sequences
    // (Detection or Tracking) + ASM/AAM
    else
    {
        CTrackingAlgs*    trackAlg = new CTrackingAlgs();
        bool isTracked = false;
        detectionTimes = 0;
        for(unsigned int i = 0; i < AllImgFiles4Testing.size(); i++)
        {
            iImage = imread(AllImgFiles4Testing[i]);
            // Explained by JIA Pei. You can use cv::resize() to ensure before fitting starts,
            // every image to be tested is of a standard size, say (320, 240)
            // iImage.copyTo(resizedImage);    // 
            cv::resize(iImage, resizedImage, Size(320, 240) );
            iImage.copyTo(fittedImage);
            size_t found1 = AllImgFiles4Testing[i].find_last_of("/\\");
            size_t found2 = AllImgFiles4Testing[i].find_last_of(".");
            string prefix = AllImgFiles4Testing[i].substr(found1+1, found2-1-found1);

            if(!isTracked)
            {
                detectionTimes++;
                fd.FullFaceDetection(   resizedImage,
                                        NULL,
                                        true,
                                        true,
                                        true,
                                        true,
                                        1.0,
                                        Size(80,80),
                                        Size( min(resizedImage.rows,resizedImage.cols), min(resizedImage.rows,resizedImage.cols) ) ); // Size(240,240)
                if( fd.IsFaceDetected() )
                {
                    fd.CalcFaceKeyPoints();
                    double tmpScaleX = (double)iImage.cols/(double)resizedImage.cols;
                    double tmpScaleY = (double)iImage.rows/(double)resizedImage.rows;
                    Rect rect = fd.GetDetectedFaceWindow();
                    ptLeftEyeCenter = fd.GetDetectedFaceKeyPoint(VO_KeyPoint::LEFTEYECENTER);
                    ptRightEyeCenter = fd.GetDetectedFaceKeyPoint(VO_KeyPoint::RIGHTEYECENTER);
                    ptMouthCenter = fd.GetDetectedFaceKeyPoint(VO_KeyPoint::MOUTHCENTER);
                    ptLeftEyeCenter.x *= tmpScaleX;
                    ptLeftEyeCenter.y *= tmpScaleY;
                    ptRightEyeCenter.x *= tmpScaleX;
                    ptRightEyeCenter.y *= tmpScaleY;
                    ptMouthCenter.x *= tmpScaleX;
                    ptMouthCenter.y *= tmpScaleY;
                
                    // Explained by JIA Pei, you can save to see the detection results.
//                    resizedImage.copyTo(drawImage);
//                    fd.VO_DrawDetection(drawImage, true, true, true, true, true);
//                    imwrite("drawImage.jpg", drawImage);
//                    imwrite("resizedImage.jpg", resizedImage);
//                    imwrite("iImage.jpg", iImage);
                    fitting2dsm->SetInputImage(iImage);
                    
                    
                    switch(fittingmtd)
                    {
                    case VO_AXM::AAM_BASIC:
                        {
                            fittingShape.clone(dynamic_cast<VO_FittingAAMBasic*>(fitting2dsm)->m_VOAAMBasic->GetAlignedMeanShape() );
                            fittingShape.Affine2D(
                                                    VO_Fitting2DSM::VO_FirstEstimationBySingleWarp(
                                                    dynamic_cast<VO_FittingAAMBasic*>(fitting2dsm)->m_VOAAMBasic->GetFaceParts(),
                                                    fittingShape,
                                                    ptLeftEyeCenter,
                                                    ptRightEyeCenter,
                                                    ptMouthCenter) );
                            fittingShape.ConstrainShapeInImage(iImage);

                            dynamic_cast<VO_FittingAAMBasic*>(fitting2dsm)
                                            ->VO_BasicAAMFitting(iImage,
                                                                fittingShape,
                                                                fittedImage,
                                                                VO_Fitting2DSM::EPOCH );
                        }
                        break;
                    case VO_AXM::AAM_DIRECT:
                        {
                            fittingShape.clone(dynamic_cast<VO_FittingAAMBasic*>(fitting2dsm)->m_VOAAMBasic->GetAlignedMeanShape() );
                            fittingShape.Affine2D(
                                                    VO_Fitting2DSM::VO_FirstEstimationBySingleWarp(
                                                    dynamic_cast<VO_FittingAAMBasic*>(fitting2dsm)->m_VOAAMBasic->GetFaceParts(),
                                                    fittingShape,
                                                    ptLeftEyeCenter,
                                                    ptRightEyeCenter,
                                                    ptMouthCenter) );
                            fittingShape.ConstrainShapeInImage(iImage);

                            dynamic_cast<VO_FittingAAMBasic*>(fitting2dsm)
                                            ->VO_DirectAAMFitting(iImage,
                                                                fittingShape,
                                                                fittedImage,
                                                                VO_Fitting2DSM::EPOCH );
                        }
                        break;
                    case VO_AXM::CLM:
                    case VO_AXM::AFM:
                        break;
                    case VO_AXM::AAM_IAIA:
                        {
                            fittingShape.clone(dynamic_cast<VO_FittingAAMInverseIA*>(fitting2dsm)->m_VOAAMInverseIA->GetAlignedMeanShape() );
                            fittingShape.Affine2D(
                                                    VO_Fitting2DSM::VO_FirstEstimationBySingleWarp(
                                                    dynamic_cast<VO_FittingAAMInverseIA*>(fitting2dsm)->m_VOAAMInverseIA->GetFaceParts(),
                                                    fittingShape,
                                                    ptLeftEyeCenter,
                                                    ptRightEyeCenter,
                                                    ptMouthCenter) );
                            fittingShape.ConstrainShapeInImage(iImage);

                            dynamic_cast<VO_FittingAAMInverseIA*>(fitting2dsm)
                                                ->VO_IAIAAAMFitting(iImage,
                                                                    fittingShape,
                                                                    fittedImage,
                                                                    VO_Fitting2DSM::EPOCH );
                        }
                        break;
                    case VO_AXM::AAM_CMUICIA:
                        {
                            fittingShape.clone(dynamic_cast<VO_FittingAAMInverseIA*>(fitting2dsm)->m_VOAAMInverseIA->GetAlignedMeanShape() );
                            fittingShape.Affine2D(
                                                    VO_Fitting2DSM::VO_FirstEstimationBySingleWarp(
                                                    dynamic_cast<VO_FittingAAMInverseIA*>(fitting2dsm)->m_VOAAMInverseIA->GetFaceParts(),
                                                    fittingShape,
                                                    ptLeftEyeCenter,
                                                    ptRightEyeCenter,
                                                    ptMouthCenter) );
                            fittingShape.ConstrainShapeInImage(iImage);

                            dynamic_cast<VO_FittingAAMInverseIA*>(fitting2dsm)
                                                ->VO_ICIAAAMFitting(iImage,
                                                                    fittingShape,
                                                                    fittedImage,
                                                                    VO_Fitting2DSM::EPOCH );
                        }
                        break;
                    case VO_AXM::AAM_FAIA:
                        break;
                    case VO_AXM::ASM_LTC:
                        {
                            fittingShape.clone(dynamic_cast<VO_FittingASMLTCs*>(fitting2dsm)->m_VOASMLTC->GetAlignedMeanShape() );
                            fittingShape.Affine2D(
                                                    VO_Fitting2DSM::VO_FirstEstimationBySingleWarp(
                                                    dynamic_cast<VO_FittingASMLTCs*>(fitting2dsm)->m_VOASMLTC->GetFaceParts(),
                                                    fittingShape,
                                                    ptLeftEyeCenter,
                                                    ptRightEyeCenter,
                                                    ptMouthCenter) );
                            fittingShape.ConstrainShapeInImage(iImage);

                            dynamic_cast<VO_FittingASMLTCs*>(fitting2dsm)
                                        ->VO_ASMLTCFitting(    iImage,
                                                            fittingShape,
                                                            fittedImage,
                                                            VO_Features::DIRECT,
                                                            VO_Fitting2DSM::EPOCH,
                                                            3);    // change this 2 to 1 for 1D profile ASM
                        }
                        break;
                    case VO_AXM::ASM_PROFILEND:    // default, 2D Profile ASM
                        {
                            fittingShape.clone(dynamic_cast<VO_FittingASMNDProfiles*>(fitting2dsm)->m_VOASMNDProfile->GetAlignedMeanShape() );
                            fittingShape.Affine2D(
                                                    VO_Fitting2DSM::VO_FirstEstimationBySingleWarp(
                                                    dynamic_cast<VO_FittingASMNDProfiles*>(fitting2dsm)->m_VOASMNDProfile->GetFaceParts(),
                                                    fittingShape,
                                                    ptLeftEyeCenter,
                                                    ptRightEyeCenter,
                                                    ptMouthCenter)
                                                );
                            fittingShape.ConstrainShapeInImage(iImage);

                            dynamic_cast<VO_FittingASMNDProfiles*>(fitting2dsm)
                                        ->VO_ASMNDProfileFitting(    iImage,
                                                                    fittingShape,
                                                                    fittedImage,
                                                                    VO_Fitting2DSM::EPOCH,
                                                                    4,
                                                                    2);    // change this 2 to 1 for 1D profile ASM
                        }
                        break;
                    }
                    // Whenever the face is re-detected, initialize the tracker and set isTracked = true;
                    Rect rect1 =    fittingShape.GetShapeBoundRect();
                    trackAlg->UpdateTracker(iImage, rect1);
                    isTracked =  true;
                }
            }
            else
            {
                switch(fittingmtd)
                {
                    case VO_AXM::AAM_BASIC:
                    {
                        dynamic_cast<VO_FittingAAMBasic*>(fitting2dsm)
                                        ->VO_BasicAAMFitting(   iImage,
                                                                fittingShape,
                                                                fittedImage,
                                                                VO_Fitting2DSM::EPOCH);    
                    }
                    break;
                    case VO_AXM::AAM_DIRECT:
                    {
                        dynamic_cast<VO_FittingAAMBasic*>(fitting2dsm)
                                        ->VO_DirectAAMFitting(  iImage,
                                                                fittingShape,
                                                                fittedImage,
                                                                VO_Fitting2DSM::EPOCH);
                    }
                    break;
                    case VO_AXM::CLM:
                    case VO_AXM::AFM:
                    break;
                    case VO_AXM::AAM_IAIA:
                    {
                        dynamic_cast<VO_FittingAAMInverseIA*>(fitting2dsm)
                                        ->VO_IAIAAAMFitting(iImage,
                                                            fittingShape,
                                                            fittedImage,
                                                            VO_Fitting2DSM::EPOCH);
                    }
                    break;
                    case VO_AXM::AAM_CMUICIA:
                    {
                        dynamic_cast<VO_FittingAAMInverseIA*>(fitting2dsm)
                                        ->VO_ICIAAAMFitting(iImage,
                                                            fittingShape,
                                                            fittedImage,
                                                            VO_Fitting2DSM::EPOCH);
                    }
                    break;
                    case VO_AXM::AAM_FAIA:
                    break;
                    case VO_AXM::ASM_LTC:
                    {
                        dynamic_cast<VO_FittingASMLTCs*>(fitting2dsm)
                                        ->VO_ASMLTCFitting( iImage,
                                                            fittingShape,
                                                            fittedImage,
                                                            VO_Features::DIRECT,
                                                            VO_Fitting2DSM::EPOCH,
                                                            3);
                    }
                    break;
                    case VO_AXM::ASM_PROFILEND:
                    {
                        dynamic_cast<VO_FittingASMNDProfiles*>(fitting2dsm)
                            ->VO_ASMNDProfileFitting(   iImage,
                                                        fittingShape,
                                                        fittedImage,
                                                        VO_Fitting2DSM::EPOCH,
                                                        4,
                                                        2);    // change this 2 to 1 for 1D profile ASM
                    }
                    break;
                }
                // Explained by JIA Pei. For every consequent image, whose previous image is regarded as tracked, 
                // we have to double-check whether current image is still a tracked one.
//                isTracked = true;
                isTracked = CRecognitionAlgs::EvaluateFaceTrackedByProbabilityImage(
                                                        trackAlg,
                                                        iImage,
                                                        fittingShape,
                                                        Size(80,80),
                                                        Size( min(iImage.rows,iImage.cols), min(iImage.rows,iImage.cols) ) );
            }

            nbOfIterations(0,i) = (float)(fitting2dsm->GetNbOfIterations());
            fittingShape = fitting2dsm->VO_GetFittedShape();
            times(0,i) = fitting2dsm->GetFittingTime();
            

            if(record)
            {
                string fn = prefix+".jpg";
                imwrite(fn.c_str(), fittedImage);
            }
            
            // For evaluation
            if(doEvaluation)
            {
                vector<float> ptErrorFreq;
                float deviation = 0.0f;
                vector<unsigned int> unsatisfiedPtList;
                unsatisfiedPtList.clear();
                CRecognitionAlgs::CalcShapeFittingEffect(   oShapes[i],
                                                            fittingShape,
                                                            deviation,
                                                            ptErrorFreq,
                                                            nb);
                deviations(0,i) = deviation;
                for(unsigned int j = 0; j < nb; j++)
                    ptsErrorFreq(i, j) = ptErrorFreq[j];
                CRecognitionAlgs::SaveShapeRecogResults(    "./",
                                                            prefix,
                                                            deviation,
                                                            ptErrorFreq);
            }
        }
        
        cout << "detection times = " << detectionTimes << endl;
        float avgIter = cv::mean(nbOfIterations).val[0];
        cout << avgIter << endl;
        float avgTime = cv::mean(times).val[0];
        cout << avgTime << endl;
        Scalar avgDev, stdDev;
        cv::meanStdDev(deviations, avgDev, stdDev);
        cout << avgDev.val[0] << " " << stdDev.val[0] << endl << endl;
        vector<float> avgErrorFreq(nb, 0.0f);
        for(int j = 0; j < nb; j++)
        {
            Mat_<float> col = ptsErrorFreq.col(j);
            avgErrorFreq[j] = cv::mean(col).val[0];
            cout << j << " " << avgErrorFreq[j] << endl;
        }
        
        delete trackAlg;
    }
    
    delete fitting2dsm;

    return 0;
}
