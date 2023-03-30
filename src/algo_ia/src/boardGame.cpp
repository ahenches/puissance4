#include "boardGame.hpp"


using namespace std;
/*/////////////////////////////////////////////////////////////////////////////
Fonction gameDisplay()

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but d'afficher le plateau dans un terminal
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu

/////////////////////////////////////////////////////////////////////////////*/
void gameDisplay(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD])
{
    int liColomn, liRow ;

    for (liRow = cnSIZE_OF_BOARD - 1; liRow >=0 ; liRow --)
    {
        cout << "\t| ";
        for(liColomn = 0; liColomn <cnSIZE_OF_BOARD ; liColomn ++)
        {
            cout << pvBoardGame[liRow][liColomn] << " | ";
        }
        cout << endl;
    }
    cout << "----------------------------------------" << endl;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction tuple <bool, int> play(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnColomnToPlay, int pnPlayer)

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but de placer un pion dans le plateau de jeu
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu
  pnColomnToPlay : la colonne dans laquelle on souhaite mettre le jeton 
  pnPlayer : le numero  du joueur qui veut placer le jeton

Sortie :
    tuple <bool, int> : le booleen permet de savoir si le pion a ete placer, 
                        l'entier represente la ligne dans laquelle se situe le jeton s'il est joué 

/////////////////////////////////////////////////////////////////////////////*/
tuple <bool, int> play(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnColomnToPlay, int pnPlayer)
{
    bool lbPlayed, lbIsLoopFinished;
    int lnBoxBoard, liIndexRow ;

    lbPlayed = false;
    lbIsLoopFinished = false;
    liIndexRow = 0;
      
    while(!lbIsLoopFinished  && liIndexRow < cnSIZE_OF_BOARD)
    {
        lnBoxBoard = pvBoardGame[liIndexRow][pnColomnToPlay];
        if(lnBoxBoard == 0)
        {
            pvBoardGame[liIndexRow][pnColomnToPlay] = pnPlayer;
            lbPlayed = true;
            lbIsLoopFinished = true;
        }
        else 
        {
            liIndexRow++;
        }
    }
    cout << "SI liIndexRow = 5 lbPlayed  = false \t\t liIndexRow :" << liIndexRow << "et lbPlayed :" << lbPlayed << endl;

    return make_tuple(lbPlayed, liIndexRow);
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction int whichColonmHasBeenPlayed(int pvOldBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pwNewBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD])

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction compare le nouveau plateau et l ancien pour connaitre la colonne jouée par le joueur 
 
Entrées :
    pvOldBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant l ancien plateau de jeu
    pvNewBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le nouveau plateau de jeu


Sortie :
    int : la colonne jouée par le joueur

/////////////////////////////////////////////////////////////////////////////*/
int whichColumnHasBeenPlayed(int pvOldBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pvNewBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD])
{
    for(int l = 0; l < cnSIZE_OF_BOARD; l++)
    {
        for(int c = 0; c < cnSIZE_OF_BOARD; c++)
        {
            if(pvOldBoardGame[l][c] == 0 && pvNewBoardGame[l][c] != 0)
            {
                return c;
            }
        }
    }
}


/*/////////////////////////////////////////////////////////////////////////////
Fonction gameStatus whatGameStatus(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but de connaître le statut actuel du jeu
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu
  pnPlayer : le numero du joueur

Sortie :
    gnGameWon : si la partie a été gagnée 
    gnStaleMate : si la partie est nulle 
    gnGameNotFinished : si la partie n'est pas finie

/////////////////////////////////////////////////////////////////////////////*/

gameStatus whatGameStatus(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)
{
    gameStatus lnStatus;

    if (isHorizontalAlignment(pvBoardGame,pnPlayer))
    {
        lnStatus = gnGameWon;
        cout << "partie gagnée" << endl;
    }
    else if(isVerticalAlignment(pvBoardGame,pnPlayer))
    {
        lnStatus = gnGameWon;
        cout << "partie gagnée" << endl;
    }
    else if (isDiagonalAlignmentTLBR(pvBoardGame,pnPlayer))
    {
        lnStatus = gnGameWon;
        cout << "partie gagnée" << endl;
    }
    else if (isDiagonalAlignmentTRBL(pvBoardGame,pnPlayer))
    {
        lnStatus = gnGameWon;
        cout << "partie gagnée" << endl;
    }
    else if (isBoardGameFull(pvBoardGame))
    {
        lnStatus = gnStaleMate;
        cout << "match nul" << endl;
    }
    else 
    {
        lnStatus = gnGameNotFinished;
    }
    return lnStatus;
}


