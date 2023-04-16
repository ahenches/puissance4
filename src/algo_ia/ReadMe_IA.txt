Partie Algorithme / Intelligence Artificielle 

- lancement avec communication 

rosrun puissance4 algo


//////////////////////////////////////////////////
Independant de la machine virtuelle avec ROS


- jeu dans le terminal

cd puissance4/src/algo_ia
make game_in_terminal
./game_in_terminal <mode de jeu>

mode de jeu : 	f -> facile
			m -> moyen
			d -> difficile

- mode apprentissage

cd puissance4/src/algo_ia
make_learning
./learning <nombre de parties> <mode>

mode :	rand -> tout random
		algo -> alternance algorithme et random
		mixte -> tirage au sort a chaque tour entre random et algorithme