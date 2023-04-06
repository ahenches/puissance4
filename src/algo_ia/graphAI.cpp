#include "graphAI.hpp"


using namespace std;

GraphAI::GraphAI(char const *pwFileName)
{
    msGraphMap[msRoot->getPositionName()] = msRoot;
    mwFileName = pwFileName;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction exportToFile()

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
  Charge dans un fichier l'état de l'IA

/////////////////////////////////////////////////////////////////////////////*/
void GraphAI::exportToFile() //ecriture
{
    FILE *
    Node *lsCurrentNode;
    map<string, Node *>::iterator liMapIterator;
    int liNumberSon;
    string *lwSonsName;
    int lnSize;

    fopen(mwFileName, "w");//Ouverture en ecriture du fichier

    ofstream lsFile(mwFileName);  

    if(lsFile)
    {
        //On s'occupe des noeuds du graphe
        for(liMapIterator = msGraphMap.begin(); liMapIterator != msGraphMap.end(); liMapIterator++)
        {
            lsCurrentNode = liMapIterator -> second;
            //Ecriture du nom du noeud coourant et de ses informations concernant son poids
            lsFile << lsCurrentNode->getPositionName() << " ";
            cout << lsCurrentNode->getPositionName() << " ";
            lsFile << lsCurrentNode->getWeight().mnGamePlayed << " "  << lsCurrentNode->getWeight().mnGameWon << " " << lsCurrentNode->getWeight().mnVictoryRate << " ";
            cout << lsCurrentNode->getWeight().mnGamePlayed << " "  << lsCurrentNode->getWeight().mnGameWon << " " << lsCurrentNode->getWeight().mnVictoryRate << " ";
            
            lwSonsName = lsCurrentNode->getChildren();
            lnSize = 5;
            //Pour chaque fils existant ont ecrit son indice dans le tableau de fils et son nom
            for(liNumberSon = 0; liNumberSon < lnSize; liNumberSon++)
            {
                if(lwSonsName[liNumberSon] != "")
                {
                    lsFile << liNumberSon << " " << lwSonsName[liNumberSon] << " ";
                    cout << liNumberSon << " " << lwSonsName[liNumberSon] << " ";
                }
                else // ne rien faire
                {}
            }
            lsFile << endl;
            cout << endl;
        }
    }
    else // Probleme ouverture du fichier
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier en ecriture" << endl;
    }
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction importFromFile()

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
  Récupère l'état de l'IA depuis un fichier

/////////////////////////////////////////////////////////////////////////////*/
void GraphAI::importFromFile()//lecture
{
    if (access(mwFileName, F_OK) != 0) // fichier n'existe pas 
    {
        return ;
    }

    ifstream lsFile(mwFileName);
    int liIndex;
    vector<string> lvLineCuts;
    string *lvSonsName;
    string lwLine;
    string lwNodeName;

    if(lsFile)
    {
        //Traitement de la racine
        getline(lsFile, lwLine);
        lvLineCuts = cutString(lwLine,' ');

        for(liIndex = 4 ; liIndex < (int)lvLineCuts.size(); liIndex+= 2)
        {
            msRoot->getChildren()[stoi(lvLineCuts[liIndex])] = lvLineCuts[liIndex + 1];
            cout << stoi(lvLineCuts[liIndex]) << " " << lvLineCuts[liIndex + 1] << " ";
        }
        cout << msRoot->getPositionName() << " " << endl;
        //Traitement des autres noeuds
        while(getline(lsFile, lwLine)){
            lvLineCuts = cutString(lwLine,' ');
            Node* lsCurrentNode = new Node(lvLineCuts[0]);
            lsCurrentNode->setWeight(stoi(lvLineCuts[1]), stoi(lvLineCuts[2]), stof(lvLineCuts[3]));
            lvSonsName = lsCurrentNode->getChildren();

            for(liIndex = 4; liIndex < (int)lvLineCuts.size(); liIndex+= 2)
            {
                lvSonsName[stoi(lvLineCuts[liIndex])] = lvLineCuts[liIndex + 1];
                cout << stoi(lvLineCuts[liIndex]) << " " << lvLineCuts[liIndex + 1] << " ";
                
            }
            msGraphMap[lvLineCuts[0]] = lsCurrentNode;
            cout << endl;
        }        
    }
    else 
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture" << endl;
    }
}
/*/////////////////////////////////////////////////////////////////////////////
Fonction cutString()

Auteur : Maud Lestienne (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
  Découpe une ligne selon un délimiteur en retirant les retours à la ligne 

Entrées :
  pwLine : La ligne à découpée
  pwDelimiter : Le caractère qui va permettre de découper la chaîne

Sorties : 
  lvLineCuts : le vecteur qui contient chaque bout de string couper

/////////////////////////////////////////////////////////////////////////////*/
vector<string> GraphAI::cutString(string pwLine, char pwDelimiter)
{
    vector<string> lvLineCuts;
    stringstream lwStringStream(pwLine);
    string lwPieceOfLine;
    while (getline(lwStringStream, lwPieceOfLine, pwDelimiter))
    {
        lvLineCuts.push_back(lwPieceOfLine);
    }

    return lvLineCuts;
}

