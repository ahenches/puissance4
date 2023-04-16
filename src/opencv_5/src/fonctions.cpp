#include "fonctions.h"

/* conversion d'un type en chaine de caractères */

string type2str(int type)
{
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth )
  {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans + '0');

  return r;
}

// remplissage des paramètres du détecteur de blob

void mireParams(SimpleBlobDetector::Params *params)
{
      params->filterByColor = false;
      // Change thresholds
      params->minThreshold = 0;
      params->maxThreshold = 255;

      // Filter by Area.
      params->filterByArea = true;
      params->minArea = 100;
//      params->maxArea = 5000;

      // Filter by Circularity
      params->filterByCircularity = false; //False au debut
      params->minCircularity = 0.1;

      // Filter by Convexity
      params->filterByConvexity = false;
      params->minConvexity = 0.5;

      // Filter by Inertia
      params->filterByInertia = false;
      params->minInertiaRatio = 0.5;

}


void targetParams(SimpleBlobDetector::Params *params)
 {
      params->filterByColor = false;
      // Change thresholds
      params->minThreshold = 0;
      params->maxThreshold = 255;

      // Filter by Area.
      params->filterByArea = true;
      params->minArea = 100; //500 permet de bien voir l'influence sur le rosbag de test
//      params->maxArea = 6000;

      // Filter by Circularity
      params->filterByCircularity = false;
      params->minCircularity = 0.1;

      // Filter by Convexity
      params->filterByConvexity = false;
      params->minConvexity = 0.5;

      // Filter by Inertia
      params->filterByInertia = false;
      params->minInertiaRatio = 0.5;

}
/* Met dans l'ordre les 4 points à mettre en correspondance */
/* 1    2 */
/* 4    3 */


// Modification By Manal: changement de nombre de point de 4 à 21

void orderKeypoints(cv::Point2f *keypointPos,vector<KeyPoint> keypoints)
{
cv::Point2f keypointTemp[4];


        for (int i = 0; i < 4; i++)
        {
			keypointTemp[i]=keypoints.at(i).pt;
			
			//cout << "Coordonée" <<i<< ":" << keypointTemp[i]<< " vertes "  << endl;
        }

        // calcul du barycentre des 4 points

        int xtemp = (keypointTemp[0].x + keypointTemp[1].x + keypointTemp[2].x + keypointTemp[3].x) / 4;
        int ytemp = (keypointTemp[0].y + keypointTemp[1].y + keypointTemp[2].y + keypointTemp[3].y) / 4;


        for (int i = 0; i < 4; i++)
        {
          //Attribution des positions des keypoints dans l'image dans l'ordre pour la correspondance
          // avec les points réels

          if (keypointTemp[i].x <= xtemp && keypointTemp[i].y <= ytemp)
          {
            keypointPos[0]= keypointTemp[i];
          }
          else if (keypointTemp[i].x >= xtemp && keypointTemp[i].y <= ytemp)
          {
            keypointPos[1]= keypointTemp[i];
          }
           else if (keypointTemp[i].x >= xtemp && keypointTemp[i].y >= ytemp)
          {
            keypointPos[2]= keypointTemp[i];
          }
         else if (keypointTemp[i].x <= xtemp && keypointTemp[i].y >= ytemp)
          {
            keypointPos[3]= keypointTemp[i];          }
        }
        
}

 

// Added By Manal:

/*
void objectiveDefinition(cv::Point2f *objectivePoints)
{
    // Define the coordinates of 20 keypoints in 2D space
    int num_points = 100;
    float spacing = 50.0f; // spacing between keypoints

    for (int i = 0; i < num_points; i++) {
        int row = i / 5; // row index
        int col = i % 5; // column index
        float x = (col - 2) * spacing; // x-coordinate
        float y = (row - 2) * spacing; // y-coordinate
        objectivePoints[i] = cv::Point2f(x, y);
    }
}   */

void objectiveDefinition(cv::Point2f *objectivePoints)
{


        // définition des coordonnées des points dans l'espace, l'origine est choisie est au centre mais pourra être changée


        objectivePoints[0].x = -largeur / 2; objectivePoints[0].y = -hauteur / 2;
        objectivePoints[1].x = largeur / 2; objectivePoints[1].y = -hauteur / 2;
        objectivePoints[2].x = largeur / 2; objectivePoints[2].y = hauteur / 2;
        objectivePoints[3].x = -largeur / 2; objectivePoints[3].y = hauteur / 2;


        objectivePoints[0].x = 0; objectivePoints[0].y = 0;
        objectivePoints[1].x = largeur; objectivePoints[1].y = 0;
        objectivePoints[2].x = largeur; objectivePoints[2].y = hauteur;
        objectivePoints[3].x = 0; objectivePoints[3].y = hauteur;
        

}

