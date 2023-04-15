#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>

int main(int argc, char **argv)
{
  // Initialisation du node avec un nom unique
  ros::init(argc, argv, "mon_publisher");

  // Création de l'objet NodeHandle
  ros::NodeHandle nh;

  // Création de l'objet Publisher pour envoyer des messages de type "std_msgs::Int32MultiArray" sur le topic "ma_matrice"
  ros::Publisher pub = nh.advertise<std_msgs::Int32MultiArray>("ma_matrice", 1000);

  // Fréquence de publication des messages
  ros::Rate rate(10);

  // Boucle principale
  while (ros::ok())
  {
    // Création d'un message de type "std_msgs::Int32MultiArray"
    std_msgs::Int32MultiArray msg;

    // Définition de la taille de la matrice (3x3)
    msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
    msg.layout.dim[0].size = 3;
    msg.layout.dim[0].stride = 3 * 3;
    msg.layout.dim[0].label = "row";
    msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
    msg.layout.dim[1].size = 3;
    msg.layout.dim[1].stride = 3;
    msg.layout.dim[1].label = "col";

    // Remplissage de la matrice
    int mat[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            msg.data.push_back(mat[i][j]);
        }
    }

    // Publication du message sur le topic
    pub.publish(msg);

    // Attente pour respecter la fréquence de publication
    rate.sleep();
  }

  return 0;
}

