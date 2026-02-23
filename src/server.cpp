#include <iostream>
#include <cstring>
#include <sys/socket.h>         //socket(),bind(),listen(),accept()
#include <netinet/in.h>         //struct sockaddr_in for ip/port numbers
#include <unistd.h>             //linux sys call utilities - close()

int main(){
    int server_fd = socket(AF_INET, SOCK_STREAM,0);
    if(server_fd==-1){
        std::cerr<<"Socket creation failed\n";
        return 1;
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

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if(bind(server_fd,(struct sockaddr*)&address ,sizeof(address)) < 0){
        std::cerr <<"Bind Failed\n";
        return 1;
    }

    if(listen(server_fd,5) < 0){
        std::cerr<<"Listen failed\n";
        return 1;
    }

    int client_fd = accept(server_fd, nullptr, nullptr);
    if(client_fd < 0){
        std::cerr<<"Accept failed\n";
        return 1;
    }
    std::cout<<"Client Connected\n";

    const char* responce = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type : text/plain\r\n"
        "Content-Length : 23\r\n"
        "\r\n"
        "Hello from C++ Server!\n";

    send(client_fd,responce,strlen(responce),0);
    close(client_fd);

    std::cout<<"server starting...\n";
    return 0;
}