// Calcul des positions des cibles dans le repère du monde

void calculPoints(cv::Point2f *keypointTargetPos,cv::Point2f *keypointTargetPosTrans, int nbTarget, cv::Mat Inv)
{
        for (int i = 0; i < nbTarget; i++)
        {
          float xi, yi;
          xi = keypointTargetPos[i].x;
          yi = keypointTargetPos[i].y;

          double p_arr[3];
          cv::Mat p(3, 1 , CV_64FC1, p_arr);

          p_arr[0] = xi;
          p_arr[1] = yi;
          p_arr[2] = 1;


          //calcul des coordonée du point dans l'espace réel à partir des coordonnées dans l'image
          cv::Mat V =  Inv * p ;


          // extraction du résultat avec conversion de type

          double xd = V.at<double>(0, 0);
          double yd = V.at<double>(1, 0);
          double zd = V.at<double>(2, 0);

          //normailisation par zd necessaire
          //calculs du points en fonction des valeurs réel blob i 
          keypointTargetPosTrans[i].x = (xd / zd);
          keypointTargetPosTrans[i].y = (yd / zd);

          printf("dans le repère fixe : x = %f y = %f\n ", keypointTargetPosTrans[i].x, keypointTargetPosTrans[i].y);

//          printf(" xtransCentre = %f\n ytransCentre = %f\n ", keypointTargetPosTransCentre[i].x, keypointTargetPosTransCentre[i].y);
        }

}

void readParam(void)

	{
		std::ifstream infile;
		string type;
		infile.open(FILE_NAME, std::ofstream::in); // append instead of overwrite
		infile >> type;
		infile >> iLowH >> iHighH >> iLowS >> iHighS >> iLowV >> iHighV;
  		cout <<type <<" "<<iLowH<<" "<< iHighH << " "<<iLowS<<" "<< iHighS << " "<<iLowV<<" "<< iHighV <<std::endl; 


  		infile.close();
//  		exit(0);

}

void TestColonne(int keypointArray, int nbMire, int PosXMire0, int DX, int* TColonneCouleur, int i)
{
	int vY = 15;
	for (int i=0; i<nbMire; i++)
	{
		if ((PosXMire0 + (1*DX) - vY < keypointArray ) && (keypointArray < PosXMire0 + (1*DX) + vY))
		{
			TColonneCouleur[i] = 0;
		}
		else if ((PosXMire0 + (2*DX) - vY < keypointArray ) && (keypointArray < PosXMire0 + (2*DX) + vY))
		{
			TColonneCouleur[i] = 1;
		}
		else if ((PosXMire0 + (3*DX) - vY < keypointArray) && (keypointArray< PosXMire0 + (3*DX) + vY))
		{
			TColonneCouleur[i] = 2;
		}
		else if ((PosXMire0 + (4*DX) - vY < keypointArray) && (keypointArray< PosXMire0 + (4*DX) + vY))
		{
			TColonneCouleur[i] = 3;
		}
		else if ((PosXMire0 + (5*DX) - vY < keypointArray) && (keypointArray< PosXMire0 + (5*DX) + vY))
		{
			TColonneCouleur[i] = 4;
		}
	}
}

void TestLigne(int keypointArray,int nbMire,int PosYMire4,int DY, int* TLigneCouleur, int i)
{
	int vY = 20;
	for (int i=0; i<nbMire; i++)
	{
		if ((PosYMire4 + 0*DY - vY < keypointArray) && (keypointArray < PosYMire4 + 0*DY + vY))
		{
			TLigneCouleur[i] = 0;
		}
		else if ((PosYMire4 + 1*DY - vY < keypointArray) && (keypointArray < PosYMire4 + 1*DY + vY))
		{
			TLigneCouleur[i] = 1;
		}
		else if ((PosYMire4 + 2*DY - vY < keypointArray) && (keypointArray < PosYMire4 + 2*DY + vY))
		{
			TLigneCouleur[i] = 2;
		}
		else if ((PosYMire4 + 3*DY - vY < keypointArray) && (keypointArray < PosYMire4 + 3*DY + vY))
		{
			TLigneCouleur[i] = 3;
		}
		else if ((PosYMire4 + 4*DY - vY < keypointArray) && (keypointArray < PosYMire4 + 4*DY + vY))
		{
			TLigneCouleur[i] = 4;
		}
	}
}
