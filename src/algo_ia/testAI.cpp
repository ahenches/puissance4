#include "graphAI.hpp"
#include <stdio.h>
#include <iostream>

using namespace std;

int main(){

    Node node("1202120011000000000000000");
    Node racine;
    Node noeud1("0100000000000000000000000");
    Node noeud3("0010000000000000000000000");
    Node noeud2("0120000000000000000000000");

/*
    GraphAI graphe;
    graphe.appendChildToParent(graphe.getRoot(),1,noeud1.getPositionName());
    graphe.appendChildToParent(graphe.getRoot(),2,noeud3.getPositionName());
    //racine = graphe.getRoot();
    //racine.addChild(1,noeud1.getPositionName());

    graphe.addNodetoMap(&noeud1);
    graphe.getGraphMap()[noeud1.getPositionName()].addChild(2,noeud2.getPositionName());
    //noeud1.addChild(2,noeud2.getPositionName());
    graphe.addNodetoMap(&noeud2);

    cout << graphe.getRoot().getChildren()[1] << endl;

    graphe.exportToFile();
    */
/*
    GraphAI graphe2;

    graphe2.importFromFile();
        cout << graphe2.getGraphMap()["0100000000000000000000000"].getChildren()[2] << endl;

    graphe2.exportToFile();


    cout << "///////////////////////////////////////////////////////" << endl;

    cout << node.getWeights()[3] << endl;

    cout << "STR :: \n" << node.printPositionName(node.getPositionName()) << endl;

    cout << "REVERSE_STR :: \n" << node.printPositionName(node.reversePosition()) << endl;

    cout << "STRNEW :: \n" << Node::printPositionName(node.calculateNewPositionValue(3, 7)) << endl;
*/
    return 0;
}