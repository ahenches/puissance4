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
#include <std_msgs/Int32MultiArray.h>

#include <iostream>
#include <fstream>
#include "opencv_5.h"
#include "fonctions.h"
#include <vector>

using namespace cv;
using namespace std;
static const int ROWS = 5;
static const int COLS = 5;
static const int BALL_SIZE = 50;

//test position couleur

int PosXMire0 = 160;
int PosYMire3 = 120;
int DX = 80;
int DY = 90;
int Valcouleur = 0;
int MatriceResultat[5][5] = {};


/* Classe principale */

class ImageConverter
{
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    
  public:
  
  //BY Manal: changer la valeur de objective points de 4 à 25
 
     cv::Point2f objectivePoints[100]; 
    image_transport::Publisher image_pub_1_;
    image_transport::Publisher image_pub_2_;//RED
    image_transport::Publisher image_pub_3_;//YELLOW
    image_transport::Publisher image_pub_4_;//Mires
    ImageConverter(): it_(nh_)
    {
      // Subscrive to input video feed and publish output video feed
      image_sub_ = it_.subscribe("/cv_camera/image_raw", 1, &ImageConverter::imageCb, this);
      image_pub_1_ = it_.advertise("/MT4/output_video_1", 1);
      image_pub_2_ = it_.advertise("/Red_By_Salmi_Manal/output_video_2", 1);//RED
      image_pub_3_ = it_.advertise("/Yellow_By_Parc_Corentin/output_video_3", 1);//Yellow
      image_pub_4_ = it_.advertise("/Mires_By_Salmi_Manal/output_video_4", 1);
      
      
      
            // Topic Pour Publier les coordonnées des balles :
       //ball_coordinates_pub_ = nh_.advertise<std_msgs::Int32MultiArray>("ball_coordinates", 1000);
       
       
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
     
     //ImgDroite = cv_ptr->image.clone();
	
      try
      {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      }
      catch (cv_bridge::Exception & e)
      {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
      }
      


      /* traitement D'image*/

      /* 1ere etape conversion BRG en HSV (teinte / Saturation / Luminosité ) plus simple pour détection couleur */

      cvtColor(cv_ptr->image, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HS
      

      // Les 4 mires vertes:
       inRange(imgHSV, Scalar(45, 50, 0), Scalar(80, 255, 255), imgThresholded_Mires);



      /* traitement des marqueurs les 4 Mires */
       // morphological opening (remove small objects from the foreground)
      erode(imgThresholded_Mires, imgThresholded_Mires, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate( imgThresholded_Mires, imgThresholded_Mires, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      //morphological closing (fill small holes in the foreground)
      dilate( imgThresholded_Mires, imgThresholded_Mires, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      erode(imgThresholded_Mires, imgThresholded_Mires, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      	// Les 4 Mires
      SimpleBlobDetector::Params params_Mires;
      mireParams(&params_Mires);
     
      // Les 4 Mires
      vector<KeyPoint> keypoints_Mires;

      // Mires Color:création des détecteurs de composantes connexes ou blob à partir des paramètres
      Ptr<SimpleBlobDetector> detector_Mires = SimpleBlobDetector::create(params_Mires);
      detector_Mires->detect( imgThresholded_Mires, keypoints_Mires );


      int nbMires_Mires = keypoints_Mires.size();
      int nbMires_Mires_Ortho = keypoints_Mires.size();
     
      //cout << "Nombre total de Mires detectés : " << nbMires_Mires << " vertes "  << endl;

        // Mettre en ordre les 4 mires:
        //*************************************************************************************************************************************************************************************************
        
        bool erreurNombreBlob;
        
        if (nbMires_Mires == 4)
        
        {
        	erreurNombreBlob = false;
        
        // Les 4 Mires
        drawKeypoints(cv_ptr->image, keypoints_Mires, im_with_keypoints_Mires, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        
        
        char Mires[4][2]={"1", "2", "3", "4"};
        
       cv::Point2f keypointPos_Mires[4];
       float keypointArray_Mires[4][2];
       
       //***********TAB 2D**********************//
       
	for(int i=0; i<4; i++) 
	{
    		keypointArray_Mires[i][0] = keypointPos_Mires[i].x;
    		keypointArray_Mires[i][1] = keypointPos_Mires[i].y;
	}
        
        
        
        orderKeypoints(keypointPos_Mires,keypoints_Mires);
        
         for (int i = 0; i < 4; i++)
        {

          cv::putText(im_with_keypoints_Mires, //target image
//                      num[i], //text
                       Mires[i],

                      keypointPos_Mires[i], //top-left position
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
        
        
         
         
	// matrices de transformation - on centre les points objectifs, on centre et on met à l'échelle:
        cv::Mat t1 = (cv::Mat_<double>(3, 3) << 1, 0, -tx, 0, 1, -ty, 0, 0, 1);
        cv::Mat t2 = (cv::Mat_<double>(3, 3) << 1/coef, 0, 0, 0, 1/coef, 0, 0, 0, 1);
        cv::Mat t3 = (cv::Mat_<double>(3, 3) << 1, 0, tix, 0, 1, tiy, 0, 0, 1);

        //calcul de la matrice de transformation entre l'espace réel et l'image
        cv::Mat transform = cv::getPerspectiveTransform(objectivePoints, keypointPos_Mires);

        //calcul de la transformation inverse
        cv::Mat Inv = transform.inv();


        // Les 4 Mires Color
        cv::warpPerspective(im_with_keypoints_Mires,
                            im_with_keypointsOrtho,
                            transform*t3*t2*t1,
                            cv::Size(max_width, max_height),
                            cv::INTER_LINEAR | cv::WARP_INVERSE_MAP); 
         //Redressement imgHSV -> imgHSVOrtho
        cv::warpPerspective(imgHSV,
                            imgHSVOrtho,
                            transform*t3*t2*t1,
                            cv::Size(max_width, max_height),
                            cv::INTER_LINEAR | cv::WARP_INVERSE_MAP); 
                            
        
       // Partie traitement couleurs
       
       //Manal
         // Red Color
      inRange(imgHSVOrtho, Scalar(143, 103, 71), Scalar(179, 255, 255), imgThresholded_Red);
      
      
      //Corentin
      // Yellow Color
      inRange(imgHSVOrtho, Scalar(18, 97, 0), Scalar(48, 255, 255), imgThresholded_Yellow);
        
        
        /* détection de la couleur des cibles résultat image seuillée imgThresholdedTarget */

      // Bitwise-AND of mask and purple only image - only used for display
      // il faut remettre à 0 la matrice de résultat à chaque fois.
      // on met 255 comme cela le masque est bon
      // on pourra utiliser or et la couleur typique.
      res = 0;
      //bitwise_and(cv_ptr->image, 255, res,imgThresholded); // couleur naturelle
      //bitwise_or(cv_ptr->image, CV_RGB(255,0,0), res,imgThresholded); // couleur prédéfinie

      /* traitement des marqueurs RED Color */
      // morphological opening (remove small objects from the foreground)
      erode(imgThresholded_Red, imgThresholded_Red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate( imgThresholded_Red, imgThresholded_Red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      // morphological closing (fill small holes in the foreground)
      dilate( imgThresholded_Red, imgThresholded_Red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      erode(imgThresholded_Red, imgThresholded_Red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      
       /* traitement des marqueurs YELLOW Color */
       // morphological opening (remove small objects from the foreground)
      erode(imgThresholded_Yellow, imgThresholded_Yellow, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate( imgThresholded_Yellow, imgThresholded_Yellow, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      //morphological closing (fill small holes in the foreground)
      dilate( imgThresholded_Yellow, imgThresholded_Yellow, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      erode(imgThresholded_Yellow, imgThresholded_Yellow, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      

      
      // Paramètre de la détection de Blob

      
      // réglage des paramètres du détecteur des Jetons
	// By Manal: Red Color
      SimpleBlobDetector::Params params_Red;
      mireParams(&params_Red);
      	
      	// Corentin : Yellow Color
      SimpleBlobDetector::Params params_Yellow;
      mireParams(&params_Yellow);
      


      
      /* Définition des taches identifiées */
      // By Manal
      //RED Color
      vector<KeyPoint> keypoints_Red;
      // Corentin
      //YELLOW Color
      vector<KeyPoint> keypoints_Yellow;
      
      
      // Set up detector with params
      // Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
      // detector->detect( im, keypoints);

      // RED Color:création des détecteurs de composantes connexes ou blob à partir des paramètres
      Ptr<SimpleBlobDetector> detector_Red = SimpleBlobDetector::create(params_Red);
      detector_Red->detect( imgThresholded_Red, keypoints_Red );
      
      
      // YELLOW Color:création des détecteurs de composantes connexes ou blob à partir des paramètres
      Ptr<SimpleBlobDetector> detector_Yellow = SimpleBlobDetector::create(params_Yellow);
      detector_Yellow->detect( imgThresholded_Yellow, keypoints_Yellow );
      


      //the total no of blobs detected are:
      // By Manal: Red Color:
      int nbMire_Red = keypoints_Red.size();
      // Corentin: Yellow Color:
      int nbMire_Yellow = keypoints_Yellow.size();
      
      cout << "Nombre de jetons rouge : " << nbMire_Red << " Objects type 1/ Rouge "  << endl;
      cout << "Nombre de jetons Jaune : " << nbMire_Yellow << " Objects type 2/Jaune "  << endl;


      //the total no of blobs detected are:
      
      // Manal: Pas besoin de la variable erreur car on veut detecter un nombre de mire superieur à 4
      
      //bool erreurNombreBlob;
      
      // trace les keypoints marqueurs en vert sur l'image  im_with_keypoints à partir de l'image de base
        // By Manal
        // RED Color
        drawKeypoints(im_with_keypointsOrtho, keypoints_Red, im_with_keypoints_Red, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        
        //déclaration tableau pour les positions
	int TColonneCouleur[20]= {};
	int TLigneCouleur[20]= {};
        
        
        //By Manal:
        
        // RED Color
        
        char Red[2]="1";
        int ValCouleur = 1;
        
       cv::Point2f keypointPos_Red[nbMire_Red];
       
       int keypointArray_Red[nbMire_Red][2];
       
       //***********TAB 2D**********************//
       
	
               for (int i = 0; i < keypoints_Red.size(); i++)
        {
			keypointPos_Red[i]=keypoints_Red.at(i).pt;
			
		
        }
        
        
        for(int i=0; i<nbMire_Red; i++) 
	{
    		keypointArray_Red[i][0] = keypointPos_Red[i].x;
    		keypointArray_Red[i][1] = keypointPos_Red[i].y;
    		//test positions
    		//cout << "coordonee mire n°" << i << "  : "<< keypointArray_Red[i][0] << endl;
    		TestColonne( keypointArray_Red[i][0], nbMire_Red, PosXMire0, DX, TColonneCouleur, i);
    		//cout << "colonne numero : "<<TColonneCouleur[i] << endl;
    		TestLigne( keypointArray_Red[i][1], nbMire_Red, PosYMire3, DY, TLigneCouleur, i);
    		//cout << "coordonee mire n°" << i << "  : "<< keypointArray_Red[i][1] << endl;
    		//cout << "ligne numero : "<<TLigneCouleur[i] << endl;
    		MatriceResultat[TLigneCouleur[i]][TColonneCouleur[i]] = ValCouleur;
    		//cout << "couleur type : "<<MatriceResultat[TColonneCouleur[i]][TLigneCouleur[i]] << endl;
    		
	}
  
       
        
        for (int i = 0; i < keypoints_Red.size(); i++)
        {

          cv::putText(im_with_keypoints_Red, //target image
//                      num[i], //text
                       Red,

                      keypointPos_Red[i], //top-left position
                      cv::FONT_HERSHEY_DUPLEX,
                      1.0,
                      CV_RGB(0, 0, 0), //font color
                      2);
        }
        
        
        drawKeypoints(im_with_keypoints_Red, keypoints_Yellow, im_with_keypoints_Yellow, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        
        //Yellow Color
        
	TLigneCouleur[20]= {};
	TColonneCouleur[20]= {};
        char Yellow[2]="2";
        ValCouleur = 2;
       cv::Point2f keypointPos_Yellow[nbMire_Yellow];
       
       float keypointArray_Yellow[nbMire_Yellow][2];
       
       //***********TAB 2D**********************//
       
               for (int i = 0; i < keypoints_Yellow.size(); i++)
        {
			keypointPos_Yellow[i]=keypoints_Yellow.at(i).pt;
			
			
		//cout << "Coordonée mire " <<i<< ":" << keypointPos_Yellow[i] << endl;
        }
        
        
        
        		for(int i=0; i<nbMire_Yellow; i++) 
	{
    		keypointArray_Yellow[i][0] = keypointPos_Yellow[i].x;
    		keypointArray_Yellow[i][1] = keypointPos_Yellow[i].y;
    		//test positions
    		//cout << "coordonee mire n°" << i << "  : "<< keypointArray_Yellow[i][0] << endl;
    		TestColonne( keypointArray_Yellow[i][0], nbMire_Yellow, PosXMire0, DX, TColonneCouleur, i);
    		//cout << "colonne numero : "<<TColonneCouleur[i] << endl;
    		TestLigne( keypointArray_Yellow[i][1], nbMire_Yellow, PosYMire3, DY, TLigneCouleur, i);
    		//cout << "coordonee mire n°" << i << "  : "<< keypointArray_Yellow[i][1] << endl;
    		//cout << "ligne numero : "<<TLigneCouleur[i] << endl;
    		MatriceResultat[TLigneCouleur[i]][TColonneCouleur[i]] = ValCouleur;
    		//cout << "couleur type : "<<MatriceResultat[TColonneCouleur[i]][TLigneCouleur[i]] << endl;
	}
 //affichage matrice pour le rouge et jaune
 ///*      
       cout <<endl;
       cout << "Matrice representant l'etat du jeu en direct :" << endl;
       for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            cout << MatriceResultat[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
 //*/       
        
        //orderKeypoints(keypointPos,keypoints);
		
        
        for (int i = 0; i < keypoints_Yellow.size(); i++)
        {

          cv::putText(im_with_keypoints_Yellow, //target image
//                      num[i], //text
                       Yellow,

                      keypointPos_Yellow[i], //top-left position
                      cv::FONT_HERSHEY_DUPLEX,
                      1.0,
                      CV_RGB(0, 0, 0), //font color
                      2);
        }


// RECHERCHE COORDONNEE 4 MIRES DANS L IMAGE ORTHO imgHSVOrtho

	// Les 4 mires vertes:
       inRange(imgHSVOrtho, Scalar(45, 50, 0), Scalar(80, 255, 255), imgThresholded_Mires);
       
       
       // traitement des marqueurs les 4 Mires 
       // morphological opening (remove small objects from the foreground)
      erode(imgThresholded_Mires, imgThresholded_Mires, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate( imgThresholded_Mires, imgThresholded_Mires, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

      //morphological closing (fill small holes in the foreground)
      dilate( imgThresholded_Mires, imgThresholded_Mires, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      erode(imgThresholded_Mires, imgThresholded_Mires, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

     
      // Les 4 Mires
      vector<KeyPoint> keypoints_MiresOrtho;

      // Mires Color:création des détecteurs de composantes connexes ou blob à partir des paramètres
      Ptr<SimpleBlobDetector> detector_Mires = SimpleBlobDetector::create(params_Mires);
      detector_Mires->detect( imgThresholded_Mires, keypoints_MiresOrtho );


      nbMires_Mires_Ortho = keypoints_MiresOrtho.size();
      
      if (  nbMires_Mires_Ortho == 4)
      {
      
        
        
       cv::Point2f keypointPos_Mires_Ortho[4];
        
        for (int i = 0; i < keypoints_MiresOrtho.size(); i++)
        {
			keypointPos_Mires_Ortho[i]=keypoints_MiresOrtho.at(i).pt;
			//cout << "Coordonée mire " <<i<< ":" << keypointPos_Mires_Ortho[i] << endl;
        }
        orderKeypoints(keypointPos_Mires_Ortho,keypoints_MiresOrtho);
        
       }
       
       else if ( nbMires_Mires_Ortho =! 4)
       
       {
       	
	printf("Erreur nombre de blob Ortho detecte\n");
	
	}

    //********************************************************************************************
    //publication matrice
    
    
    
    
    
    
    
    
    
    
       }
       else if (nbMires_Mires != 4)
      {
        erreurNombreBlob = true;
        printf("Erreur nombre de blob detecte\n");
      }
                   
      
      //Affichage image brut

      image_pub_1_.publish(cv_ptr->toImageMsg());
 //     imshow(OPENCV_WINDOW, cv_ptr->image);
       waitKey(3);
       
        
        
        
		//objectiveDefinition(objectivePoints);
        

       	sensor_msgs::ImagePtr msg_output_2 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", imgHSVOrtho).toImageMsg();
       	sensor_msgs::ImagePtr msg_output_3 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", im_with_keypoints_Yellow).toImageMsg();
	sensor_msgs::ImagePtr msg_output_4 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", im_with_keypointsOrtho).toImageMsg();
	//image_pub_2_.publish(msg_output_2);
	image_pub_3_.publish(msg_output_3);
	//image_pub_4_.publish(msg_output_4);
	
	
	//Publication de la matrice:
	
	ros::NodeHandle nh1("node1");
	//ros::init(argc, argv, "node1");
	
	ros::Publisher pub = nh1.advertise<std_msgs::Int32MultiArray>("ma_matrice",1000);
	
	std_msgs::Int32MultiArray matrice;
  	matrice.layout.dim.push_back(std_msgs::MultiArrayDimension());
  	matrice.layout.dim[0].size = 5;
  	matrice.layout.dim[0].stride = 5;
  	matrice.layout.dim[0].label = "rows";
  	matrice.layout.dim.push_back(std_msgs::MultiArrayDimension());
  	matrice.layout.dim[1].size = 5;
  	matrice.layout.dim[1].stride = 1;
  	matrice.layout.dim[1].label = "cols";
  	matrice.data.resize(25); // Définit la taille du tableau de données

  	int rows = 5;
  	int cols = 5;

  	memcpy(&matrice.data[0], &MatriceResultat[0][0], 25 * sizeof(int));

  	
/*  	
  	 ROS_INFO("Matrice publiée :");
  	for(int i = 0; i < rows; i++)
  	{
      		for(int j = 0; j < cols; j++)
      		{
          	ROS_INFO("%d\t", matrice.data[i*cols + j]);
      	}
      	ROS_INFO("\n");
  }
*/
  	pub.publish(matrice);
  
}
};

int main(int argc, char** argv)
{
  //ros::init(argc, argv, "image_converter");
  
  // Le publisher pour publier la matrice des couleurs:
  ros::init(argc, argv, "node1");
	
	
      	
            
	
	
  			

  ImageConverter ic;
  ros::spin();
  return 0;
}
