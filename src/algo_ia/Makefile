ifeq ($(origin FOO), undefined)
CC = g++
endif
ifeq ($(origin FOO), default)
CC = g++
endif
FLAGS=-Wall -g -O0 -std=c++17 -D_GLIBCXX_DEBUG


all : clean PLAY learning

PLAY : src/boardGame.cpp src/graphAI.cpp src/node.cpp src/game.cpp
		$(CC) $(FLAGS) -o $@ $^

learning : src/boardGame.cpp src/graphAI.cpp src/node.cpp src/learning.cpp
		$(CC) $(FLAGS) -o $@ $^

test : clean src/boardGame.cpp src/graphAI.cpp src/node.cpp src/testAI.cpp
		$(CC) $(FLAGS) -o $@ $^

clean : 
		rm -f src/*~ *~ test learning PLAY 
