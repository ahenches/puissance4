#ifndef GRAPHAI
#define GRAPHAI
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include "node.hpp"
#include "boardGame.hpp"

#define FILE_NAME "./src/files/graphAI.txt"
/**
 * @brief classe representant le graphe de l'IA
 * 
 */
class GraphAI{
    private:
    /**
     * @brief Map contenant le graphe de l'IA
     * 
     */
    std::map<std::string, Node *> msGraphMap;
    
    /**
     * @brief Le noeud racine du graphe
     * 
     */
    Node *msRoot = new Node("0000000000000000000000000");
    // Node msActual;

    public:
    GraphAI();
    void exportToFile();
    void importFromFile();
    std::vector<std::string> cutString(std::string pwLine, char pwDelimiter);
    void appendChildToParent(std::string psParent, int pnColunm, std::string pwPositionValue); // Si le fils existe déja l'ajouter au parent Sinon on le créer et on le rajoute au parent
    void calculateWeights(std::vector<std::string> pvEncounteredNodes, bool pbStaleFinish);
    int playAI(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD],  Node *psActual );
    int evaluateKnownPosition();

    std::map<std::string, Node *>& getGraphMap();
    Node *getRoot();
    void addNodetoMap(Node *psNode);

    void deleteNodes();
};
#endif
