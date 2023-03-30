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
  Exécute le kernel (2 ordinateurs jouent ensemble afin d'étoffer l'ia

/////////////////////////////////////////////////////////////////////////////*/
void doGame(vector<GraphAI> *lvGraphs, char *lwMode)
{
    vector<string> lvEncounteredPositions;
    lvEncounteredPositions.reserve(cnSIZE_OF_BOARD * cnSIZE_OF_BOARD);
    vector<string>::iterator lnEncounteredIterator;
    lnEncounteredIterator = lvEncounteredPositions.begin();
    
    string lsActual = graph->getRoot()->getPositionName();
    
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
        string lwNextNodeRepr = *(lvGraph)[0].getNode(lsActual)->calculateNewPositionValue(lnSelectedColomn, lnMoveCounter);
        graph->appendChildToParent(lsActual, lnSelectedColomn, lwNextNodeRepr);
        lsActual = move(lwNextNodeRepr);
        gameDisplay(lvBoardGame);
        lnEncounteredIterator = lvEncounteredPositions.insert(lnEncounteredIterator, lsActual->getPositionName());
        
        lnPositionStatus = whatGameStatus(lvBoardGame,lnCurrentPlayer);

        lnCurrentPlayer = (lnCurrentPlayer) % 2 + 1;
        
        lnMoveCounter++;
    }
    //for (int i = 0; i < lvEncounteredPositions.size(); i++)
    //    cout << (new Node)->printPositionName(lvEncounteredPositions[i]) << endl;

    graph->calculateWeights(lvEncounteredPositions, false);
    for (auto lsGraph : lvGraphs)
    {
        lsGraph.exportToFile();
        lsGraph.deleteNodes();
    }
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
    }
    else 
    {
        cout << "Le Match est ex aequo  " << endl;
    }
}

int main(int argc, char **argv) // nb_parties, lwMode
{
    if (argc != 3)
    {
        printf("Erreur nombre d'arguments invalide.\nUtilisation : ./<executable> <nb_parties> <lwMode>\n");
        exit(1);
    }
    else 
    {
        int lnNbReps = atoi(argv[1]);
        char *lwMode = argv[2];
        
        vector<GraphAI> lvGraphs;

        GraphAI lsMainGraph(MAIN_GRAPH);
        lvGraphs.push_back(lsMainGraph);
        
        GraphAI lsParticularGraph(PARTICULAR_GRAPH);
        lvGraphs.push_back(lsParticularGraph);

        cout << "Graphs import" << endl;
        for (auto lsGraph : lvGraphs)
        {
            lsGraph.importFromFile();
        }
        
        for (int i = 0 ; i < lnNbReps; i++)
        {
            doGame(&lvGraphs, lwMode);
        }

        cout << "Graphs export" << endl;
        for (auto lsGraph : lvGraphs)
        {
            lsGraph.exportToFile();
            lsGraph.deleteNodes();
        }
        return 0;
    }
}
