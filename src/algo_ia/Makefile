ifeq ($(origin CC), undefined)
CC = g++
endif
ifeq ($(origin CC), default)
CC = g++
endif
FLAGS=-Wall -g -std=c++17 -fstack-protector
PRE=DEBUG_OFF


all : clean PLAY learning game_in_terminal

PLAY : boardGame.cpp graphAI.cpp node.cpp game.cpp
		$(CC) -D $(PRE) $(FLAGS) -o $@ $^

learning : boardGame.cpp graphAI.cpp node.cpp learning.cpp
		$(CC) -D $(PRE) $(FLAGS) -o $@ $^

game_in_terminal : boardGame.cpp graphAI.cpp node.cpp game_in_terminal.cpp
		$(CC) -D $(PRE) $(FLAGS) -o $@ $^

clean : 
		rm -f *~ *~ test learning PLAY 
