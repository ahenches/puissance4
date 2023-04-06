#include "boardGame.hpp"
#include <stdio.h>
#include <iostream>


using namespace std;

int main(){

    int gvBoardGame[5][5] = {{0}};
    int gnCurrentPlayer, gnSelectedColomn, lnRowPlayed;
    bool gbIsGameFinished, lbIsPlayed;

    //gameDisplay(gvBoardGame);
    gbIsGameFinished = false;
    gnCurrentPlayer = cnIA;
    lbIsPlayed = false;

    //gvBoardGame[0][1] = 1;
    //gvBoardGame[0][3] = 2;
    //gvBoardGame[1][1] = 1;
    //gvBoardGame[0][2] = 2;

    gameDisplay(gvBoardGame);
   // cout << calculateBestMove(gvBoardGame) << endl;
    while(!gbIsGameFinished)
    {
        if(gnCurrentPlayer == cnIA)
        {
            cout << "C est le tour de l'IA " << endl;
            while (!lbIsPlayed)
            {
                gnSelectedColomn = calculateBestMove(gvBoardGame);
                tie(lbIsPlayed,lnRowPlayed) = play(gvBoardGame, gnSelectedColomn, gnCurrentPlayer); 

            }
            lbIsPlayed = false;
            gameDisplay(gvBoardGame);
            gbIsGameFinished = isGameFinished(gvBoardGame, gnCurrentPlayer);

            gnCurrentPlayer = cnPLAYER;
        }
        else 
        {
            cout << "C est votre tour "  << endl;
            
            while (!lbIsPlayed)
            {
                cout << "Sur quelle colonne voulez vous jouez (0 à 4)"<< endl;
                cin >> gnSelectedColomn;
                gnSelectedColomn = gnSelectedColomn % 5;
                tie(lbIsPlayed,lnRowPlayed) = play(gvBoardGame,gnSelectedColomn,gnCurrentPlayer);
                if (!lbIsPlayed)
                    cout << "Colonne Pleine !" << endl;
            }
            lbIsPlayed = false;
            gameDisplay(gvBoardGame);
            gbIsGameFinished = isGameFinished(gvBoardGame,gnCurrentPlayer);

            gnCurrentPlayer = cnIA;
        }
    }

    if(gnCurrentPlayer == cnIA)
    {
        cout << "Vous avez gagné " << endl;
    }
    else 
    {
        cout << "Le gagnant est l'IA  "  << endl;
    }

    return 0;
}