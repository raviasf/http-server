#include "server.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>

#include <sys/socket.h>         //socket(),bind(),listen(),accept()
#include <netinet/in.h>         //struct sockaddr_in for ip/port numbers
#include <unistd.h>             //linux sys call utilities - close()

#include <sys/epoll.h>            
#include <fcntl.h>              

Server::Server(int port):port(port),server_fd(-1){}

void Server::setupSocket(){
    //socket() give a TCP socket, AF_INET for IPv4, SOCK_STREAM for TCP, 0 for default protocol
    server_fd = socket(AF_INET, SOCK_STREAM,0);
    if(server_fd==-1){
        throw std::runtime_error("Socket creation failed");
    }

    //setsockopt() allow reuse of the address and port
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0){
        throw std::runtime_error("setsockopt failed");
    }

/*---------------------------------
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET(IPv4),AF_INET6(IPv6)
    unsigned short   sin_port;     // e.g. htons(3490),htons(8080)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};
------------------------------------*/

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd,(struct sockaddr*)&address ,sizeof(address)) < 0){
        throw std::runtime_error("Bind failed");
    }

    if(listen(server_fd,SOMAXCONN) < 0){
        throw std::runtime_error("Listen failed");
    }

    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    std::cout<<"server listening on port "<<port<<"\n";
}

//---------server socket setup finished--------
//---------------client handling----------------

void Server::start(){
    setupSocket();

    epoll_fd = epoll_create1(0);
    if(epoll_fd == -1){
        throw std::runtime_error("Failed to create epoll instance");
    }

    struct epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1){
        throw std::runtime_error("Failed to add server socket to epoll");
    }

    struct epoll_event events[MAX_EVENTS];
    while(running){
        std::cout<<"Waiting for a client to connect...\n";

        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if(n == -1){
            if(errno == EINTR){
                continue; // Interrupted by signal, check running flag again
            }
            std::cerr<<"Epoll wait failed\n";
            break;
        }

        for(int i=0; i<n; i++){
            int fd=events[i].data.fd;
            if(fd == server_fd){
                int client_fd = accept(server_fd,nullptr,nullptr);
                fcntl(client_fd, F_SETFL, O_NONBLOCK);

                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1){
                    std::cerr<<"Failed to add client socket to epoll\n";
                    close(client_fd);
                    continue;
                }else{
                    std::cout<<"New client connected, fd: "<<client_fd<<"\n";
                    handleClient(client_fd);
                }
            }
        }
    }
}

void Server::stop(){
    running = false;
    close(server_fd);
    close(epoll_fd);
}

void Server::handleClient(int client_fd){
    char buffer[3000] = {0};
    recv(client_fd, buffer, sizeof(buffer), 0);

    std::cout<<"Received from client: "<<buffer<<"\n";
    std::string body = "Hello from Ravi's C++ server!";
    std::string response = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: "+std::to_string(body.size())+"\r\n"
    "\r\n" +
    body;

    send(client_fd, response.c_str(), response.length(), 0);
    close(client_fd);
}
