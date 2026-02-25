#include "server.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>         //socket(),bind(),listen(),accept()
#include <netinet/in.h>         //struct sockaddr_in for ip/port numbers
#include <unistd.h>             //linux sys call utilities - close()

Server::Server(int port):port(port),server_fd(-1){}

void Server::setupSocket(){
    //socket() give me a TCP socket, AF_INET for IPv4, SOCK_STREAM for TCP, 0 for default protocol
    server_fd = socket(AF_INET, SOCK_STREAM,0);
    if(server_fd==-1){
        throw std::runtime_error("Socket creation failed");
    }

    //setsockopt() to allow reuse of the address and port
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

    //bind() attach the socket to the port and IP address specified in sockaddr_in
    if(bind(server_fd,(struct sockaddr*)&address ,sizeof(address)) < 0){
        throw std::runtime_error("Bind failed");
    }

    //listen() start queueing incoming connections, 5 is the max number of pending connections
    if(listen(server_fd,5) < 0){
        throw std::runtime_error("Listen failed");
    }

    std::cout<<"server listening on port "<<port<<"\n";
}

void Server::start(){
    setupSocket();

    while(true){
        std::cout<<"Waiting for a client to connect...\n";

        /*
        accept() :
        blocks until a client connects, 
        then returns a new socket file descriptor for the connection.
        The original server_fd continues to listen for new connections, 
        while the returned client_fd is used for communication with the connected client.
        */
        int client_fd = accept(server_fd,nullptr,nullptr);
        if(client_fd < 0){
            std::cerr<<"Accept failed\n";
            continue;
        }
        std::cout<<"Client connected\n";

        handleClient(client_fd);
        close(client_fd);
    }
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

    sleep(5); 

    send(client_fd, response.c_str(), response.length(), 0);
    close(client_fd);
}