#ifndef __ZEPOLLER_H__
#define __ZEPOLLER_H__
#ifdef SERVER_COMPARE
#include "Log.h"
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
#include "ComManger.h"
#include <string.h>
#include "SeqToBin.h"
#define MAX_PENDING 1024
#define BUFFER_SIZE 512

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
			LOG_ERR("Failed to insert handler to epoll" );
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
		epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL);
	}

private:
	IOLoop()
	{
		epfd_ = epoll_create1(0);  //flag=0 等价于epll_craete
		if (epfd_ < 0) {
			LOG_ERR("Failed to create epoll");
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
				if (send(fd, buffer, received, 0) != received)
				{
					LOG_ERR("Error writing to socket");
				}
			}

			IOLoop::Instance()->modifyHandler(fd, EPOLLIN);
		}

		if (e.events & EPOLLIN)
		{
			received = recv(fd, buffer, BUFFER_SIZE, 0);
			if (received <= 0) {
                IOLoop::Instance()->removeHandler(fd);
				LOG_ERR("Error reading from socket");
			}
			else if (received > 0) {
				// buffer[received] = 0;

				

				if (strcmp(buffer, "stop") == 0) {
					LOG_INFO("epoll stop----");
				}
				MsgHandler& msgHandler =  MsgHandler::getInstance();
				auto obj = make_shared<TransObj>();
				obj->transBuff2Obj(buffer);
				msgHandler.handle(obj, fd);
			}
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
			LOG_ERR("Failed to create server socket" );
			exit(1);
		}

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);

		if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
			LOG_ERR("Failed to bind server socket" );
			exit(1);
		}

		if (listen(fd, MAX_PENDING) < 0)
		{
			LOG_ERR("Failed to listen on server socket" );
			exit(1);
		}
		setnonblocking(fd);
		LOG_INFO("server start !!!!!!!!!!!!!!!!!!!!!!!!!");
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
			LOG_ERR("Error accepting connection");
			return -1;
		}
		LOG_INFO("accept connected: "  + string(inet_ntoa(client_addr.sin_addr)));
		Handler* clientHandler = new EchoHandler();
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


#endif
#endif /* __ZEPOLLER_H__ */