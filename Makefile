

# all:Actor.o Command.o Game.o ReadKey.o test.o
# 	clang++ -std=c++11 test Command.o Game.o ReadKey.o Actor.o test.o

CC=g++ -D SERVER_COMPARE

# $(CC) -D SERVER=1

server:Actor.o Command.o Game.o ReadKey.o Server.o Client.o HandleMsg.o redis_pool.o 
	$(CC) -std=c++11 -g -o $@ $^ -l rediscluster
Command.o:Command.cpp
	$(CC) -std=c++11 -g -c Command.cpp
Actor.o:Actor.cpp
	$(CC) -std=c++11 -g -c Actor.cpp
Game.o:Game.cpp
	$(CC) -std=c++11 -g -c Game.cpp
ReadKey.o:ReadKey.cpp
	$(CC) -std=c++11 -g -c ReadKey.cpp
Server.o:Server.cpp
	$(CC) -std=c++11 -g -c Server.cpp
Client.o:Client.cpp
	$(CC) -std=c++11 -g -c Client.cpp
HandleMsg.o:HandleMsg.cpp
	$(CC) -std=c++11 -g -c HandleMsg.cpp
redis_pool.o:redis_pool.cpp
	$(CC) -std=c++11 -g -c redis_pool.cpp

clean:
	rm Actor.o Command.o Game.o ReadKey.o Server.o Client.o HandleMsg.o redis_pool.o
