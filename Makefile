

# all:Actor.o Command.o Game.o ReadKey.o test.o
# 	clang++ -std=c++11 test Command.o Game.o ReadKey.o Actor.o test.o

CPP_FLAGS= -std=c++11

PLAT_MACRO= -D SERVER_COMPARE -D _GLIBCXX_USE_CXX11_ABI=0

LINK= -lrediscluster -lmysqlcppconn -lpthread

CC=g++ -g ${CPP_FLAGS} ${PLAT_MACRO}

objs = Actor.o Command.o Game.o ReadKey.o Server.o Client.o HandleMsg.o redis_pool.o mysql_pool.o SeqAbleObj.o Player.o ComManger.o SysManger.o


server: $(objs)
	$(CC)  -o $@ $^ ${LINK}
SeqAbleObj.o:SeqAbleObj.cpp SeqAbleObj.h
	$(CC) -c SeqAbleObj.cpp
Command.o:Command.cpp Command.h MyEnum.h
	$(CC) -c Command.cpp
Actor.o:Actor.cpp Actor.h MyEnum.h ReadKey.h Command.h
	$(CC) -c Actor.cpp
Game.o:Game.cpp Game.h ReadKey.h MyEnum.h HandleMsg.h SeqToBin.h ConcQueue.h SysManger.h
	$(CC) -c Game.cpp 
ReadKey.o:ReadKey.cpp ReadKey.h Command.h SeqToBin.h IOManger.h
	$(CC) -c ReadKey.cpp
Server.o:Server.cpp Zepoll.h mysql_pool.h redis_pool.h ThreadPool.h HandleMsg.h Client.h SeqToBin.h Player.h
	$(CC) -c Server.cpp
Client.o:Client.cpp Client.h ConcQueue.h SeqToBin.h SeqAbleObj.h Command.h
	$(CC) -c Client.cpp
mysql_pool.o:mysql_pool.cpp mysql_pool.h
	$(CC) -c mysql_pool.cpp
HandleMsg.o:HandleMsg.cpp HandleMsg.h redis_pool.h ThreadPool.h ComManger.h SeqToBin.h SeqAbleObj.h mysql_pool.h Player.h
	$(CC) -c HandleMsg.cpp
redis_pool.o:redis_pool.cpp redis_pool.h ComManger.h
	$(CC) -c redis_pool.cpp
Player.o:Player.cpp Player.h
	$(CC) -c Player.cpp
ComManger.o:ComManger.cpp ComManger.h
	$(CC) -c ComManger.cpp
SysManger.o:SysManger.cpp SysManger.h
	$(CC) -c SysManger.cpp


# .PHONY
clean:
	-rm $(objs) server test
