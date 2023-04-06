ifeq ($(origin FOO), undefined)
CC = g++
endif
ifeq ($(origin FOO), default)
CC = g++
endif
FLAGS=-Wall -g -std=c++17


all : clean PLAY learning

PLAY : boardGame.cpp graphAI.cpp node.cpp game.cpp
		$(CC) $(FLAGS) -o $@ $^

learning : boardGame.cpp graphAI.cpp node.cpp learning.cpp
		$(CC) $(FLAGS) -o $@ $^

test : clean boardGame.cpp graphAI.cpp node.cpp testAI.cpp
		$(CC) $(FLAGS) -o $@ $^

clean : 
		rm -f *~ *~ test learning PLAY 
