#include "Servertest.h"
Servertest::Servertest(){
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if((serverSocket = socket(PF_INET, SOCK_STREAM, 0))<0){
        perror("socket create error");
        exit(-1);
    }
    if((bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))<0){
        perror("socket bind error");
        exit(-1);
    }
    if(listen(serverSocket, LISTEN_NUM)<0){
        perror("socket listen error");
        exit(-1);
    }
    if((epollFd = epoll_create(EPOLL_SIZE))<0){
        perror("epoll create error");
        exit(-1);
    }
    event.events = EPOLLIN | EPOLLET;                             //设置监听事件
    event.data.fd = serverSocket;                                      //监听socket
    epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event);
}
void Servertest::serverRun(){
    std::cout<<"serverRun..."<<std::endl;
    for(;;){
        int eventCount = epoll_wait(epollFd, events, EPOLL_SIZE, -1);
        if(eventCount< 0){
            perror("epoll wait error");
            exit(-1);
        }
        for(int i = 0; i<eventCount; i++){
            int clientFd = events[i].data.fd;
            if(clientFd == serverSocket){
                //新客户端socket
                std::cout<<"new client"<<std::endl;
                event.events = EPOLLIN | EPOLLET;
                //std::cout<<typeid(sizeof(sockaddr_in)).name()<<endl;
                socklen_t size = sizeof(sockaddr_in);
                int newClient = accept(serverSocket, (struct sockaddr *)&clientAddr, &size);
                event.data.fd = newClient;
                epoll_ctl(epollFd, EPOLL_CTL_ADD, event.data.fd, &event);
                clientSocket.push_back(newClient);
                if(send(newClient, "connect success!", BUFF_SIZE, 0)<0){
                    perror("send error");
                    exit(-1);
                }
            }
            else{
                //客户端消息
                if(recv(clientFd, buf, BUFF_SIZE, 0)<0){
                    perror("recv error");
                    exit(0);
                }
                else{
                    std::cout<<"recv:"<<buf<<std::endl;
                    memset(&buf, 0, sizeof(buf));
                }
            }
        }
    }
}
int main(){
    Servertest *a = new Servertest;
    a->serverRun();
    return 0;
}