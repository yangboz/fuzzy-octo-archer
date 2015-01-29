#ifndef __IO_h_
#define __IO_h_
//#include <opencv\cv.h>
//#include <opencv\highgui.h>
//#include <opencv\ml.h>
#include "OpenCV/opencv.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <io.h>
//#include <direct.h>
using namespace std;

namespace FACETRACKER
{
  typedef std::vector<std::string> file_lists;
  //===========================================================================
  /** 
      Input-output Operations
  */
  class IO{
  public:
    enum{PDM = 0,PAW,PATCH,MPATCH,CLM,FDET,FCHECK,MFCHECK,TRACKER};
    static void ReadMat(std::ifstream& s,cv::Mat &M);
    static void WriteMat(std::ofstream& s,cv::Mat &M);
    static cv::Mat LoadCon(const char* fname);
    static cv::Mat LoadTri(const char* fname);
	static cv::Mat LoadEdge(const char* fname);
  };

  //===========================================================================
}
#endif
