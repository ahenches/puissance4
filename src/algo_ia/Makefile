ifeq ($(origin CC), undefined)
CC = g++
endif
ifeq ($(origin CC), default)
CC = g++
endif
FLAGS=-Wall -g -std=c++17 -fstack-protector
PRE=DEBUG_OFF


all : clean PLAY learning

PLAY : boardGame.cpp graphAI.cpp node.cpp game.cpp
		$(CC) -D $(PRE) $(FLAGS) -o $@ $^

learning : boardGame.cpp graphAI.cpp node.cpp learning.cpp
		$(CC) -D $(PRE) $(FLAGS) -o $@ $^

test : clean boardGame.cpp graphAI.cpp node.cpp testAI.cpp
		$(CC) -D $(PRE) $(FLAGS) -o $@ $^

clean : 
		rm -f *~ *~ test learning PLAY 
