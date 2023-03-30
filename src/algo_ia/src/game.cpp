#include "graphAI.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

int main() {

    GraphAI graph;
    graph.importFromFile();
    map<std::string, Node *>& lsGraphMap = graph.getGraphMap();
    vector<string> lvEncounteredPositions;
    lvEncounteredPositions.reserve(cnSIZE_OF_BOARD * cnSIZE_OF_BOARD);
    vector<string>::iterator lnEncounteredIterator;
    lnEncounteredIterator = lvEncounteredPositions.begin();

    string lsActual = graph.getRoot()->getPositionName();
    
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

        if(lnCurrentPlayer == cnIA)
        {
            cout << "C est le tour de l'IA " << endl;
cout << "\t\t\t1:" << lsActual << endl;
            while (!lbIsPlayed)
            {
cout << "\t\t\t2:" << lsActual << endl;
                // Fonction a faire
cout << "\t\t\t3:" << lsActual << endl;
                lnSelectedColomn = graph.playAI(lvBoardGame, lsGraphMap[lsActual]);
cout << "\t\t\t4:" << lsActual << endl;
                tie(lbIsPlayed,lnRowPlayed) = play(lvBoardGame, lnSelectedColomn, lnCurrentPlayer); 
cout << "\t\t\t5:" << lsActual << endl;
            }
        }
        else 
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
        string lwNextNodeRepr = lsGraphMap[lsActual]->calculateNewPositionValue(lnSelectedColomn, lnMoveCounter);
        graph.appendChildToParent(lsActual, lnSelectedColomn, lwNextNodeRepr);
        lsActual = move(lwNextNodeRepr);
 
        lnEncounteredIterator = lvEncounteredPositions.insert(lnEncounteredIterator, lsActual);
        
        gameDisplay(lvBoardGame);
        lnPositionStatus = whatGameStatus(lvBoardGame,lnCurrentPlayer);

        lnCurrentPlayer = (lnCurrentPlayer) % 2 + 1;
        lnMoveCounter++;

        cout << lvEncounteredPositions.size() << endl;
    }
    for (int i = 0; i < (int)lvEncounteredPositions.size(); i++)
        cout << (new Node)->printPositionName(lvEncounteredPositions[i]) << endl;

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

    graph.exportToFile();
    graph.deleteNodes();
    return 0;
}
