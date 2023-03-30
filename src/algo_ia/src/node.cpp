#include "node.hpp"

using namespace std;
/*/////////////////////////////////////////////////////////////////////////////
Fonction Node()

Auteur : Maud Lestienne (IATIC-4), Arnaud HENCHES (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
  Constructeur vide

/////////////////////////////////////////////////////////////////////////////*/
Node::Node()
{
    msWeight = {0, 0, 0};
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction Node()

Auteur : Maud Lestienne (IATIC-4), Arnaud HENCHES (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
  Constructeur avec la chaine de caractère de la position en entrée

Entrées :
  pwPositionName : la chaîne de caractère qui représente la position

/////////////////////////////////////////////////////////////////////////////*/
Node::Node(string pwPositionName)
{
    msWeight = {0, 0, 0};

    mwPositionName = pwPositionName;
    for (int i = 0; i < 5; i++)
    {
        mvChildren[i] = "";
    }
}

void Node::addChild(int pnColumn, std::string mvChildrenName)
{
    mvChildren[pnColumn] = mvChildrenName;
}

// CALCULS

/*/////////////////////////////////////////////////////////////////////////////
Fonction reversePosition()

Auteur : Arnaud HENCHES (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
  Renvoie la chaîne de caractère de la position avec une symmètrie verticale

Sorties : 
  lwReturnString : La position résultante

/////////////////////////////////////////////////////////////////////////////*/
string Node::reversePosition()
{
    string lwReturnString = mwPositionName;
    int liLeftCursor, liRightCursor;
    for (int liIndexRow = 0 ; liIndexRow < SIZE_SIDE ; liIndexRow ++)
    {
        liLeftCursor = liIndexRow * SQUARE_REPR_SIZE_IN_STRING * SIZE_SIDE;
        liRightCursor = (liIndexRow + 1) * SQUARE_REPR_SIZE_IN_STRING * SIZE_SIDE - 1;
        lwReturnString[liLeftCursor] = mwPositionName[liRightCursor];
        lwReturnString[liRightCursor] = mwPositionName[liLeftCursor];
        

        liLeftCursor += SQUARE_REPR_SIZE_IN_STRING;
        liRightCursor -= SQUARE_REPR_SIZE_IN_STRING;
        lwReturnString[liLeftCursor] = mwPositionName[liRightCursor];
        lwReturnString[liRightCursor] = mwPositionName[liLeftCursor];
    }
    return lwReturnString;
}
/*/////////////////////////////////////////////////////////////////////////////
Fonction calculateNewPositionValue()

Auteur : Maud Lestienne (IATIC-4), Arnaud HENCHES (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
  Calcul la chaîne de caractère de la nouvelle position après le coup

Entrées :
  pnColumnChosen : La colonne choisie
  pnCompteur : le compteur de coup joués depuis le début

Sorties : 
  lwReturnString : La position résultante

/////////////////////////////////////////////////////////////////////////////*/
string Node::calculateNewPositionValue(int pnColumnChosen, int pnCompteur) // player  
{
    string lwReturnString = mwPositionName;
    int lnPlayer = (pnCompteur % 2) + 1; 
    int lnIndexLineCurrent = 0, lnIndexCharCurrent;
    do
    {
        lnIndexCharCurrent = lnIndexLineCurrent * SIZE_SIDE + pnColumnChosen;
        lnIndexLineCurrent++;
    }
    while (lnIndexLineCurrent < 5 && lwReturnString[lnIndexCharCurrent] != 48);
    
    lwReturnString[lnIndexCharCurrent] = to_string(lnPlayer)[0];
    return lwReturnString;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction printPositionName()

Auteur : Maud Lestienne (IATIC-4), Arnaud HENCHES (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
  Renvoie un bel affichage de la position

Entrées :
  pwPositionName : La chaîne de caractère qui représente la position

Sorties : 
  lwReturnString : Le bel affichage sous forme de chaîne de caractère

/////////////////////////////////////////////////////////////////////////////*/
string Node::printPositionName(std::string pwPositionName)
{
    string lwReturnString = "", lwBufferLineString = "";
    const int lnCharInLine = SIZE_SIDE * SQUARE_REPR_SIZE_IN_STRING;
    int lnFirstStringOfRow;
    for (int liIndexRow = 4 ; liIndexRow >= 0 ; liIndexRow --)
    {
        lnFirstStringOfRow = liIndexRow * SIZE_SIDE * SQUARE_REPR_SIZE_IN_STRING;
        lwBufferLineString = pwPositionName.substr(lnFirstStringOfRow, lnCharInLine); 
        lwReturnString.append(std::move(lwBufferLineString));
        lwReturnString.append(CHAR_SEPARATOR_LINE);
    }
    return lwReturnString;
}
/*/////////////////////////////////////////////////////////////////////////////
Fonction getPositionName()

Auteur : Généré automatiquement
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

/////////////////////////////////////////////////////////////////////////////*/
string Node::getPositionName()
{
    return mwPositionName;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction getWeight()

Auteur : Généré automatiquement
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

/////////////////////////////////////////////////////////////////////////////*/
weight_t Node::getWeight()
{
    return msWeight;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction setWeight(int, int, float)

Auteur : Généré automatiquement
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

/////////////////////////////////////////////////////////////////////////////*/
void Node::setWeight(int pnGamePlayed, int pnGameWon, float pnVictoryRate)
{
    weight_t lsNewWeight = {pnGamePlayed, pnGameWon, pnVictoryRate};
    msWeight = lsNewWeight;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction getChildren()

Auteur : Généré automatiquement
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

/////////////////////////////////////////////////////////////////////////////*/
string * Node::getChildren()
{
    return mvChildren;
}