int GraphAI::playAI(int pvBoardGame[cnSIZE_OF_BOARD][cnSIZE_OF_BOARD],  Node *psActual )
{
    int lwChildSelected = -1;
    int lnSelectedColomn, liIndex;

    for(liIndex = 0; liIndex < 5;  liIndex++)
    {
        if(psActual->getChildren()[liIndex] != "")
        {
            if(msGraphMap.find(psActual->getChildren()[liIndex]) != msGraphMap.end())
            {
                if(msGraphMap[psActual->getChildren()[liIndex]]->getWeight().mnVictoryRate >= 50)
                {
                    if(lwChildSelected == -1)
                    {
                        lwChildSelected = liIndex;
                    }
                    else
                    {
                        if(msGraphMap[psActual->getChildren()[liIndex]]->getWeight().mnVictoryRate > msGraphMap[psActual->getChildren()[lwChildSelected]]->getWeight().mnVictoryRate)
                        {
                            lwChildSelected = liIndex;
                        }
                    }
                }
            }
        }
    }

    if (lwChildSelected != -1)
    {
        lnSelectedColomn = lwChildSelected;
    }
    else 
    {
        lnSelectedColomn = calculateBestMove(pvBoardGame);
    }

    return lnSelectedColomn ;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction appendChildToParent()

Auteur : Arnaud HENCHES (IATIC-4)
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

But de la fonction :
    ajoute le fils(string) à un père.  

Entrées :
  psParent : Le père 
  pnColunm : La colonne qui va permettre de passer du père au fils
  pwPositionRepresentation : la valeur de la position du fils 

Sorties : 
  lsNodeChild : la structure Node du fils

/////////////////////////////////////////////////////////////////////////////*/
void GraphAI::appendChildToParent(string psParent, int pnColunm, string pwPositionRepresentation)
{
    msGraphMap[psParent]->addChild(pnColunm, pwPositionRepresentation);
    if(msGraphMap.find(pwPositionRepresentation) == msGraphMap.end()) // Le fils n'existe pas dans le graphe
    {
        Node *n = new Node(pwPositionRepresentation);
        msGraphMap[pwPositionRepresentation] = move(n);
        // msGraphMap.insert(pair<string, Node *>(pwPositionRepresentation, move(n)));
    }
    else // Le fils existe déja
    {
    }
}

void GraphAI::calculateWeights(vector <string> pvEncounteredNodes, bool pbStaleFinish)
{
    weight_t lsWeights;
    int lnTailleEncounteredNode = pvEncounteredNodes.size(), lnIterator = 0;
    for(string lsCurentPositionName : pvEncounteredNodes)
    {
        cout << "Nom position : "<< lsCurentPositionName << endl;
        lsWeights = msGraphMap[lsCurentPositionName]->getWeight();
        lsWeights.mnGamePlayed += 1;
        if(!pbStaleFinish && (lnTailleEncounteredNode - lnIterator) % 2 == 1)
        {
            lsWeights.mnGameWon += 1;
        }
        lsWeights.mnVictoryRate = (lsWeights.mnGameWon / lsWeights.mnGamePlayed) * 100;
        msGraphMap[lsCurentPositionName]->setWeight(lsWeights.mnGamePlayed, lsWeights.mnGameWon, lsWeights.mnVictoryRate);
        lnIterator++;
    }
}

/*************   GET and SET    **************/

/*/////////////////////////////////////////////////////////////////////////////
Fonction getPositionName()

Auteur : Généré automatiquement
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

/////////////////////////////////////////////////////////////////////////////*/
map<std::string, Node *>& GraphAI::getGraphMap()
{
    return msGraphMap;
}

/*/////////////////////////////////////////////////////////////////////////////
Fonction getPositionName()

Auteur : Généré automatiquement
Nom du projet : Robot Niryo - Puissance 4
Nom du package : AI

/////////////////////////////////////////////////////////////////////////////*/
Node *GraphAI::getRoot()
{
    return msRoot;
}

Node *GraphAI::getNode(string pwPositionRepresentation)
{
    if(msGraphMap.find(pwPositionRepresentation) != msGraphMap.end()) // Le fils n'existe pas dans le graphe
    {
        return msGraphMap[pwPositionRepresentation];
    }
    else 
    {
        return NULL;
    }
}

void GraphAI::addNodetoMap(Node *psNode)
{
    msGraphMap[psNode->getPositionName()] = psNode;
}

/*************   Destroyer    **************/
void GraphAI::deleteNodes()
{
    for(auto it = msGraphMap.cbegin(); it != msGraphMap.cend(); ++it)
    {
        free(it->second);
    }
}
