#ifndef SERVERTEST_H_INCLUDED
#define SERVERTEST_H_INCLUDED
#include <stdio.h>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <list>
#include <typeinfo>
#include <cstring>
#define SERVER_IP           "0.0.0.0"
#define SERVER_PORT      6666
#define EPOLL_SIZE          100
#define BUFF_SIZE            2048
#define LISTEN_NUM       10
class Servertest
{
public:
    int serverSocket;                       //服务器socket
    struct sockaddr_in serverAddr;
    struct epoll_event event;
    struct epoll_event events[EPOLL_SIZE];      //事件监听队列
    std::list<int> clientSocket;               //客户端socket
    struct sockaddr_in clientAddr;
    int epollFd;                                //epoll句柄
    char buf[BUFF_SIZE];                 //数据缓冲
    void serverRun();
    Servertest();
    ~Servertest();
};
#endif