/*/////////////////////////////////////////////////////////////////////////////
Fonction bool isVerticalAlignment(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but si il y a un alignement vertical de pions de la part du joueur 
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu
  pnPlayer : le numero  du joueur 

Sortie :
    bool : si le joueur a aligner 4 pions alors vrai 
            sinon faux

/////////////////////////////////////////////////////////////////////////////*/
bool isVerticalAlignment(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)
{
    bool lbIsAlignment;
    int liIndexColomn;

    lbIsAlignment = false;

    for (liIndexColomn =0 ; liIndexColomn < cnSIZE_OF_BOARD; liIndexColomn++)
    {
        if(pvBoardGame[1][liIndexColomn]== pnPlayer)
        {
            if(pvBoardGame[2][liIndexColomn]== pnPlayer && pvBoardGame[3][liIndexColomn]== pnPlayer) // 3 pions alignés
            {
                if(pvBoardGame[0][liIndexColomn]== pnPlayer || pvBoardGame[4][liIndexColomn]== pnPlayer) // 4 pions alignés
                {
                    lbIsAlignment = true;
                }
            }
        }
    }
    return lbIsAlignment;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction bool isHorizontalAlignment(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but si il y a un alignement horizontal de pions de la part du joueur 
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu
  pnPlayer : le numero  du joueur 

Sortie :
    bool : si le joueur a aligner 4 pions alors vrai 
            sinon faux

/////////////////////////////////////////////////////////////////////////////*/
bool isHorizontalAlignment(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)
{
    bool lbIsAlignment;
    int liIndexRow;

    lbIsAlignment = false;

    for (liIndexRow =0 ; liIndexRow < cnSIZE_OF_BOARD; liIndexRow++)
    {
        if(pvBoardGame[liIndexRow][1]== pnPlayer)
        {
            if(pvBoardGame[liIndexRow][2]== pnPlayer && pvBoardGame[liIndexRow][3]== pnPlayer) // 3 pions alignés
            {
                if(pvBoardGame[liIndexRow][0]== pnPlayer || pvBoardGame[liIndexRow][4]== pnPlayer) // 4 pions alignés
                {
                    lbIsAlignment = true;
                }
            }
        }
    }
    return lbIsAlignment;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction bool isDiagonalAlignmentTRBL(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but si il y a un alignement diagonal (la diagonale a son haut a droite et son bas a gauche ) de pions de la part du joueur 
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu
  pnPlayer : le numero  du joueur 

Sortie :
    bool : si le joueur a aligner 4 pions alors vrai 
            sinon faux

/////////////////////////////////////////////////////////////////////////////*/
bool isDiagonalAlignmentTRBL(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)//Diagonale le Bas à Gauche et le Haut à Droite
{
    bool lbIsAlignment;

    lbIsAlignment = false;

    if (pvBoardGame[2][2]== pnPlayer)
    {
        if(pvBoardGame[1][1]== pnPlayer && pvBoardGame[3][3]== pnPlayer) // 3 pions alignés
        {
            if(pvBoardGame[0][0]== pnPlayer || pvBoardGame[4][4]== pnPlayer) // 4 pions alignés
            {
                lbIsAlignment = true;
            }
        }
    }
    if (pvBoardGame[1][0]== pnPlayer && pvBoardGame[2][1]== pnPlayer && pvBoardGame[3][2]== pnPlayer && pvBoardGame[4][3]== pnPlayer )
    {
        lbIsAlignment = true;
    }
    if (pvBoardGame[0][1]== pnPlayer && pvBoardGame[1][2]== pnPlayer && pvBoardGame[2][3]== pnPlayer && pvBoardGame[3][4]== pnPlayer )
    {
        lbIsAlignment = true;
    }
    return lbIsAlignment;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction bool isDiagonalAlignmentTLBR(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but si il y a un alignement diagonal (la diagonale a son haut a gauche et son bas a droite ) de pions de la part du joueur 
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu
  pnPlayer : le numero  du joueur 

Sortie :
    bool : si le joueur a aligner 4 pions alors vrai 
            sinon faux

/////////////////////////////////////////////////////////////////////////////*/
bool isDiagonalAlignmentTLBR(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayer)//Diagonale le Bas à Droite et le Haut à Gauche
{
    bool lbIsAlignment;

    lbIsAlignment = false;

    if (pvBoardGame[2][2]== pnPlayer)
    {
        if(pvBoardGame[3][1]== pnPlayer && pvBoardGame[1][3]== pnPlayer) // 3 pions alignés
        {
            if(pvBoardGame[0][4]== pnPlayer || pvBoardGame[4][0]== pnPlayer) // 4 pions alignés
            {
                lbIsAlignment = true;
            }
        }
    }
    if (pvBoardGame[3][0]== pnPlayer && pvBoardGame[2][1]== pnPlayer && pvBoardGame[1][2]== pnPlayer && pvBoardGame[0][3]== pnPlayer )
    {
        lbIsAlignment = true;
    }
    if (pvBoardGame[4][1]== pnPlayer && pvBoardGame[3][2]== pnPlayer && pvBoardGame[2][3]== pnPlayer && pvBoardGame[1][4]== pnPlayer )
    {
        lbIsAlignment = true;
    }
    return lbIsAlignment;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction bool isBoardGameFull(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD])

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but si le plateau est completement rempli 
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu

Sortie :
    bool : si le plateau est complet alors vrai 
            sinon faux

/////////////////////////////////////////////////////////////////////////////*/
bool isBoardGameFull(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD])
{
    bool lbIsBoardGameFull;
    int liColomnIndex;

    lbIsBoardGameFull = true;
    for (liColomnIndex = 0 ; liColomnIndex < cnSIZE_OF_BOARD ; liColomnIndex++)
    {
        if(pvBoardGame[cnSIZE_OF_BOARD-1][liColomnIndex] == 0)
        {
            lbIsBoardGameFull = false; // si on a un zero dans la derniere ligne le plateau n est pas plein
        }
        else {}// on ne s occupe pas des autres valeur
    }

    return lbIsBoardGameFull;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction int moveAtRandom(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD])

Auteur : Arnaud Henches (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction donne un coup possible dans le plateau au hazard pour le joueur ordinateur 
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu

Sortie :
    int : la colonne a jouer pour faire le mouvement calculer

/////////////////////////////////////////////////////////////////////////////*/
int moveAtRandom(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD])
{
    int lnRowOfPiece;
    bool lbIsPiecePlayed;
    vector<int> lvPossibleMoves;
    for(int liIndexColumn = 0; liIndexColumn < 5; liIndexColumn++)
    {
        std::tie(lbIsPiecePlayed, lnRowOfPiece) = play(pvBoardGame, liIndexColumn, cnIA); // joue temporairement le coup à tester
        if (lbIsPiecePlayed)
        {
            lvPossibleMoves.push_back(liIndexColumn);
        }
        else 
        {

        }
        pvBoardGame[lnRowOfPiece][liIndexColumn] = 0; // reset le coup ayant été testé
    }
    return lvPossibleMoves[rand() % lvPossibleMoves.size()];

}
/*/////////////////////////////////////////////////////////////////////////////
Fonction int calculateBestMove(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD])

Auteur : Arnaud Henches (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but de calculer le meilleur coup a jouer dans le plateau pour le joueur IA 
 
Entrées :
  pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu

Sortie :
    int : la colonne a jouer pour faire le mouvement calculer

/////////////////////////////////////////////////////////////////////////////*/
int calculateBestMove(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD])
{
    int lnColumnOfBestMoveFor1 = -1;
    int lnBestMoveValueFor1 = -2;
    int lnColumnOfBestMoveFor2 = -1;
    int lnBestMoveValueFor2 = -2;
    

    int lnRowOfPiece;
    bool lbIsPiecePlayed;
    int lnValuePosition;
    printf("HEY\n");
    int liIndexColumn = 0;
    for(; liIndexColumn < 5; liIndexColumn++)
    {
        std::tie(lbIsPiecePlayed, lnRowOfPiece) = play(pvBoardGame, liIndexColumn, cnIA); // joue temporairement le coup à tester
        if (lbIsPiecePlayed)
        {
            lnValuePosition = calculatePositionValue(pvBoardGame, cnIA, lnRowOfPiece, liIndexColumn);
            //cout << liIndexColumn << " : " << lnValuePosition << endl;
            if (lnBestMoveValueFor1 < lnValuePosition) // test la valeur de la position 
            {
                lnBestMoveValueFor1 = lnValuePosition; // la nouvelle meilleure valeur
                lnColumnOfBestMoveFor1 = liIndexColumn; // l'indice pour avoir la nouvelle meilleure valeur
            }
            else 
            {
                // la meilleure valeur ne change pas
            }
            pvBoardGame[lnRowOfPiece][liIndexColumn] = 0; // reset le coup ayant été testé
        }
        else 
        {
            // le coup n'est pas possible donc pas besoin de replacer la piece 
        }
    }
    //cout << cnIA << " : " << lnColumnOfBestMoveFor1 << " : " << lnBestMoveValueFor1 << endl;
    liIndexColumn = 0;
    for(; liIndexColumn < 5; liIndexColumn++)
    {
        std::tie(lbIsPiecePlayed, lnRowOfPiece) = play(pvBoardGame, liIndexColumn, cnPLAYER); // joue temporairement le coup à tester
        if (lbIsPiecePlayed)
        {
            lnValuePosition = calculatePositionValue(pvBoardGame, cnPLAYER, lnRowOfPiece, liIndexColumn);
        // cout << liIndexColumn << " : " << lnValuePosition << endl;
            if (lnBestMoveValueFor2 < lnValuePosition) // test la valeur de la position 
            {
                lnBestMoveValueFor2 = lnValuePosition; // la nouvelle meilleure valeur
                lnColumnOfBestMoveFor2 = liIndexColumn; // l'indice pour avoir la nouvelle meilleure valeur
            }
            else 
            {
                // la meilleure valeur ne change pas
            }
            
            pvBoardGame[lnRowOfPiece][liIndexColumn] = 0; // reset le coup ayant été testé
        }
        else 
        {
            // le coup n'est pas possible donc pas besoin de replacer la piece 
        }
    }
   // cout << cnPLAYER << " : " << lnColumnOfBestMoveFor2 << " : " << lnBestMoveValueFor2 << endl;

    if (lnBestMoveValueFor1 >= lnBestMoveValueFor2)
    {
        return lnColumnOfBestMoveFor1;
    }
    else 
    {
        return lnColumnOfBestMoveFor2;
    }
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction int calculatePositionValue(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayeur, int pnRowOfMove, int pnColumnOfMove)

Auteur : Arnaud Henches (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but de calculer la valeur de la position potentielle du jeton
On calcule la valeur d'une position en fonction des pions placé autour de la position est des places qu'il reste autour 
 
Entrées :
    pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD] : tableau représentant le plateau de jeu
    pnPlayeur : le numero du joueur courant
    pnRowOfMove : la ligne du plateau ou le coup est joue
    pnColumnOfMove : la colonne du plateau ou le coup est joue

Sortie :
    int : la valeur de la position donnée en parametre

/////////////////////////////////////////////////////////////////////////////*/
int calculatePositionValue(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD], int pnPlayeur, int pnRowOfMove, int pnColumnOfMove)
{
    int lnPositionValue = 0;
    int lnDirectionValue;

    int lnOpposingPlayer;
    int lnActualRow;
    int lnActualColumn;
    int lvMemoEmpty[3][3] = {{0}}; 
    int lvMemoPlayerTerritory[3][3] = {{0}};
    int lnRangeFromPiece;
    lvMemoEmpty[1][1] = -1; 
    lvMemoPlayerTerritory[1][1] = -1; 

    if (pnPlayeur == cnIA)
    {
        lnOpposingPlayer = cnPLAYER;
    }
    else 
    {
        lnOpposingPlayer = cnIA;
    }

    for (int liDeltaRow = -1; liDeltaRow <= 1; liDeltaRow++)
    {
        for (int liDeltaColumn = -1; liDeltaColumn <= 1; liDeltaColumn++)
        {
            if (liDeltaRow == 0 && liDeltaColumn == 0)
            {
                // ne rien faire
            }
            else 
            {
                lnActualRow = pnRowOfMove + liDeltaRow;
                lnActualColumn = pnColumnOfMove + liDeltaColumn;
                lnRangeFromPiece = 1;
                while ((lnActualRow < cnSIZE_OF_BOARD && lnActualRow >= 0) && (lnActualColumn < cnSIZE_OF_BOARD && lnActualColumn >= 0) && (pvBoardGame[lnActualRow][lnActualColumn] != lnOpposingPlayer) && lnRangeFromPiece < 4)
                {
                    if (pvBoardGame[lnActualRow][lnActualColumn] == 0)
                    {
                        lvMemoEmpty[liDeltaRow + cnOffsetIndex1][liDeltaColumn + cnOffsetIndex1] += 1;
                    }
                    else 
                    {
                        if (lvMemoEmpty[liDeltaRow + cnOffsetIndex1][liDeltaColumn + cnOffsetIndex1] != 0)
                        {
                            lvMemoEmpty[liDeltaRow + cnOffsetIndex1][liDeltaColumn + cnOffsetIndex1] += 1;
                        }
                        else 
                        {
                            lvMemoPlayerTerritory[liDeltaRow + cnOffsetIndex1][liDeltaColumn + cnOffsetIndex1] += 1;
                        }
                    }
                    lnActualRow += liDeltaRow;
                    lnActualColumn += liDeltaColumn;
                    lnRangeFromPiece++;
                }
            }
        }
    }
    // Direction (bas_gauche -- haut_droite)
    lnDirectionValue = calculateValueDirection(lvMemoEmpty[0][0], lvMemoEmpty[2][2], lvMemoPlayerTerritory[0][0], lvMemoPlayerTerritory[2][2]);
    if (lnDirectionValue >= 0)
    {
        lnPositionValue += lnDirectionValue;
    }
    else 
    {
        return cnMaxValueWhen4Piece;
    }
    // Direction (gauche -- droite)
    lnDirectionValue = calculateValueDirection(lvMemoEmpty[1][0], lvMemoEmpty[1][2], lvMemoPlayerTerritory[1][0], lvMemoPlayerTerritory[1][2]);
    if (lnDirectionValue >= 0)
    {
        lnPositionValue += lnDirectionValue;
    }
    else 
    {
        return cnMaxValueWhen4Piece;
    }
    // Direction (haut_gauche -- bas_droite)
    lnDirectionValue = calculateValueDirection(lvMemoEmpty[2][0], lvMemoEmpty[0][2], lvMemoPlayerTerritory[2][0], lvMemoPlayerTerritory[0][2]);
    if (lnDirectionValue >= 0)
    {
        lnPositionValue += lnDirectionValue;
    }
    else 
    {
        return cnMaxValueWhen4Piece;
    }
    // Direction (haut -- bas)
    lnDirectionValue = calculateValueDirection(lvMemoEmpty[0][1], lvMemoEmpty[2][1], lvMemoPlayerTerritory[0][1], lvMemoPlayerTerritory[2][1]);
    if (lnDirectionValue >= 0)
    {
        lnPositionValue += lnDirectionValue;
    }
    else 
    {
        return cnMaxValueWhen4Piece;
    }
    
    return lnPositionValue;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction int calculateValueDirection(int pnValueEmpty1, int pnValueEmpty2, int pnValuePT1, int pnValuePT2)

Auteur : Arnaud Henches (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4 
Nom du package : AI

But de la fonction :
Cette fonction a pour but de calculer la valeur des directions autour d'une position
Directions possible : vertical, horizontal et les diagonales
 
Entrées :
    pnValueEmpty1 : nombre de cases vides dans le sens 1 
    pnValueEmpty2 : nombre de cases vides dans le sens 2
    pnValuePT1 : nombre de cases remplies d'un jeton de meme couleur dans le sens 1 
    pnValuePT2 : nombre de cases remplies d'un jeton de meme couleur dans le sens 2

Sortie :
    int : la valeur de la direction

/////////////////////////////////////////////////////////////////////////////*/
int calculateValueDirection(int pnValueEmpty1, int pnValueEmpty2, int pnValuePT1, int pnValuePT2)
{
    int lnDirectionValue;
    if ((pnValueEmpty1 + pnValueEmpty2 + pnValuePT1 + pnValuePT2) >= 3) // si dans la direction il y a de la place pour faire un puissance 4
    {
        lnDirectionValue = 1 + pnValuePT1 + pnValuePT2;
        if (lnDirectionValue >= 4)
            return -1;
        else if (lnDirectionValue == 3)
            return cnValueOf3Piece;
        else if (lnDirectionValue == 2)
            return cnValueOf2Piece;
        else
            return cnValueOf1Piece;
    }
    else // il n'y a pas la place ;(
    {
        return 0;
    }
}
