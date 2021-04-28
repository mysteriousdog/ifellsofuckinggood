

# all:Actor.o Command.o Game.o ReadKey.o test.o
# 	clang++ -std=c++11 test Command.o Game.o ReadKey.o Actor.o test.o

CC=clang++

test:Actor.o Command.o Game.o ReadKey.o test.o
	$(CC) -std=c++11 -o $@ $^
Command.o:Command.cpp
	$(CC) -std=c++11 -c Command.cpp
Actor.o:Actor.cpp
	$(CC) -std=c++11 -c Actor.cpp
Game.o:Game.cpp
	$(CC) -std=c++11 -c Game.cpp
ReadKey.o:ReadKey.cpp
	$(CC) -std=c++11 -c ReadKey.cpp
test.o:test.cpp
	$(CC) -std=c++11 -c test.cpp
clean:
	rm Actor.o Command.o Game.o ReadKey.o test.o
