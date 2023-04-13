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
#include "opencv_5.h"
#include "fonctions.h"
#include <vector>



using namespace cv;
using namespace std;

/* Classe principale */

class ImageConverter
{
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    
  public:
 
     cv::Point2f objectivePoints[4]; 
    image_transport::Publisher image_pub_1_;
    image_transport::Publisher image_pub_2_;
    image_transport::Publisher image_pub_3_;
    ImageConverter(): it_(nh_)
    {
      // Subscrive to input video feed and publish output video feed
      image_sub_ = it_.subscribe("/cv_camera/image_raw", 1, &ImageConverter::imageCb, this);
      image_pub_1_ = it_.advertise("/snpi5/output_video_1", 1);
      image_pub_2_ = it_.advertise("/snpi5/output_video_2", 1);
      image_pub_3_ = it_.advertise("/snpi5/output_video_3", 1);
            
      //namedWindow(OPENCV_WINDOW);
      readParam();
        
      
    }
    ~ImageConverter()
    {
      //destroyWindow(OPENCV_WINDOW);
    }


	
    /* Fonction appelée à chaque nouvelle image */

    void imageCb(const sensor_msgs::ImageConstPtr& msg)
    {
      /* conversion topic ROS en format image OpenCV */
      cv_bridge::CvImagePtr cv_ptr;

      try
      {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      }
      catch (cv_bridge::Exception & e)
      {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
      }

      // Draw an example circle on the video stream
      //if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
      //circle(cv_ptr->image, Point(50, 50), 10, CV_RGB(255,0,0));

      /* traitement à proprement parler */

      /* 1ere etape conversion BRG en HSV (teinte / Saturation / Luminosité ) plus simple pour détection couleur */

      cvtColor(cv_ptr->image, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

      /* détection de la couleur des marqueurs résultat image seuillée imgThresholded */
      inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      /* détection de la couleur des cibles résultat image seuillée imgThresholdedTarget */

      // Bitwise-AND of mask and purple only image - only used for display
      // il faut remettre à 0 la matrice de résultat à chaque fois.
      // on met 255 comme cela le masque est bon
      // on pourra utiliser or et la couleur typique.
      res = 0;
      //bitwise_and(cv_ptr->image, 255, res,imgThresholded); // couleur naturelle
      //bitwise_or(cv_ptr->image, CV_RGB(255,0,0), res,imgThresholded); // couleur prédéfinie

      /* traitement des marqueurs */
      // morphological opening (remove small objects from the foreground)
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      // morphological closing (fill small holes in the foreground)
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      
      
      // Paramètre de la détection de Blob

      
      // réglage des paramètres du détecteur des 4 mires

      SimpleBlobDetector::Params params;
      mireParams(&params);

     
      
      /* Définition des taches identifiées */
      vector<KeyPoint> keypoints;

      // Set up detector with params
      // Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
      // detector->detect( im, keypoints);

      // création des détecteurs de composantes connexes ou blob à partir des paramètres

      Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
      detector->detect( imgThresholded, keypoints );


      //the total no of blobs detected are:
      int nbMire = keypoints.size();

      cout << "Nombre total de Mires detectés : " << nbMire << " Objects type 1 "  << endl;

      //the total no of blobs detected are:
      bool erreurNombreBlob;
      //Affichage image brut

      image_pub_1_.publish(cv_ptr->toImageMsg());
 //     imshow(OPENCV_WINDOW, cv_ptr->image);
       waitKey(3);

      if (nbMire == 4)
      {
        bool erreurNombreBlob = false;
        
        // trace les keypoints marqueurs en vert sur l'image  im_with_keypoints à partir de l'image de base
        drawKeypoints(cv_ptr->image, keypoints, im_with_keypoints, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

        char num[4][2] = {"1", "2", "3", "4"};
        
        cv::Point2f keypointPos[4];
        
        orderKeypoints(keypointPos,keypoints);
        
        
		
        
        for (int i = 0; i < 4; i++)
        {

          cv::putText(im_with_keypoints, //target image
                      num[i], //text
                      keypointPos[i], //top-left position
                      cv::FONT_HERSHEY_DUPLEX,
                      1.0,
                      CV_RGB(0, 0, 0), //font color
                      2);
        }
        
		
		objectiveDefinition(objectivePoints);


		
        // taille de l'image résultat

        int centre_x = max_width / 2;
        int centre_y = max_height / 2;

	// coefficients nécessaire pour centrer et mettre à l'échelle l'image
        double coef = 0.6*max_width/largeur; // à voir
        double tx = max_width/2;
        double ty = max_height/2;
        double tix = (objectivePoints[1].x-objectivePoints[0].x)/2;
        double tiy = (objectivePoints[3].y-objectivePoints[0].y)/2;
        
	// matrices de transformation - on centre les points objectifs, on centre et on met à l'échelle
        cv::Mat t1 = (cv::Mat_<double>(3, 3) << 1, 0, -tx, 0, 1, -ty, 0, 0, 1);
        cv::Mat t2 = (cv::Mat_<double>(3, 3) << 1/coef, 0, 0, 0, 1/coef, 0, 0, 0, 1);
        cv::Mat t3 = (cv::Mat_<double>(3, 3) << 1, 0, tix, 0, 1, tiy, 0, 0, 1);

        //calcul de la matrice de transformation entre l'espace réel et l'image
        cv::Mat transform = cv::getPerspectiveTransform(objectivePoints, keypointPos);

        //calcul de la transformation inverse
        cv::Mat Inv = transform.inv();



        // perspective.
        cv::warpPerspective(im_with_keypoints,
                            im_with_keypointsOrtho,
                            transform*t3*t2*t1,
                            cv::Size(max_width, max_height),
                            cv::INTER_LINEAR | cv::WARP_INVERSE_MAP);

        // Update GUI Window seulement si les 4 points de mire avaient été identifiés
//        imshow(OPENCV_IMAGE_RESULTAT, im_with_keypoints);
//        imshow(OPENCV_IMAGE_ORTHO, im_with_keypointsOrtho);

       	sensor_msgs::ImagePtr msg_output_2 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", im_with_keypoints).toImageMsg();
	sensor_msgs::ImagePtr msg_output_3 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", im_with_keypointsOrtho).toImageMsg();
	image_pub_2_.publish(msg_output_2);
	image_pub_3_.publish(msg_output_3);


      }
      else if (nbMire != 4)
      {
        erreurNombreBlob = true;
        printf("Erreur nombre de blob detecte\n");
      }
   }
};


int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}

