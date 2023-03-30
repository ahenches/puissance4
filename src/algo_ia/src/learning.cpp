#include "boardGame.hpp"

#include "graphAI.hpp"
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;


void doGame(GraphAI *graph, char *mode)
{
    //vector<string *> lvEncounteredPositions;
    //lvEncounteredPositions.reserve(cnSIZE_OF_BOARD * cnSIZE_OF_BOARD);
    //vector<string *>::iterator lnEncounteredIterator;
    //lnEncounteredIterator = lvEncounteredPositions.begin();

    // Node *lsActual = graph->getRoot();
    map<std::string, Node *>& lsGraphMap = graph->getGraphMap();
    string lsActual = graph->getRoot()->getPositionName();
    
    int lvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] = {{0}};
    int lnCurrentPlayer, lnSelectedColomn, lnRowPlayed, lnMoveCounter;
    gameStatus lnPositionStatus;
    bool lbIsPlayed;


    //gameDisplay(lvBoardGame);
    lnPositionStatus = gnGameNotFinished;
    lnCurrentPlayer = cnIA;
    lnMoveCounter = 0;
    
    gameDisplay(lvBoardGame);
    //cout << calculateBestMove(lvBoardGame) << endl;
    while(lnPositionStatus == gnGameNotFinished)
    {
        lbIsPlayed = false;
        while (!lbIsPlayed)
        {
            if(strcmp("rand", mode) == 0) 
            {
                lnSelectedColomn = moveAtRandom(lvBoardGame);
            }
            else if (strcmp("algo", mode) == 0)
            { 
                lnSelectedColomn = calculateBestMove(lvBoardGame);
            }
            cout << "lsActual " << lsActual << endl;
            tie(lbIsPlayed,lnRowPlayed) = play(lvBoardGame,lnSelectedColomn, lnCurrentPlayer); 
        }
        cout << "Ceci est la colonne jouée " << lnSelectedColomn << endl;
        string lwNextNodeRepr = lsGraphMap[lsActual]->calculateNewPositionValue(lnSelectedColomn, lnMoveCounter);
        graph->appendChildToParent(lsActual, lnSelectedColomn, lwNextNodeRepr);
        lsActual = move(lwNextNodeRepr);
        // cout << "CC " << *lnEncounteredIterator << endl;
        //cout << "SIZE lvEncounteredPositions " << lvEncounteredPositions.size() << endl;
        // lvEncounteredPositions[lnMoveCounter] = lsActual->getPositionName();
        gameDisplay(lvBoardGame);
        //cout << "Le string : " << endl << Node::printPositionName(lsActual->getPositionName()) << endl;
        // lnEncounteredIterator = lvEncounteredPositions.insert(lnEncounteredIterator, lsActual->getPositionName());
        // string h = lsActual->getPositionName();
        //lvEncounteredPositions.push_back(&h);
        
        // gameDisplay(lvBoardGame);
        lnPositionStatus = whatGameStatus(lvBoardGame,lnCurrentPlayer);

        lnCurrentPlayer = (lnCurrentPlayer) % 2 + 1;
        
        lnMoveCounter++;

        //cout << lvEncounteredPositions.size() << endl;
    }
    //for (int i = 0; i < lvEncounteredPositions.size(); i++)
    //    cout << (new Node)->printPositionName(lvEncounteredPositions[i]) << endl;

    //graph->calculateWeights(lvEncounteredPositions, false);
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

int main(int argc, char **argv) // nb_parties, mode
{
    if (argc != 3)
    {
        printf("Erreur nombre d'arguments invalide.\nUtilisation : ./<executable> <nb_parties> <mode>\n");
        exit(1);
    }
    else 
    {
        int lnNbReps = atoi(argv[1]);
        char *mode = argv[2];
        GraphAI graph;
        for (int i = 0 ; i < lnNbReps; i++)
        {
            doGame(&graph, mode);
        }

        cout << endl << "Graph1 export" << endl;
        graph.exportToFile();
        graph.deleteNodes();
        return 0;
    }
}
