#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <boost/bind.hpp>
#include <unistd.h>

#include "graphAI.hpp"

using namespace std;
int lvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] = {{0}};
int lnSelectedColumn;

void receive_int_callback( const std_msgs::Int32::ConstPtr& psMsg)
{
    ROS_INFO("Valeur recu : %d", psMsg->data);
}
/*void matriceCallback(const std_msgs::Int32MultiArray::ConstPtr& msg)
{
    int lvMatrice[5][5];
    int rows = msg->layout.dim[0].size;
    int cols = msg->layout.dim[1].size;
    int index = 0;
    ROS_INFO("Matrice reçue :");
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            ROS_INFO("%d\t", msg->data[index]);
            lvMatrice[i][j] = msg->data[index];
            index++;
        }
        ROS_INFO("\n");
    }
}
*/
int main(int argc, char **argv)
{
    int lnGameMode = gn_EASY_MODE;
    string lw_file_path;
    switch (lnGameMode)
    {
    case gn_EASY_MODE:
        lw_file_path = MAIN_GRAPH;
        break;
    case gn_MEDIUM_MODE:
        lw_file_path = MAIN_GRAPH;
        break;
    case gn_HARD_MODE:
        lw_file_path = MAIN_GRAPH;
        break;
    default:
        lw_file_path = MAIN_GRAPH;
        break;
    }
    //Importation du graphe principal de l IA 
    GraphAI graph(MAIN_GRAPH);
    graph.importFromFile();
    map<std::string, Node *>& lsGraphMap = graph.getGraphMap();
    
    //Vecteur contenant les differnts plateau recontrés lors d'une partie
    vector<string> lvEncounteredPositions;
    lvEncounteredPositions.reserve(cnSIZE_OF_BOARD * cnSIZE_OF_BOARD);
    vector<string>::iterator lnEncounteredIterator;
    lnEncounteredIterator = lvEncounteredPositions.begin();

    //Noeud courant du graphe
    string lsActual = graph.getRoot()->getPositionName();
    
    //Plateau de jeu
    
    //Joueur courant, colonne selectionnée pour jouer, la ligne jouee, le nombre de coup joues dans la partie
    int lnCurrentPlayer, lnSelectedColomn, lnRowPlayed, lnMoveCounter;
    //Statue du jeux, pas fini, fini avec gagnant, fini match null
    gameStatus lnPositionStatus;
    //Permet de savoir si le coup a ete joué
    bool lbIsPlayed;

    //Init ROS
    ros::init(argc, argv, "node2");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("ma_matrice", 1000, receive_int_callback);
    ros::spin();

    //Initialisation des valeurs
    lnPositionStatus = gnGameNotFinished;
    lnCurrentPlayer = cnIA;
    lnMoveCounter = 0;
    
    //Affichage du plateau
    gameDisplay(lvBoardGame);
    // while(lnPositionStatus == gnGameNotFinished)
    {
        lbIsPlayed = false;

        if(lnCurrentPlayer == cnIA) // Tour de l'IA
        {
            cout << "C est le tour de l'IA " << endl;
            while (!lbIsPlayed)
            {
                //Choix de la colonne dans laquelle jouée
                lnSelectedColomn = graph.playAI(lvBoardGame, lsGraphMap[lsActual], lnGameMode);
                //Placement du jeton et verification de a possibilite de jeu
                tie(lbIsPlayed,lnRowPlayed) = play(lvBoardGame, lnSelectedColomn, lnCurrentPlayer); 
            }
        }
        else // Tour du joueur
        {
            cout << "C est votre tour "  << endl;
            
            while (!lbIsPlayed)
            {
                cout << "Sur quelle colonne voulez vous jouez (0 à 4)"<< endl;
                cin >> lnSelectedColomn;
                lnSelectedColomn = lnSelectedColomn % 5;
                tie(lbIsPlayed,lnRowPlayed) = play(lvBoardGame,lnSelectedColomn,lnCurrentPlayer);
                if (!lbIsPlayed)
                    cout << "Colonne Pleine !" << endl;
            }
        }
        //Ajout du plateau courant dans le graphe
        string lwNextNodeRepr = lsGraphMap[lsActual]->calculateNewPositionValue(lnSelectedColomn, lnMoveCounter);
        graph.appendChildToParent(lsActual, lnSelectedColomn, lwNextNodeRepr);
        lsActual = move(lwNextNodeRepr);
 
        lnEncounteredIterator = lvEncounteredPositions.insert(lnEncounteredIterator, lsActual);
        
        //Affichage et verification status du jeu
        gameDisplay(lvBoardGame);
        lnPositionStatus = whatGameStatus(lvBoardGame,lnCurrentPlayer);
        lnCurrentPlayer = (lnCurrentPlayer) % 2 + 1;
        lnMoveCounter++;

    }
    //Jeu fini verification gagnant pu match null
    if (lnPositionStatus != gnStaleMate)
    {
        graph.calculateWeights(lvEncounteredPositions, false);
        if(lnCurrentPlayer == cnIA)
        {
            cout << "Vous avez gagné  " << endl;
        }
        else
        {
            cout << "Le gagnant est l'IA  " << endl;
        }
    }
    else 
    {
        graph.calculateWeights(lvEncounteredPositions, true);
        cout << "Le Match est ex aequo  " << endl;
    }
    //Exportation du graphe dans son fichier
    graph.exportToFile();
    graph.deleteNodes();
    return 0;
}
