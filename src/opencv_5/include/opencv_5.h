#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <std_msgs/UInt16.h>
#include <sensor_msgs/Joy.h>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

static const std::string OPENCV_WINDOW = "Image window";
static const std::string OPENCV_IMAGE_RESULTAT = "Image résultat";
static const std::string OPENCV_IMAGE_ORTHO = "Image redressée";


struct myPoint {
    float x;
    float y;
};

Mat imgHSV;
Mat imgThresholded_Yellow;
Mat imgThresholded_Red;
Mat imgThresholded_Mires;
Mat imgThresholded_MiresOrtho;
Mat im_with_keypoints_Yellow;
Mat im_with_keypoints_Red;
Mat im_with_keypoints_Mires;
Mat im_with_keypoints_MiresOrtho;

static Mat imgHSVOrtho;

// perspective image.
Mat im_with_keypointsOrtho;
Mat im_with_keypointsOrtho_Red;
Mat im_with_keypointsOrtho_Yellow;

Mat mask;
Mat res;

/* couleur des marqueurs latéraux */

int iLowH = 0;
int iHighH = 255;

int iLowS = 0;
int iHighS = 255;

int iLowV = 0;
int iHighV = 255;
