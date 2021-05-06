#ifndef __ZEPOLLER_H__
#define __ZEPOLLER_H__

#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <unordered_map>
#include "ConRedis.h"
#include "HandleMsg.h"
#include "Actor.h"
#include <string.h>

#define MAX_PENDING 1024
#define BUFFER_SIZE 255

class Handler {
public:
	virtual ~Handler() {}
	virtual int handle(epoll_event e) = 0;
};

/**
 * epoll 事件轮询
 */ 
class IOLoop {
public:
	static IOLoop *Instance()
	{
		static IOLoop instance;
		return &instance;
	}
	~IOLoop() 
	{
		for (auto it : handlers_) {
			delete it.second;
		}
	}

	void start()
	{
		const uint64_t MAX_EVENTS = 32;
		epoll_event events[MAX_EVENTS];
		while (true)
		{
			// -1 只没有事件一直阻塞
			int nfds = epoll_wait(epfd_, events, MAX_EVENTS, -1/*Timeout*/);
			for (int i = 0; i < nfds; ++i) {
				int fd = events[i].data.fd;
                std::cout << "find fd" << fd << std::endl;
                std::cout << "find event" << events[i].events << std::endl;
				Handler* handler = handlers_[fd];
				handler->handle(events[i]);
			}
		}
	}

	void addHandler(int fd, Handler* handler, unsigned int events)
	{
		handlers_[fd] = handler;
		epoll_event e;
		e.data.fd = fd;
		e.events = events;

		if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &e) < 0) {
			std::cout << "Failed to insert handler to epoll" << std::endl;
		}
	}

	void modifyHandler(int fd, unsigned int events) 
	{
		struct epoll_event event;
		event.events = events;
		epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &event);
	}

	void removeHandler(int fd) 
	{
		Handler* handler = handlers_[fd];
		handlers_.erase(fd);
		delete handler;
		//将fd从epoll堆删除
		epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL);
	}

private:
	IOLoop()
	{
		epfd_ = epoll_create1(0);  //flag=0 等价于epll_craete
		if (epfd_ < 0) {
			std::cout << "Failed to create epoll" << std::endl;
			exit(1);
		}
	}

private:
	int epfd_;
	std::unordered_map<int, Handler*> handlers_;
};

class EchoHandler : public Handler {
public:
	EchoHandler() {}
	virtual int handle(epoll_event e) override
	{
		int fd = e.data.fd;
		if (e.events & EPOLLHUP) {
			IOLoop::Instance()->removeHandler(fd);
			return -1;
		}

		if (e.events & EPOLLERR) {
			return -1;
		}

		if (e.events & EPOLLOUT)
		{
			if (received > 0)
			{
				std::cout << "Writing: " << buffer << std::endl;
				if (send(fd, buffer, received, 0) != received)
				{
					std::cout << "Error writing to socket" << std::endl;
				}
			}

			IOLoop::Instance()->modifyHandler(fd, EPOLLIN);
		}

		if (e.events & EPOLLIN)
		{
			std::cout << "read" << std::endl;
			received = recv(fd, buffer, BUFFER_SIZE, 0);
			if (received <= 0) {
                IOLoop::Instance()->removeHandler(fd);
				std::cout << "Error reading from socket" << std::endl;
			}
			else if (received > 0) {
				// buffer[received] = 0;

				

				std::cout << "Reading: " << std::endl;
				if (strcmp(buffer, "stop") == 0) {
					std::cout << "stop----" << std::endl;
				}
                std::cout << "Writing: " << std::endl;
				// MsgHandler& msgHandler =  MsgHandler::getInstance();
				// msgHandler.handle((TransObj*)buffer);
				TransObj* obj = (TransObj*)buffer;
				cout<<"obj->msgType "<<obj->msgType<<endl;
				cout<<"obj->id "<<obj->id<<endl;
				cout<<"obj->len "<<obj->len<<endl;
				// Command* cmd = (Command*)obj->msg;
				Command cmd(0);
				memcpy((void*)&cmd, (void*)obj->msg, obj->len);
				// CMD_TYPE_UINT32_ENUM a = cmd->getType();
				cout<<"cmd->type "<<cmd.getType()<<endl;
				// Actor& actor = Actor::getInstance();
				// cmd.doCommand(actor);
                if (send(fd, buffer, received, 0) != received)
				{
					std::cout << "Error writing to socket" << std::endl;
				}
			}

			// if (received > 0) {
			// 	IOLoop::Instance()->modifyHandler(fd, EPOLLOUT);
			// } else {
			// 	std::cout << "disconnect fd=" << fd << std::endl;
			// 	IOLoop::Instance()->removeHandler(fd);
			// }
		}

		return 0;
	}

private:
	int received = 0;
	char buffer[BUFFER_SIZE];
};

class ServerHandler : public Handler {
public:
	ServerHandler(int port)
	{
		int fd;
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));

		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			std::cout << "Failed to create server socket" << std::endl;
			exit(1);
		}

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);

		if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
			std::cout << "Failed to bind server socket" << std::endl;
			exit(1);
		}

		if (listen(fd, MAX_PENDING) < 0)
		{
			std::cout << "Failed to listen on server socket" << std::endl;
			exit(1);
		}
		// setnonblocking(fd);
        std::cout << "server !!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		IOLoop::Instance()->addHandler(fd, this, EPOLLIN);
	}

	virtual int handle(epoll_event e) override
	{
		int fd = e.data.fd;
		struct sockaddr_in client_addr;
		socklen_t ca_len = sizeof(client_addr);

		int client = accept(fd, (struct sockaddr*)&client_addr, &ca_len);

		if (client < 0)
		{
			std::cout << "Error accepting connection" << std::endl;
			return -1;
		}

		std::cout << "accept connected: " << inet_ntoa(client_addr.sin_addr) << std::endl;
		Handler* clientHandler = new EchoHandler();
		// IOLoop::Instance()->addHandler(client, clientHandler, EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR);
        IOLoop::Instance()->addHandler(client, clientHandler, EPOLLIN);
		return 0;
	}

private:
	void setnonblocking(int fd)
	{
		int flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	}
};

#endif /* __ZEPOLLER_H__ */