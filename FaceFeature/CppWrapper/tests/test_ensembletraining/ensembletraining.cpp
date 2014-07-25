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
* Create Date:      2010-02-16                                              *
* Revise Date:      2012-03-22                                              *
*****************************************************************************/

// FIXME: to be finished
// Actually, current OpenCV affords a module named "traincascade", which is robust and has good performance
// However, some header files of VOSM's ensembletraining module are required for successful VOSM building!

#include <iostream>
#include <fstream>

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/ml.h"
#include "VO_ScanFilesInDir.h"
#include "VO_BaggingRandomForestClassifier.h"
#include "VO_BoostingSingleEnsembleClassifier.h"
#include "VO_BoostingCascadeClassifier.h"
#include "VO_LBPFeatures.h"
#include "VO_HaarFeatures.h"

using namespace std;
using namespace cv;



void usage_build()
{
    cout << "Usage: ensembletraining [options] save_directory pos_image_directory neg_image_directory ensemble_type boosting_type feature_type stage mintruepositive maxwrongclassification false" << endl
        << "options: " << endl
        << "   -o    output directory (default './') " << endl
        << "   -p    positive training image directory (containing training positive images of the same size ) " << endl
        << "   -n    negative training image directory " << endl
        << "   -a    additive model type (BOOSTING or BAGGING. default BOOSTING ) " << endl
        << "   -b    boosting type (SINGLECASCADE, PARALLELCASCADE, CASCADEDECISIONTREE or BFSCASCADE. If using BOOSTING additive model, you must specify this vale. default SINGLECASCADE )" << endl
        << "   -f    multi-scale feature type (DIRECT, LBP, HAAR_BASIC, HAAR_CORE, HAAR_ALL, GABOR, etc. default LBP ) " << endl
        << "   -s   boosting stage " << endl
        << "   -t   minimum true positive (default 0.95) " << endl
        << "   -c   maximum wrong classification, including true negative plus false positive (default 0.5) " << endl;
    
    exit(0);
}


void parse_option(  int argc,
                    char **argv,
                    string& outputDir,
                    vector<string>& posImageFNs, 
                    vector<string>& negImageFNs, 
                    unsigned int& ensembleType,
                    unsigned int& boostingType,
                    string& featureType,
                    unsigned int& stage,
                    float& minFalsePositive,
                    float& maxWrongClassification)
{
    char *arg = NULL;
    int optindex;
    
    /* parse options */
    optindex = 0;
    while (++optindex < argc)
    {
        if(argv[optindex][0] != '-') break;
        if(++optindex >= argc) usage_build();

        switch(argv[optindex-1][1])
        {
        case 'o':
            outputDir         = argv[optindex];
            break;
        case 'p':
        {
            if ( ! boost::filesystem::is_directory( argv[optindex] ) )
            {
                cerr << "positive image path does not exist!" << endl;
                exit(EXIT_FAILURE);
            }
            posImageFNs   = VO_IO::ScanNSortImagesInDirectory ( argv[optindex] );
        }
            break;
        case 'n':
        {
            if ( ! boost::filesystem::is_directory( argv[optindex] ) )
            {
                cerr << "negative image path does not exist!" << endl;
                exit(EXIT_FAILURE);
            }
            negImageFNs    = VO_IO::ScanNSortImagesInDirectory ( argv[optindex] );
        }
            break;
        case 'a':
        {
            if(strcmp(argv[optindex], "BOOSTING") == 0)
                ensembleType     = VO_AdditiveStrongerClassifier::BOOSTING;
            else if(strcmp(argv[optindex], "BAGGING") == 0)
                ensembleType    = VO_AdditiveStrongerClassifier::BAGGING;
        }
            break;
        case 'b':
        {
            if(strcmp(argv[optindex], "SINGLECASCADE") == 0)
                boostingType     = VO_BoostingCascadeClassifier::SINGLECASCADE;
            else if(strcmp(argv[optindex], "PARALLELCASCADE") == 0)
                boostingType    = VO_BoostingCascadeClassifier::PARALLELCASCADE;
            else if(strcmp(argv[optindex], "CASCADEDECISIONTREE") == 0)
                boostingType     = VO_BoostingCascadeClassifier::CASCADEDECISIONTREE;
            else if(strcmp(argv[optindex], "BFSCASCADE") == 0)
                boostingType    = VO_BoostingCascadeClassifier::BFSCASCADE;
        }
            break;
        case 'f':
        {
            featureType = argv[optindex];
        }
            break;
        case 's':
        {
            stage = atoi(argv[optindex]);
        }
            break;
        case 't':
        {
            minFalsePositive = atof(argv[optindex]);
        }
            break;
        case 'c':
        {
            maxWrongClassification = atof(argv[optindex]);
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
    
    if (posImageFNs.size() == 0)
    {
        cerr << " No positive images loaded" << endl;
        usage_build();
        exit(EXIT_FAILURE);
    }
    if (negImageFNs.size() == 0)
    {
        cerr << " No negative image loaded" << endl;
        usage_build();
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char **argv)
{
    string          outputDir = "./";
    vector<string>  posImageFNs;
    vector<string>  negImageFNs;
    unsigned int    ensembleType = VO_AdditiveStrongerClassifier::BOOSTING;
    unsigned int    boostingType = VO_BoostingCascadeClassifier::SINGLECASCADE;
    string          featureType = "LBP";
    unsigned int    numStages =16;
    float           minTruePositive = 0.95f;;
    float           maxWrongClassification = 0.5f;


    const int precalcValBufSize = 128;
    const int precalcIdxBufSize = 128;
    
    parse_option(   argc,
                    argv,
                    outputDir,
                    posImageFNs, 
                    negImageFNs, 
                    ensembleType,
                    boostingType,
                    featureType,
                    numStages,
                    minTruePositive,
                    maxWrongClassification);

    VO_Features* featureParams = NULL;
    
    if(featureType == "LBP")
    {
        featureParams   = new VO_LBPFeatures();
    }
    else if(featureType == "HAAR_BASIC")
    {
        featureParams   = new VO_HaarFeatures();
    }
    else if(featureType == "HAAR_CORE")
    {
        featureParams   = new VO_HaarFeatures();
    }
    else if(featureType == "HAAR_ALL")
    {
        featureParams   = new VO_HaarFeatures();
    }

    if(ensembleType == VO_AdditiveStrongerClassifier::BOOSTING)
    {
        switch(boostingType)
        {
            case VO_BoostingCascadeClassifier::PARALLELCASCADE:
            {
                VO_BoostingCascadeClassifier boostcascadeclassifier;
            }
            break;
            case VO_BoostingCascadeClassifier::CASCADEDECISIONTREE:
            {
                VO_BoostingCascadeClassifier boostcascadeclassifier;
            }
            break;
            case VO_BoostingCascadeClassifier::BFSCASCADE:
            {
                VO_BoostingCascadeClassifier boostcascadeclassifier;
            }
            break;
            case VO_BoostingCascadeClassifier::SINGLECASCADE:
            {
                VO_BoostingCascadeClassifier boostcascadeclassifier;
                boostcascadeclassifier.train(   "./",
                                                posImageFNs,
                                                negImageFNs,
                                                precalcValBufSize,
                                                precalcIdxBufSize,
                                                numStages,
                                                minTruePositive,
                                                maxWrongClassification,
                                                featureParams);
            }
            default:
            break;
        }
    }
    else if (boostingType == VO_AdditiveStrongerClassifier::BAGGING)
    {
        VO_BaggingRandomForestClassifier baggingrandomforestclassifier;
    }

    return 0;
}
