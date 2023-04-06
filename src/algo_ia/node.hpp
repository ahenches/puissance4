#ifndef NODE
#define NODE
#include <string>
#include <iostream>
#include <vector>
/**
 * @brief  Le nombre de caractere qui represente une case dans une chaine de carractere
 * 
 */
#define SQUARE_REPR_SIZE_IN_STRING 1
/**
 * @brief Le nombre de case sur une ligne dans le plateau
 * 
 */
#define SIZE_SIDE 5

//#define CHAR_SEPARATOR_COL " "
/**
 * @brief Separateur pour l affichage des lignes du plateau
 * 
 */
#define CHAR_SEPARATOR_LINE "\n"

typedef struct weight_s {
	int mnGamePlayed;
	int mnGameWon;
	float mnVictoryRate;
} weight_t;


/**
 * @brief Classe reprensentant un noeud du graphe
 * 
 */
class Node 
{
    private:
    /**
     * @brief Tableau contenant les fils du noeud
     * 
     */
    std::string mvChildren[5];
    /**
     * @brief Valeurs indiquant le poids de l'arete pour atteindre ce noeud
     * (le poids est un calcul de reussite en suivant le chemin de ce fils)
     * 
     */
    weight_t msWeight;
    
    /**
     * @brief Chaine de caractere representant le plateau de jeu et des pions joués
     * Il represente une "position" de jeu dans une partie
     * 
     */
    std::string mwPositionName;

    public:
    Node();
    Node(std::string pwPositionName);
    void addChild(int pnColumn, std::string mvChildrenName); // ajoute le fils à la position place dans mvChildren 
    int ** stringToArray(); // attention au malloc

    static std::string arrayToString();
    
    std::string reversePosition();
    std::string calculateNewPositionValue(int pnColumnChosen, int pnCompteurDeCoup);

    static std::string printPositionName(std::string pwPositionName);
    
    std::string getPositionName();
    // void setPositionName(std::string pwNewPositionName);
    weight_t getWeight();
    void setWeight(int pnGamePlayed, int pnGameWon, float pnVictoryRate);
    std::string *getChildren();
};
#endif
