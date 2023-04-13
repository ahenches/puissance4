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


// taille de la mire (en mm par exemple) mais le résultat est indépendant. 1000.0 est là pour exemple
const float largeur = 200.0/1000.0;
const float hauteur = 150.0/1000.0;


//dimension de l'image résultat
const int max_width = 800;
const int max_height = 600;

extern int iLowH;
extern int iHighH;

extern int iLowS;
extern int iHighS;

extern int iLowV;
extern int iHighV;

static const string FILE_NAME = "/home/ros/catkin_ws/config/config_opencv_5.txt";

string type2str(int type);
void mireParams(SimpleBlobDetector::Params *params);
void targetParams(SimpleBlobDetector::Params *params);
void orderKeypoints(cv::Point2f *keypointPos,vector<KeyPoint> keypoints);
void objectiveDefinition(cv::Point2f *objectivePoints);
void calculPoints(cv::Point2f *keypointTargetPos, cv::Point2f *keypointTargetPosTrans, int nbTarget,cv::Mat Inv);
void readParam(void);


//BY COCO

void TestColonne(int keypointArray,int nbMire, int PosXMire0, int DX, int* TColonneCouleur, int i);
void TestLigne(int keypointArray,int nbMire,int PosYMire4, int DY, int *TLigneCouleur, int i);
