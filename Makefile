

# all:Actor.o Command.o Game.o ReadKey.o test.o
# 	clang++ -std=c++11 test Command.o Game.o ReadKey.o Actor.o test.o

CC=clang++

test:Actor.o Command.o Game.o ReadKey.o test.o Client.o
	$(CC) -std=c++11 -g -o $@ $^
Command.o:Command.cpp
	$(CC) -std=c++11 -g -c Command.cpp
Actor.o:Actor.cpp
	$(CC) -std=c++11 -g -c Actor.cpp
Game.o:Game.cpp
	$(CC) -std=c++11 -g -c Game.cpp
ReadKey.o:ReadKey.cpp
	$(CC) -std=c++11 -g -c ReadKey.cpp
test.o:test.cpp
	$(CC) -std=c++11 -g -c test.cpp
Client.o:Client.cpp
	$(CC) -std=c++11 -g -c Client.cpp
clean:
	rm Actor.o Command.o Game.o ReadKey.o test.o Client.o
