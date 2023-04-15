#include <stdio.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>

#include "graphAI.hpp"

using namespace std;

int main(int argc, char **argv)
{
    // Mode de jeu de la partie
    int lnGameMode;
    string lwTmp;
    cout << argc << endl;
    if(argc == 2)
    {
        if(strcmp(argv[1], "f") == 0)
        {
            lnGameMode = gn_EASY_MODE;
            cout << "MODE : FACILE" << endl;
        }
        else if(strcmp(argv[1], "m") == 0)
        {
            lnGameMode = gn_MEDIUM_MODE;
            cout << "MODE : MOYEN" << endl;
        }
        else if(strcmp(argv[1], "d") == 0)
        {
            lnGameMode = gn_HARD_MODE;
            cout << "MODE : DIFFICILE" << endl;
        }
        else {
            lnGameMode = gn_MEDIUM_MODE;
            cout << "MODE : MOYEN" << endl;
        }
    }
    else
    {
       lnGameMode = gn_MEDIUM_MODE;
       cout << "MODE : MOYEN" << endl;
    }
     
    string lw_file_path;
    switch (lnGameMode)
    {
    case gn_EASY_MODE:
        lw_file_path = EASY_GRAPH;
        break;
    case gn_MEDIUM_MODE:
        lw_file_path = MEDIUM_GRAPH;
        break;
    case gn_HARD_MODE:
        lw_file_path = MAIN_GRAPH;
        break;
    default:
        lw_file_path = MAIN_GRAPH;
        break;
    }
    const char *lw_file_path_char = lw_file_path.c_str();
    //Importation du graphe principal de l IA 
    GraphAI graph(lw_file_path_char);
    GraphAI mainGraph(MAIN_GRAPH);
    mainGraph.importFromFile();
    graph.importFromFile();
    map<std::string, Node *>& lsGraphMap = mainGraph.getGraphMap();
    
    //Vecteur contenant les differnts plateau recontrés lors d'une partie
    vector<string> lvEncounteredPositions;
    lvEncounteredPositions.reserve(cnSIZE_OF_BOARD * cnSIZE_OF_BOARD);
    vector<string>::iterator lnEncounteredIterator;
    lnEncounteredIterator = lvEncounteredPositions.begin();

    //Noeud courant du graphe
    string lsActual = mainGraph.getRoot()->getPositionName();
    
    //Plateau de jeu
    int lvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] = {{0}};

    //Joueur courant, colonne selectionnée pour jouer, la ligne jouee, le nombre de coup joues dans la partie
    int lnCurrentPlayer, lnSelectedColomn, lnRowPlayed, lnMoveCounter;
    //Statue du jeux, pas fini, fini avec gagnant, fini match null
    gameStatus lnPositionStatus;
    //Permet de savoir si le coup a ete joué
    bool lbIsPlayed;

    //Initialisation des valeurs
    lnPositionStatus = gnGameNotFinished;
    lnCurrentPlayer = cnIA;
    lnMoveCounter = 0;
    
    //Affichage du plateau
    gameDisplay(lvBoardGame);
    while(lnPositionStatus == gnGameNotFinished)
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
                do
                {
                    cout << "Sur quelle colonne voulez vous jouez (1 à 5)"<< endl;
                    cin >> lwTmp;
                } while (lwTmp.compare("1") != 0 && lwTmp.compare("2") != 0 && lwTmp.compare("3") != 0 && lwTmp.compare("4") != 0 && lwTmp.compare("5") != 0);
                
                lnSelectedColomn = stoi(lwTmp);
                lnSelectedColomn = lnSelectedColomn - 1;
                tie(lbIsPlayed,lnRowPlayed) = play(lvBoardGame,lnSelectedColomn,lnCurrentPlayer);
                if (!lbIsPlayed)
                    cout << "Colonne Pleine !" << endl;
            }
        }
        //Ajout du plateau courant dans le graphe
        string lwNextNodeRepr = lsGraphMap[lsActual]->calculateNewPositionValue(lnSelectedColomn, lnMoveCounter);
        mainGraph.appendChildToParent(lsActual, lnSelectedColomn, lwNextNodeRepr);
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
        mainGraph.calculateWeights(lvEncounteredPositions, false);
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
        mainGraph.calculateWeights(lvEncounteredPositions, true);
        cout << "Le Match est ex aequo  " << endl;
    }
    //Exportation du graphe dans son fichier
    mainGraph.exportToFile();
    graph.deleteNodes();
    mainGraph.deleteNodes();
    return 0;
}
