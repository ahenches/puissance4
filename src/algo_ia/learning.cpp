#include "boardGame.hpp"

#include "graphAI.hpp"
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <vector>

#define PARTICULAR_GRAPH 

using namespace std;

/*/////////////////////////////////////////////////////////////////////////////
Fonction doGame()

Auteur : Arnaud HENCHES (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
  Exécute le kernel (2 ordinateurs jouent ensemble afin d'étoffer l'ia)

/////////////////////////////////////////////////////////////////////////////*/
void doGame(vector<GraphAI *> *lvGraphs, char *lwMode)
{
    vector<string> lvEncounteredPositions;
    lvEncounteredPositions.reserve(cnSIZE_OF_BOARD * cnSIZE_OF_BOARD);
    vector<string>::iterator lnEncounteredIterator;
    lnEncounteredIterator = lvEncounteredPositions.begin();
    
    string lsActual = ((*lvGraphs)[0])->getRoot()->getPositionName();
    
    int lvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] = {{0}};
    int lnCurrentPlayer, lnSelectedColomn, lnRowPlayed, lnMoveCounter;
    gameStatus lnPositionStatus;
    bool lbIsPlayed;


    lnPositionStatus = gnGameNotFinished;
    lnCurrentPlayer = cnIA;
    lnMoveCounter = 0;
    
    gameDisplay(lvBoardGame);
    while(lnPositionStatus == gnGameNotFinished)
    {
        lbIsPlayed = false;
        while (!lbIsPlayed)
        {
            if(strcmp("rand", lwMode) == 0) 
            {
                lnSelectedColomn = moveAtRandom(lvBoardGame);
            }
            else if (strcmp("algo", lwMode) == 0)
            { 
                lnSelectedColomn = calculateBestMove(lvBoardGame);
            }
            cout << "lsActual " << lsActual << endl;
            tie(lbIsPlayed,lnRowPlayed) = play(lvBoardGame,lnSelectedColomn, lnCurrentPlayer);
        }
        cout << "Ceci est la colonne jouée " << lnSelectedColomn << endl;
        string lwNextNodeRepr = ((*lvGraphs)[0])->getNode(lsActual)->calculateNewPositionValue(lnSelectedColomn, lnMoveCounter);
	for (GraphAI * lsGraph : *lvGraphs)
        lsGraph->appendChildToParent(lsActual, lnSelectedColomn, lwNextNodeRepr);
        lsActual = move(lwNextNodeRepr);
        gameDisplay(lvBoardGame);
        lnEncounteredIterator = lvEncounteredPositions.insert(lnEncounteredIterator, lsActual);
        
        lnPositionStatus = whatGameStatus(lvBoardGame,lnCurrentPlayer);

        lnCurrentPlayer = (lnCurrentPlayer) % 2 + 1;
        
        lnMoveCounter++;
    }
    //for (int i = 0; i < lvEncounteredPositions.size(); i++)
    //    cout << (new Node)->printPositionName(lvEncounteredPositions[i]) << endl;

    if (lnPositionStatus != gnStaleMate)
    {

        if(lnCurrentPlayer == cnIA)
        {
            cout << "Vous avez gagné  " << endl;
        }
        else
        {
            cout << "Le gagnant est l'IA  " << endl;
        }
        for (GraphAI * lsGraph : *lvGraphs)
            lsGraph->calculateWeights(lvEncounteredPositions, false);
    }
    else 
    {
        cout << "Le Match est ex aequo  " << endl;
        for (GraphAI * lsGraph : *lvGraphs)
            lsGraph->calculateWeights(lvEncounteredPositions, true);
    }
}

int main(int argc, char **argv) // nb_parties, lwMode
{
    if (argc != 3)
    {
        cout << "Erreur nombre d'arguments invalide.\nUtilisation : ./<executable> <nombre de parties> <mode>\n" << endl;
        exit(1);
    }
    else 
    {
        char * lwNbReps = argv[1];
        int lnNbReps = atoi(lwNbReps);
        char * lwMode = argv[2];
        char lwGraphParticular[12+strlen(lwMode)+1+strlen(lwNbReps)+4] = "files/graph_";
        strcat(lwGraphParticular, lwMode);  
        strcat(lwGraphParticular, "_");  
        strcat(lwGraphParticular, lwNbReps);        
        strcat(lwGraphParticular, ".txt");        
        
        printf("%s\n", lwGraphParticular);

        vector<GraphAI *> lvGraphs;
        
        
        lvGraphs.push_back(new GraphAI(MAIN_GRAPH));
        lvGraphs.push_back(new GraphAI(lwGraphParticular));

        cout << "Graphs import" << endl;
        for (GraphAI * lsGraph : lvGraphs)
        {
            lsGraph->importFromFile();
        }
        
        for (int i = 0 ; i < lnNbReps; i++)
        {
            doGame(&lvGraphs, lwMode);
        }

        cout << "Graphs export" << endl;
        for (GraphAI * lsGraph : lvGraphs)
        {
            lsGraph->exportToFile();

            lsGraph->deleteNodes();
            delete(lsGraph);
        }
        return 0;
    }
}
